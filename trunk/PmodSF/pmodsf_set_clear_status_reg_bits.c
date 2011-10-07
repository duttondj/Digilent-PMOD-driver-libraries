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

/** PmodSFSetStatusRegBit
**
**	Synopsis:
**  Set bits in the status register based on a bit mask passed in
**	Input: SpiChannel chn - spi channel to set status register bits on
**         uint8_t bitMask - bitmask to apply to status register                          
**  Returns: none
**	Errors: none
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
**  Decription: 
**  Status register is read in, an OR operation is performed on the 
**  value of the status register, this value is then written back to the
**  status register.
*/
void PmodSFSetStatusRegBits(SpiChannel chn,uint8_t bitMask)
{
	PmodSFWriteStatusRegister(chn,PmodSFReadStatusRegister(chn)| bitMask); //write new status register
}

/** PmodSFClearStatusRegBit
**
**	Synopsis:
**  Clear bits in the status register based on a bit mask passed in, the bits
**  to be cleared should be set to 1 in the bitmask
**	Input: SpiChannel chn - spi channel to clear status register bits on
**         uint8_t bitMask - bitmask to apply to status register                          
**  Returns: none
**	Errors: none
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
**  Decription: 
**  Status register is read in, a bitwise OR is performed on the bitmask passed in,
**  an AND operation is performed on the value of the status register and the bitmask,
**  this value is then written back to the status register.
*/
void PmodSFClearStatusRegBits(SpiChannel chn,uint8_t bitMask)
{
	PmodSFWriteStatusRegister(chn,PmodSFReadStatusRegister(chn) & (~bitMask)); //write new status register
}
