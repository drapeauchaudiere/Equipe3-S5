/*
 * pitchshifter.h
 *
 *  Created on: Apr 3, 2018
 *      Author: simon
 */

#ifndef PITCHSHIFTER_H_
#define PITCHSHIFTER_H_

#include "definitions.h"

#define NUMBER_SAMPLES      512        // 1024 input samples
#define WINDOW_SIZE         256         // Size of processing frames
#define FRAME_HOP           (WINDOW_SIZE / 8)    // 12.5% frame overlap
#define FRAME_OVERLAP       (WINDOW_SIZE - FRAME_HOP)
#define NUMBER_FRAMES       ((NUMBER_SAMPLES - WINDOW_SIZE) / (FRAME_HOP)+1)  // Number of frames overlapped in the samples
#define FRAME_TABLE_SIZE    (WINDOW_SIZE * NUMBER_FRAMES)     // Size of the table to hold the frames
#define MAX_OUTPUT_HOP      ((FRAME_HOP) * 2)   // 2^(10/12) -> 1.7818, step of 10
#define TIME_STRETCHED_SIZE (((NUMBER_FRAMES)*(MAX_OUTPUT_HOP))-(MAX_OUTPUT_HOP)+WINDOW_SIZE)
#define WINDOW_CONSTANT     2 //(sqrt((WINDOW_SIZE/(FRAME_HOP))/2))
static const float pi = 3.1416;

void pitchShift(uint16_t *samples, int8_t step);
void splitFrames(uint16_t *samples, uint16_t *frames);
void mergeFrames(uint16_t *samples, uint16_t *frames, uint16_t overlap);
void windowFrame(uint16_t * samples, uint16_t *frames, uint16_t overlapConstant);
void fftFrame(uint16_t *frame);
void ifftFrame(uint16_t *frame, uint16_t *phase);
void frameMagnitudes(void);
void framePhases(uint16_t *phases);
void processFrame(uint16_t *phases, uint16_t *phaseCumul, uint16_t overlap);
void interpolateSamples(uint16_t *samples, uint16_t *processedFrames, uint16_t alpha);
float findAlpha(int8_t step);




#endif /* INCLUDE_PITCHSHIFTER_H_ */
