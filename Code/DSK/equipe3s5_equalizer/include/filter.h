
#include "definitions.h"


//int _IIR_2ndOrder_directII_ASM(short, int , const short);
int IIR_2ndOrder_directII_ASM(int x, int w[], const short C[]);
int IIR_2ndOrder_directII(int x, int w[], const short C[]);

short FPB_700(int signal);
short FPB_5000(int signal);
short FPH_7000(int signal);
short FPH_1000(int signal);
short FPB_1000_5000(int signal);

void init_w();
short sat_16bits(int x);

