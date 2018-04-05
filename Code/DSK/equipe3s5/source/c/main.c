/*
 * main.c
 */

#include "spi_driver.h"
#include "audio_driver.h"
#include "effects.h"
#include "pitchshifter.h"
#include "buffer.dat"

/****************************************************************************
    Private macros and constants :
****************************************************************************/

#define SDRAM_CE0_ADDR  0x80000000                  // Start of the CE0 memory
static const uint32_t max_buffer_size = 160000;     // 16k samples / seconds * 10seconds

#define PITCH_DEBUG

/****************************************************************************
    Extern content declaration :
****************************************************************************/

extern far void vectors();  // Vecteurs d'interruption
extern bool spiRxFlag;      // SPI received data is ready
extern EFFECT_CONFIG_U effectConfiguration;
extern uint16_t *currentBuffer;
extern bool bufferReady;

/****************************************************************************
    Private function prototypes :
****************************************************************************/

static void initialization(void);

/****************************************************************************
    Main Program :
****************************************************************************/

int main(void)
{

    //initialization();
#ifdef PITCH_DEBUG
    /*float constant = FRAME_OVERLAP;
    constant = FRAME_HOP;
    constant = NUMBER_FRAMES;
    constant = FRAME_TABLE_SIZE;
    constant = MAX_OUTPUT_HOP;
    constant = TIME_STRETCHED_SIZE;
    constant = WINDOW_CONSTANT;*/
    //uint16_t buffer[512] = {1};
    pitchShift((uint16_t *)buffer,12);

#elif
    while(1)
    {
        if(bufferReady)
        {
            pitchShift(currentBuffer, 12);
        }

        if(SPI_rrdy())                          // If a new configuration has been sent,
        {
            effectConfiguration.reg = SPI_read();       // Go read the SPI buffer
            DSK6713_LED_toggle(0);                  // Toggle LED0 when a packet is read
        }

    }
#endif
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


