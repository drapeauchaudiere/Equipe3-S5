/* 
 * File:   spi.h
 * Author: simon
 *
 * Created on March 20, 2018, 3:44 PM
 */

#ifndef SPI_H
#define	SPI_H

#include "definitions.h"
#include "gpio.h"

#define SSP_MEMORY_JUMP 0xAD
typedef enum e_spi_index
{
    SPI_INDEX_1 = 0x0,
    SPI_INDEX_2,
    SPI_NUMBER
            
}SPI_INDEX_E;

typedef enum e_spi_state
{
    SPI_STATE_UNINIT = 0x0,
    SPI_STATE_IDLE,
    SPI_STATE_BUSY,
    SPI_STATE_ERROR
            
}SPI_STATE_E;

typedef struct s_spi_peripheral
{
    SPI_INDEX_E index;
    SPI_STATE_E state;
    uint8_t baudrate;    
    
    uint8_t port;
    uint8_t pin;
    
    uint8_t *txdata;
    uint8_t *rxdata;
        
} SPI_PERIPHERAL_S;

SPI_PERIPHERAL_S SPI1_config = 
{
    SPI_INDEX_1,
    SPI_STATE_UNINIT
};

#define SPI2_PORT   &LATD
#define SPI2_CS_PIN    3

SPI_PERIPHERAL_S SPI2_config =
{
    SPI_INDEX_2,
    SPI_STATE_UNINIT,
    SPI2_PORT,
    SPI2_CS_PIN
};



 #define SPI2_CS         LATDbits.LATD3        // CSN output pin, PORTD pin 3
 #define LED            LATCbits.LATC0        // LED output pin, PORTC pin 0 

#ifdef	__cplusplus
extern "C" {
#endif

    //void spi_init(SPI_INDEX_E peripheral);
    void SPI_init(void);
    void SPI_config(SPI_PERIPHERAL_S *peripheral);
    void SPI_write(SPI_PERIPHERAL_S *peripheral, uint8_t size);
    uint8_t SPI2_read(uint8_t *data);
    SPI_PERIPHERAL_S *getSpiPeripheral(SPI_INDEX_E index);
//    void spi_wrtie(SPI_INDEX_E peripheral, uint8_t *data);
//    void spi_read(SPI_INDEX_E peripheral, uint8_t *data);

    
    // Macros
#define SPI_BAUDRATE  // Fosc/(4*(SSPxADD+1))

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

