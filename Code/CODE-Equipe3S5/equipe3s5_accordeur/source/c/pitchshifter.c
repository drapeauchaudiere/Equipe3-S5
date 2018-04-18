/*
 * pitchshifter.c
 *
 *  Created on: Apr 3, 2018
 *      Author: simon
 */

#include <hann.dat>
#include <phaseTest.dat>
#include "pitchShifter.h"
#include "DSPF_sp_cfftr2_dit.h"
#include "DSPF_sp_icfftr2_dif.h"
#include "string.h"
#include <math.h>
#include "twiddles.h"
#include "bitrev_index.h"

#include "C6713Helper_UdeS.h"
#include "DSPF_sp_cfftr4_dif.h"
#include <DSPF_sp_bitrev_cplx.h>

#pragma DATA_ALIGN(TableFFT, 8);  /* Remove this if using DATA_MEM_BANK */
float TableFFT[2*FRAME_SIZE];

float prevPhase[FRAME_SIZE];
float newPhase[FRAME_SIZE];
float newFrame[FRAME_SIZE];
float sumBuffer[FRAME_SIZE];
float phaseCumulative[FRAME_SIZE];
short index[32];
float frameWindowed[FRAME_SIZE];
float frame2add[FRAME_SIZE];
float delta[FRAME_SIZE];

float Facto[11] = {
  1.0,
  0.5,
  0.166666667,
  0.041666667,
  0.008333333,
  0.001388889,
  0.000198413,
  0.000024802,
  0.000002756,
  0.00000027,
  0.00000003
};

void pitchShift(float *newSample, float *outSample)
{
    static int counter = 0;
    static counter2 = 0;
    if(counter<7)
    {
        createNewframe(newSample);
        counter++;
    }
    else
    {
        /*
        if (counter2 == 8)
        {
            tableClear(prevPhase, FRAME_SIZE);
            tableClear(newPhase, FRAME_SIZE);
            //tableClear(newFrame, FRAME_SIZE);
            tableClear(sumBuffer, FRAME_SIZE);
            tableClear (phaseCumulative,FRAME_SIZE);
            counter2=0;
        }
        else
        {
            counter2++;
        }
        */
        createNewframe(newSample);
        processFrame();
        addNewFrame();
        outputSample(outSample);
    }
}

void outputSample(float *outSample)
{
    int i;
    for (i = 0; i<FRAME_HOP; i++)
    {
        outSample[i] = sumBuffer[2*i];
    }
}

void addNewFrame()
{
    int i;

    for (i=0; i<FRAME_SIZE-FRAME_HOP_OUT; i++)
    {
        sumBuffer[i] = sumBuffer[i+FRAME_HOP_OUT] + frame2add[i];
    }

    for (i=FRAME_SIZE-FRAME_HOP_OUT; i<FRAME_SIZE; i++)
    {
        sumBuffer[i] = frame2add[i];
    }
}



void fftFrame(float *frame, float *frameFFT)
{
    int i;
    for (i=0;i<FRAME_SIZE;++i)
    {
        frameFFT[2*i]=frame[i];
        frameFFT[2*i+1]=0;
    }


    DSPF_sp_cfftr2_dit(frameFFT,w,FRAME_SIZE);

    //bitrev_index(index, FRAME_SIZE);
    //DSPF_sp_bitrev_cplx((double*)frameFFT, index, FRAME_SIZE);

    /*
    //Amplitude
    float resultatFFT[FRAME_SIZE];
    for (i = 0; i < FRAME_SIZE; i++)
        resultatFFT[i] = sqrt(frameFFT[(2*i)]*frameFFT[(2*i)] + frameFFT[(2*i+1)]*frameFFT[(2*i+1)]);
    //DSPF_sp_bitrev_cplx((double*)TableFFT,index,WINDOW_SIZE);
     * */

}

void ifftFrame(float *frame, float *phase)
{
    int i;
    float real = 0;
    float imag = 0;
    float oldEvenTableFFT;
    // Load tableFFT
    for(i=0; i<FRAME_SIZE; i++)
    {
        //getComplex(phaseCumulative[i], &real, &imag);
        real = cosf(phaseCumulative[i]);
        imag = sinf(phaseCumulative[i]);
        oldEvenTableFFT = TableFFT[2*i];
        TableFFT[2*i] = TableFFT[2*i] * real - TableFFT[2*i+1]*imag;
        TableFFT[2*i+1] = oldEvenTableFFT * imag + TableFFT[2*i+1]*real;
    }

    //DSPF_sp_bitrev_cplx((double*)TableFFT,index,FRAME_SIZE);
    //DSPF_sp_bitrev_cplx((double*)TableFFT, index, FRAME_SIZE);
    DSPF_sp_icfftr2_dif(TableFFT,w,FRAME_SIZE);
    for (i=0; i<FRAME_SIZE; ++i){
        frame2add[i]=TableFFT[2*i]*hann[i];///sqrt((FRAME_SIZE/FRAME_HOP_OUT)/2);
    }
}



void processFrame(void)
{
    int i;
    for(i = 0 ; i < FRAME_SIZE ; i++ )
    {
        frameWindowed[i] = newFrame[i]*hann[i];
    }

    fftFrame(frameWindowed, TableFFT);

    getAngleFrame();

    for(i = 0; i<FRAME_SIZE; i++)
    {
        delta[i] = newPhase[i] - prevPhase[i];
        prevPhase[i] = newPhase[i];
        delta[i] = delta[i] - pi/4;
        //delta[i] = angleModulo(delta[i])-pi;
        delta[i] = 2*pi/FRAME_SIZE + delta[i]/FRAME_HOP;
        phaseCumulative[i] =  FRAME_HOP_OUT* delta[i] + phaseCumulative[i];
    }


    ifftFrame(TableFFT, phaseCumulative);

}


float angleModulo(float angle)
{
    if (angle>0)
          angle = fmod(angle+pi, 2.0*pi)-pi;
      else
          angle = fmod(angle-pi, 2.0*pi)+pi;
    return angle;
}

void tableClear(float *table, int length)
{
    int i;

    for(i=0; i<length; i++)
    {
        table[i] = 0;
    }
}


void createNewframe(float newSample[])
{
    int i,n;
    for (i = 0; i < FRAME_SIZE - FRAME_HOP; i++)
    {
        newFrame[i] = newFrame[i+FRAME_HOP];
    }
    for (n  = 0; n < FRAME_HOP; n++)
    {
        newFrame[i] = newSample[n];
        i++;
    }
}



void initPitchShifter()
{
    // Clears all the buffer of the pitchShifter
    tableClear(prevPhase, FRAME_SIZE);
    tableClear(newPhase, FRAME_SIZE);
    tableClear(newFrame, FRAME_SIZE);
    tableClear(sumBuffer, FRAME_SIZE);
    tableClear (phaseCumulative,FRAME_SIZE);
}



void getAngleFrame(void)
{
    int i;
    float x;
    for (i = 0; i < FRAME_SIZE ; i++)
    {
        if (TableFFT[2*i+1] == 0.0){
            newPhase[i] = 0.0;
        }
        else if (TableFFT[2*i] == 0.0){
            newPhase[i] = pi/2;
        }
        else
        {
            newPhase[i] = atan2_approximation1(TableFFT[2*i+1], TableFFT[2*i]);
        }
    }
}




float atan2_approximation1(float y, float x)
{
    float ONEQTR_PI = pi / 4.0;
    float THRQTR_PI = 3.0 * pi / 4.0;
    float r, angle;
    float abs_y = fabs(y) + 1e-10f;      // kludge to prevent 0/0 condition
    if ( x < 0.0f )
    {
        r = (x + abs_y) / (abs_y - x);
        angle = THRQTR_PI;
    }
    else
    {
        r = (x - abs_y) / (x + abs_y);
        angle = ONEQTR_PI;
    }
    angle += (0.1963f * r * r - 0.9817f) * r;
    if ( y < 0.0f )
        return( -angle );     // negate if in quad III or IV
    else
        return( angle );
}



void getComplex(float x,float *real, float *imag)
{
    static float xpow [11];
    int i;
    xpow[0] = x;
    for (i = 1; i< 11; i++)
    {
        xpow[i] = xpow[i-1]*x;
    }

    *real = 1;
    *imag = x;

    // Add
    for(i=3; i<11; i+=4)
    {
        *real += xpow[i]*Facto[i];
        *imag += xpow[i+1]*Facto[i+1];
    }

    // Sub
    for(i=1; i<11; i+=4)
    {
        *real += xpow[i]*Facto[i];
        *imag += xpow[i+1]*Facto[i];
    }
}

