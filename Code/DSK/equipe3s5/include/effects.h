/*
 * effects.h
 *
 *  Created on: Feb 18, 2018
 *      Author: simon
 */

#ifndef EFFECTS_H_
#define EFFECTS_H_

#include "definitions.h"

/***************************************************************************
    Types declaration here :
***************************************************************************/

typedef union u_effect_config
{
        uint32_t reg;
        uint8_t raw[4];
        struct
        {
            unsigned outputIsEnabled:1;
            unsigned effectsAreEnable:1;
            unsigned gain:6;             // Global output volume gain
            unsigned lowGain:4;          // Low freq filter gain
            unsigned midGain:4;          // Mid freq filter gain
            unsigned highGain:4;         // High freq filter gain
            unsigned octave:4;           // Octave multiplier
            uint8_t  rsvd;
        } fields;

} EFFECT_CONFIG_U;



/***************************************************************************
    Global variables declaration :
***************************************************************************/
extern EFFECT_CONFIG_U effectConfiguration;

/***************************************************************************
    Public function prototypes :
***************************************************************************/

void EFFECTS_init(void);
void EFFECTS_equalizer(uint16_t *pSample);
void EFFECTS_harmonizer(uint16_t *pSample);



#endif /* EFFECTS_H_ */







