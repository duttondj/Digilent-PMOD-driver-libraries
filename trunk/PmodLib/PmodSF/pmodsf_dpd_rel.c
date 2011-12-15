/************************************************************************/
/*                                                                      */
/*   pmodsf_dpd_rel.c Implementation file for:							*/
/*                           PmodSFDeepPowerDown                        */
/*                           PmodSFDeepPowerDownRelease                 */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*              Copyright (C) 2011 Ryan Hoffman                         */
/************************************************************************/
/*  Module Description: 												*/
/*  Implementation file for PmodSF:                                     */
/*                           PmodSFDeepPowerDown                        */
/*                           PmodSFDeepPowerDownRelease                 */
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

/*  PmodSFDeepPowerDown (PmodSF-16 only)
**
**	Synopsis:
**  Performs a deep power down on the selected channel
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
**  For a complete description see Release from Deep Power-down (DP)
**  in the M25P16 reference manual. 
*/
void PmodSFDeepPowerDown(SpiChannel chn)
{
	PmodSFBlockWhileWriteInProgress(chn); 
	fnPmodSFSendCommand(chn,PMODSF_DEEP_POWER_DOWN);
}

/*  PmodSFDeepPowerDownRelease (PmodSF-16 only)
**
**	Synopsis:
**  Release deep power down on the selected channel,
**  old style electronic signature 14h is returned
**
**	Input: SpiChannel chn - Spi Channel
**
**  Returns: uint8_t electronic signature 14h
**
**	Errors: none
**
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
**
**  Description:
**
**  For a complete description see Release from Deep Power-down and Read
**  Electronic Signature (RES)in the M25P16 reference manual. 
*/
uint8_t PmodSFDeepPowerDownRelease(SpiChannel chn)
{
	uint8_t i = 0;
	uint8_t signature = 0;
	
	PmodSFBlockWhileWriteInProgress(chn); 

	PmodSPISetSSLow(chn); //SS to low 

    SpiChnPutC(chn,PMODSF_RELEASE_FROM_DPD); //send read status register command
	SpiChnGetC(chn);

	for(i = 0;i < 3;i++) //read 3 dummy bytes
	{
		SpiChnPutC(chn,0);		
		SpiChnGetC(chn);
	}
	
	SpiChnPutC(chn,0);		
	signature = SpiChnGetC(chn); //get signature
	
	PmodSPISetSSHigh(chn);

	return signature;
}
