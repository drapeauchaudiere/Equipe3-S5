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
    TRISE = 0x0;
    LATE = 0x0;
    TRISE = 0xFF;
}

uint8_t GPIO_getPort(uint16_t port)
{
    return *(uint16_t *)port;
}
