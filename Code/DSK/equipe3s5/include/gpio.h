/*
 * gpio.h
 *
 *  Created on: Jan 30, 2018
 *      Author: simon
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "definitions.h"

// Definitions

#define CPLD_USER_REG    0x90080000
#define CPLD_LEDS_INIT  0xC    // LED 2 and 3 are set

typedef enum e_gpio_reg_addr
{
    GPEN_ADDR = 0x01B00000,
    GPDIR_ADDR = 0x01B00004,
    GPPOL_ADDR = 0x01B00024

} GPIO_REG_ADDR_E;

typedef enum e_cpld_leds
{
    CPLD_LED0 = 0x01,
    CPLD_LED1 = 0x02,
    CPLD_LED2 = 0x04,
    CPLD_LED3 = 0x08

} CPLD_LEDS_E;

typedef enum e_cpld_dip
{
    CPLD_DIP0 = 0x10,
    CPLD_DIP1 = 0x20,
    CPLD_DIP2 = 0x40,
    CPLD_DIP3 = 0x80

} CPLD_DIP_E;


// Public functions

void gpio_init(void);
void gpio_set(CPLD_LEDS_E pins, bool state);
void gpio_toggle(CPLD_LEDS_E pin);
bool gpio_get(CPLD_DIP_E pin);


// Interrupt service routines

void c_adcExt4ISR(void);


#endif /* INCLUDE_GPIO_H_ */
