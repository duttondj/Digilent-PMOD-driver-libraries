/************************************************************************/
/*                                                                      */
/*   pmodsf_page_program.c -- Implementation file for PmodSFPageProgram */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*              Copyright (C) 2011 Ryan Hoffman                         */
/************************************************************************/
/*  Module Description: 												*/
/*  Implementation file for PmodSFPageProgram for PmodSF                */
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  10/6/2011(RyanH):                                                   */
/*                                                                      */
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "./PmodSF/pmodsf.h"
#include "./PmodCommon/spi/pmod_spi_common.h"

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */


/*  PmodSFPageProgram
**
**	Synopsis:
**  The Page Program (PP) instruction programs bytes into a single
**  page of flash memory (changing bits from 1 to 0).
**
**	Input: SpiChannel chn - Spi channel
**         uint32_t numBytes - number of bytes to write 
**         uint8_t *data - data to write
**         uint32_t address - 24bit representation of the page address
**
**  Returns: none
**
**	Errors: none
**
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
**
**  Description:
**
**  For a complete description see Page Program (PP) in the M25P16/M25P128 
**	reference manual. 
*/
void PmodSFPageProgram(SpiChannel chn,uint32_t numBytes,uint8_t *data,uint32_t address)
{
	int32_t byteCounter = 0;

	PmodSFBlockWhileWriteInProgress(chn); 

	PmodSFWriteEnable(chn);

	PmodSPISetSSLow(chn); //SS to low 

	SpiChnPutC(chn,PMODSF_PAGE_PGM); //send page program command
	SpiChnGetC(chn);
	
	//SEND IN THE 24 BIT ADDRESS
	for(byteCounter = 2;byteCounter >= 0;byteCounter--)    
	{
		uint8_t byteAdd = fnPmodGetByteFromUint32(address,byteCounter);
		SpiChnPutC(chn,fnPmodGetByteFromUint32(address,byteCounter));
		SpiChnGetC(chn);
	}
	//TRANSFER BYTES TO BE WRITTEN TO PMODSF
	for(byteCounter = 0;byteCounter < numBytes ;byteCounter++)    
	{
		SpiChnPutC(chn,data[byteCounter]);
		SpiChnGetC(chn);
	}

	PmodSPISetSSHigh(chn); //SS to High
}
