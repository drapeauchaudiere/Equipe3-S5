/*
 * File:   main.c
 * Author: simon milhomme
 *
 * Created on March 18, 2018, 11:37 AM
 */

#include <xc.h>
#include "definitions.h"
#include "gpio.h"
#include "lcd.h"
#include "spi.h"
#include "effects.h"
#include "config_bits.h"

void PIC_init(void);
void INT_init(void);
uint8_t saturateValue(uint8_t value, bool operation);
void checkKeys();

EFFECT_CONFIG_U *effects;
char *main_menu;

void main(void) {
    
    
    PIC_init();
    SPI_init();
    INT_init();
    GPIO_init();
    main_menu = LCD_init(SPI_getPeripheral(SPI_INDEX_2));
    effects = EFFECTS_init(SPI_getPeripheral(SPI_INDEX_1));   
    
    while(1)
    {   
        checkKeys();
        EFFECTS_send(effects);
        LCD_place_cursor_C0L1(0,1);
        LCD_write_menu(main_menu);
    }
    
    return;
}

void PIC_init(void)
{
    OSCCON3 = 0x0;
    ANCON1 = 0;
    ANCON2 = 0;
    ANCON3 = 0;
}

void INT_init(void)
{
    // Initialize interrupts
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
}

void checkKeys(void)
{
    uint8_t keys = GPIO_getPort(&PORTE);
    
    switch(keys)
    {
        // Volume control
        case KEY_1:
            effects->fields.gain = saturateValue(effects->fields.gain, DECREMENT);
            *((char *)main_menu+10) = (char)(0x30 + effects->fields.gain);
            break;
        case KEY_2:
            effects->fields.gain = 5;
            break;
        case KEY_3:
            effects->fields.gain = saturateValue(effects->fields.gain, INCREMENT);
            *((char *)main_menu+10) = (char) (0x30 + effects->fields.gain);
            break;
        
        // Low frequencies control    
        case KEY_4:
            effects->fields.lowGain --;
            break;
        case KEY_5:
            effects->fields.lowGain = 5;
            break;
        case KEY_6:
            effects->fields.lowGain ++;
            break;
            
        // Mid frequencies control    
        case KEY_7:
            effects->fields.midGain --;
            break;
        case KEY_8:
            effects->fields.midGain = 5;
            break;
        case KEY_9:
            effects->fields.midGain ++;
            break;
            
        // High frequencies control
        case KEY_A:
            effects->fields.highGain --;
            break;
        case KEY_0:
            effects->fields.highGain = 5;
            break;
        case KEY_B:
            effects->fields.highGain ++;
            break;  
            
        // Output/effects control    
        case KEY_F:
            effects->fields.effectsAreEnabled = !(effects->fields.effectsAreEnabled) ;
            break;
        case KEY_E:
            effects->fields.octave = !(effects->fields.octave) ;
            break;
        case KEY_D:
            effects->fields.outputIsEnabled = !(effects->fields.outputIsEnabled);
            break;
        case KEY_C:
            EFFECTS_init(SPI_getPeripheral(SPI_INDEX_1));
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

void interrupt low_priority low_isr(void)
{
    if(PIR1bits.SSP1IF & PIE1bits.SSP1IE)
    {
        SPI_isr(SPI_INDEX_1);
        PIR1bits.SSP1IF = 0;
    }
    
    if(PIR2bits.SSP2IF & PIE2bits.SSP2IE)
    {
        SPI_isr(SPI_INDEX_2);
        PIR2bits.SSP2IF = 0;
    }
}