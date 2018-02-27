/*
 * emif.c
 *
 *  Created on: Feb 3, 2018
 *      Author: simon
 */

#include "emif.h"


void emif_init(void)
{
    *((uint32_t *)CECTL3_ADDR) = CECTL3_CONFIG;

}
