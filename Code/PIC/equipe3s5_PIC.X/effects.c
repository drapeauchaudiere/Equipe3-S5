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
SPI_PERIPHERAL_S *dskSpi;         // Pointer to the SPI peripheral to the DSK 


//EFFECT_CONFIG_U initConfig =
//{
//    .fields =
//    {
//        .outputIsEnabled = 0,
//        .effectsAreEnabled = 0,
//        .gain = 5,            
//        .lowGain = 5,         
//        .midGain = 5,         
//        .highGain = 5,        
//        .octave = 0,          
//        .rsvd = 0
//    }
//            
//};


void EFFECTS_init(SPI_PERIPHERAL_S *peripheral)
{
    dskSpi = peripheral;
    
    effectConfiguration.fields.outputIsEnabled = 0;
    effectConfiguration.fields.effectsAreEnabled = 0;
    effectConfiguration.fields.gain = 5;            
    effectConfiguration.fields.lowGain = 5;         
    effectConfiguration.fields.midGain = 5;         
    effectConfiguration.fields.highGain = 5;        
    effectConfiguration.fields.octave = 0;          
    effectConfiguration.fields.rsvd = 0;
    
    EFFECTS_send();
    
}

void EFFECTS_send(void)
{
    dskSpi->txdata = effectConfiguration.raw;
    SPI_write(dskSpi,3);
}