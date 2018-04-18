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
#include "uart.h"
#include "config_bits.h"

void PIC_init(void);
void INT_init(void);

bool uartFlag = 0;

void main(void) 
{        
    EFFECT_CONFIG_U *effects;
    char *main_menu;
    uint8_t timer = 0;
    uint8_t index = 0;
    
    PIC_init();
    SPI_init();
    INT_init();
    GPIO_init();
    main_menu = LCD_init(SPI_getPeripheral(SPI_INDEX_2));
    effects = EFFECTS_init(UART_getPeripheral(UART_INDEX_1));  
    UART_init(); 
    
    while(1)
    {   
        checkKeys(effects, main_menu);
        if(uartFlag)
        {
            index = UART_read();
            
            if((index&0xF0) == 0x90)
            {              
                EFFECTS_send((index&0x0F));
            }
            uartFlag = 0;            
            PIE1bits.RC1IE = 1;
        }
        LCD_write_menu(main_menu); 
        for(timer=0; timer<10; timer++)
        {
            __delay_ms(10);
        }
    }
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
    // UART Rx interrupt
    if(PIE1bits.RC1IE && PIR1bits.RC1IF)
    {
        if(!UART_errorCheck())
        {
            uartFlag = 1;
            PIE1bits.RC1IE = 0;
            PIR1bits.RC1IF = 0;
        }
    }
    
    // SPI2 interrupt
    if(PIR2bits.SSP2IF & PIE2bits.SSP2IE)
    {
        SPI_isr(SPI_INDEX_2);
        PIR2bits.SSP2IF = 0;
    }
}