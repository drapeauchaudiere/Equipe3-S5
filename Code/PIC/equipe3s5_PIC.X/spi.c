/*
 * File:   spi.c
 * Author: simon
 *
 * Created on March 20, 2018, 1:15 PM
 */

#include "spi.h"

SPI_PERIPHERAL_S SPI[SPI_NUMBER];

// Functions

void SPI_init(void)
{        
    // Pin configuration
    // SPI2    
    TRISDbits.TRISD1 = 1;   // MISO : Input
    TRISDbits.TRISD2 = 0;   // SCLK : Output
    TRISDbits.TRISD3 = 0;   // SS : Output
    TRISDbits.TRISD4 = 0;   // MOSI : Output
    
    // PPS-Lite configuration of GPIO pins to SPI peripherial
    // SPI2
    //RPOR24_25<3:0> = 0x4    --> RP24 = SDO2 (output)
    RPOR24_25 |= 0x04; 
    RPOR24_25 &= 0xF4;
    //RPOR22_23<3:0> = 0x5    --> RP22 = SCK2 (output)
    RPOR22_23 |= 0x05;
    RPOR22_23 &= 0xF5;
    //RPINR12_13<3:0> = 0x5    --> RP21 = SDI2 (input)
    RPINR12_13 |= 0x05;
    RPINR12_13 &= 0xF5;
        
//    SPI[SPI_INDEX_1] = SPI1_config;
    SPI[SPI_INDEX_2] = SPI2_config;
    
//    SPI_config(&SPI[SPI_INDEX_1]);
    SPI_config(&SPI[SPI_INDEX_2]);
    
    //IPR1bits.SSP1IP = 0;    // High priority
    IPR2bits.SSP2IP = 0;    // Low priority
    
}

void SPI_config(SPI_PERIPHERAL_S *peripheral)
{
    uint32_t clock = peripheral->clockrate;
    *((uint8_t *)(&SSP1CON1) - peripheral->index * 0xAD) &= 0xDF;   // SPEN = 0
    
    // SPI peripherial configuration
     
    *((&SSP1ADD) - peripheral->index * 0xC6) = (64000000/(clock*4000))-1;
    *((&SSP1CON1) - peripheral->index * 0xAD) = 0x0A;       // No collision, no overflow, low level clock idle state, Clock = Fosc/4 * (SSPxADD + 1)    
    *((&SSP1STAT) - peripheral->index * 0xAD) = 0x40;      // Data sampled in middle of output time, data transmitted on rising edge     
    *((&SSP2CON3) - peripheral->index * 0x30) = 0x40;       // Stop condition interrupt
    
    *((&SSP1CON1) - peripheral->index * 0xAD) |= 0x20;  // SPEN = 1
    *((uint16_t *)peripheral->port) = (*(&peripheral->port)) | (1 << peripheral->pin);
    peripheral->state = SPI_STATE_IDLE;
}

void SPI_write(SPI_PERIPHERAL_S *peripheral)
{
    while(!(peripheral->state == SPI_STATE_IDLE));
    *((uint16_t *)peripheral->port) = *((uint16_t *)peripheral->port) & (~(1 << peripheral->pin)); // Assert CS
    peripheral->state = SPI_STATE_BUSY;     // Reserve the peripheral
    peripheral->txcount = 1;                // Reset the transmit count
    *((uint16_t *)peripheral->intreg) = *((uint16_t *)peripheral->intreg) | (1 << peripheral->intindex);   // Enable interrupt
    *(&SSP1BUF-(peripheral->index * 0xC6)) = peripheral->txdata[0];     // Send the first byte       
}

SPI_PERIPHERAL_S *SPI_getPeripheral(SPI_INDEX_E index)
{
    return &SPI[index];
}
    
void SPI_isr(SPI_INDEX_E index)
{
    if(SPI[index].txsize > SPI[index].txcount)
    {
        *(&SSP1BUF-(index * 0xC6)) = SPI[index].txdata[SPI[index].txcount];
        SPI[index].txcount++;
    }
    else
    {
        *((uint16_t *)SPI[index].port) = *((uint16_t *)SPI[index].port) | (1 << SPI[index].pin);   // Deassert CS
        *((uint16_t *)SPI[index].intreg) = *((uint16_t *)SPI[index].intreg) & (~(1 << SPI[index].intindex));   // Disable interrupts
        SPI[index].state = SPI_STATE_IDLE;      // Free the peripheral
    }
}

