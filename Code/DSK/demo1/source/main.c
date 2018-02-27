/*
 * main.c
 */

#include <stdio.h>
#include <stdint.h>

int32_t *correlate(int32_t *x_h, int32_t *y_h, int32_t N);

void correlateASM(int32_t *, int32_t *, int32_t *, int32_t);

#define N_SAMPLES 30
int32_t signal[N_SAMPLES] = {0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9};

int main(void) {
	
    int32_t *correlation = correlate(signal, signal, N_SAMPLES);
    //for()
	printf("Hello world");
}


int32_t *correlate(int32_t *x_n, int32_t *y_n, int32_t N)
{
    int32_t i,j, somme;
    int32_t *correlation;

    for(i = 0; i <= N; i++)
    {
        somme = 0;
        for(j = 0; j <= i; j++)
        {
            somme = somme + (x_n[j] * y_n[N-i+j]);
        }
        correlation[i] = somme;
    }

    for(i = N-1; i >= 1; i--)
    {
        somme = 0;
        for(j = 0; j<i; j++)
        {
            somme = somme + (x_n[N-j] * y_n[i-j]);
        }
        correlation[N+(N-i)] = somme;
    }

    return correlation;
}
