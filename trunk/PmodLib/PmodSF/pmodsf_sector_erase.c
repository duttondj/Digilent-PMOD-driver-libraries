/************************************************************************/
/*                                                                      */
/*   pmodsf_dpd_rel.c Implementation file for PmodSFSectorErase         */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*              Copyright (C) 2011 Ryan Hoffman                         */
/************************************************************************/
/*  Module Description: 												*/
/*  Implementation file for PmodSFSectorErase for PmodSF                */
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

/*  PmodSFSectorErase
**
**	Synopsis:
**  Erases a sector on the PmodSF on the specified channel
**  containing the 24-bit address passed in
**
**	Input: SpiChannel chn - spi channel 
**         uint32_t address - 24-bit address contained in sector
**                            to erase
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
**  For a complete description see Sector Erase (SE) in the M25P16/M25P128 
**	reference manual.
*/
void PmodSFSectorErase (SpiChannel chn,uint32_t address)
{	
	int8_t byteCounter = 0;
	
	PmodSFBlockWhileWriteInProgress(chn); 

	PmodSFWriteEnable(chn); //set the WREN bit in status register

	PmodSPISetSSLow(chn); //SS to low 

    SpiChnPutC(chn,PMODSF_SECTOR_ERASE); //send read status register command
	SpiChnGetC(chn);
	
	//SEND IN THE 24 BIT ADDRESS
	for(byteCounter = 2;byteCounter >= 0;byteCounter--)    
	{
		SpiChnPutC(chn,fnPmodGetByteFromUint32(address,byteCounter));
		SpiChnGetC(chn);
	}		

	PmodSPISetSSHigh(chn); //SS to High
}
