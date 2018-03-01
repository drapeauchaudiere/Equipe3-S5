/*
 * main.c
 */

#include <stdio.h>
#include <stdint.h>

void correlate(int32_t *correlation, int32_t *x_n, int32_t *y_n, int32_t N);

void correlateASM(int32_t *, int16_t *, int16_t *, int32_t);

#define N_SAMPLES 30
#pragma DATA_ALIGN(signal,8);
int16_t signal[N_SAMPLES] = {32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767};

int main(void) {
	
    int32_t result[N_SAMPLES*2-1];

    // Changer correlate pour correlateASM pour la fonction assembleur
    correlateASM(result,signal, signal, N_SAMPLES-1);

	printf("Hello world");
}


void correlate(int32_t *correlation, int32_t *x_n, int32_t *y_n, int32_t N)
{
    int32_t i,j,x,y;
    int64_t somme;

    for(i = 0; i <= N; i++)
    {
        somme = 0;
        for(j = 0; j <= i; j++)
        {
            somme = somme + (x_n[j] * y_n[N-i+j]);
        }
        correlation[i] = (int32_t)somme;
    }

    for(i = N-1; i > 0; i--)
    {
        somme = 0;
        for(j = 0; j<i; j++)
        {
            somme = somme + (x_n[N-j] * y_n[i-j]);
        }
        correlation[N+(N-i)] = (int32_t)somme;
    }
}
