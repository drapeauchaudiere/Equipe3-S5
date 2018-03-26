/*
 * effects.h
 *
 *  Created on: Feb 18, 2018
 *      Author: simon
 */

#ifndef EFFECTS_H_
#define EFFECTS_H_

#include "definitions.h"

typedef struct s_effect_config
{
    union
    {
        uint8_t raw[5];
        struct
        {
            uint8_t outputIsEnabled:1;
            uint8_t gain:7;             // Global output volume gain
            int8_t octave;              // Octave multiplier
            uint8_t lowGain;
            uint8_t midGain;
            uint8_t highGain;
        } fields;
    } configuration;

} EFFECT_CONFIG_S;



#endif /* EFFECTS_H_ */
