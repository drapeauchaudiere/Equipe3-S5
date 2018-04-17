//****************************************************************************//
// File : Effects.c
//
// Functions :  void EFFECTS_init(void);
//              void EFFECTS_send(void);
//
// Includes : "effects.h"
//
//Simon Milhomme
//MPLab X v4.01 02/04/2018
//****************************************************************************//
#include "effects.h"

EFFECT_CONFIG_U effectConfiguration;     // Variable to old the current effect configuration
UART_PERIPHERAL_S *dskUart;         // Pointer to the SPI peripheral to the DSK 
uint8_t acknowledge;


EFFECT_CONFIG_U *EFFECTS_init(UART_PERIPHERAL_S *peripheral)
{
    dskUart = peripheral;
    dskUart->rxdata = &acknowledge;
    acknowledge = 0xFF;
    
    effectConfiguration.fields.outputIsEnabled = 0;
    effectConfiguration.fields.effectsAreEnabled = 0;      
    effectConfiguration.fields.octave = 0;  
    effectConfiguration.fields.gain = 5;            
    effectConfiguration.fields.lowGain = 5;         
    effectConfiguration.fields.midGain = 5;         
    effectConfiguration.fields.highGain = 5;  
    
    //EFFECTS_send(&effectConfiguration);
    return &effectConfiguration;
    
}

void EFFECTS_send(uint8_t index)
{
    uint8_t value = effectConfiguration.raw[index];
    UART_write((value & 0x0F) | (0xB0+index*0x10));

}