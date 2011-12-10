/************************************************************************/
/*                                                                      */
/*  pmod_spi_common.c -- implementation for common Pmod Spi functions   */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*              Copyright (C) 2011 Ryan Hoffman                         */
/************************************************************************/
/*  Module Description: 												*/
/*  Common SPI IO tables and functions are defined in this module.      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  10/18/2011(RyanH):                                                  */
/*                                                                      */
/* -------------------------------------------------------------------- */

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include "./PmodCommon/spi/pmod_spi_common.h"


/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

//SpiIO contains SPI port to IO_PORT/SS pin mappings for use in
//SPI IO operations such as driving SS high and low
static const SpiPortSS SpiIO[] = {
	{0,0},  // SPI 0 is invalid
#if  ((__PIC32_FEATURE_SET__ >= 300) && (__PIC32_FEATURE_SET__ <= 499))
	{IOPORT_D,BIT_9}, //SPI1
	{IOPORT_G,BIT_9}  //SPI2
#elif ((__PIC32_FEATURE_SET__ >= 500) && (__PIC32_FEATURE_SET__ <= 799))
	{IOPORT_D,BIT_9}, //SPI1
	{IOPORT_G,BIT_9},//SPI2/SPI2A
    {IOPORT_D,BIT_14},//SPI1A/SPI3
	{IOPORT_F,BIT_12} //SPI3A/SPI4
#endif
};

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

/*  PmodSPISetSSLow
**
**	Synopsis:
**  Sets the slave select bit on the selected SPI channel to low
**  
**  Input: SpiChannel chn  - spi channel to set SS bit low on
**
**  Returns: none
*/
void PmodSPISetSSLow(SpiChannel chn)
{
	PORTClearBits(SpiIO[chn].portSS,SpiIO[chn].ssMask);
}

/*  PmodSPISetSSHigh
**
**	Synopsis:
**  Sets the slave select bit on the selected SPI channel to high
**
**  Input: SpiChannel chn  - spi channel to set SS bit high on
**
**  Returns: none
**
**	Errors:	none
*/
void PmodSPISetSSHigh(SpiChannel chn)
{
	PORTSetBits(SpiIO[chn].portSS,SpiIO[chn].ssMask);
}
