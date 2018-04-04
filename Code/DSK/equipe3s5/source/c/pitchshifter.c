/*
 * pitchshifter.c
 *
 *  Created on: Apr 3, 2018
 *      Author: simon
 */
#include "pitchshifter.h"



void pitchShift(uint16_t *samples, int8_t step)
{
    uint8_t i,j;
    float alpha = findAlpha(step);
    uint16_t outputFrameOverlap = alpha * frameOverlap;
    // Allocate ton heap (malloc, realloc)
    uint16_t frameTable[frameTableSize];     // Table to hold the frames
    uint16_t currentFrame[windowSize];
    uint16_t framePhase[windowSize];
    uint16_t cumulFramePhase[windowSize];
    uint16_t timeStretched[timeStretchedSize];
    uint16_t

    splitFrames(samples, &frameTable);

    for(i=0; i<numFrames; i++)
    {
        for(j=0; j<windowSize; j++)
        {
            currentFrame[j] = frameTable[i*windowSize+j] * hanning[j] / windowConstant;  // windowConstant = constant -> sqrt((windowSize/frameOverlap)/2)
        }

        fftFrame(&currentFrame);

        framePhase(&currentFrame, &framePhase);
        frameMag(&currentFrame);

        processFrame(&framePhase);

        ifftFrame(&currentFrame,&framePhase);

        for(j=0; j<windowSize; j++)
        {
            frameTable[i*windowSize+j] = currentFrame[j] * hanning[j] / outputWindowConstant;  // windowConstant = constant -> sqrt((windowSize/outputFrameOverlap)/2)
        }

    }

    mergeFrames(&timeStretched, &frameTable, outputFrameOverlap);

    interpolateSamples(&samples, &timeStretched, alpha)
}


void splitFrames(uint16_t *samples, uint16_t *frames)
{
    uint8_t i,j,k;

    for(i=0; i<numFrames; i++)
    {
        for(j=(i*frameHop), k=i*windowSize; j<((i+1)*windowSize)); j++)
        {
            frames[k] = samples[j];
            k += 1;
        }
    }
}

void mergeFrames(uint16_t *samples, uin16_t *frames, uint16_t overlap)
{
    uint8_t i,j,k;
    uint16_t outputFrameHop = windowSize-overlap;

    for(i=0; i<windowSize; i++)          // The first frame is put in as is
    {
        samples[i] = frames[i];
    }

    for(i=1; i<numFrames; i++)
    {
        k = outputFrameHop * i;

        for(j=i*windowSize ; j<(i*outputFrameHop) ; j++)    // The first samples are overlapped with the previous frame
        {
            samples[k] = samples[k] + frames[j];
            k +=1;
        }
        for(; j<(i+1)*windowSize ;j++)   // Then the samples are put in as is
        {
            samples[k] = frames[j];
            k +=1;
        }
    }
}

void fftFrame(uint16_t *frames)
{

}

void ifftFrame(uint16_t *frame, uint16_t *phase)
{

}

void frameMagnitudes(uint16_t *frames)
{
    uint8_t i;

    for(i=0; i<windowSize; i++)
    {
        frames[i] = _abs(frames[i]);
    }
}

void framePhase(uint16_t *frames, uint16_t *phases)
{
    uint8_t i;
    for(i=0; i<windowSize; i++)
    {
       // atan(frames[2*i+1]/frames[2*i]);
    }
}

void processFrame(uint16_t *phase, uint16_t *phaseCumul, uint16_t outputOverlap)
{
    static uint16_t prevFramePhase[windowSize];
    uint16_t delta[windowSize];
    uint8_t i;

    for(i=0; i<windowSize; i++)     // DeltaPhi : get the phase difference, remove expected phase difference, map to -pi/pi range, get the true frequency
    {
        phaseCumul[i] = phaseCumul[i] + outputOverlap + 2*pi*i/windowSize + ((((phase[i] - prevFramePhase[i] - (frameOverlap * 2 * pi * i/windowSize))+pi) % (2*pi))-pi)/frameOverlap;
        prevFramePhase[i] = phase[i];
    }
}
    /*
    for(i=0; i<windowSize; i++)     // DeltaPhi : get the phase difference
    {
        delta[i] = phase[i] - prevFramePhase[i];
        prevFramePhase[i] = phase[i];
    }
    for(i=0; i<windowSize; i++)     // DeltaPhiPrime : remove expected phase difference
    {
        delta[i] = delta[i] - (frameOverlap * 2 * pi * i/windowSize);
    }
    for(i=0; i<windowSize; i++)     // DeltaPhiPrimeMod : map to -pi/pi range
    {
        delta[i] = ((delta[i]+pi)%(2*pi)) - pi;
    }
    for(i=0; i<windowSize; i++)     // Get the true frequency
    {
        delta[i] = 2*pi*i/windowSize + delta[i]/frameOverlap;
    }
    for(i=0; i<windowSize; i++)
    {
        phaseCumul[i] = phaseCumul[i] + outputOverlap + delta[i];
    }*/


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
        alpha = 1 / (1 << (_abs(step)/12);  // 1 / 2^(abs(step)/12) = 2^(step/12) when step < 0
    }
    return alpha;
}






