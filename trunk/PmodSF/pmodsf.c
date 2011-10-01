/************************************************************************/
/*                                                                      */
/*   pmodsf.c -- Library to manipulate the Digilent PMODSF	            */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  This file contains functions used in manipulating the Digilent      */
/*  PMODSF on the Digilent CEREBOT32MX4 and CEREBOT32MX7                */
/*  For details on the operation or the PMODSF see the  instruction     */
/*  manual for the ST MP25P16 flash memory chip, some excerpts have     */
/*  provided for functions provided in this library                     */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  9/30/2011(RyanH):                                                    */
/*                                                                      */
/************************************************************************/


#include "pmodsf.h"


void PmodSFInit(SpiChannel chn,uint32_t pbClock,uint32_t ulBaud)
{
    SpiChnOpen(chn, SPI_OPEN_MSTEN | SPI_OPEN_SSEN |  SPI_OPEN_MODE8 , pbClock/ulBaud);
}

void PmodSFSetSSLow(SpiChannel chn)
{
	PORTClearBits(SpiIO[chn].portSS,SpiIO[chn].ssMask);
}

void PmodSFSetSSHigh(SpiChannel chn)
{
	PORTSetBits(SpiIO[chn].portSS,SpiIO[chn].ssMask);
}


/** fnPmodSFReadID
**
**	Synopsis:
** The Read Identification (RDID) instruction allows
** the 8-bit manufacturer identification to be read
**	PMODSF that provides no return values 
**	Input: SpiChannel chn
**       chn - spi channel to read PmodSF ID from
**  returns uin32_t => Bits 0 - 7: Memory Capacity
**                     Bits 8 - 15: Memory Type
**                     Bits 16- 23 : Manufacturer ID
** Description from the M25P16 reference manual:
** Read Identification (RDID)
** The Read Identification (RDID) instruction allows
** the 8-bit manufacturer identification to be read, followed
** by two bytes of device identification. The
* *manufacturer identification is assigned by JEDEC,
** and has the value 20h for STMicroelectronics. The
** device identification is assigned by the device
** manufacturer, and indicates the memory type in
** the first byte (20h), and the memory capacity of the
** device in the second byte (15h).
** Any Read Identification (RDID) instruction while
** an Erase or Program cycle is in progress, is not
** decoded, and has no effect on the cycle that is in
** progress.
** The device is first selected by driving Chip Select
** (S) Low. Then, the 8-bit instruction code for the instruction
** is shifted in. This is followed by the 24-bit
** device identification, stored in the memory, being
** shifted out on Serial Data Output (Q), each bit being
** shifted out during the falling edge of Serial
** Clock (C).
** The instruction sequence is shown in Figure 11..
** The Read Identification (RDID) instruction is terminated
** by driving Chip Select (S) High at any time
** during data output.
** When Chip Select (S) is driven High, the device is
** put in the Stand-by Power mode. Once in the
** Stand-by Power mode, the device waits to be selected,
** so that it can receive, decode and execute
** instructions
**
*/
uint32_t PmodSFReadID(SpiChannel chn)
{
    uint32_t pmodID = 0;

    PmodSFSetSSLow(chn); //SS to low 

    SpiChnPutC(chn,PMODSF_READ_ID); //Send read ID command
    SpiChnGetC(chn);//first byte will be garbage
  
    SpiChnPutC(chn,0);
    pmodID = SpiChnGetC(chn); //get manufacturer ID
 	
	pmodID = pmodID << 8;
    SpiChnPutC(chn,0);
    pmodID |= SpiChnGetC(chn); //get memory type
	
	pmodID = pmodID << 8;
    SpiChnPutC(chn,0);
    pmodID |= SpiChnGetC(chn); //get memory capacity
 
    PmodSFSetSSHigh(chn); //SS to High
 
    return pmodID;
}

void PmodSFSectorErase (SpiChannel chn,uint8_t address)
{
	BlockWhileWriteInProgress(chn);
	
	PmodSFWriteEnable(chn);
	
	fnPmodSFSendCommand(chn,PMODSF_SECTOR_ERASE);
	
	BlockWhileWriteInProgress(chn);
}


void PmodSFWriteStatusRegister(SpiChannel chn,uint8_t statusReg)
{

	PmodSFWriteEnable(chn);

	PmodSFSetSSLow(chn); //SS to low 
	SpiChnPutC(chn,PMODSF_WRITE_STATUS_REG); //send write status register command
	SpiChnGetC(chn);		
	SpiChnPutC(chn,statusReg); //write to the status register
	SpiChnGetC(chn);
	PmodSFSetSSHigh(chn); //SS to High
}

/** PmodSFPageProgram
**
**	Synopsis:
**  The Page Program (PP) instruction allows bytes to
**  be programmed in the memory (changing bits from
**  1 to 0).
**	Input: SpiChannel chn,uint8_t numBytes,uint8_t *data,uint32_t address
** 
**	chn - spi channe
**  numBytes - number of bytes to write to the PmodSF
**  data - data to write to pmod sf
**  address - 24bit repsresentation of the page address
**
**	Errors:
**		none
** Description from the M25P16 reference manual:
** Page Program (PP)
** The Page Program (PP) instruction allows bytes to
** be programmed in the memory (changing bits from
** 1 to 0). Before it can be accepted, a Write Enable
** (WREN) instruction must previously have been executed.
** After the Write Enable (WREN) instruction
** has been decoded, the device sets the Write Enable
** Latch (WEL).
** The Page Program (PP) instruction is entered by
** driving Chip Select (S) Low, followed by the instruction
** code, three address bytes and at least
** one data byte on Serial Data Input (D). If the 8
** least significant address bits (A7-A0) are not all
** zero, all transmitted data that goes beyond the end
** of the current page are programmed from the start
** address of the same page (from the address
** whose 8 least significant bits (A7-A0) are all zero).
** Chip Select (S) must be driven Low for the entire
** duration of the sequence.
** The instruction sequence is shown in Figure 16..
** If more than 256 bytes are sent to the device, previously
** latched data are discarded and the last 256
** data bytes are guaranteed to be programmed correctly
** within the same page. If less than 256 Data
** bytes are sent to device, they are correctly programmed
** at the requested addresses without having
** any effects on the other bytes of the same
** page.
** Chip Select (S) must be driven High after the
** eighth bit of the last data byte has been latched in,
** otherwise the Page Program (PP) instruction is not
** executed.
** As soon as Chip Select (S) is driven High, the selftimed
** Page Program cycle (whose duration is tPP)
** is initiated. While the Page Program cycle is in
** progress, the Status Register may be read to
** check the value of the Write In Progress (WIP) bit.
** The Write In Progress (WIP) bit is 1 during the selftimed
** Page Program cycle, and is 0 when it is
** completed. At some unspecified time before the
** cycle is completed, the Write Enable Latch (WEL)
** bit is reset.
** A Page Program (PP) instruction applied to a page
** which is protected by the Block Protect (BP2, BP1,
** BP0) bits (see Table 2. and Table 3.) is not executed
**/

void PmodSFPageProgram(SpiChannel chn,uint8_t numBytes,uint8_t *data,uint32_t address)
{
	int8_t byteCounter = 0;
	uint8_t statusReg = 0;
    BlockWhileWriteInProgress(chn);
	
	PmodSFWriteEnable(chn);

	PmodSFSetSSLow(chn); //SS to low 
	SpiChnPutC(chn,PMODSF_PAGE_PGM); //send page program command
	SpiChnGetC(chn);
	
	//SEND IN THE 24 BIT ADDRESS
	for(byteCounter = 2;byteCounter >= 0;byteCounter--)    
	{
		uint8_t byteAdd = fnGetByteFromUint32(address,byteCounter);
		SpiChnPutC(chn,fnGetByteFromUint32(address,byteCounter));
		SpiChnGetC(chn);
	}
	
	//TRANSFER BYTES TO BE WRITTEN TO PMODSF

	for(byteCounter = 0;byteCounter < numBytes ;byteCounter++)    
	{
		SpiChnPutC(chn,data[byteCounter]);
		SpiChnGetC(chn);
	}

	PmodSFSetSSHigh(chn); //SS to High
 	
	BlockWhileWriteInProgress(chn);
}

/** PmodSFReadBytes
**
**	Synopsis:
**  The Read Data bytes  instruction allows bytes to
**  be read from memory into the data field of the PMODSFCOMMAND.data
**	Input: SpiChannel chn,uint8_t numBytes,uint8_t *data,uint32_t address
** 
**	chn - spi channel
**  numBytes - number of bytes to read from the PmodSF
**  data - buffer to store data read in from the PmodSF
**  address - 24bit repsresentation of the page address
**	Errors:
**		none
** Description from the M25P16 reference manual:
** Read Data Bytes (READ)
** The device is first selected by driving Chip Select
** (S) Low. The instruction code for the Read Data
** Bytes (READ) instruction is followed by a 3-byte
** address (A23-A0), each bit being latched-in during
** the rising edge of Serial Clock (C). Then the memory
** contents, at that address, is shifted out on Serial
** Data Output (Q), each bit being shifted out, at
** a maximum frequency fR, during the falling edge of
** Serial Clock (C).
** The instruction sequence is shown in Figure 14..
** The first byte addressed can be at any location.
** The address is automatically incremented to the
** next higher address after each byte of data is shifted
** out. The whole memory can, therefore, be read
** with a single Read Data Bytes (READ) instruction.
** When the highest address is reached, the address
** counter rolls over to 000000h, allowing the read
** sequence to be continued indefinitely.
** The Read Data Bytes (READ) instruction is terminated
** by driving Chip Select (S) High. Chip Select
** (S) can be driven High at any time during data output.
** Any Read Data Bytes (READ) instruction,
** while an Erase, Program or Write cycle is in
** progress, is rejected without having any effects on
** the cycle that is in progress.
*/
void PmodSFReadBytes(SpiChannel chn,uint8_t numBytes,uint8_t *data,uint32_t address)
{
	
   int8_t byteCounter = 0;
	
   BlockWhileWriteInProgress(chn);

	//SEND COMMAND FOR PAGE PROGRAM OPERATION
	PmodSFSetSSLow(chn); //SS to low 
	SpiChnPutC(chn,PMODSF_READ_DATA_BYTES); //send page read databytes command
	SpiChnGetC(chn);

	
	//SEND IN THE 24 BIT ADDRESS
	for(byteCounter = 2;byteCounter >= 0;byteCounter--)    
	{
		SpiChnPutC(chn,fnGetByteFromUint32(address,byteCounter));
		SpiChnGetC(chn);
	}
	
	//GET BYTES FROM PMODSF

	for(byteCounter = 0;byteCounter < numBytes ;byteCounter++)    
	{
		SpiChnPutC(chn,0);
		data[byteCounter] = SpiChnGetC(chn);
	}
	
	PmodSFSetSSHigh(chn); //SS to High

}


void BlockWhileWriteInProgress(SpiChannel chn)
{
	uint8_t statusReg = 0;
	//CHECK FOR WRITE IN PROGRESS	
	do
	{
		statusReg = PmodSFReadStatusRegister(chn);	
	}while((statusReg & PMODSF_SR_WIP) == PMODSF_SR_WIP); 
}


uint8_t PmodSFReadStatusRegister(SpiChannel chn)
{
	uint8_t statusRegister = 0;
	PmodSFSetSSLow(chn); //SS to low 

    SpiChnPutC(chn,PMODSF_READ_STATUS_REG); //send read status register command
	SpiChnGetC(chn);
	
	SpiChnPutC(chn,0);		
	statusRegister =  SpiChnGetC(chn);		//read status register

	PmodSFSetSSHigh(chn); //SS to High
	return statusRegister;
}

void PmodSFBulkErase(SpiChannel chn)
{
	BlockWhileWriteInProgress(chn);
	
	PmodSFWriteEnable(chn);
	
	fnPmodSFSendCommand(chn,PMODSF_BULK_ERASE);

	BlockWhileWriteInProgress(chn);

}

void PmodSFEnableleBlockProtection(SpiChannel chn,uint8_t blockMask)
{
    uint8_t statusReg = 0;
	
	statusReg = PmodSFReadStatusRegister(chn);
	statusReg |= blockMask;
	PmodSFWriteEnable(chn);
	PmodSFWriteStatusRegister(chn,statusReg);
	
}

void PmodSFDisableBlockProtection(SpiChannel chn,uint8_t blockMask)
{
    uint8_t statusReg = 0;
	
	statusReg = PmodSFReadStatusRegister(chn);
	statusReg &= ~blockMask;
	PmodSFWriteEnable(chn);
	PmodSFWriteStatusRegister(chn,statusReg);
	
}

void PmodSFWriteEnable(SpiChannel chn)
{
	fnPmodSFSendCommand(chn,PMODSF_WRITE_ENABLE);
}

void PmodSFDeepPowerDown(SpiChannel chn)
{
	fnPmodSFSendCommand(chn,PMODSF_DEEP_POWER_DOWN);
}

uint8_t PmodSFDeepPowerDownRelease(SpiChannel chn)
{
	uint8_t i = 0;
	uint8_t signature = 0;
	PmodSFSetSSLow(chn); //SS to low 

    SpiChnPutC(chn,PMODSF_RELEASE_FROM_DPD); //send read status register command
	SpiChnGetC(chn);

	for(i = 0;i < 3;i++)
	{
		SpiChnPutC(chn,0);		
		SpiChnGetC(chn);
	}
	
	SpiChnPutC(chn,0);		
	signature = SpiChnGetC(chn);
	
	PmodSFSetSSHigh(chn);

	return signature;
}

/** fnGetByteFromUint32
**	Synopsis:
**  Takes in an unsigned 32 bit value and a byte position and returns
**  a byte represending the positions requested
**          --------------------------------------
** Position |	  3    |    2    |   1    |   0  |	
**          --------------------------------------
** Bits     | 32 - 24 | 23 - 16 | 15 - 8 | 7 - 0 |
**          --------------------------------------
**  INPUT:
**  uint32_t value  32 bit value to retrieve byte from
**  uint8_t bytePos byte position to retrieve (0 - 3)
**  OUTPUT:
**  uint8_t representing the byte specified in the bytePos from value
**	Errors:
**	if bytePos > 3 0 is returned since all bits will be shitfed out
*/
static uint8_t fnGetByteFromUint32(uint32_t value,uint8_t bytePos)
{
	return value >> (bytePos * 8) & 255;
}


static void fnPmodSFSendCommand(SpiChannel chn,uint8_t command)
{
	PmodSFSetSSLow(chn); //SS to low 
    SpiChnPutC(chn,command); //send read status register command
	SpiChnGetC(chn);
	PmodSFSetSSHigh(chn); //SS to High
}