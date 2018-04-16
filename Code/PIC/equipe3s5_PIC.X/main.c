/*
 * File:   main.c
 * Author: simon milhomme
 *
 * Created on March 18, 2018, 11:37 AM
 */

#include "definitions.h"
#include "gpio.h"
#include "lcd.h"
#include "spi.h"
#include "effects.h"
#include "keyboard.h"
#include "config_bits.h"

void PIC_init(void);
void INT_init(void);

void main(void) 
{        
    EFFECT_CONFIG_U *effects;
    char *main_menu;
    uint8_t timer;
    
    PIC_init();
    SPI_init();
    INT_init();
    GPIO_init();
<<<<<<< HEAD
    //main_menu = LCD_init(SPI_getPeripheral(SPI_INDEX_2));
=======
    main_menu = LCD_init(SPI_getPeripheral(SPI_INDEX_2));
>>>>>>> 12dff8c7dbab35ac12ef38b2a58c3f731f24a2d5
    effects = EFFECTS_init(SPI_getPeripheral(SPI_INDEX_1));   
    
    while(1)
    {   
<<<<<<< HEAD
        //checkKeys(effects, main_menu);        
        EFFECTS_send(effects);
        //LCD_place_cursor_C0L1(0,1);
        //LCD_write_menu(main_menu); 
        /*for(timer=0; timer<10; timer++)
        {
            __delay_ms(10);
        }*/
=======
        checkKeys(effects, main_menu);
        EFFECTS_send(effects);
        //LCD_place_cursor_C0L1(0,1);
        LCD_write_menu(main_menu); 
        for(timer=0; timer<10; timer++)
        {
            __delay_ms(10);
        }
>>>>>>> 12dff8c7dbab35ac12ef38b2a58c3f731f24a2d5
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