/************************************************************************/
/*                                                                      */
/*   pmodsf.c -- Library to manipulate the Digilent PmodSF              */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*              Copyright (C) 2011 Ryan Hoffman                         */
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  This file contains functions used in manipulating the Digilent      */
/*  PMODSF on the Digilent PIC32 based microcontrollers                 */
/* 	                                                                    */
/*  For details on the operation or the PMODSF see the  instruction     */
/*  manual for the ST MP25P16/MP25P128 flash memory chip,               */
/*  some excerpts have been provided for functions provided             */
/*  in this library                                                     */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  9/30/2011(RyanH):                                                   */
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

/*  PmodSFInit
**
**	Synopsis:
**  Initializes the PmodSF module on the selected SPI channel. 
**  
**  Input: SpiChannel chn  - spi channel initialize
**         uint32_t pbClock - peripheral bus clock in Hz
**         uint32_t bitRate - bit rate desired in Hz
**
**  Returns: none
**
**  Description:
**
**  Opens the desired SPI channel in 8-bit mode as a master, enables the slave select bit,
**  and sets the desired bit rate as a function of pbClock/bitRate.  Examples of peripheral bus
**  bit rate combinations are available in the table labeld "Excerpt from PIC32 Familiy Reference 
**  Manual Chapter 23 section 23.3.7" in pmodsf.h.
*/
void PmodSFInit(SpiChannel chn,uint32_t pbClock,uint32_t bitRate)
{
    SpiChnOpen(chn, SPI_OPEN_MSTEN | SPI_OPEN_SSEN |  SPI_OPEN_MODE8 , pbClock/bitRate);
}

/*  PmodSFWriteStatusRegister
**
**	Synopsis: Writes configuration bits to the status register
**            on the specified SPI channel
**
**	Input: SpiChannel chn - spi channel to status reg write
**         uint8_t statusReg - status register bits to write
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
** The Write Status Register (WRSR) instruction allows
** new values to be written to the Status Register.
** Before it can be accepted, a Write Enable
** (WREN) instruction must previously have been executed.
** After the Write Enable (WREN) instruction
** has been decoded and executed, the device sets
** the Write Enable Latch (WEL).
**
** The Write Status Register (WRSR) instruction is
** entered by driving Chip Select (S) Low, followed
** by the instruction code and the data byte on Serial
** Data Input (D).
** The Write Status Register (WRSR) instruction has
** no effect on b6, b5, b1 and b0 of the Status Register.
** b6 and b5 are always read as 0.
**
** Chip Select (S) must be driven High after the
** eighth bit of the data byte has been latched in. If
** not, the Write Status Register (WRSR) instruction
** is not executed. As soon as Chip Select (S) is driven
** High, the self-timed Write Status Register cycle
** (whose duration is tW) is initiated. While the Write
** Status Register cycle is in progress, the Status
** Register may still be read to check the value of the
** Write In Progress (WIP) bit. The Write In Progress
** (WIP) bit is 1 during the self-timed Write Status
** Register cycle, and is 0 when it is completed.
** When the cycle is completed, the Write Enable
** Latch (WEL) is reset.
**
** The Write Status Register (WRSR) instruction allows
** the user to change the values of the Block
** Protect (BP2, BP1, BP0) bits, to define the size of
** the area that is to be treated as read-only, as defined
** in Table 2.. The Write Status Register
** (WRSR) instruction also allows the user to set or
** reset the Status Register Write Disable (SRWD)
** bit in accordance with the Write Protect (W) signal.
** The Status Register Write Disable (SRWD) bit and
** Write Protect (W) signal allow the device to be put
** in the Hardware Protected Mode (HPM). The Write
** Status Register (WRSR) instruction is not executed
** once the Hardware Protected Mode (HPM) is
** entered.
*/
void PmodSFWriteStatusRegister(SpiChannel chn,uint8_t statusReg)
{
	PmodSFBlockWhileWriteInProgress(chn); 

	PmodSFWriteEnable(chn);

	PmodSPISetSSLow(chn); //SS to low 

	SpiChnPutC(chn,PMODSF_WRITE_STATUS_REG); //send write status register command
	SpiChnGetC(chn);		

	SpiChnPutC(chn,statusReg); //write to the status register
	SpiChnGetC(chn);

	PmodSPISetSSHigh(chn); //SS to High
}

/*  PmodSFBlockWhileWriteInProgress
**
**	Synopsis:
**  Blocks while a write is in progress
**
**	Input: SpiChannel chn - channel to poll for write in progress
**
**  Returns: none
**
**	Errors: none
**
**	Description:
**
**  During a write operation the Write In Progress(WIP) bit
**  is set, operations that write to the PmodSF will be
**  ignored while this bit is set. Calling this function 
**  blocks further operations until the WIP cleared condition
**  is met.	
*/
void PmodSFBlockWhileWriteInProgress(SpiChannel chn)
{
	uint8_t statusReg = 0;
	//CHECK FOR WRITE IN PROGRESS	
	do
	{
		statusReg = PmodSFReadStatusRegister(chn);	
	}while((statusReg & PMODSF_SR_WIP) == PMODSF_SR_WIP); 
}

/*  PmodSFReadStatusRegister
**
**	Synopsis:
**  Reads the value of the status register
**
**	Input: SpiChannel chn - SPI channel to read status register
**
**  Returns: uint8_t - 8-bit representation of the status register
**           (see "PMODSF Status Register Format")
**
**	Errors: none
**
**	Description:
**
**	The Read Status Register (RDSR) instruction allows
**	the Status Register to be read. The Status
**	Register may be read at any time, even while a
**	Program, Erase or Write Status Register cycle is in
**	progress. When one of these cycles is in progress,
**	it is recommended to check the Write In Progress
**	(WIP) bit before sending a new instruction to the
**	device. It is also possible to read the Status Register
**	continuously.
**
**	See table "PMODSF Status Register Format" for a
**	description of the status register format.
**
**	The status and control bits of the Status Register
**	are as follows:
**
**	WIP bit:
**  The Write In Progress (WIP) bit indicates
**	whether the memory is busy with a Write Status
**	Register, Program or Erase cycle. When set to 1,
**	such a cycle is in progress, when reset to 0 no
**	such cycle is in progress.
**
**	WEL bit:
**  The Write Enable Latch (WEL) bit indicates
**	the status of the internal Write Enable Latch.
**	When set to 1 the internal Write Enable Latch is
**	set, when set to 0 the internal Write Enable Latch
**	is reset and no Write Status Register, Program or
**	Erase instruction is accepted.
**
**	BP2, BP1, BP0 bits:
**  The Block Protect (BP2,
**	BP1, BP0) bits are non-volatile. They define the
**	size of the area to be software protected against
**	Program and Erase instructions. These bits are
**	written with the Write Status Register (WRSR) instruction.
**	When one or both of the Block Protect
**	(BP2, BP1, BP0) bits is set to 1, the relevant memory
**	area (as defined in Table 2.) becomes protected
**	against Page Program (PP) and Sector Erase
**	(SE) instructions. The Block Protect (BP2, BP1,
**	BP0) bits can be written provided that the Hardware
**	Protected mode has not been set. The Bulk
**	Erase (BE) instruction is executed if, and only if,
**	both Block Protect (BP2, BP1, BP0) bits are 0.
**
**	SRWD bit:
**  The Status Register Write Disable
**	(SRWD) bit is operated in conjunction with the
**	Write Protect (W) signal. The Status Register
**	Write Disable (SRWD) bit and Write Protect (W)
**	signal allow the device to be put in the Hardware
**	Protected mode (when the Status Register Write
**	Disable (SRWD) bit is set to 1, and Write Protect
**	(W) is driven Low). In this mode, the non-volatile
**	bits of the Status Register (SRWD, BP2, BP1,
**	BP0) become read-only bits and the Write Status
**	Register (WRSR) instruction is no longer accepted
**	for execution.
*/
uint8_t PmodSFReadStatusRegister(SpiChannel chn)
{
	uint8_t statusRegister = 0;
	PmodSPISetSSLow(chn); //SS to low 

    SpiChnPutC(chn,PMODSF_READ_STATUS_REG); //send read status register command
	SpiChnGetC(chn);
	
	SpiChnPutC(chn,0);		
	statusRegister =  SpiChnGetC(chn);		//read status register

	PmodSPISetSSHigh(chn); //SS to High
	return statusRegister;
}

/*  PmodSFWriteEnable
**
**	Synopsis:
**  Enables writing by setting the Write Enable Latch(WEL)
**  bit on the selected chanel
**
**	Input: SpiChannel chn - channel to enable writes
**
**  Returns: none
**
**	Errors: none
**
**  Description from the M25P16/M25P128 reference manual:
**
**  The Write Enable (WREN) instruction 
**  sets the Write Enable Latch (WEL) bit.
**
**  The Write Enable Latch (WEL) bit must be set prior
**  to every Page Program (PP), Sector Erase
**  (SE), Bulk Erase (BE) and Write Status Register
**  (WRSR) instruction.
**
**  The Write Enable (WREN) instruction is entered
**  by driving Chip Select (S) Low, sending the instruction
**  code, and then driving Chip Select (S)
**  High.
*/
void PmodSFWriteEnable(SpiChannel chn)
{
	fnPmodSFSendCommand(chn,PMODSF_WRITE_ENABLE);
}


