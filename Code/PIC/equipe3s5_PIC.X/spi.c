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
        // SPI2
    TRISDbits.TRISD3 = 0;
    TRISDbits.TRISD1 = 1;
    TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD4 = 0;
    
    // PPS-Lite configuration of GPIO pins to SPI peripherial
    //RPOR24_25<3:0> = 0x4    --> RP24 = SDO2 (output)
    RPOR24_25 |= 0x04; 
    RPOR24_25 &= 0xF4;
    //RPOR22_23<3:0> = 0x5    --> RP22 = SCK2 (output)
    RPOR22_23 |= 0x05;
    RPOR22_23 &= 0xF5;
    //RPINR12_13<7:4> = 0x5    --> RP21 = SDI2 (input)
    RPINR12_13 |= 0x50;
    RPINR12_13 &= 0x5F;
        
    SPI[SPI_INDEX_2] = SPI2_config;
    
    SPI_config(&SPI[SPI_INDEX_2]);
}

void SPI_config(SPI_PERIPHERAL_S *peripheral)
{
    SSP2CON1bits.SSPEN = 0;
    
    // SPI peripherial configuration
    
    SSP2ADD = 161;
    SSP2CON1bits.SSPM = 0x0A;    
    SSP2STATbits.CKE = 1;    
    SSP2STATbits.SMP = 0;    
    SSP2CON1bits.CKP = 0;
    
    SSP2CON1bits.SSPEN = 1;
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

