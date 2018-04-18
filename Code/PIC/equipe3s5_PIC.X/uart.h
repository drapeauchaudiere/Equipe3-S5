/* 
 * File:   uart.h
 * Author: royn3109
 *
 * Created on 16 avril 2018, 14:37
 */

#ifndef UART_H
#define	UART_H

#include "definitions.h"

typedef enum e_uart_index
{
    UART_INDEX_1 = 0x0,
    UART_INDEX_2,
    UART_INDEX_3,
    UART_INDEX_4,
    UART_NUMBER
            
} UART_INDEX_E;

typedef struct s_uart_peripheral
{
    UART_INDEX_E index;
    uint8_t *txdata;
    uint8_t txsize;
    uint8_t txcount;
    uint8_t *rxdata;
    uint8_t rxcount;    
    
} UART_PERIPHERAL_S;


void UART_init(void);
void UART_config(void);
void UART_write(unsigned char data);
uint8_t UART_read(void);
bool UART_errorCheck(void);
UART_PERIPHERAL_S *UART_getPeripheral(UART_INDEX_E index);


#endif	/* UART_H */

