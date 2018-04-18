/*
 * main.c
 */


#include "pitchshifter.h"
#include "buffer.dat"
#include "C6713Helper_UdeS.h"
#include <math.h>
#include <cos2000Hz.dat>
#include <cos10000HZ.dat>
#include <testWav.dat>
#include "dsk6713_led.h"
#include <stdint.h>
#include "HANN_512.dat"
#define size 4096


//static float trame[4096];
//static float signal[size];



#define DSK6713_AIC23_INPUT_LINE 0x0011
#define DSK6713_AIC23_INPUT_MIC 0x0015
#define BUFFER_SIZE 128
#define TUNER_BUFFER_SIZE 512
const float F0_NOMINAL[6] = // Fréquences fondamentales de chacune des cordes
     { 82.407,110.000,146.832,195.998,246.942,329.628 };

void correlateASM(int32_t *, int16_t *, int16_t *, int32_t);
float getF0(int xx[], float fDesirer);

Uint16 inputsource = DSK6713_AIC23_INPUT_MIC; //= DSK6713_AIC23_INPUT_LINE;
Uint32 fs = DSK6713_AIC23_FREQ_16KHZ;          //= DSK6713_AIC23_FREQ_48KHZ;   //set sampling rate

interrupt void c_int11(void);

float pitchShiftInputBuffer[128];
float pitchShiftOutBuffer0[128];
float pitchShiftOutBuffer1[128];
short tunerBuffer [TUNER_BUFFER_SIZE];

int newTrameFlagBuf0 = 0;
int newTrameFlagBuf1= 0;
int DSPstate = 0;
float f0 = 0;
float err=0;
int newTunerBuffer = 0;
float Gage = 0.05; // Gage de fréquence en Hz autour de la fonction visée, doit être proportionnelle à la fréquence chercher
//const float FREQ_REF = F0_NOMINAL[4];

#define FS 16000

int16_t signal[TUNER_BUFFER_SIZE];
int32_t resultASM[TUNER_BUFFER_SIZE*2-1];

int main(void)
{
    short n= 0;
    tableClear(pitchShiftInputBuffer,BUFFER_SIZE);
    tableClear(pitchShiftOutBuffer0,BUFFER_SIZE);
    tableClear(pitchShiftOutBuffer0,BUFFER_SIZE);
    DSK6713_LED_off(0);
    DSK6713_LED_off(1);
    DSK6713_LED_off(2);
    DSK6713_LED_off(3);
    int i;

    comm_intr();
    initPitchShifter();

    while(1)
    {
        if(DSPstate == 1)
        {

            if (newTrameFlagBuf0 == 1)
            {
                pitchShift(pitchShiftInputBuffer, pitchShiftOutBuffer0);              ////////24 kHz
                newTrameFlagBuf0 = 0;
            }
            if (newTrameFlagBuf1 == 1)
            {
                pitchShift(pitchShiftInputBuffer, pitchShiftOutBuffer1);              ////////24 kHz
                newTrameFlagBuf1 = 0;
            }
        }

        else
        {
            if (newTunerBuffer)
            {

                for (i=0; i<TUNER_BUFFER_SIZE; i++)
                {
                    tunerBuffer[i] = (short)((float)tunerBuffer[i]*HANN_512[i]);
                }


                correlateASM(resultASM,tunerBuffer, tunerBuffer, TUNER_BUFFER_SIZE);
                // Fen^trage afin d'évité détections de mauvais peaks pour fréquence fondammentale,
                // cependant pas très précis pour les bornes de fréquence
                /*for (i=0; i<(TUNER_BUFFER_SIZE*2-1); i++)
                {
                    resultASM[i] = (int32_t)((float)resultASM[i]*hann1023[i]);
                }*/

                f0 = getF0(&resultASM[TUNER_BUFFER_SIZE], F0_NOMINAL[n]);
                err =(f0-F0_NOMINAL[n])/F0_NOMINAL[n];
                if (err <= Gage && err >= -Gage)
                {
                    DSK6713_LED_off(0);
                    DSK6713_LED_on(1);
                    DSK6713_LED_off(2);
                }
                if (err < -Gage)
                {
                    DSK6713_LED_off(0);
                    DSK6713_LED_off(1);
                    DSK6713_LED_on(2);
                }
                if (err > Gage)
                {
                    DSK6713_LED_on(0);
                    DSK6713_LED_off(1);
                    DSK6713_LED_off(2);
                }
                if (f0 == 0)
                {
                    DSK6713_LED_off(0);
                    DSK6713_LED_off(1);
                    DSK6713_LED_off(2);
                }
                newTunerBuffer = 0;
            }
        }

    }

    return 0;
}



interrupt void c_int11(void)
{

    static int index = 0;
    static int playBuffState = 0;
    int sample = 0;
    int outputAudio = 0;
    sample  = input_left_sample();
    static int indexTuner = 0;

    if(DSPstate ==1 )
    {

        if (playBuffState == 0)
        {
            if (index < BUFFER_SIZE)
            {
                pitchShiftInputBuffer[index] = input_left_sample();
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
                pitchShiftInputBuffer[index] = input_left_sample();
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
    }

    else
    {

        if(indexTuner < TUNER_BUFFER_SIZE)
        {
            tunerBuffer[indexTuner] = 0.01*input_left_sample();
            indexTuner++;
        }
        else
        {
            newTunerBuffer = 1;
            indexTuner = 0;
        }
    }


    outputAudio = sample << 16;                   // Pour avoir un int
    outputAudio = outputAudio | sample;
    output_sample(0.1*sample);
    //output_left_sample(input_sample());
}



float getF0(int xx[], float fDesirer)
{
    float f0;                   // Fréquence fondamentale de sortie
    int delaiF0=1;              // Délai de la fréquence fondamentale et son amplitude
    float amp_MAX;              // Amplitude de l'autocorrélation de la fréquence fondamentale
    float f_hi;                 // Fréquences maximum possible pour la corde sélectionnée
    int delaiMin;               // Délais minimum possible pour la corde sélectionnée
    int delai;

    // Variables pour limiter la recherche dans l'autocorrélation
    f_hi = fDesirer*2;
    delaiMin = (int)((float)FS/f_hi);

    // Recherche du délai de la fréquence fondamentale
    amp_MAX = 0;
    for (delai = delaiMin; delai<TUNER_BUFFER_SIZE ; delai++) {
        if (xx[delai] > amp_MAX) { /* Si l'amplitude courrante de l'autoccorélation est
                                      plus grande que les amplitudes précédentes */
            amp_MAX =  xx[delai];
            delaiF0 = delai;        // Enregistrement du délai actuel
        }
        else {}
    }

    // Calcul de la fréquence fondamentale si une note est jouée
    if (delaiF0 >= delaiMin)
        f0 = (float)FS/delaiF0;     // Calcul de la fréquence fondamentale
    else
        f0 = 0;                     // Aucune note ne semble être jouée

    return f0;
}

