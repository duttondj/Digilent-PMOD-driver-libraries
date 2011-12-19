/************************************************************************/
/*                                                                      */
/*                           pmodMic.c                                 */
/*                                                                      */
/*                                                                      */
/* -------------------------------------------------------------------- */
/*	Author: 	Skylar Graika											*/
/*              Copyright (C) 2011 Skylar Graika                         */
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  11/07/2011(SkylarG):                                                  */
/*                                                                      */
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "./PmodMic/PmodMic.h"
#include "./PmodCommon/spi/pmod_spi_common.h"

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */



/*  PmodMicInit
**
**	Synopsis:
**  Initializes the PmodMic module on the selected SPI channel. 
**  
**  Input: SpiChannel chn  - spi channel initialize
**         uint32_t pbClock - peripheral bus clock in Hz
**         uint32_t bitRate - bit rate desired in Hz
**
**  Returns: none
**
**  Description:
**
**  Opens the desired SPI channel in 8-bit mode as a master, enables the slave select bit,
**  and sets the desired bit rate as a function of pbClock/bitRate.  Examples of peripheral bus
**  bit rate combinations are available in the table labeld "Excerpt from PIC32 Familiy Reference 
**  Manual Chapter 23 section 23.3.7" in pmodsf.h.
*/
void PmodMicInit(SpiChannel chn, uint32_t pbClock,uint32_t bitRate)
{
    SpiChnOpen(chn, SPI_OPEN_MSTEN | SPI_OPEN_SSEN |  SPI_OPEN_MODE8 , pbClock/bitRate);
}

/*  PmodMicGetData
**
**	Synopsis:
**  Gets a 12-bit sample from the PmodMIC on each call
**  
**  Input: SpiChannel chn  - spi channel that the PmodMIC is connected to
**
**  Returns: uint16_t - a 12-bit sample from the PmodMIC
**
**  Description:
**
**  Sets the SPI slave select high and then transfers two 8-bit values from the
**  PmodMIC and puts then back together as a single 16-bit word. Only 12-bits of
**	which actually contain relavent data
*/
uint16_t PmodMicGetData(SpiChannel chn)
{
	uint16_t data = 0;

	PmodSPISetSSLow(chn); //SS to low 

	SpiChnPutC(chn,0);
	data = ((uint16_t)SpiChnGetC(chn)) << 8;
	SpiChnPutC(chn,0);
	data |= ((uint16_t)SpiChnGetC(chn));

	PmodSPISetSSHigh(chn); //SS to High

	return data;	
}