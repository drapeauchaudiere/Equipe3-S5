/*
 * spi.h
 *
 *  Created on: Feb 18, 2018
 *      Author: simon
 */

#ifndef SPI_H_
#define SPI_H_

#include "definitions.h"

#define SPI_WRITE_CONFIG 0xE001
#define SPI_READ_CONFIG 0x4000
#define SPI_WRITE_DATA 0x8000
#define SPI_READ_DATA 0x0

/***************************************************************************
    Global variables declaration :
***************************************************************************/

/***************************************************************************
    Function prototype :
***************************************************************************/
void SPI_init(void);
void SPI_write(uint8_t data);
uint8_t SPI_read(void);
bool SPI_rrdy(void);

#endif /* INCLUDE_SPI_H_ */
