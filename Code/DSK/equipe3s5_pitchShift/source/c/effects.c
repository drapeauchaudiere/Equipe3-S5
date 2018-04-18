/*
 * effects.c
 *
 *  Created on: Feb 18, 2018
 *      Author: simon
 */

#include "effects.h"

/****************************************************************************
    Private global variables :
****************************************************************************/
EFFECT_CONFIG_U effectConfiguration;


/****************************************************************************
    Public functions :
****************************************************************************/

void EFFECTS_init(void)
{
    effectConfiguration.fields.outputIsEnabled = 0;
    effectConfiguration.fields.effectsAreEnabled = 0;
    effectConfiguration.fields.octave = 0;
    effectConfiguration.fields.gain = 5;
    effectConfiguration.fields.lowGain = 5;
    effectConfiguration.fields.midGain = 5;
    effectConfiguration.fields.highGain = 5;
}

