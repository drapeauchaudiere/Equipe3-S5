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

// PitchShifter variables
float pitchShiftInputBuffer0[128];
float pitchShiftInputBuffer1[128];
float pitchShiftOutBuffer0[128];
float pitchShiftOutBuffer1[128];

bool newTrameFlagBuf0 = 0;
bool newTrameFlagBuf1= 0;

// Filter variables
float echFiltrer700;
float echFiltrer1000;
float echFiltrer5000;
float echFiltrer7000;
float echFiltrer1000_5000;
float output;
short gainPbas = 1;
short gainPhaut = 1;
short gainPbande = 1;

// VARIABLES GLOBALES POUR DSK
Uint32 fs=DSK6713_AIC23_FREQ_44KHZ;           // Fréquence d'échantillonnage
#define DSK6713_AIC23_INPUT_LINE 0x0011      // Définition de l'entrée LINE IN
Uint16 inputsource=DSK6713_AIC23_INPUT_LINE; // Selection de l'entrée LINE IN

/****************************************************************************
    Public functions :
****************************************************************************/
void AUDIO_init(void)
{
    /*tableClear(pitchShiftInputBuffer0,BUFFER_SIZE);
    tableClear(pitchShiftInputBuffer1,BUFFER_SIZE);
    tableClear(pitchShiftOutBuffer0,BUFFER_SIZE);
    tableClear(pitchShiftOutBuffer0,BUFFER_SIZE);
    */
    comm_intr(fs,inputsource);
}

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

    short echLineIn;     // Ampliutde de l'échantillon provenant de l'entrée LINE IN

    // Capture de l'échantillon provenant de l'entrée "IN"
    echLineIn = input_sample();

    //Filtrage de l'équalizer
    echFiltrer700 = effectConfiguration.fields.lowGain*(FPB_700(echLineIn));
    echFiltrer7000 = effectConfiguration.fields.highGain*(FPH_7000(echLineIn));
    echFiltrer1000_5000 = effectConfiguration.fields.midGain*(FPB_1000_5000(echLineIn));
    output = echFiltrer700 + echFiltrer7000 + echFiltrer1000_5000;

    //sortie de l'équalizer dans canal gauche
    AIC23_output.channel[LEFT] = (short)(output)*effectConfiguration.fields.gain;
    //Échantillon IN dans la sortie
    AIC23_output.channel[RIGHT] = (short)4*echLineIn*effectConfiguration.fields.gain;


    // Sortir les deux signaux sur "HP/OUT"
    output_sample(AIC23_output.uint);

}

/*interrupt void c_aic23_ISR(void)
{
    static int index = 0;
    static int playBuffState = 0;
    int sample = 0;
    int outputAudio = 0;
    sample  = input_left_sample();

    if (playBuffState == 0)
    {
        if (index < BUFFER_SIZE)
        {
            pitchShiftInputBuffer0[index] = input_left_sample();
            sample = 0.01*pitchShiftOutBuffer0[index];
            index ++;
        }
        else
        {
            index = 0;
            newTrameFlagBuf0 = 1;
            playBuffState = 1;
        }

    }

    if (playBuffState == 1)
    {
        if (index < BUFFER_SIZE)
        {
            pitchShiftInputBuffer0[index] = input_left_sample();
            sample = 0.01*pitchShiftOutBuffer1[index];
            index ++;
        }
        else
        {
            index = 0;
            newTrameFlagBuf1 = 1;
            playBuffState = 0;
        }

    }

    outputAudio = sample << 16;                   // Pour avoir un int
    outputAudio = outputAudio | sample;
    output_sample(0.1*sample);


}*/
