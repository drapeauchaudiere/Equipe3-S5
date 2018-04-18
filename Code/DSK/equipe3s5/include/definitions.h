/*
 * definitions.h
 *
 *  Created on: Jan 29, 2018
 *   Author: simon
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include <csl_chip.h>
#include <csl_gpio.h>
#include <csl_irq.h>
#include <csl_mcbsp.h>

#include <stdint.h>     // get standardized integer types
#include <stdio.h>      // get standard I/O functions (as printf)
#include <stddef.h>     // get null and size_t definition
#include <stdbool.h>    // get boolean, true and false definition
#include <math.h>

#include "dsk6713.h"
#include "dsk6713_led.h"
#include "dsk6713_dip.h"
#include "C6713Helper_UdeS.h"
#include "DSK6713_AIC23.h" //codec support

// Definitions

#define SDRAM_CE0_ADDR  0x80000000  // Start of the CE0 memory

#define DSK6713_AIC23_INPUT_MIC 0x0015
#define DSK6713_AIC23_INPUT_LINE 0x0011


#define LEFT    0
#define RIGHT   1



#endif /* INCLUDE_DEFINITIONS_H_ */
