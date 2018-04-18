
#include <stdio.h>
#include <math.h>
#include "filter.h"


#define IIR_NB_COEFFS 6     // Nombre total de coeffcients a & b (b0 b1 b2 a0 a1 a2)
#define IIR_NB_FILTRES 4        // Nombre de filtres
#define IIR_NB_SECTIONS_MAX 4   // nb de sections d'ordre 2 du plus gros filtre

const short IIR_COEFFS[][IIR_NB_COEFFS] = {
// 1er filtre (ordre 4, ripple de 3dB, fc = 700Hz, fe = 24000 Hz)
    {   2048, 4096, 2048, 8192, -14271, 6322},
// 2e filtre (ordre 4, ripple de 3dB, fc = 5000Hz, fe = 24000 Hz)
    {   2377, 4753, 2377, 8192, -2520, 1543},
// 3e filtre (ordre 4, ripple de 3dB, fc = 7000Hz, fe = 24000 Hz)
    {  2377, -4753, 2377, 8192, 2520, 1543},
// 4e filtre (ordre 4, ripple de 3dB, fc = 1000Hz, fe = 24000 Hz)
    { 4545, -9091, 4545, 8192, -13377, 5657 },
};

// Gain global de chacun des filtres (format : 2Q13)
const short IIR_GAINS[IIR_NB_FILTRES] = {243, 12268, 6218, 6218}; // À COMPLÉTER

#pragma DATA_ALIGN(IIR_W, 4);
int IIR_W[IIR_NB_SECTIONS_MAX][3] = {0};

short FPB_700(int signal){

    int y;

    //y = IIR_2ndOrder_directII(signal, &IIR_W[1][0], &IIR_COEFFS[0][0]); // y->15Q13
    y = IIR_2ndOrder_directII_ASM(signal, &IIR_W[0][0], &IIR_COEFFS[0][0]); // y->15Q13

    // Appliquer le gain global
    y = IIR_GAINS[0]*y; // 2Q13 x 15Q13 = 17Q26
    y = (y>>13);                     // y->17Q13

    return sat_16bits(y);            // y->2Q13*/
}


short FPB_5000(int signal){

    int y;

    //y = IIR_2ndOrder_directII(signal, &IIR_W[0][0], &IIR_COEFFS[1][0]); // y->15Q13
    y = IIR_2ndOrder_directII_ASM(signal, &IIR_W[1][0], &IIR_COEFFS[1][0]); // y->15Q13

    // Appliquer le gain global
    y = IIR_GAINS[1]*y; // 2Q13 x 15Q13 = 17Q26
    y = (y>>13);                     // y->17Q13

    return (y);            // y->2Q13*/

}

short FPH_7000(int signal){

    int y;

    //y = IIR_2ndOrder_directII_ASM(signal, &IIR_W[1][2], &IIR_COEFFS[2][0]); // y->15Q13
    y = IIR_2ndOrder_directII_ASM(signal, &IIR_W[2][0], &IIR_COEFFS[2][0]); // y->15Q13

    // Appliquer le gain global
    y = IIR_GAINS[2]*y; // 2Q13 x 15Q13 = 17Q26
    y = (y>>13);                     // y->17Q13

    return sat_16bits(y);            // y->2Q13*/

}

short FPH_1000(int signal){

    int y;

    //y = IIR_2ndOrder_directII(signal, &IIR_W[0][3], &IIR_COEFFS[3][0]); // y->15Q13
    y = IIR_2ndOrder_directII_ASM(signal, &IIR_W[3][0], &IIR_COEFFS[3][0]); // y->15Q13

    // Appliquer le gain global
    y = IIR_GAINS[3]*y; // 2Q13 x 15Q13 = 17Q26
    y = (y>>13);                     // y->17Q13

    return y;            // y->2Q13*/

}

short FPB_1000_5000(int signal){
    int y,temp;
    temp = FPH_1000(signal);
    y = FPB_5000(temp);

    return y;
}


/***********************************************************************************
DESCRIPTION : Initialisation des variables intermédiaires w(n-i)
***********************************************************************************/

void init_w()
{
    int i, j;

    for (i=0; i<IIR_NB_SECTIONS_MAX; i++)   {
        for (j=0; j<3; j++)     IIR_W[i][j] = 0;
    }
}

/***********************************************************************************
DESCRIPTION : Permet de faire une saturation à 16bits
***********************************************************************************/

short sat_16bits(int x) {

    short r;

    if (x>(pow(2,15)-1))
        r =(short)pow(2,15)-1;
    else if (x<-pow(2,15))
        r = (short)-pow(2,15);
    else
        r = (short)x;

    return r;
}



