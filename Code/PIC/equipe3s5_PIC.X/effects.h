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
#include "uart.h"

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


EFFECT_CONFIG_U *EFFECTS_init(UART_PERIPHERAL_S *peripheral);
void EFFECTS_send(uint8_t index);

#ifdef	__cplusplus
}
#endif

#endif	/* EFFECTS_H */

