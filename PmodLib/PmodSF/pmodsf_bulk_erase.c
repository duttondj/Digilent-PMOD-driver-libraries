/************************************************************************/
/*                                                                      */
/*   pmodsf_bulk_erase.c -- Implementation file for PmodSFBulkErase     */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*              Copyright (C) 2011 Ryan Hoffman                         */
/************************************************************************/
/*  Module Description: 												*/
/*  Implementation file for PmodSFBulkErase for PmodSF                  */
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

/*  PmodSFBulkErase
**
**	Synopsis:
**  Performs a bulk erase of the entire PmodSF setting
**  all bits to 1.
**
**	Input: SpiChannel chn - Spi channel
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
**  For a complete description see Bulk Erase (BE) in the M25P16/M25P128 
**	reference manual.
*/
void PmodSFBulkErase(SpiChannel chn)
{
	PmodSFBlockWhileWriteInProgress(chn); 

	PmodSFWriteEnable(chn);
	
	PmodSFSendCommand(chn,PMODSF_BULK_ERASE);
}
