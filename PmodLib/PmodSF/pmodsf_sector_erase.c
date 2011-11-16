/************************************************************************/
/*                                                                      */
/*   pmodsf_dpd_rel.c Implementation file for PmodSFSectorErase         */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*  Implementation file for PmodSFSectorErase for PmodSF                */
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

/*  PmodSFSectorErase
**
**	Synopsis:
**  Erases a sector on the PmodSF on the specified channel
**  containing the 24-bit address passed in
**
**	Input: SpiChannel chn - spi channel to perform sector erase
**         uint32_t address - 24-bit address contained in sector
**                            to erase
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
** The Sector Erase (SE) instruction sets to 1 (FFh)
** all bits inside the chosen sector. Before it can be
** accepted, a Write Enable (WREN) instruction
** must previously have been executed. After the
** Write Enable (WREN) instruction has been decoded,
** the device sets the Write Enable Latch (WEL).
**
** The Sector Erase (SE) instruction is entered by
** driving Chip Select (S) Low, followed by the instruction
** code, and three address bytes on Serial
** Data Input (D). Any address inside the Sector (see
** Table 3.) is a valid address for the Sector Erase
** (SE) instruction. Chip Select (S) must be driven
** Low for the entire duration of the sequence.
**
** Chip Select (S) must be driven High after the
** eighth bit of the last address byte has been latched
** in, otherwise the Sector Erase (SE) instruction is
** not executed. As soon as Chip Select (S) is driven
** High, the self-timed Sector Erase cycle (whose duration
** is tSE) is initiated. While the Sector Erase cycle
** is in progress, the Status Register may be read
** to check the value of the Write In Progress (WIP)
** bit. The Write In Progress (WIP) bit is 1 during the
** self-timed Sector Erase cycle, and is 0 when it is
** completed. At some unspecified time before the
** cycle is completed, the Write Enable Latch (WEL)
** bit is reset.
**
** A Sector Erase (SE) instruction applied to a page
** which is protected by the Block Protect (BP2, BP1,
** BP0) bits (see Table 2. and Table 3.) is not executed
*/
void PmodSFSectorErase (SpiChannel chn,uint32_t address)
{	
	int8_t byteCounter = 0;
	
	PmodSFBlockWhileWriteInProgress(chn); 

	PmodSFWriteEnable(chn); //set the WREN bit in status register

	PmodSPISetSSLow(chn); //SS to low 

    SpiChnPutC(chn,PMODSF_SECTOR_ERASE); //send read status register command
	SpiChnGetC(chn);
	
	//SEND IN THE 24 BIT ADDRESS
	for(byteCounter = 2;byteCounter >= 0;byteCounter--)    
	{
		SpiChnPutC(chn,fnPmodGetByteFromUint32(address,byteCounter));
		SpiChnGetC(chn);
	}		

	PmodSPISetSSHigh(chn); //SS to High
}
