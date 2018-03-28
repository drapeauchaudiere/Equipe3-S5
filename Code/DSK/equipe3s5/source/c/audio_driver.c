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
bool aic23AdcFlag = 0;
union {uint32_t uint; uint16_t channel[2];} AIC23_output;     // Holds left and right output samples
uint16_t AIC23_input;   // Holds the ADC sample

// VARIABLES GLOBALES POUR DSK
Uint32 fs=DSK6713_AIC23_FREQ_8KHZ;           // Fréquence d'échantillonnage
#define DSK6713_AIC23_INPUT_LINE 0x0011      // Définition de l'entrée LINE IN
Uint16 inputsource=DSK6713_AIC23_INPUT_LINE; // Selection de l'entrée LINE IN

/****************************************************************************
    Public functions :
****************************************************************************/
void AUDIO_adcRead(uint16_t *pSample)
{
    *pSample = (uint16_t) input_sample();
}

void AUDIO_dacWrite(uint16_t *pSample)
{
    AIC23_output.channel[LEFT] = *pSample;
    AIC23_output.channel[RIGHT] = *pSample;
}


/****************************************************************************
    Interrupt Service Routines :
****************************************************************************/

interrupt void c_aic23_ISR(void)
{
    aic23AdcFlag = 1;                 // Set the conversion ready flag

    if(effectConfiguration.fields.outputIsEnabled)
    {
        output_sample(AIC23_output.uint); // Output the modified sample
    }

    return;
}
