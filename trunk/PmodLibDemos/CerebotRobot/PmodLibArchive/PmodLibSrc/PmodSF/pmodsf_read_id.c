/************************************************************************/
/*                                                                      */
/*   pmodsf_read_id.c -- Implementation file for PmodSFReadID           */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*              Copyright (C) 2011 Ryan Hoffman                         */
/************************************************************************/
/*  Module Description: 												*/
/*  Implementation file for PmodSFReadID for PmodSF                     */
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

/*  PmodSFReadID
**
**	Synopsis:
**  The Read Identification (RDID) instruction allows
**  the 24-bit device identification to be read
**  into a 32 bit unsigned integer 
**
**	Input: SpiChannel chn - Spi channel
**
**  Returns: uin32_t => Bits 0 - 7:   Memory Capacity
**                      Bits 8 - 15:  Memory Type
**                      Bits 16- 23 : Manufacturer ID
**	Errors: none
** 
**  Description:
**
**  For a complete description see Read Identification (RDID)in the M25P16/M25P128 
**	reference manual. 
*/
uint32_t PmodSFReadID(SpiChannel chn)
{
    uint32_t pmodID = 0;

	PmodSFBlockWhileWriteInProgress(chn); 

    PmodSPISetSSLow(chn); //SS to low 

    SpiChnPutC(chn,PMODSF_READ_ID); //Send read ID command
    SpiChnGetC(chn);//first byte will be garbage
  
    SpiChnPutC(chn,0);
    pmodID = SpiChnGetC(chn); //get manufacturer ID
 	
	pmodID = pmodID << 8;
    SpiChnPutC(chn,0);
    pmodID |= SpiChnGetC(chn); //get memory type
	
	pmodID = pmodID << 8;
    SpiChnPutC(chn,0);
    pmodID |= SpiChnGetC(chn); //get memory capacity
 
    PmodSPISetSSHigh(chn); //SS to High
 
    return pmodID;
}
