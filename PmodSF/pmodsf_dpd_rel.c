/************************************************************************/
/*                                                                      */
/*   pmodsf_dpd_rel.c Implementation file for:							*/
/*                           PmodSFDeepPowerDown                        */
/*                           PmodSFDeepPowerDownRelease                 */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*  Implementation file for PmodSF:                                      */
/*                           PmodSFDeepPowerDown                        */
/*                           PmodSFDeepPowerDownRelease                 */
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  10/6/2011(RyanH):                                                   */
/*                                                                      */
/************************************************************************/

#include "pmodsf.h"
#include "pmodsf_helper.h"

/** PmodSFDeepPowerDown (PmodSF-16 only)
**
**	Synopsis:
**  Performs a deep power down on the selected channel
**	Input: SpiChannel chn - channel to perform Deep 
**                          power down
**  Returns: none
**	Errors: none
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
** Description from the M25P16/M25P128 reference manual:
** Executing the Deep Power-down (DP) instruction
** is the only way to put the device in the lowest consumption
** mode (the Deep Power-down mode). It
** can also be used as an extra software protection
** mechanism, while the device is not in active use,
** since in this mode, the device ignores all Write,
** Program and Erase instructions.
** Driving Chip Select (S) High deselects the device,
** and puts the device in the Standby mode (if there
** is no internal cycle currently in progress). But this
** mode is not the Deep Power-down mode. The
** Deep Power-down mode can only be entered by
** executing the Deep Power-down (DP) instruction,
** to reduce the standby current.
** Once the device has entered the Deep Powerdown
** mode, all instructions are ignored except the
** Release from Deep Power-down and Read Electronic
** Signature (RES) instruction. This releases
** the device from this mode. The Release from
** Deep Power-down and Read Electronic Signature
** (RES) instruction also allows the Electronic Signature
** of the device to be output on Serial Data Output
** (Q).
** The Deep Power-down mode automatically stops
** at Power-down, and the device always Powers-up
** in the Standby mode.
** The Deep Power-down (DP) instruction is entered
** by driving Chip Select (S) Low, followed by the instruction
** code on Serial Data Input (D). Chip Select
** (S) must be driven Low for the entire duration
** of the sequence.
** Chip Select (S) must be driven High after the
** eighth bit of the instruction code has been latched
** in, otherwise the Deep Power-down (DP) instruction
** is not executed. As soon as Chip Select (S) is
** driven High, it requires a delay of tDP before the
** supply current is reduced to ICC2 and the Deep
** Power-down mode is entered.
** Any Deep Power-down (DP) instruction, while an
** Erase, Program or Write cycle is in progress, is rejected
** without having any effects on the cycle that
** is in progress.
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
**	Input: SpiChannel chn - channel to perform Deep 
**                          power down release on
**
**  Returns: uint8_t electronic signature 14h
**
**	Errors: none
**
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
**
**  Description from the M25P16/M25P128 reference manual:
**
**  Once the device has entered the Deep Powerdown
**  mode, all instructions are ignored except the
**  Release from Deep Power-down and Read Electronic
**  Signature (RES) instruction. Executing this
**  instruction takes the device out of the Deep Power-
**  down mode.
**
**  The instruction can also be used to read, on Serial
**  Data Output (Q), the old-style 8-bit Electronic Signature,
**  whose value for the M25P16 is 14h.

**  Please note that this is not the same as, or even a
**  subset of, the JEDEC 16-bit Electronic Signature
**  that is read by the Read Identifier (RDID) instruction.
**  The old-style Electronic Signature is supported
**  for reasons of backward compatibility, only, and
**  should not be used for new designs. New designs
**  should, instead, make use of the JEDEC 16-bit
**  Electronic Signature, and the Read Identifier
**  (RDID) instruction.
**
**  Except while an Erase, Program or Write Status
**  Register cycle is in progress, the Release from
**  Deep Power-down and Read Electronic Signature
**  (RES) instruction always provides access to the
**  old-style 8-bit Electronic Signature of the device,
**  and can be applied even if the Deep Power-down
**  mode has not been entered.
**
**  Any Release from Deep Power-down and Read
**  Electronic Signature (RES) instruction while an
**  Erase, Program or Write Status Register cycle is in
**  progress, is not decoded, and has no effect on the
**  cycle that is in progress.
**
**  The device is first selected by driving Chip Select
**  (S) Low. The instruction code is followed by 3
**  dummy bytes, each bit being latched-in on Serial
**  Data Input (D) during the rising edge of Serial
**  Clock (C). Then, the old-style 8-bit Electronic Signature,
**  stored in the memory, is shifted out on Serial
**  Data Output (Q), each bit being shifted out
**  during the falling edge of Serial Clock (C).
**
**  The Release from Deep Power-down and Read
**  Electronic Signature (RES) instruction is terminated
**  by driving Chip Select (S) High after the Electronic
**  Signature has been read at least once.
**  Sending additional clock cycles on Serial Clock
**  (C), while Chip Select (S) is driven Low, cause the
**  Electronic Signature to be output repeatedly.
**
**  When Chip Select (S) is driven High, the device is
**  put in the Stand-by Power mode. If the device was
**  not previously in the Deep Power-down mode, the
**  transition to the Stand-by Power mode is immediate.
**  If the device was previously in the Deep Power-
**  down mode, though, the transition to the Standby
**  Power mode is delayed by tRES2, and Chip Select
**  (S) must remain High for at least tRES2(max)
**  Once in the Stand-by Power mode, the device waits 
**  to be selected, so that it can receive, decode and
**  execute instructions.
*/
uint8_t PmodSFDeepPowerDownRelease(SpiChannel chn)
{
	uint8_t i = 0;
	uint8_t signature = 0;
	
	PmodSFBlockWhileWriteInProgress(chn); 

	PmodSFSetSSLow(chn); //SS to low 

    SpiChnPutC(chn,PMODSF_RELEASE_FROM_DPD); //send read status register command
	SpiChnGetC(chn);

	for(i = 0;i < 3;i++) //read 3 dummy bytes
	{
		SpiChnPutC(chn,0);		
		SpiChnGetC(chn);
	}
	
	SpiChnPutC(chn,0);		
	signature = SpiChnGetC(chn); //get signature
	
	PmodSFSetSSHigh(chn);

	return signature;
}
