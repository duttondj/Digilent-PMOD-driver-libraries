/************************************************************************/
/*                                                                      */
/*   pmodsf_read_bytes.c -- Implementation file for PmodSFReadBytes     */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*              Copyright (C) 2011 Ryan Hoffman                         */
/************************************************************************/
/*  Module Description: 												*/
/*  Implementation file for PmodSFReadBytes for PmodSF                  */
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

/*  PmodSFReadBytes
**
**	Synopsis:
**  The Read Data bytes  reads N number of bytes from flash memory 
**  into a buffer from the specified 24-bit address on the SPI channel 
**  selected.
**
**	Input: SpiChannel chn - Spi channel
**         uint32_t numBytes - number of bytes to read
**         uint8_t *data - buffer to store data read 
**         uint32_t address - 24bit representation of the page address
**
**  Returns: none
**
**	Errors:	none
**
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
**
**  Description:
**
**  For a complete description see Read Data Bytes (READ) in the M25P16/M25P128 
**	reference manual.
*/
void PmodSFReadBytes(SpiChannel chn,uint32_t numBytes,uint8_t *data,uint32_t address)
{
    int32_t byteCounter = 0;

    PmodSFBlockWhileWriteInProgress(chn); 

	//SEND COMMAND FOR PAGE PROGRAM OPERATION
	PmodSPISetSSLow(chn); //SS to low 
	SpiChnPutC(chn,PMODSF_READ_DATA_BYTES); //send page read databytes command
	SpiChnGetC(chn);
	
	//SEND IN THE 24 BIT ADDRESS
	for(byteCounter = 2;byteCounter >= 0;byteCounter--)    
	{
		SpiChnPutC(chn,fnPmodGetByteFromUint32(address,byteCounter));
		SpiChnGetC(chn);
	}
	
	//GET BYTES FROM PMODSF
	
	for(byteCounter = 0;byteCounter < numBytes ;byteCounter++)    
	{
		uint8_t oneByte = 0;
		SpiChnPutC(chn,0);
	    data[byteCounter] = SpiChnGetC(chn);
	}
	
	PmodSPISetSSHigh(chn); //SS to High
}
