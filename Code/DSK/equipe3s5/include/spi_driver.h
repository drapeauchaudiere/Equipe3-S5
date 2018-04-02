/*
 * spi.h
 *
 *  Created on: Feb 18, 2018
 *      Author: simon
 */

#ifndef SPI_H_
#define SPI_H_

#include "definitions.h"

/***************************************************************************
    Global variables declaration :
***************************************************************************/

extern bool spiRxFlag;        // SPI received data is ready

/***************************************************************************
    Function prototype :
***************************************************************************/
void SPI_init(void);
void SPI_write(uint8_t data);
uint32_t SPI_read(void);
bool SPI_rrdy(void);

void c_McBSP0Rx_ISR(void);


#endif /* INCLUDE_SPI_H_ */
