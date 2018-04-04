/*
 * pitchshifter.h
 *
 *  Created on: Apr 3, 2018
 *      Author: simon
 */

#ifndef PITCHSHIFTER_H_
#define PITCHSHIFTER_H_

#include "definitions.h"
#include "hanning.dat"

static const uint16_t numSamples = 1024;        // 1024 input samples
static const uint16_t windowSize = 256;         // Size of processing frames
static const uint16_t frameOverlap = windowSize / 4;    // 25% frame overlap
static const uint16_t frameHop = windowSize - frameOverlap;
static const uint16_t numFrames = (numSamples - windowSize) / frameOverlap;  // Number of frames overlapped in the samples
static const uint16_t frameTableSize = windowSize * numFrames;      // Size of the table to hold the frames
static const uint16_t maxOutputOverlap = frameOverlap * 1.7818;   // 2^(10/12) -> 1.7818, step of 10
static const uint16_t timeStretchedSize = (numFrames*maxOutputOverlap)-maxOutputOverlap+windowSize;
static const float pi = 3.1416;

void pitchShift(uint16_t *samples, uint8_t step);
void splitFrames(uint16_t *samples, uint16_t *frames);
void mergeFrames(uint16_t *samples, uint16_t *frames);
void windowFrame(uint16_t * samples, uint16_t *frames, uint16_t overlapConstant);
void fftFrame(uint16_t *frames);
void ifftFrame(uint16_t *frame, uint16_t *phase);
void frameMagnitudes(uint16_t *frames);
void framePhase(uint16_t *frames, uint16_t *phases);
void processFrame(uint16_t *phases, uint16_t *phaseCumul);
void interpolateSamples(uint16_t *samples, uint16_t *processedFrames, uint16_t alpha);
float findAlpha(int8_t step);




#endif /* INCLUDE_PITCHSHIFTER_H_ */
