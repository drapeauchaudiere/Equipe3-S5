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

void pic_init(void);
void main(void) {
    
    SPI_init();
   LCD_init(getSpiPeripheral(SPI_INDEX_2));

    
    while(1)
    {
        LCD_place_cursor_C0L1(0,1);
       LCD_write_char('a');
    }
    
    return;
}

void pic_init(void)
{
    OSCCON3 = 0x0;
    ANCON1 = 0;
    ANCON2 = 0;
    ANCON3 = 0;
}