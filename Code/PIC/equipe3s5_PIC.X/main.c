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
#include "config_bits.h"

void PIC_init(void);
void INT_init(void);

void main(void) {
    
    PIC_init();
    SPI_init();
    INT_init();
    LCD_init(SPI_getPeripheral(SPI_INDEX_2));
   
    
    while(1)
    {   
    EFFECTS_init(SPI_getPeripheral(SPI_INDEX_1));
        LCD_place_cursor_C0L1(0,1);
        LCD_write_menu(menu_main);
        //SPI_write(SPI_getPeripheral(SPI_INDEX_1),);
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
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
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