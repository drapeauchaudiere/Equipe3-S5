/*
 * direct1FIR.h
 *
 *  Created on: 2018-10-23
 *      Author: lp_tatlock
 */

#ifndef AUTOCORRELATION_H_
#define AUTOCORRELATION_H_

void correlateASM(int32_t *, int32_t *, int32_t *, int32_t);

int detection_trame();

short max(short * sample);

#endif /* AUTOCORRELATION_H_ */