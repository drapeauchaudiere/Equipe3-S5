/*
 * direct1FIR.h
 *
 *  Created on: 2018-10-23
 *      Author: lp_tatlock
 */

#ifndef HARMONIZER_H_
#define HARMONIZER_H_

void harmonizer(short * signal);
//Faire une copie du signal dans harmonizer

void maxFFT(short * signal, short * frerquency, short * ampitude);

void reconstruction_signal(short * frequency, short * ampitude);

void vecteur_tournant(short * frequency, short * ampitude);
//phase à 0 pour c0
//calculer delta
//normaliser l'amplitude


#endif /* HARMONIZER_H_ */