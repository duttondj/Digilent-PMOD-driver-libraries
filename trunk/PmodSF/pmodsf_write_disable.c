/************************************************************************/
/*                                                                      */
/*   pmodsf_write_disable.c Implementation file for PmodSFWriteDisable  */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
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
#include "pmodsf.h"
#include "pmodsf_helper.h"
/** PmodSFWriteDisable
**
**	Synopsis:
**  Dsiable writing by resetting the Write Enable Latch(WEL)
**  bit on the selected chanel
**	Input: SpiChannel chn - channel to disable writes
**  Returns: none
**	Errors: none
**  Description from the M25P16/M25P128 reference manual:
**  The Write Disable (WRDI) instruction (Figure 10.)
**  resets the Write Enable Latch (WEL) bit.
**  The Write Disable (WRDI) instruction is entered by
**  driving Chip Select (S) Low, sending the instruction
**  code, and then driving Chip Select (S) High.
**  The Write Enable Latch (WEL) bit is reset under
**  the following conditions:
**  – Power-up
**  – Write Disable (WRDI) instruction completion
**  – Write Status Register (WRSR) instruction
**  completion
**  – Page Program (PP) instruction completion
**  – Sector Erase (SE) instruction completion
**  – Bulk Erase (BE) instruction completion
*/
void PmodSFWriteDisable(SpiChannel chn)
{
	fnPmodSFSendCommand(chn,PMODSF_WRITE_DISABLE);
}
