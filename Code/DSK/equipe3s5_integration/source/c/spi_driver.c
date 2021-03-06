/*
 * spi.c
 *
 *  Created on: Feb 18, 2018
 *      Author: simon
 */

#include "spi_driver.h"

/****************************************************************************
    Extern content declaration :
****************************************************************************/

extern far void vectors();   // Vecteurs d'interruption
extern MCBSP_Handle DSK6713_AIC23_CONTROLHANDLE;

/****************************************************************************
    Private macros and constants :
****************************************************************************/

MCBSP_Config MCBSP_SPIConfig = {
        MCBSP_FMKS(SPCR, FREE, NO)              |   // SOFT bit determines operation of McBSP
        MCBSP_FMKS(SPCR, SOFT, YES)              |   // Serial port stops immediately during emulation halt
        MCBSP_FMKS(SPCR, FRST, YES)             |   // Enable frame sync generator
        MCBSP_FMKS(SPCR, GRST, YES)             |   // Enable sample-rate generator
        MCBSP_FMKS(SPCR, XINTM, XRDY)           |   // XINT is driven by XRDY (end of word)
        MCBSP_FMKS(SPCR, XSYNCERR, NO)          |   // No synchronization error is detected
        MCBSP_FMKS(SPCR, XRST, YES)             |   // Serial port transmitter is enable
        MCBSP_FMKS(SPCR, DLB, OFF)              |   // Digital loop back mode is disable
        MCBSP_FMKS(SPCR, RJUST, RZF)            |   // Right-justify and zero-fill MSBs in DRR
        MCBSP_FMKS(SPCR, CLKSTP, DELAY)       |   // Clock start delay
        MCBSP_FMKS(SPCR, DXENA, OFF)            |   // DX enabler is off
        MCBSP_FMKS(SPCR, RINTM, RRDY)           |   // RINT is driven by RRDY (end of word)
        MCBSP_FMKS(SPCR, RSYNCERR, NO)          |   // No synchronization error is detected
        MCBSP_FMKS(SPCR, RRST, YES),                // Serial port receiver is enabled

        MCBSP_FMKS(RCR, RPHASE, SINGLE)         |   // Single-phase frame
        MCBSP_FMKS(RCR, RFRLEN2, DEFAULT)       |   // Phase 2 is not set
        MCBSP_FMKS(RCR, RWDLEN2, DEFAULT)       |
        MCBSP_FMKS(RCR, RCOMPAND, MSB)          |   // No companding (MSB first)
        MCBSP_FMKS(RCR, RFIG, NO)               |   //
        MCBSP_FMKS(RCR, RDATDLY, 1BIT)          |   // Set to 0 for SPI slave
        MCBSP_FMKS(RCR, RFRLEN1, OF(0))         |   // 3 messages per frame
        MCBSP_FMKS(RCR, RWDLEN1, 16BIT)         |   // This changes to 8 bits per message
        MCBSP_FMKS(RCR, RWDREVRS, DISABLE),         // 32-bit reversal is disable

        MCBSP_FMKS(XCR, XPHASE, SINGLE)         |   // Single-phase frame
        MCBSP_FMKS(XCR, XFRLEN2, DEFAULT)       |   // Phase 2 is not set
        MCBSP_FMKS(XCR, XWDLEN2, DEFAULT)       |
        MCBSP_FMKS(XCR, XCOMPAND, MSB)          |   // No companding (MSB first)
        MCBSP_FMKS(XCR, XFIG, NO)               |   //
        MCBSP_FMKS(XCR, XDATDLY, 1BIT)          |   // Set to 0 for SPI slave
        MCBSP_FMKS(XCR, XFRLEN1, OF(0))         |   // This changes to 1 FRAME
        MCBSP_FMKS(XCR, XWDLEN1, 16BIT)          |   // This changes to 32 bits per frame
        MCBSP_FMKS(XCR, XWDREVRS, DISABLE),         // 32-bit reversal is disable

        MCBSP_FMKS(SRGR, GSYNC, DEFAULT)        |   // Free running
        MCBSP_FMKS(SRGR, CLKSP, DEFAULT)        |   // Rising falling ?
        MCBSP_FMKS(SRGR, CLKSM, DEFAULT)       |   // Internal clock
        MCBSP_FMKS(SRGR, FSGM, DEFAULT)         |   // Frame-sync signal is generated by the frame-sync generator
        MCBSP_FMKS(SRGR, FPER, DEFAULT)         |   // For SPI, default
        MCBSP_FMKS(SRGR, FWID, DEFAULT)         |   // For SPI, default
        MCBSP_FMKS(SRGR, CLKGDV, OF(27)),            // Fosc/2 = 112.5MHz, 112.5MHz / 27 = 4.16MHz ~ 4.2MHz

        MCBSP_MCR_DEFAULT,
        MCBSP_RCER_DEFAULT,
        MCBSP_XCER_DEFAULT,

        MCBSP_FMKS(PCR, XIOEN, SP)              |
        MCBSP_FMKS(PCR, RIOEN, SP)              |
        MCBSP_FMKS(PCR, FSXM, INTERNAL)         |   // Input from master (/CS)
        MCBSP_FMKS(PCR, FSRM, EXTERNAL)         |   // Input from master (/CS)
        MCBSP_FMKS(PCR, CLKXM, OUTPUT)           |   // Input from master (SCK)
        MCBSP_FMKS(PCR, CLKRM, OUTPUT)           |   // Input from master (SCK)
        MCBSP_FMKS(PCR, CLKSSTAT, DEFAULT)      |
        MCBSP_FMKS(PCR, DXSTAT, DEFAULT)        |
        MCBSP_FMKS(PCR, FSXP, ACTIVELOW)        |
        MCBSP_FMKS(PCR, FSRP, ACTIVELOW)        |
        MCBSP_FMKS(PCR, CLKXP, RISING)         |
        MCBSP_FMKS(PCR, CLKRP, FALLING)
};


/****************************************************************************
    Private global variables :
****************************************************************************/
MCBSP_Handle hSpi;          // Handle to McBSP0

/****************************************************************************
    Public functions :
****************************************************************************/

void SPI_init(void)
{
    DSK6713_rset(DSK6713_MISC, (DSK6713_rget(DSK6713_MISC) | 0x1));

    hSpi = DSK6713_AIC23_CONTROLHANDLE;
    MCBSP_config(hSpi, &MCBSP_SPIConfig);   // Configure the SPI communication
    MCBSP_start(hSpi, MCBSP_RCV_START | MCBSP_XMIT_START | MCBSP_SRGR_START, 0x00003000);

    DSK6713_waitusec(50);

    while(!MCBSP_xrdy(hSpi));
    MCBSP_write(hSpi,SPI_WRITE_CONFIG);     // Configure MAX3111

    MCBSP_read(hSpi);

    while(!MCBSP_xrdy(hSpi));
    MCBSP_write(hSpi, SPI_READ_CONFIG);     // Read MAX3111 configuration

}

void SPI_write(uint8_t data)
{

    while(!MCBSP_xrdy(hSpi));
        MCBSP_write(hSpi, SPI_WRITE_DATA | data);        // Send the data over SPI

}

uint8_t SPI_read(void)
{
    while(!MCBSP_xrdy(hSpi));
    MCBSP_write(hSpi, SPI_READ_DATA);
    while(!MCBSP_rrdy(hSpi));
    return  (MCBSP_read(hSpi))&0xFF;
}

bool SPI_rrdy(void)
{
    return MCBSP_rrdy(hSpi);    // Return the state of RRDY
}



