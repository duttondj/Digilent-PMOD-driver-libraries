/************************************************************************/
/*                                                                      */
/*   pmodsf_read_bytes.c Implementation file for PmodSFReadBytes        */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*  Implementation file for PmodSFReadBytes for PmodSF                  */
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  10/6/2011(RyanH):                                                   */
/*                                                                      */
/************************************************************************/

#include "pmodsf.h"
#include "pmodsf_helper.h"

/*  PmodSFReadBytes
**
**	Synopsis:
**  The Read Data bytes  instruction allows bytes to
**  be read from memory into a buffer from the specified
**  24-bit address on the SPI channel selected.
**
**	Input: SpiChannel chn - spi channel
**         uint32_t numBytes - number of bytes to read from the PmodSF
**         uint8_t *data - buffer to store data read in from the PmodSF
**         uint32_t address - 24bit repsresentation of the page address
**
**  Returns: none
**
**	Errors:	none
**
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
**
** Description from the M25P16/M25P128 reference manual:
**
** The device is first selected by driving Chip Select
** (S) Low. The instruction code for the Read Data
** Bytes (READ) instruction is followed by a 3-byte
** address (A23-A0), each bit being latched-in during
** the rising edge of Serial Clock (C). Then the memory
** contents, at that address, is shifted out on Serial
** Data Output (Q), each bit being shifted out, at
** a maximum frequency fR, during the falling edge of
** Serial Clock (C).
**
** The first byte addressed can be at any location.
** The address is automatically incremented to the
** next higher address after each byte of data is shifted
** out. The whole memory can, therefore, be read
** with a single Read Data Bytes (READ) instruction.
** When the highest address is reached, the address
** counter rolls over to 000000h, allowing the read
** sequence to be continued indefinitely.
**
** The Read Data Bytes (READ) instruction is terminated
** by driving Chip Select (S) High. Chip Select
** (S) can be driven High at any time during data output.
** Any Read Data Bytes (READ) instruction,
** while an Erase, Program or Write cycle is in
** progress, is rejected without having any effects on
** the cycle that is in progress.
*/
void PmodSFReadBytes(SpiChannel chn,uint32_t numBytes,uint8_t *data,uint32_t address)
{
    int32_t byteCounter = 0;

    PmodSFBlockWhileWriteInProgress(chn); 

	//SEND COMMAND FOR PAGE PROGRAM OPERATION
	PmodSFSetSSLow(chn); //SS to low 
	SpiChnPutC(chn,PMODSF_READ_DATA_BYTES); //send page read databytes command
	SpiChnGetC(chn);
	
	//SEND IN THE 24 BIT ADDRESS
	for(byteCounter = 2;byteCounter >= 0;byteCounter--)    
	{
		SpiChnPutC(chn,fnPMODGetByteFromUint32(address,byteCounter));
		SpiChnGetC(chn);
	}
	
	//GET BYTES FROM PMODSF
	
	for(byteCounter = 0;byteCounter < numBytes ;byteCounter++)    
	{
		uint8_t oneByte = 0;
		SpiChnPutC(chn,0);
	    data[byteCounter] = SpiChnGetC(chn);
	}
	
	PmodSFSetSSHigh(chn); //SS to High
}
