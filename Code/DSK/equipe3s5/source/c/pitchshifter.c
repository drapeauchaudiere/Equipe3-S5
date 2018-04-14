/*
 * pitchshifterV2.c
 *
 *  Created on: Apr 11, 2018
 *      Author: simon
 */

#include "pitchshifter.h"
#include "DSPF_sp_cfftr2_dit.h"
#include "DSPF_sp_icfftr2_dif.h"
#include "DSPF_sp_cfftr4_dif.h"
#include "DSPF_sp_bitrev_cplx.h"
#include "bitrev_index.h"
#include "hanning.dat"
#include "utility.h"
#include "twiddle_1024.h"

//#pragma DATA_ALIGN(w, 8);
//float w[3*FRAME_SIZE/2];

#pragma DATA_ALIGN(TableFFT,8);
float TableFFT[2*FRAME_SIZE];

void pitchShift(float *inSamples, float *outSamples)
{
    int16_t fftIndex[16];
    int16_t i;
    static float sampleBuffer[FRAME_SIZE];
    static float frame[FRAME_SIZE];
    static float sumBuffer[FRAME_SIZE];
    static float framePhase[FRAME_SIZE];
    static float cumulFramePhase[FRAME_SIZE];
    static float prevFramePhase[FRAME_SIZE];

    clearBuffer(cumulFramePhase, FRAME_SIZE);

    for(i=0; i<FRAME_SIZE; i++)
    {
        sampleBuffer[i] = outputWindowConstant;
    }


    //tw_genr4fft(w,FRAME_SIZE*2);
    //bit_rev(w,3*FRAME_SIZE/2);
    bitrev_index(fftIndex,FRAME_SIZE);

    shiftFrame(inSamples, sampleBuffer, frame);
    windowFrame(frame,inputWindowConstant);
    fftFrame(frame,fftIndex);
    framePhases(framePhase);
    frameMagnitudes();
    processFrame(framePhase,cumulFramePhase,prevFramePhase);
    ifftFrame(frame,cumulFramePhase,fftIndex);
    windowFrame(frame,outputWindowConstant);
    mergeFrame(sumBuffer,frame);
    interpolateSamples(outSamples, sumBuffer);

}

void windowFrame(float * samples, float constant)
{
    uint16_t j;

    for(j=0; j<FRAME_SIZE; j++)    // Apply the window function (precalculated hanning window)
    {
        samples[j] = samples[j] * hanning[j] / constant;  // windowConstant = constant -> sqrt((FRAME_SIZE/frameOverlap)/2)
    }

}

void shiftFrame(float *samples, float *buffer, float *currentFrame)
{
    uint16_t i,j=0;

    for(i=0; i<FRAME_SIZE-HOP_SIZE; i++)
    {
        buffer[i] = buffer[HOP_SIZE+i];
        currentFrame[i] = buffer[i];
    }

    for(; i<FRAME_SIZE; i++)
    {
        buffer[i] = samples[j];
        currentFrame[i] = buffer[i];
        j++;
    }
}

void fftFrame(float *frame, int16_t *index)
{
    uint16_t i;

    for (i=0;i<FRAME_SIZE;++i)
    {
        TableFFT[2*i]=frame[i];
        TableFFT[2*i+1]=0;
    }

    DSPF_sp_cfftr4_dif(TableFFT,(float *)w,FRAME_SIZE);
    DSPF_sp_bitrev_cplx((double*)TableFFT,index,FRAME_SIZE);
}

void ifftFrame(float *frame, float *phase, int16_t *index)
{
    uint16_t i,n;

    // Load tableFFT
    for(i=0; i<FRAME_SIZE; i++)
    {
        TableFFT[2*i+1] = TableFFT[2*i] * sin(phase[i]);
        TableFFT[2*i] = TableFFT[2*i] * cos(phase[i]);
    }

    DSPF_sp_bitrev_cplx((double*)TableFFT,index,FRAME_SIZE);

    DSPF_sp_icfftr2_dif(TableFFT,(float *)w,FRAME_SIZE);

    for (i=0; i<FRAME_SIZE; ++i){
           frame[i]=TableFFT[2*i]/FRAME_SIZE;
    }
}

void frameMagnitudes(void)
{
    uint16_t i;
    for(i=0; i<FRAME_SIZE; i++)
    {
        TableFFT[2*i] = sqrt (pow(TableFFT[2*i],2) + pow(TableFFT[2*i+1],2));   // sqrt(TableFFT[2*i]^2+TableFFT[2*i+1]^2);
    }
}

void framePhases(float *phases)
{
    uint16_t i;
    for(i=0; i<FRAME_SIZE; i++)
    {
        phases[i] = atan2(TableFFT[2*i+1], TableFFT[2*i]);      // atan(frames[2*i+1]/frames[2*i]);
    }
}

void processFrame(float *phases, float *phaseCumul, float * prevPhase)
{
    float delta[FRAME_SIZE];
    uint16_t i;

/*    for(i=0; i<FRAME_SIZE; i++)     // DeltaPhi : get the phase difference, remove expected phase difference, map to -pi/pi range, get the true frequency
    {
        phaseCumul[i] = (uint16_t)(phaseCumul[i] + overlap + 2*pi*i/FRAME_SIZE + ((((phase[i] - prevFramePhase[i] - (FRAME_OVERLAP * 2 * pi * i/FRAME_SIZE))+pi) % (2*pi)))-pi)/FRAME_OVERLAP);
        prevFramePhase[i] = phase[i];
    }
}*/

    for(i=0; i<FRAME_SIZE; i++)     // DeltaPhi : get the phase difference
    {
        delta[i] = phases[i] - prevPhase[i];
        prevPhase[i] = phases[i];
    }
    for(i=0; i<FRAME_SIZE; i++)     // DeltaPhiPrime : remove expected phase difference
    {
        delta[i] = delta[i] - (HOP_SIZE * 2 * pi * i/FRAME_SIZE);
    }
    for(i=0; i<FRAME_SIZE; i++)     // DeltaPhiPrimeMod : map to -pi/pi range
    {
        delta[i] = angleModulo(delta[i]);
    }
    for(i=0; i<FRAME_SIZE; i++)     // Get the true frequency
    {
        delta[i] = 2*pi*i/FRAME_SIZE + delta[i]/HOP_SIZE;
    }
    for(i=0; i<FRAME_SIZE; i++)        // Get the new phase adjusted to the pitch
    {
        phaseCumul[i] = phaseCumul[i] + HOP_SIZE * 2 * delta[i];
    }
}

void mergeFrame(float *outFrame, float *frame)
{
    uint16_t i,j;

    for(i=0; i<FRAME_SIZE-(HOP_SIZE*2); i++)
    {
        outFrame[i] = outFrame[(HOP_SIZE*2)+i] + frame[i];
    }

    for(;i<FRAME_SIZE; i++)
    {
        outFrame[i] = frame[i];
    }

}

void interpolateSamples(float *outSamples, float *processedFrame)
{
    uint16_t i;

    for(i=0; i<HOP_SIZE; i++)
    {
        outSamples[i] = processedFrame[2*i];
    }
}

float angleModulo(float angle)
{
    if (angle>0)
          angle = fmod(angle+pi, 2.0*pi)-pi;
      else
          angle = fmod(angle-pi, 2.0*pi)+pi;
    return angle;
}

void clearBuffer(float *buffer, uint16_t size)
{
    uint16_t i;
    for(i=0; i<size; i++)
    {
        buffer[i] = 0;
    }
}


