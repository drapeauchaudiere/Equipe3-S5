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

void main(void)
{
    uint8_t value = 0, index = 0;
    bool txReady = 1;
    initialization();
    while(1)
    {
        SPI_write(0xA0 | (index));
        if(SPI_rrdy())                   // Data write has been sent and received by MAX3111
        {
            // Read config pour voir si bit 15 (Read ready) est 1
            value = SPI_read();        // Go read the SPI buffer
            if((value&0xF0) ==  (0xB0+index*0x10))
            {
                effectConfiguration.raw[index] = (value & 0x0F);
                index++;                                            // Increment table index
                if(index == 5)
                {
                    index = 0;
                }
                txReady = 1;
            }
            DSK6713_LED_toggle(0);
        }
    }
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


