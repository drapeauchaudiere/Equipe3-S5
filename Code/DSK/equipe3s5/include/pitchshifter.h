/*
 * pitchshifter.h
 *
 *  Created on: Apr 3, 2018
 *      Author: simon
 */

#ifndef PITCHSHIFTER_H_
#define PITCHSHIFTER_H_

#include "definitions.h"

#define NUMBER_SAMPLES      512  // 512 input samples
#define WINDOW_SIZE         256  // Size of processing frames
#define FRAME_HOP           (WINDOW_SIZE / 8)    // The hop is the distance between two frames (samples that aren't overlapped iwth the previous)
#define FRAME_OVERLAP       (WINDOW_SIZE - FRAME_HOP)   // 87.5% overlap
#define NUMBER_FRAMES       ((NUMBER_SAMPLES - WINDOW_SIZE) / (FRAME_HOP)+1)  // Number of frames overlapped in the samples
#define FRAME_TABLE_SIZE    (WINDOW_SIZE * NUMBER_FRAMES)     // Size of the table to hold the frames
#define MAX_OUTPUT_HOP      ((FRAME_HOP) * 2)   // 2^(10/12) -> 1.7818, step of 10, max is 12
#define TIME_STRETCHED_SIZE (((NUMBER_FRAMES)*(MAX_OUTPUT_HOP))-(MAX_OUTPUT_HOP)+WINDOW_SIZE)   // Maximum size of the stretched samples
#define WINDOW_CONSTANT     2 //(sqrt((WINDOW_SIZE/(FRAME_HOP))/2))
static const float pi = 3.1416;

void pitchShift(float *samples, int8_t step);
void splitFrames(float *samples, float *frames);
void mergeFrames(float *samples, float *frames, uint16_t overlap);
void windowFrame(uint16_t * samples, uint16_t *frames, uint16_t overlapConstant);
void fftFrame(float *frame, int16_t *index);
void ifftFrame(float *frame, float *phase,int16_t *index);
void frameMagnitudes(void);
void framePhases(float *phases);
void processFrame(float *phases, float *phaseCumul, uint16_t overlap);
void interpolateSamples(float *samples, float *processedFrames, uint16_t step);
float findAlpha(int8_t step);
float angleModulo(float angle);
void tableClear(float* table, uint16_t length);


#endif /* INCLUDE_PITCHSHIFTER_H_ */
