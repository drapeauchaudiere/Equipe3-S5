/*
 * pitchshifter.c
 *
 *  Created on: Apr 3, 2018
 *      Author: simon
 */
#include "pitchshifter.h"
#include "DSPF_sp_cfftr2_dit.h"
#include "DSPF_sp_icfftr2_dif.h"
#include "DSPF_sp_bitrev_cplx.h"
#include "bitrev_index.h"
#include "hanning.dat"
#include "utility.h"

#pragma DATA_ALIGN(w, 8);
float w[WINDOW_SIZE];

#pragma DATA_ALIGN(TableFFT,8);
float TableFFT[2*WINDOW_SIZE];

void pitchShift(float *samples, int8_t step)
{
    uint16_t i,j;
    float alpha = findAlpha(step);
    uint16_t outputFrameHop = alpha * FRAME_HOP; // The hop is the delay between two frames
    float outputWindowConstant = sqrt((WINDOW_SIZE/outputFrameHop)/2);
    int16_t index[16];
    // Allocate ton heap (malloc, realloc) ?
    static float frameTable[FRAME_TABLE_SIZE];        // Table to hold the frames
    static float currentFrame[WINDOW_SIZE];          // Vector to hold the samples of the current frame
    static float framePhase[WINDOW_SIZE];            // Vector to hold the phase
    static float cumulFramePhase[WINDOW_SIZE];
    static float timeStretched[TIME_STRETCHED_SIZE];   // Allocate dynamically ?

    tableClear(cumulFramePhase, WINDOW_SIZE);
    tableClear(timeStretched, TIME_STRETCHED_SIZE);

    tw_genr2fft(w, WINDOW_SIZE);
    bit_rev(w,(WINDOW_SIZE>>1));
    bitrev_index(index,WINDOW_SIZE);

    splitFrames(samples, frameTable);   // Separates the samples into frames

    for(i=0; i<NUMBER_FRAMES; i++)      // One frame at a time,
    {
        for(j=0; j<WINDOW_SIZE; j++)    // Apply the window function (precalculated hanning window)
        {
            currentFrame[j] = frameTable[i*WINDOW_SIZE+j] * hanning[j] / WINDOW_CONSTANT;  // windowConstant = constant -> sqrt((WINDOW_SIZE/frameOverlap)/2)
        }

        fftFrame(currentFrame,index);   // Extract the frequency data

        framePhases(framePhase);        // Find the phases
        frameMagnitudes();              // Find the magnitudes

        processFrame(framePhase,cumulFramePhase,outputFrameHop);    // Find adjust the frequency data to the new pitch

        ifftFrame(currentFrame,cumulFramePhase,index);  // Put the new frequency data back to the time domain

        for(j=0; j<WINDOW_SIZE; j++)    // Apply another window function to the new data
        {
            frameTable[i*WINDOW_SIZE+j] = currentFrame[j] * hanning[j] / outputWindowConstant;  // windowConstant = constant -> sqrt((WINDOW_SIZE/outputFrameOverlap)/2)
        }
    }

    mergeFrames(timeStretched, frameTable, outputFrameHop); // Merge the frames back together, while applying the pitch shift between the samples

    interpolateSamples(samples, timeStretched, step);    // Resample the data to match the input samples length
}

/* |------------------Samples---------------------|                      *
 *                                                                       *
 * |------0------|                                                       *
 *     |------1------|                                                   *
 *         |------2------|                                               *
 *             |------3------|                                           *
 *                      ...                                              *
 *                                                                       *
 * Index            Frame                                                *
 *   0         |------0------|                                           *
 *   1         |------1------|                                           *
 *   2         |------2------|                                           *
 *   3         |------3------|                                           *
 *  ...              ...

***************************************************************************/

void splitFrames(float *samples, float *frames)
{
    uint16_t i,j,k;

    for(i=0; i<NUMBER_FRAMES; i++)
    {
        k=i*WINDOW_SIZE;
        for(j=(i*FRAME_HOP); j<((i*FRAME_HOP)+WINDOW_SIZE); j++)
        {
            frames[k] = samples[j];
            k++;
        }
    }
}


/* Index            Frame                                                *
 *   0         |------0------|                                           *
 *   1         |------1------|                                           *
 *   2         |------2------|                                           *
 *   3         |------3------|                                           *
 *  ...              ...                                                 *
 *                                                                       *
 * ---n-1----| <- last frame from the previous samples                   *
 * |------0------|                                                       *
 *   + |------1------|                                                   *
 *       + |------2------|                                               *
 *           + |------3------|                                           *
 *               +      ...                                              *
 *                                                                       *
 * |------------------Input vector---------------------|                 */

void mergeFrames(float *samples, float *frames, uint16_t hop)
{
    uint16_t i,j,k,n;
    uint16_t outputFrameOverlap = WINDOW_SIZE-hop;
    static float prevSamples[WINDOW_SIZE];

    tableClear(prevSamples,WINDOW_SIZE);

    for(i=0; i<outputFrameOverlap; i++)          // The first frame is added to the frame of the previous samples
    {
        samples[i] = prevSamples[hop+i]+frames[i];
    }
    for(; i<WINDOW_SIZE; i++)
    {
        samples[i] = frames[i];
    }

    for(i=1; i<NUMBER_FRAMES-1; i++)
    {
        k = hop * i;

        for(j=i*WINDOW_SIZE ; j<(i*WINDOW_SIZE+outputFrameOverlap) ; j++)    // The first samples are overlapped with the previous frame
        {
            samples[k] = samples[k] + frames[j];
            k++;
        }
        for(; j<(i+1)*WINDOW_SIZE; j++)                                     // Then the samples are put in as is
        {
            samples[k] = frames[j];
            k++;
        }
    }

    k = hop * i;
    for(j=i*WINDOW_SIZE,n=0; j<(i*WINDOW_SIZE+outputFrameOverlap); j++)  // The last frame samples are saved for the next samples
    {
        samples[k] = samples[k] + frames[j];
        prevSamples[n] = samples[k];
        k++;
        n++;
    }
    for(; j<(i+1)*WINDOW_SIZE; j++)
    {
        samples[k] = frames[j];
        prevSamples[n] = samples[k];
        k++;
        n++;
    }
}

void fftFrame(float *frame, int16_t *index)
{
    uint16_t i;

    for (i=0;i<WINDOW_SIZE;++i)
    {
        TableFFT[2*i]=frame[i];
        TableFFT[2*i+1]=0;
    }

    DSPF_sp_cfftr2_dit(TableFFT,w,WINDOW_SIZE);
    DSPF_sp_bitrev_cplx((double*)TableFFT,index,WINDOW_SIZE);
}

void ifftFrame(float *frame, float *phase, int16_t *index)
{
    uint16_t i,n;

    // Load tableFFT
    for(i=0; i<WINDOW_SIZE; i++)
    {
        TableFFT[2*i+1] = TableFFT[2*i] * sin(phase[i]);
        TableFFT[2*i] = TableFFT[2*i] * cos(phase[i]);
    }

    DSPF_sp_bitrev_cplx((double*)TableFFT,index,WINDOW_SIZE);

    DSPF_sp_icfftr2_dif(TableFFT,w,WINDOW_SIZE);

    for (i=0; i<WINDOW_SIZE; ++i){
           frame[i]=TableFFT[2*i]/WINDOW_SIZE;
    }

}

void frameMagnitudes(void)
{
    uint16_t i;
    for(i=0; i<WINDOW_SIZE; i++)
    {
        TableFFT[2*i] = sqrt (pow(TableFFT[2*i],2) + pow(TableFFT[2*i+1],2));   // sqrt(TableFFT[2*i]^2+TableFFT[2*i+1]^2);
    }
}

void framePhases(float *phases)
{
    uint16_t i;
    for(i=0; i<WINDOW_SIZE; i++)
    {
        phases[i] = atan2(TableFFT[2*i+1], TableFFT[2*i]);      // atan(frames[2*i+1]/frames[2*i]);
    }
}

void processFrame(float *phase, float *phaseCumul, uint16_t outHop)
{
    //static float prevFramePhase[WINDOW_SIZE] = {0};
    float delta[WINDOW_SIZE];
    float temp = 0;
    uint16_t i;

/*    for(i=0; i<WINDOW_SIZE; i++)     // DeltaPhi : get the phase difference, remove expected phase difference, map to -pi/pi range, get the true frequency
    {
        phaseCumul[i] = (uint16_t)(phaseCumul[i] + overlap + 2*pi*i/WINDOW_SIZE + ((((phase[i] - prevFramePhase[i] - (FRAME_OVERLAP * 2 * pi * i/WINDOW_SIZE))+pi) % (2*pi)))-pi)/FRAME_OVERLAP);
        prevFramePhase[i] = phase[i];
    }
}*/

    for(i=0; i<WINDOW_SIZE; i++)     // DeltaPhi : get the phase difference
    {
        delta[i] = phase[i] ;//- prevFramePhase[i];
        //prevFramePhase[i] = phase[i];
    }
    for(i=0; i<WINDOW_SIZE; i++)     // DeltaPhiPrime : remove expected phase difference
    {
        delta[i] = delta[i] - (FRAME_HOP * 2 * pi * i/WINDOW_SIZE);
    }
    for(i=0; i<WINDOW_SIZE; i++)     // DeltaPhiPrimeMod : map to -pi/pi range
    {
        delta[i] = angleModulo(delta[i]);
    }
    for(i=0; i<WINDOW_SIZE; i++)     // Get the true frequency
    {
        delta[i] = 2*pi*i/WINDOW_SIZE + delta[i]/FRAME_HOP;
    }
    for(i=0; i<WINDOW_SIZE; i++)        // Get the new phase adjusted to the pitch
    {
        temp = outHop * delta[i];
        phaseCumul[i] = phaseCumul[i] + outHop * delta[i];
    }
}


void interpolateSamples(float *samples, float *processedFrames, uint16_t step)
{
    uint16_t i;
    tableClear(samples,NUMBER_SAMPLES);
    if(step == 12)
    {
        for(i=0; i<NUMBER_SAMPLES; i++)
        {
            samples[i] = processedFrames[3*i/2]; // *** Mettre en fractionnaire (2Q13?)
        }
    }
    else if(step == 0)
    {
        for(i=0; i<NUMBER_SAMPLES; i++)
        {
            samples[i] = 100000*processedFrames[i];    // *** Mettre en fractionnaire (2Q13?)
        }
    }

}

float findAlpha(int8_t step)
{
    float alpha;

    if(step > 0)
    {
        alpha = 1 << (step/12);             // 2^(step/12)
    }
    else
    {
        alpha = 1 / (1 << (_abs(step)/12));  // 1 / 2^(abs(step)/12) = 2^(step/12) when step < 0
    }
    return alpha;
}

float angleModulo(float angle)
{
    if (angle>0)
          angle = fmod(angle+pi, 2.0*pi)-pi;
      else
          angle = fmod(angle-pi, 2.0*pi)+pi;
    return angle;
}

void tableClear(float *table, uint16_t length)
{
    uint16_t i;

    for(i=0; i<length; i++)
    {
        table[i] = 0;
    }
}


