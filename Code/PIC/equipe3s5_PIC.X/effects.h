/* 
 * File:   effects.h
 * Author: simon
 *
 * Created on April 2, 2018, 3:09 PM
 */

#ifndef EFFECTS_H
#define	EFFECTS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "definitions.h"
#include "spi.h"

typedef union u_effect_config
{
    uint32_t reg;
    uint8_t raw[4];
    struct
    {
        unsigned outputIsEnabled:1;
        unsigned effectsAreEnabled:1;
        unsigned gain:6;             // Global output volume gain
        unsigned lowGain:4;          // Low freq filter gain
        unsigned midGain:4;          // Mid freq filter gain
        unsigned highGain:4;         // High freq filter gain
        unsigned octave:4;           // Octave multiplier
        uint8_t  rsvd;
    } fields;

} EFFECT_CONFIG_U;


void EFFECTS_init(SPI_PERIPHERAL_S *peripheral);
void EFFECTS_send(void);

#ifdef	__cplusplus
}
#endif

#endif	/* EFFECTS_H */

