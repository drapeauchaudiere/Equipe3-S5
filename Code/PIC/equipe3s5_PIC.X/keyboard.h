/* 
 * File:   keyboard.h
 * Author: simon
 *
 * Created on April 15, 2018, 5:18 PM
 */

#ifndef KEYBOARD_H
#define	KEYBOARD_H

#include "definitions.h"
#include "effects.h"

typedef enum e_keyboard_keys
{
    KEY_0 = 0xBE,       // X2 Y4    1011 1110   
    KEY_1 = 0x77,       // X1 Y1    0111 0111
    KEY_2 = 0xB7,       // X2 Y1    1011 0111
    KEY_3 = 0xD7,       // X3 Y1    1101 0111
    KEY_4 = 0x7B,       // X1 Y2    0111 1011
    KEY_5 = 0xBB,       // X2 Y2    1011 1011
    KEY_6 = 0xDB,       // X3 Y2    1101 1011
    KEY_7 = 0x7D,       // X1 Y3    1110 1101
    KEY_8 = 0xBD,       // X2 Y3    1110 1101
    KEY_9 = 0xDD,       // X3 Y3    1110 1101
    KEY_A = 0x7E,       // X1 Y4    0111 1110
    KEY_B = 0xDE,       // X3 Y4    1101 1110
    KEY_C = 0xEE,       // X4 Y1    1110 1110
    KEY_D = 0xED,       // X4 Y2    1110 1101
    KEY_E = 0xEB,       // X4 Y3    1110 1011
    KEY_F = 0xE7,       // X4 Y4    1110 0111
    NO_KEY = 0xFF
            
}KEYBOARD_KEYS_E;

#define DECREMENT 0
#define INCREMENT 1

uint8_t saturateValue(uint8_t value, bool operation);
void checkKeys(EFFECT_CONFIG_U *effects, char *menu);
void setEffects(uint8_t keys, EFFECT_CONFIG_U *effects, char *menu);

#endif	/* KEYBOARD_H */

