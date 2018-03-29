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
        // SPI1
    TRISBbits.TRISB2 = 0;   // SS : Output
    TRISBbits.TRISB3 = 0;   // SCLK : Output
    TRISBbits.TRISB4 = 1;   // MISO : Input
    TRISBbits.TRISB5 = 0;   // MOSI : Output
    
        // SPI2    
    TRISDbits.TRISD1 = 1;   // MISO : Input
    TRISDbits.TRISD2 = 0;   // SCLK : Output
    TRISDbits.TRISD3 = 0;   // SS : Output
    TRISDbits.TRISD4 = 0;   // MOSI : Output
    
    // PPS-Lite configuration of GPIO pins to SPI peripherial
        // SPI1
    //RPOR12_13<7:4> = 0x4   --> RP13 = SDO1 (output)
    RPOR12_13 |= 0x40; 
    RPOR12_13 &= 0x4F;
    //RPOR6_7<7:4> = 0x3     --> RP7 = SCK1 (output)
    RPOR6_7 |= 0x30;
    RPOR6_7 &= 0x3F;
    //RPINR8_9<7:4> = 0x3    --> RP12 = SDI1 (input)
    RPINR8_9 |= 0x30;
    RPINR8_9 &= 0x3F;
    
    
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
        
    SPI[SPI_INDEX_2] = SPI2_config;
    
    SPI_config(&SPI[SPI_INDEX_2]);
}

void SPI_config(SPI_PERIPHERAL_S *peripheral)
{
    *((&SSP2ADD) + peripheral->index * 0xAD) &= 0xDF;   // SPEN = 0
    
    // SPI peripherial configuration
    
    *((&SSP2ADD) + peripheral->index * 0xC6) = SPI_CLOCKRATE(peripheral->clockrate);
    *((&SSP2CON1) + peripheral->index * 0xAD) = 0x0A;   // No collision, no overflow, low level clock idle state, Clock = Fosc/4 * (SSPxADD + 1)    
    *((&SSP2STAT) + peripheral->index * 0xAD) = 0;      // Data sampled in middle of output time, data transmitted on rising edge  
    
    *((&SSP2CON1) + peripheral->index * 0xAD) |= 0x20;  // SPEN = 1
    peripheral->state = SPI_STATE_IDLE;
}

void SPI_write(SPI_PERIPHERAL_S *peripheral, uint8_t size)
{
    *(&peripheral->port) = *(&peripheral->port) & (~(1 << peripheral->pin));
    for(uint8_t i = 0; i < size; i++)
    {
        *(&SSP1BUF-(peripheral->index * 0xC6)) = peripheral->txdata[i];
        while(!(*(&SSP1STAT-(peripheral->index * 0xAD)) & _SSP1STAT_BF_MASK ));
    }    
    *(&peripheral->port) = *(&peripheral->port) | (1 << peripheral->pin);
}

uint8_t SPI2_read(uint8_t *data)
{
    
}

SPI_PERIPHERAL_S *getSpiPeripheral(SPI_INDEX_E index)
{
    return &SPI[index];
}
    
//void spi_init(SPI_PERIPHERAL_S *peripheral)
//{
//    
//    *((&SSP1CON1)-(peripheral->index * SSP_MEMORY_JUMP)) = 0x31;    // Enable serial port, High level idle state, Clock = Fosc/16 
//    *((&SSP1STAT)-(peripheral->index * SSP_MEMORY_JUMP)) = 0x80;    // Input data sampled at end, Transmit occurs idle to active
//    *((&SSP1CON3)-(peripheral->index * 0x30)) = 0;  // Overwrite alert
//    
//    peripheral->state = SPI_STATE_IDLE;
//}

