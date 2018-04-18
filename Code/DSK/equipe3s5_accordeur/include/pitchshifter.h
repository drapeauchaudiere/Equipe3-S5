/*
 * pitchshifter.h
 *
 *  Created on: Apr 3, 2018
 *      Author: Emile Laplante
 */

#ifndef PITCHSHIFTER_H_
#define PITCHSHIFTER_H_


#define FRAME_SIZE      1024  // 512 input samples
#define FRAME_HOP           (FRAME_SIZE / 8)  // The hop is the distance between two frames (samples that aren't overlapped iwth the previous)
#define FRAME_HOP_OUT       FRAME_HOP*2
#define pi  3.141592
#define HALF_PI pi/2
#define TWO_PI 2*pi
#define TWO_PI_INV 1/pi



void initPitchShifter();
void pitchShift(float newSample[], float outSample[]);
void fftFrame(float *frame, float *frameFFT);
void ifftFrame(float *frame, float *phase);
void processFrame(void);
float angleModulo(float angle);
void tableClear(float* table, int length);
void initPitchShifter();
void getAngleFrame(void);
void addNewFrame();
void outputSample(float *outSample);
void createNewframe( float newSample[]);
float atan2_approximation1(float y, float x);
void getComplex(float x, float *real, float *imag);

#endif /* INCLUDE_PITCHSHIFTER_H_ */
