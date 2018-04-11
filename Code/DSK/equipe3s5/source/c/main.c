/*
 * main.c
 */

#include "spi_driver.h"
#include "audio_driver.h"
#include "effects.h"
#include <math.h>

/****************************************************************************
    Private macros and constants :
****************************************************************************/

#define INT_IF5 0x20                                // Interrupt 4 flag mask
#define SDRAM_CE0_ADDR  0x80000000                  // Start of the CE0 memory
static const uint32_t max_buffer_size = 160000;     // 16k samples / seconds * 10seconds

//#define SPI_DEBUG

/****************************************************************************
    Extern content declaration :
****************************************************************************/

extern far void vectors();  // Vecteurs d'interruption
extern bool spiRxFlag;      // SPI received data is ready
extern bool aic23AdcFlag;   // ADC data is ready
extern EFFECT_CONFIG_U effectConfiguration;


uint32_t getIFRASM(void);

/****************************************************************************
    Private function prototypes :
****************************************************************************/

static void initialization(void);

/****************************************************************************
    Main Program :
****************************************************************************/

int main(void)
{
    initialization();
    while(1)
    {
        if(getIFRASM() & INT_IF5)                   // If a new configuration has been sent,
        {
            effectConfiguration.reg = SPI_read();       // Go read the SPI buffer
            DSK6713_LED_toggle(0);                      // Toggle LED0 when a packet is read
            IRQ_clear(IRQ_EVT_RINT0);                   // Clear the interrupt flag
        }
    }
    return 0;
}


/****************************************************************************
    Private functions :
****************************************************************************/

void initialization(void)
{
    // INITIALISATION DU HARDWARE
    DSK6713_DIP_init(); // Initialisation de la communication du DSP avec les 4 DIP swichs
    DSK6713_LED_init(); // Initialisation des 4 LEDS (éteindre)

    AUDIO_init();
    SPI_init();
}


