/*
 * File:   gpio.c
 * Author: simon
 *
 * Created on March 18, 2018, 1:15 PM
 */


#include <xc.h>
#include "gpio.h"


KEYBOARD_KEYS_E getKeys(void)
{
    return (KEYBOARD_KEYS_E)LATE;
}
