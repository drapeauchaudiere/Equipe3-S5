/*
 * Audio_driver.h
 *
 *  Created on: Mar 28, 2018
 *      Author: simon
 */

#ifndef INCLUDE_AUDIO_DRIVER_H_
#define INCLUDE_AUDIO_DRIVER_H_

#include "definitions.h"
#include "effects.h"
#include "filter.h"

#define BUFFER_SIZE 128
extern float pitchShifterBuffer_Input0[BUFFER_SIZE];
extern float pitchShifterBuffer_Input1[BUFFER_SIZE];
extern float pitchShifterBuffer_Out0[BUFFER_SIZE];
extern float pitchShifterBuffer_Out1[BUFFER_SIZE];
extern bool newTrameFlagBuf0;
extern bool newTrameFlagBuf1;

static const uint32_t max_record_size = 80000;      // 80 000 samples
static const uint8_t max_playback_gain = 10;        // Max playback volume gain
static const uint16_t output_buffer_offset = 4000;  // Output buffer outputs 4000 samples after the input buffer

// Main function declarations
void AUDIO_init(void);
void c_aic23_ISR(void);
void recordingStart(void);
void recordingStop(void);


#endif /* INCLUDE_AUDIO_DRIVER_H_ */
