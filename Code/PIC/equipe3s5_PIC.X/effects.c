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
uint8_t acknowledge;


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


EFFECT_CONFIG_U *EFFECTS_init(SPI_PERIPHERAL_S *peripheral)
{
    dskSpi = peripheral;
    dskSpi->rxdata = &acknowledge;
    acknowledge = 0xFF;
    
    effectConfiguration.fields.outputIsEnabled = 0;
    effectConfiguration.fields.effectsAreEnabled = 0;
    effectConfiguration.fields.gain = 7;            
    effectConfiguration.fields.lowGain = 5;         
    effectConfiguration.fields.midGain = 5;         
    effectConfiguration.fields.highGain = 6;        
    effectConfiguration.fields.octave = 0;          
    effectConfiguration.fields.rsvd = 0;
    
    EFFECTS_send(&effectConfiguration);
    return &effectConfiguration;
    
}

void EFFECTS_send(EFFECT_CONFIG_U *config)
{
    static index = 0;
    if(dskSpi->rxdata[0] == 0xFF)  // Check if the DSK sent an acknowledge
    {
        index++;                // Increment the table index
        if(index == 3)
        {
            index = 0;
        }
        dskSpi->rxdata[0] = 0;     // Clear the rx buffer
        dskSpi->rxcount = 0;
            
        dskSpi->txdata = (uint8_t *)config->raw + index;
        dskSpi->txsize = 1;
        SPI_write(dskSpi);
    }
}