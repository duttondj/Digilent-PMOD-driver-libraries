/************************************************************************/
/*                                                                      */
/*   pmodsf_set_clear_status_reg_bits.c Implementation file for         */
/*                     PmodSFSetStatusRegBits                           */
/*                     PmodSFSetStatusRegBits                           */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*  Implementation file for	PmodSF:                                     */
/*                     PmodSFSetStatusRegBits                           */
/*                     PmodSFSetStatusRegBits                           */
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  10/6/2011(RyanH):                                                   */
/*                                                                      */
/************************************************************************/
#include "pmodsf.h"
#include "pmodsf_helper.h"
void PmodSFSetStatusRegBits(SpiChannel chn,uint8_t bitMask)
{
    uint8_t statusReg = 0;
	BlockWhileWriteInProgress(chn); 
	statusReg = PmodSFReadStatusRegister(chn);
	statusReg |= bitMask;
	PmodSFWriteEnable(chn);
	PmodSFWriteStatusRegister(chn,statusReg);
	
}

void PmodSFClearStatusRegBits(SpiChannel chn,uint8_t bitMask)
{
    uint8_t statusReg = 0;
	BlockWhileWriteInProgress(chn); 
	statusReg = PmodSFReadStatusRegister(chn);
	statusReg &= ~bitMask;
	PmodSFWriteEnable(chn);
	PmodSFWriteStatusRegister(chn,statusReg);	
}
