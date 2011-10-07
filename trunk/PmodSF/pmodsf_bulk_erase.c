/************************************************************************/
/*                                                                      */
/*   pmodsf_bulk_erase.c Implementation file for PmodSFBulkErase        */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
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

#include "pmodsf.h"
#include "pmodsf_helper.h"

/*  PmodSFBulkErase
**
**	Synopsis:
**  Performs a bulk erase of the entire PmodSF setting
**  all bits to 1.
**
**	Input: SpiChannel chn - channel to perform bulk erase on
**
**  Returns: none
**
**	Errors: none
**
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
**
** Description from the M25P16/M25P128 reference manual:
**
** The Bulk Erase (BE) instruction sets all bits to 1
** (FFh). Before it can be accepted, a Write Enable
** (WREN) instruction must previously have been executed.
** After the Write Enable (WREN) instruction
** has been decoded, the device sets the Write Enable
** Latch (WEL).
**
** The Bulk Erase (BE) instruction is entered by driving
** Chip Select (S) Low, followed by the instruction
** code on Serial Data Input (D). Chip Select (S)
** must be driven Low for the entire duration of the
** sequence.
**
** Chip Select (S) must be driven High after the
** eighth bit of the instruction code has been latched
** in, otherwise the Bulk Erase instruction is not executed.
** As soon as Chip Select (S) is driven High,
** the self-timed Bulk Erase cycle (whose duration is
** tBE) is initiated. While the Bulk Erase cycle is in
** progress, the Status Register may be read to
** check the value of the Write In Progress (WIP) bit.
** The Write In Progress (WIP) bit is 1 during the selftimed
** Bulk Erase cycle, and is 0 when it is completed.
** At some unspecified time before the cycle
** is completed, the Write Enable Latch (WEL) bit is
** reset.
**
** The Bulk Erase (BE) instruction is executed only if
** all Block Protect (BP2, BP1, BP0) bits are 0. The
** Bulk Erase (BE) instruction is ignored if one, or
** more, sectors are protected.
*/
void PmodSFBulkErase(SpiChannel chn)
{
	BlockWhileWriteInProgress(chn); 

	PmodSFWriteEnable(chn);
	
	fnPmodSFSendCommand(chn,PMODSF_BULK_ERASE);
}
