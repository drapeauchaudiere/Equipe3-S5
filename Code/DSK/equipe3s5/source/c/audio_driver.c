/*
 * audio_driver.c
 *
 *  Created on: Feb 18, 2018
 *      Author: simon
 */

#include "audio_driver.h"

/****************************************************************************
    Extern content declaration :
****************************************************************************/

extern far void vectors();                      // Vecteurs d'interruption
extern EFFECT_CONFIG_U effectConfiguration;     // Current effect configuration
/****************************************************************************
    Private global variables :
****************************************************************************/
bool bufferReady= 0;
uint16_t *currentBuffer;
float *outputBuffer;
union {uint32_t uint; uint16_t channel[2];} AIC23_output;     // Holds left and right output samples
uint16_t AIC23_input;   // Holds the ADC sample

// VARIABLES GLOBALES POUR DSK
Uint32 fs=DSK6713_AIC23_FREQ_44KHZ;           // Fréquence d'échantillonnage
#define DSK6713_AIC23_INPUT_LINE 0x0011      // Définition de l'entrée LINE IN
Uint16 inputsource=DSK6713_AIC23_INPUT_LINE; // Selection de l'entrée LINE IN

/****************************************************************************
    Public functions :
****************************************************************************/
void AUDIO_init(void)
{
    comm_intr();
    //CODEC_stop();
}

void AUDIO_adcRead(uint16_t *pSample)
{
    *pSample = (uint16_t) input_sample();
}

void AUDIO_dacWrite(float *pSample)
{
    outputBuffer = pSample;
    CODEC_start();
}




/****************************************************************************
    Interrupt Service Routines :
****************************************************************************/

interrupt void c_aic23_ISR(void)
{
    static uint16_t sampleCounter = 0;//, bufferCounter = 0;

   /* *((uint16_t *) SDRAM_CE0_ADDR + sampleCounter + bufferCounter*buffer_size) = (uint16_t) input_sample();

    sampleCounter++;

    if(sampleCounter >= buffer_size)
    {
        currentBuffer = ((uint16_t *)SDRAM_CE0_ADDR + bufferCounter*buffer_size);
        bufferReady = true;
        bufferCounter++;
        if(bufferCounter >= number_buffers)
        {
            bufferCounter = 0;
            outputBuffer = ((uint16_t *)SDRAM_CE0_ADDR + 1*buffer_size);
        }
        else if(bufferCounter == 1)
        {
            outputBuffer = ((uint16_t *)SDRAM_CE0_ADDR + 2*buffer_size);
        }
        else if(buffer)


    }

    if(effectConfiguration.fields.outputIsEnabled)
    {*/
        AIC23_output.channel[LEFT] = (uint16_t)outputBuffer[sampleCounter];
        AIC23_output.channel[RIGHT] = (uint16_t)outputBuffer[sampleCounter];
        output_sample(AIC23_output.uint); // Output the modified sample
        sampleCounter++;
        if(sampleCounter >= buffer_size)
        {
            CODEC_stop();
        }
   // }

    return;
}
