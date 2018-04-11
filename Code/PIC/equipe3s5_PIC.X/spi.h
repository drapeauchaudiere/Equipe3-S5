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
    uint16_t clockrate;    
    
    uint16_t port;
    uint8_t pin;
    uint16_t intreg;
    uint8_t intindex;
    
    uint8_t *txdata;
    uint8_t txsize;
    uint8_t txcount;
    uint8_t *rxdata;
    uint8_t rxcount;
        
} SPI_PERIPHERAL_S;

#define SPI1_CLOCK      100 // kHz
#define SPI1_PORT       &LATB
#define SPI1_CS_PIN     2
#define SPI1_INT_REG    &PIE1
#define SPI1_INT_INDEX  3

SPI_PERIPHERAL_S SPI1_config = 
{
    SPI_INDEX_1,
    SPI_STATE_UNINIT,
    SPI1_CLOCK,
    SPI1_PORT,
    SPI1_CS_PIN,
    SPI1_INT_REG,
    SPI1_INT_INDEX            
};

#define SPI2_CLOCK      100     // kHz
#define SPI2_PORT       &LATD
#define SPI2_CS_PIN     3
#define SPI2_INT_REG    &PIE2
#define SPI2_INT_INDEX  6

SPI_PERIPHERAL_S SPI2_config =
{
    SPI_INDEX_2,
    SPI_STATE_UNINIT,
    SPI2_CLOCK,
    SPI2_PORT,
    SPI2_CS_PIN,
    SPI2_INT_REG,
    SPI2_INT_INDEX
};

#ifdef	__cplusplus
extern "C" {
#endif

    //void spi_init(SPI_INDEX_E peripheral);
    void SPI_init(void);
    void SPI_config(SPI_PERIPHERAL_S *peripheral);
    void SPI_write(SPI_PERIPHERAL_S *peripheral);
    uint8_t SPI2_read(uint8_t *data);
    SPI_PERIPHERAL_S *SPI_getPeripheral(SPI_INDEX_E index);
    void SPI_isr(SPI_INDEX_E index);

    
    // Macros
#define SPI_CLOCKRATE(clock)  (64000000/(clock*4000))-1 // clock = Fosc/(4*(SSPxADD+1))

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

