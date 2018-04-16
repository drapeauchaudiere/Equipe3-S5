/*
 * File:   gpio.c
 * Author: simon
 *
 * Created on March 18, 2018, 1:15 PM
 */


#include <xc.h>
#include "gpio.h"


void GPIO_init(void)
{
    TRISE = 0x0F;   // RE4-RE7 : output, RE0-RE3 : input
    PORTE = 0xF0;    // Outputs to 1
    PORTE = 0xF0;
}

