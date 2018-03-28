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


extern uint8_t recordingState;  // State-Machine variable
static const uint32_t max_record_size = 80000;      // 80 000 samples
static const uint8_t max_playback_gain = 10;        // Max playback volume gain
static const uint16_t output_buffer_offset = 4000;  // Output buffer outputs 4000 samples after the input buffer

// Main function declarations
void c_aic23_ISR(void);
void recordingStart(void);
void recordingStop(void);


#endif /* INCLUDE_AUDIO_DRIVER_H_ */
