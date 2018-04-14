/*
 * pitchshifterV2.h
 *
 *  Created on: Apr 11, 2018
 *      Author: simon
 */

#ifndef INCLUDE_PITCHSHIFTERV2_H_
#define INCLUDE_PITCHSHIFTERV2_H_

#include "definitions.h"

#define FRAME_SIZE  256
#define HOP_SIZE    64
#define HOPOUT_SIZE 128
static const float inputWindowConstant = 2;
static const float outputWindowConstant = 1.1421;
static const float pi = 3.14159;

void pitchShift(float *inSamples, float *outSamples);
void windowFrame(float * samples, float constant);
void shiftFrame(float *samples, float *buffer, float *currentFrame);
void fftFrame(float *frame, int16_t *index);
void ifftFrame(float *frame, float *phase, int16_t *index);
void frameMagnitudes(void);
void framePhases(float *phases);
void processFrame(float *phases, float *phaseCumul, float *prevPhase);
void mergeFrame(float *samples, float *fame);
void interpolateSamples(float *outSamples, float *processedFrame);
float angleModulo(float angle);
void clearBuffer(float *buffer, uint16_t size);



#endif /* INCLUDE_PITCHSHIFTERV2_H_ */
