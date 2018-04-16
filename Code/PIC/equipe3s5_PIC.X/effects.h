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
        bool outputIsEnabled:1;
        bool effectsAreEnabled:1;
        uint8_t gain:6;             // Global output volume gain
        uint8_t lowGain:4;          // Low freq filter gain
        uint8_t midGain:4;          // Mid freq filter gain
        uint8_t highGain:4;         // High freq filter gain
        bool octave:1;              // Octave multiplier
        uint8_t rsvd3:3;
        uint8_t  rsvd;
    } fields;

} EFFECT_CONFIG_U;


EFFECT_CONFIG_U *EFFECTS_init(SPI_PERIPHERAL_S *peripheral);
void EFFECTS_send(EFFECT_CONFIG_U *config);

#ifdef	__cplusplus
}
#endif

#endif	/* EFFECTS_H */

