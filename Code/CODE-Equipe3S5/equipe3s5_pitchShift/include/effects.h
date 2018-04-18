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
    uint8_t raw[5];
    struct
    {
        bool outputIsEnabled:1;
        bool effectsAreEnabled:1;
        bool octave:1;
        bool rsvd:1;
        uint8_t code1:4;
        uint8_t gain:4;             // Global output volume gain
        uint8_t code2:4;
        uint8_t lowGain:4;          // Low freq filter gain
        uint8_t code3:4;
        uint8_t midGain:4;          // Mid freq filter gain
        uint8_t code4:4;
        uint8_t highGain:4;         // High freq filter gain
        uint8_t code5:4;
    } fields;

} EFFECT_CONFIG_U;

/*unsigned octave:4;           // Octave multiplier
unsigned highGain:4;         // High freq filter gain
unsigned midGain:4;          // Mid freq filter gain
unsigned lowGain:4;          // Low freq filter gain
unsigned gain:6;             // Global output volume gain
unsigned effectsAreEnabled:1;
unsigned outputIsEnabled:1;
uint8_t  rsvd;*/

/*unsigned outputIsEnabled:1;
unsigned effectsAreEnable:1;
unsigned gain:6;             // Global output volume gain
unsigned lowGain:4;          // Low freq filter gain
unsigned midGain:4;          // Mid freq filter gain
unsigned highGain:4;         // High freq filter gain
unsigned octave:4;           // Octave multiplier
uint8_t  rsvd;*/

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







