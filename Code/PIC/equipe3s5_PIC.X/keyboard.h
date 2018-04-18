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

//#define NEG_LOGIC

#ifdef NEG_LOGIC
typedef enum e_keyboard_keys
{
    KEY_0 = 0x7D,       // X2 Y4    1011 1110   
    KEY_1 = 0xEE,       // X1 Y1    0111 0111
    KEY_2 = 0xED,       // X2 Y1    1011 0111
    KEY_3 = 0xEB,       // X3 Y1    1101 0111
    KEY_4 = 0xDE,       // X1 Y2    0111 1011
    KEY_5 = 0xDD,       // X2 Y2    1011 1011
    KEY_6 = 0xDB,       // X3 Y2    1101 1011
    KEY_7 = 0xBE,       // X1 Y3    1110 1101
    KEY_8 = 0xBD,       // X2 Y3    1110 1101
    KEY_9 = 0xBB,       // X3 Y3    1110 1101
    KEY_A = 0x7E,       // X1 Y4    0111 1110
    KEY_B = 0x7B,       // X3 Y4    1101 1110
    KEY_C = 0x77,       // X4 Y1    1110 1110
    KEY_D = 0xB7,       // X4 Y2    1110 1101
    KEY_E = 0xD7,       // X4 Y3    1110 1011
    KEY_F = 0xE7,       // X4 Y4    1110 0111
    NO_KEY = 0x0F
            
}KEYBOARD_KEYS_E;

#else

typedef enum e_keyboard_keys
{
    KEY_0 = 0x82,       // X2 Y4    1011 1110   
    KEY_1 = 0x11,       // X1 Y1    0111 0111
    KEY_2 = 0x12,       // X2 Y1    1011 0111
    KEY_3 = 0x14,       // X3 Y1    1101 0111
    KEY_4 = 0x21,       // X1 Y2    0111 1011
    KEY_5 = 0x22,       // X2 Y2    1011 1011
    KEY_6 = 0x24,       // X3 Y2    1101 1011
    KEY_7 = 0x41,       // X1 Y3    1110 1101
    KEY_8 = 0x42,       // X2 Y3    1110 1101
    KEY_9 = 0x44,       // X3 Y3    1110 1101
    KEY_A = 0x81,       // X1 Y4    0111 1110
    KEY_B = 0x84,       // X3 Y4    1101 1110
    KEY_C = 0x88,       // X4 Y1    1110 1110
    KEY_D = 0x48,       // X4 Y2    1110 1101
    KEY_E = 0x28,       // X4 Y3    1110 1011
    KEY_F = 0x18,       // X4 Y4    1110 0111
    NO_KEY = 0x00
            
}KEYBOARD_KEYS_E;

#endif

#define DECREMENT 0
#define INCREMENT 1

uint8_t saturateValue(uint8_t value, bool operation);
void checkKeys(EFFECT_CONFIG_U *effects, char *menu);
void setEffects(uint8_t keys, EFFECT_CONFIG_U *effects, char *menu);

#endif	/* KEYBOARD_H */

