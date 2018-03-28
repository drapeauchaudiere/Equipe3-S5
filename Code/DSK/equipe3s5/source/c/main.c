/*
 * main.c
 */

#include "spi_driver.h"
#include "audio_driver.h"
#include "effects.h"

/****************************************************************************
    Private macros and constants :
****************************************************************************/

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

    if(SPI_rrdy())                          // If a new configuration has been sent,
    {
        effectConfiguration.reg = SPI_read();       // Go read the SPI buffer
        DSK6713_LED_toggle(0);                  // Toggle LED0 when a packet is read
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

    SPI_init();
    //AUDIO_init();
}


