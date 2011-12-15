/************************************************************************/
/*                                                                      */
/* pmodsf_write_disable.c -- Implementation file for PmodSFWriteDisable */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*              Copyright (C) 2011 Ryan Hoffman                         */
/************************************************************************/
/*  Module Description: 												*/
/*  Implementation file for PmodSFWriteDisable for PmodSF               */
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
#include "./PmodSF/pmodsf_helper.h"
#include "./PmodCommon/spi/pmod_spi_common.h"

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

/*  PmodSFWriteDisable
**
**	Synopsis:
**  Disable writing by resetting the Write Enable Latch(WEL)
**  bit on the selected chanel
**
**	Input:  SpiChannel chn - Spi channel
**
**  Returns: none
**
**	Errors: none
**
**	Description:
**
**  For a complete description see Write Disable (WRDI) in the M25P16/M25P128 
**	reference manual. 
*/
void PmodSFWriteDisable(SpiChannel chn)
{
	PmodSFSendCommand(chn,PMODSF_WRITE_DISABLE);
}
