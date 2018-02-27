/*
 * emif.h
 *
 *  Created on: Feb 3, 2018
 *      Author: simon
 */

#ifndef EMIF_H_
#define EMIF_H_

#include "definitions.h"

// Definitions

#define CECTL3_CONFIG   0x01400510  // STROBE = 50ns MTYPE = 16 bits async


typedef enum e_emif_reg_addr
{
    GBLCTL_ADDR = 0x01800000,
    CECTL0_ADDR = 0x01800008,
    CECTL1_ADDR = 0x01800004,
    CECTL2_ADDR = 0x01800010,
    CECTL3_ADDR = 0x01800018,
    SDCTL_ADDR = 0x01800018,
    SDTIM_ADDR = 0x0180001C

} EMIF_REG_ADDR_E;


// Public function declarations

void emif_init(void);


#endif /* INCLUDE_EMIF_H_ */
