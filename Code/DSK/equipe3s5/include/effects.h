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
        uint8_t raw[2];
        struct
        {
            uint8_t outputIsEnabled:1;
            uint8_t gain:7;             // Global output volume gain
            int8_t octave;              // Octave multiplier
        } fields;
    } global;

    union
    {
      uint8_t raw[6];
      struct
      {
          uint16_t lowGain;
          uint16_t midGain;
          uint16_t highGain;
      } fields;
    } equalizer;

} EFFECT_CONFIG;



#endif /* EFFECTS_H_ */
