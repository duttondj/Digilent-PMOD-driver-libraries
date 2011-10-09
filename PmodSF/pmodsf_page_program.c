/************************************************************************/
/*                                                                      */
/*   pmodsf_page_program.c Implementation file for PmodSFPageProgram    */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*  Implementation file for PmodSFPageProgram for PmodSF                */
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  10/6/2011(RyanH):                                                   */
/*                                                                      */
/************************************************************************/

#include "pmodsf.h"
#include "pmodsf_helper.h"

/*  PmodSFPageProgram
**
**	Synopsis:
**  The Page Program (PP) instruction allows bytes to
**  be programmed in the memory (changing bits from
**  1 to 0).
**
**	Input: SpiChannel chn - spi channel
**         uint32_t numBytes - number of bytes to write to the PmodSF
**         uint8_t *data - data to write to PmodSF
**         uint32_t address - 24bit repsresentation of the page address
**
**  Returns: none
**
**	Errors: none
**
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
**
**  Description from the M25P16/M25P128 reference manual:
**
**  The Page Program (PP) instruction allows bytes to
**  be programmed in the memory (changing bits from
**  1 to 0). Before it can be accepted, a Write Enable
**  (WREN) instruction must previously have been executed.
**  After the Write Enable (WREN) instruction
**  has been decoded, the device sets the Write Enable
**  Latch (WEL).
**
**  The Page Program (PP) instruction is entered by
**  driving Chip Select (S) Low, followed by the instruction
**  code, three address bytes and at least
**  one data byte on Serial Data Input (D). If the 8
**  least significant address bits (A7-A0) are not all
**  zero, all transmitted data that goes beyond the end
**  of the current page are programmed from the start
**  address of the same page (from the address
**  whose 8 least significant bits (A7-A0) are all zero).
**  Chip Select (S) must be driven Low for the entire
**  duration of the sequence.
**  The instruction sequence is shown in Figure 16..
**
**  If more than 256 bytes are sent to the device, previously
**  latched data are discarded and the last 256
**  data bytes are guaranteed to be programmed correctly
**  within the same page. If less than 256 Data
**  bytes are sent to device, they are correctly programmed
**  at the requested addresses without having
**  any effects on the other bytes of the same
**  page.
**
**  Chip Select (S) must be driven High after the
**  eighth bit of the last data byte has been latched in,
**  otherwise the Page Program (PP) instruction is not
**  executed.
**
**  As soon as Chip Select (S) is driven High, the selftimed
**  Page Program cycle (whose duration is tPP)
**  is initiated. While the Page Program cycle is in
**  progress, the Status Register may be read to
**  check the value of the Write In Progress (WIP) bit.
**  The Write In Progress (WIP) bit is 1 during the selftimed
**  Page Program cycle, and is 0 when it is
**  completed. At some unspecified time before the
**  cycle is completed, the Write Enable Latch (WEL)
**  bit is reset.
**
**  A Page Program (PP) instruction applied to a page
**  which is protected by the Block Protect (BP2, BP1,
**  BP0) bits (see Table 2. and Table 3.) is not executed
*/ 
void PmodSFPageProgram(SpiChannel chn,uint32_t numBytes,uint8_t *data,uint32_t address)
{
	int32_t byteCounter = 0;

	PmodSFBlockWhileWriteInProgress(chn); 

	PmodSFWriteEnable(chn);

	PmodSFSetSSLow(chn); //SS to low 

	SpiChnPutC(chn,PMODSF_PAGE_PGM); //send page program command
	SpiChnGetC(chn);
	
	//SEND IN THE 24 BIT ADDRESS
	for(byteCounter = 2;byteCounter >= 0;byteCounter--)    
	{
		uint8_t byteAdd = fnPMODGetByteFromUint32(address,byteCounter);
		SpiChnPutC(chn,fnPMODGetByteFromUint32(address,byteCounter));
		SpiChnGetC(chn);
	}
	
	//TRANSFER BYTES TO BE WRITTEN TO PMODSF

	for(byteCounter = 0;byteCounter < numBytes ;byteCounter++)    
	{
		SpiChnPutC(chn,data[byteCounter]);
		SpiChnGetC(chn);
	}

	PmodSFSetSSHigh(chn); //SS to High
}
