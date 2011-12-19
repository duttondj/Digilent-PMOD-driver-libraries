/* -------------------------------------------------------------------- */
/*                                                                      */
/*            pmodDA2.h - Implementation of the PmodDA2 library			*/
/*																		*/
/* -------------------------------------------------------------------- */
/*	Author: 	Ross Kristof											*/
/*	Copyright (C) 2011 Ross Kristof 									*/
/************************************************************************/
/*  Module Description: 												*/
/*           									            			*/
/* -------------------------------------------------------------------- */
/*  Revision History:													*/
/*                                                                      */
/*  112/3/2011(RossK):                                                   */
/*                                                                      */
/* -------------------------------------------------------------------- */

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include "./PmodDA2/pmodDA2.h"
#include "./PmodCommon/spi/pmod_spi_common.h"

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */

/*  PmodDA2Init
**
**	Synopsis:
**  Initailzes the PmodDA2 and all the buffers needed for playback
**
**  Input: SpiChannel chn  - spi channel initialize
**         uint32_t pbClock - peripheral bus clock in Hz
**         uint32_t bitRate - bit rate desired in Hz
**
**  Returns: none
**
**	Errors:	none
*/
void PmodDA2Init(SpiChannel chn,uint32_t pbClock,uint32_t bitRate)
{
	SpiChnOpen(chn, SPI_OPEN_MSTEN | SPI_OPEN_SSEN |  SPI_OPEN_MODE8 , pbClock/bitRate);
}

/*  PmodDA2Enable
**
**	Synopsis:
**  Enables the PmodDA2
**
**  Input: SpiChannel chn  - spi channel
**
**  Returns: none
**
**	Errors:	none
*/
void PmodDA2Enable(SpiChannel chn)
{
	SpiChnEnable(chn, 1);
}

/*  PmodDA2Disable
**
**	Synopsis:
**  Disables the PmodDA2
**
**  Input: SpiChannel chn  - spi channel
**
**  Returns: none
**
**	Errors:	none
*/
void PmodDA2Disable(SpiChannel chn)
{
	SpiChnEnable(chn, 0);
}

/*  PmodDA2Send
**
**	Synopsis:
**  sends a 16 bit value from the PmodDA2
**
**  Input: SpiChannel chn  - spi channel
**		   uint_16 data - the digital representation of the analog
**						  signal to send to the PmodDA2
**
**  Returns: none
**
**	Errors:	none
*/
void PmodDA2Send(SpiChannel chn, uint16_t data)
{
	uint8_t dataPartA, dataPartB;

	dataPartB = data;
	dataPartA = data >> 8;

	PmodSPISetSSLow(chn);
	SpiChnPutC(chn,dataPartA);
	SpiChnGetC(chn);
	SpiChnPutC(chn,dataPartB);
	SpiChnGetC(chn);
	PmodSPISetSSHigh(chn);
}