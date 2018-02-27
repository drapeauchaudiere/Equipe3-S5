/*
 * definitions.h
 *
 *  Created on: Jan 29, 2018
 *   Author: simon
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "DSK6713_AIC23.h" //codec support

// Definitions

#define SDRAM_CE0_ADDR  0x80000000  // Start of the CE0 memory

#define DSK6713_AIC23_INPUT_MIC 0x0015
#define DSK6713_AIC23_INPUT_LINE 0x0011

extern uint8_t recordingState;  // State-Machine variable
static const uint32_t max_record_size = 80000;      // 80 000 samples
static const uint8_t max_playback_gain = 10;        // Max playback volume gain
static const uint16_t output_buffer_offset = 4000;  // Output buffer outputs 4000 samples after the input buffer

typedef enum e_control_reg_addr
{
    ICR_ADDR = 0x03,
    IFR_ADDR = 0x02, // Write
    IER_ADDR = 0x04,
    ISR_ADDR = 0x02  // Read

} CONTROL_REG_ADDR_E;


typedef enum e_interrupt_masks
{
    INT4_MASK = 0x10,
    INT5_MASK = 0x20,
    INT6_MASK = 0x40,
    INT7_MASK = 0x80,
    INT8_MASK = 0x100,
    INT9_MASK = 0x200,
    INT10_MASK = 0x400,
    INT11_MASK = 0x800,
    INT12_MASK = 0x1000,
    INT13_MASK = 0x2000,
    INT14_MASK = 0x4000,
    INT15_MASK = 0x8000

} INTERRUPT_MASKS_E;

// Main function declarations
void c_aic23_ISR(void);
void recordingStart(void);
void recordingStop(void);

// Assembly function declarations
void enableInterrupts(void);
void enableInterrupt11(void);
void disableInterrupt11(void);


#endif /* INCLUDE_DEFINITIONS_H_ */
