/*
 * pitchshifter.c
 *
 *  Created on: Apr 3, 2018
 *      Author: simon
 */
#include "pitchshifter.h"
#include "DSPF_sp_cfftr2_dit.h"
#include "DSPF_sp_icfftr2_dif.h"
#include "hanning.dat"
#include "utility.h"

#pragma DATA_ALIGN(w, 8);
float w[WINDOW_SIZE];

#pragma DATA_ALIGN(TableFFT,8);
float TableFFT[2*WINDOW_SIZE];

void pitchShift(uint16_t *samples, int8_t step)
{
    uint16_t i,j;
    float alpha = findAlpha(step);
    uint16_t outputFrameHop = alpha * FRAME_HOP;
    uint16_t outputWindowConstant = sqrt((WINDOW_SIZE/outputFrameHop)/2);
    // Allocate ton heap (malloc, realloc) ?
    static uint16_t frameTable[FRAME_TABLE_SIZE];        // Table to hold the frames
    static uint16_t currentFrame[WINDOW_SIZE];          // Vector to hold the samples of the current frame
    static uint16_t framePhase[WINDOW_SIZE];            // Vector to hold the phase
    static uint16_t cumulFramePhase[WINDOW_SIZE];
    static uint16_t timeStretched[TIME_STRETCHED_SIZE];   // Allocate dynamically ?

    tw_genr2fft(w, WINDOW_SIZE);
    bit_rev(w,(WINDOW_SIZE>>1));

    splitFrames(samples, frameTable);

    for(i=0; i<NUMBER_FRAMES; i++)
    {
        for(j=0; j<WINDOW_SIZE; j++)
        {
            currentFrame[j] = frameTable[i*WINDOW_SIZE+j] * hanning[j] / WINDOW_CONSTANT;  // windowConstant = constant -> sqrt((WINDOW_SIZE/frameOverlap)/2)
        }

        fftFrame(currentFrame);

        framePhases(framePhase);
        frameMagnitudes();

        processFrame(framePhase,cumulFramePhase,outputFrameHop);

        ifftFrame(currentFrame,framePhase);

        for(j=0; j<WINDOW_SIZE; j++)
        {
            frameTable[i*WINDOW_SIZE+j] = currentFrame[j] * hanning[j] / outputWindowConstant;  // windowConstant = constant -> sqrt((WINDOW_SIZE/outputFrameOverlap)/2)
        }
    }

    mergeFrames(timeStretched, frameTable, outputFrameHop);

    interpolateSamples(samples, timeStretched, alpha);
}


void splitFrames(uint16_t *samples, uint16_t *frames)
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

void mergeFrames(uint16_t *samples, uint16_t *frames, uint16_t hop)
{
    uint16_t i,j,k,n;
    uint16_t outputFrameOverlap = WINDOW_SIZE-hop;
    static uint16_t prevSamples[WINDOW_SIZE] = {0};


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
        for(; j<(i+1)*WINDOW_SIZE; j++)                          // Then the samples are put in as is
        {
            samples[k] = frames[j];
            k++;
        }
    }

    k = hop * i;
    for(j=i*WINDOW_SIZE,n=0; j<(i*WINDOW_SIZE+outputFrameOverlap); j++)                     // The last frame samples are saved for the next samples
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

void fftFrame(uint16_t *frame)
{
    uint16_t i;

    for (i=0;i<WINDOW_SIZE;++i)
    {
        TableFFT[2*i]=frame[i];
        TableFFT[2*i+1]=0;
    }

    DSPF_sp_cfftr2_dit(TableFFT,w,WINDOW_SIZE);
}

void ifftFrame(uint16_t *frame, uint16_t *phase)
{
    uint16_t i,n;

    // Load tableFFT
    for(i=0; i<WINDOW_SIZE; i++)
    {
        TableFFT[2*i+1] = TableFFT[2*i] * sin(phase[i]);
        TableFFT[2*i] = TableFFT[2*i] * cos(phase[i]);
    }

    DSPF_sp_icfftr2_dif(TableFFT,w,WINDOW_SIZE);

    for (i=0; i<WINDOW_SIZE; ++i){
           TableFFT[i]=TableFFT[2*i]/WINDOW_SIZE;
    }

    n = WINDOW_SIZE-1;

    for (i=0; i<WINDOW_SIZE; ++i){
        frame[n]=TableFFT[2*i];
        n--;
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

void framePhases(uint16_t *phases)
{
    uint16_t i;
    for(i=0; i<WINDOW_SIZE; i++)
    {
        phases[i] = atan2(TableFFT[2*i+1], TableFFT[2*i]);      // atan(frames[2*i+1]/frames[2*i]);
    }
}

void processFrame(uint16_t *phase, uint16_t *phaseCumul, uint16_t overlap)
{
    static uint16_t prevFramePhase[WINDOW_SIZE];
    uint16_t delta[WINDOW_SIZE];
    uint16_t i;

/*    for(i=0; i<WINDOW_SIZE; i++)     // DeltaPhi : get the phase difference, remove expected phase difference, map to -pi/pi range, get the true frequency
    {
        phaseCumul[i] = (uint16_t)(phaseCumul[i] + overlap + 2*pi*i/WINDOW_SIZE + ((((phase[i] - prevFramePhase[i] - (FRAME_OVERLAP * 2 * pi * i/WINDOW_SIZE))+pi) % (2*pi)))-pi)/FRAME_OVERLAP);
        prevFramePhase[i] = phase[i];
    }
}*/

    for(i=0; i<WINDOW_SIZE; i++)     // DeltaPhi : get the phase difference
    {
        delta[i] = phase[i] - prevFramePhase[i];
        prevFramePhase[i] = phase[i];
    }
    for(i=0; i<WINDOW_SIZE; i++)     // DeltaPhiPrime : remove expected phase difference
    {
        delta[i] = delta[i] - (FRAME_OVERLAP * 2 * pi * i/WINDOW_SIZE);
    }
    for(i=0; i<WINDOW_SIZE; i++)     // DeltaPhiPrimeMod : map to -pi/pi range
    {
        delta[i] = fmod(delta[i]+pi,2*pi) - pi;
    }
    for(i=0; i<WINDOW_SIZE; i++)     // Get the true frequency
    {
        delta[i] = 2*pi*i/WINDOW_SIZE + delta[i]/FRAME_OVERLAP;
    }
    for(i=0; i<WINDOW_SIZE; i++)
    {
        phaseCumul[i] = phaseCumul[i] + overlap + delta[i];
    }
}


void interpolateSamples(uint16_t *samples, uint16_t *processedFrames, uint16_t alpha)
{

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






