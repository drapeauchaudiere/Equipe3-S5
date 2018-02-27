/*
 * gpio.c
 *
 *  Created on: Jan 30, 2018
 *      Author: simon
 */


#include "gpio.h"
#include "timers.h"


// Public functions

void gpio_init(void)
{
    *(uint32_t *)CPLD_USER_REG = CPLD_LEDS_INIT;

    *(uint32_t *)GPEN_ADDR |= 0x10;
    *(uint32_t *)GPDIR_ADDR &= 0xFFFFFFEF;
    *(uint32_t *)GPPOL_ADDR |= 0x10;

}

void gpio_set(CPLD_LEDS_E pins, bool state)
{
    if(state)
    {
        *(uint32_t *)CPLD_USER_REG |= pins;
    }
    else
    {
        *(uint32_t *)CPLD_USER_REG &= ~pins;
    }
}

void gpio_toggle(CPLD_LEDS_E pin)
{
    gpio_set(pin,!(*(uint32_t *)CPLD_USER_REG&pin));
}

bool gpio_get(CPLD_DIP_E pin)
{
    return (*((uint32_t *)CPLD_USER_REG) & pin);
}




