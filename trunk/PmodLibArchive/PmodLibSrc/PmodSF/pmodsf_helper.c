/************************************************************************/
/*                                                                      */
/*  pmodsf_helper.c -- Implementation file for PmodSF helper functions  */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman                                            */
/*              Copyright (C) 2011 Ryan Hoffman                         */
/************************************************************************/
/*  Module Description: 												*/
/*  Implementation file for PmodSF helper functions                     */
/*                           fnPmodSFSendCommand  						*/
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  10/6/2011(RyanH):                                                   */
/*                                                                      */
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "./PmodSF/pmodsf_helper.h"
#include "./PmodCommon/spi/pmod_spi_common.h"

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

/** fnPmodSFSendCommand
**
**	Synopsis:
**  Sends a command to the PmodSF on the selected SPI channel,
**  use this fungtion only for commands that do not return values
**	Input: SpiChannel chn - spi channel to send command to
**     	   uint8_t command - 8-bit command from the "PMODSF INSTRUCTION SET"
**  Returns: none
**	Errors: none
**  Description:
**  SS is driven low, the 8 bit command is shifted out, 1 dummy byte
**  is shifted in, SS is driven high 
*/

void fnPmodSFSendCommand(SpiChannel chn,uint8_t command)
{
	PmodSPISetSSLow(chn); //SS to low 
    SpiChnPutC(chn,command); //send read status register command
	SpiChnGetC(chn);
	PmodSPISetSSHigh(chn); //SS to High
}
