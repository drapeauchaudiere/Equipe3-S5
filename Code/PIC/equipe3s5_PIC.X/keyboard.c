/*
 * File:   spi.c
 * Author: simon
 *
 * Created on April 15, 2018, 1:15 PM
 */

#include "keyboard.h"

void checkKeys(EFFECT_CONFIG_U *effects, char *menu)
{
    uint8_t i, regVal;
    regVal = LATE;
    
    for(i=0; i<4; i++)
    {
        PORTE = regVal & (~(1 <<(7-i)));  
        if((PORTE&0x0F) != 0x0F)
        {
            __delay_ms(30);
            if((PORTE&0x0F) != 0x0F)
            {
                setEffects(PORTE,effects,menu); 
                PORTE = regVal;
                return;
            }
        }
    }
    
    PORTE = regVal;
}

void setEffects(uint8_t keys, EFFECT_CONFIG_U *effects, char *menu)
{
    switch(keys)
    {
        // Volume control
        case KEY_1:            
            effects->fields.gain = saturateValue((uint8_t)effects->fields.gain, DECREMENT);
            *((char *)menu+11) = (char)(0x30 + (uint8_t)effects->fields.gain);
            break;
        case KEY_2:
            effects->fields.gain = 5;
            *((char *)menu+11) = (char)(0x30 + (uint8_t)effects->fields.gain);
            break;
        case KEY_3:            
            effects->fields.gain = saturateValue((uint8_t)effects->fields.gain, INCREMENT);
            *((char *)menu+11) = (char)(0x30 + (uint8_t)effects->fields.gain);
            break;
            
        // Low frequency controls    
        case KEY_4:
            effects->fields.lowGain = saturateValue((uint8_t)effects->fields.lowGain, DECREMENT);
            *((char *)menu+31) = (char)(0x30 + (uint8_t)effects->fields.lowGain);
            break;
        case KEY_5:            
            effects->fields.lowGain = 5;
            *((char *)menu+31) = (char)(0x30 + (uint8_t)effects->fields.lowGain);
            break;
        case KEY_6:
            effects->fields.lowGain = saturateValue((uint8_t)effects->fields.lowGain, INCREMENT);
            *((char *)menu+31) = (char)(0x30 + (uint8_t)effects->fields.lowGain);
            break;
            
        // Mid frequency controls
        case KEY_7:            
            effects->fields.midGain = saturateValue((uint8_t)effects->fields.midGain, DECREMENT);
            *((char *)menu+51) = (char)(0x30 + (uint8_t)effects->fields.midGain);
            break;
        case KEY_8:
            effects->fields.midGain = 5;
            *((char *)menu+51) = (char)(0x30 + (uint8_t)effects->fields.midGain);
            break;
        case KEY_9:            
            effects->fields.midGain = saturateValue((uint8_t)effects->fields.midGain, INCREMENT);
            *((char *)menu+51) = (char)(0x30 + (uint8_t)effects->fields.midGain);
            break;
            
        // High frequency controls    
        case KEY_A:
            effects->fields.highGain = saturateValue((uint8_t)effects->fields.highGain, DECREMENT);
            *((char *)menu+71) = (char)(0x30 + (uint8_t)effects->fields.highGain);
            break;
        case KEY_0:            
            effects->fields.highGain = 5;
            *((char *)menu+71) = (char)(0x30 + (uint8_t)effects->fields.highGain);
            break;
        case KEY_B:
            effects->fields.highGain = saturateValue((uint8_t)effects->fields.highGain, INCREMENT);
            *((char *)menu+71) = (char)(0x30 + (uint8_t)effects->fields.highGain);
            break;
            
        // Output/effects controls    
        case KEY_F:
            if(effects->fields.octave)
            {
                effects->fields.octave = 0;
            }
            else
            {
                effects->fields.octave = 1;
            }
            *((char *)menu+19) = (char)(0x20 + (effects->fields.octave * 0x26));
            break;
        case KEY_E:            
            if(effects->fields.effectsAreEnabled)
            {
                effects->fields.effectsAreEnabled = 0;
            }
            else
            {
                effects->fields.effectsAreEnabled = 1;
            }
            *((char *)menu+39) = (char)(0x20 + (effects->fields.effectsAreEnabled * 0x25));
            break;
        case KEY_D:
            if(effects->fields.outputIsEnabled)
            {
                effects->fields.outputIsEnabled = 0;
            }
            else
            {
                effects->fields.outputIsEnabled = 1;
            }
            *((char *)menu+59) = (char)(0x20 + (effects->fields.outputIsEnabled * 0x24));
            break;
        default :
            break;            
    }
}

uint8_t saturateValue(uint8_t value, bool operation)
{
    if(operation)       // If incrementing
    {
        if(value < 9)       // And if the value is lower than 9
        {
            value ++;           // Increment value
        }
        return value;       // Return 9 or value++
    }
    else                // If decrementing
    {
        if(value)           // And the value is higher than 0
        {
            value --;           // Decrement value
        }        
        return value;       // Return 0 or value--
    }
}