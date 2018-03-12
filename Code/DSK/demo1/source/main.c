/*
 * main.c
 */

#include <stdio.h>
#include <stdint.h>

void correlate(int32_t *correlation, int32_t *x_n, int32_t *y_n, int32_t N);

void correlateASM(int32_t *, int32_t *, int32_t *, int32_t);

#define N_SAMPLES 30
int32_t signal[N_SAMPLES] = {0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9};

int main(void) {
	
    int32_t result[N_SAMPLES*2-1];

    // Changer correlate pour correlateASM pour la fonctions assembleur
    correlateASM(result,signal, signal, N_SAMPLES);

	printf("Hello world");
}


void correlate(int32_t *correlation, int32_t *x_n, int32_t *y_n, int32_t N)
{
    // Test Git
    int32_t i,j, somme;

    for(i = 0; i <= N; i++)
    {
        somme = 0;
        for(j = 0; j <= i; j++)
        {
            somme = somme + (x_n[j] * y_n[N-i+j]);
        }
        correlation[i] = somme;
    }

    for(i = N-2; i > 0; i--)
    {
        somme = 0;
        for(j = 0; j<i; j++)
        {
            somme = somme + (x_n[N-j] * y_n[i-j]);
        }
        correlation[N+(N-i)] = somme;
    }
}
