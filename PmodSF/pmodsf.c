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
/*  manual for the ST MP25P16/MP25P128 flash memory chip,               */
/*  some excerpts have been provided for functions provided             */
/*  in this library                                                     */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  9/30/2011(RyanH):                                                    */
/*                                                                      */
/************************************************************************/


#include "pmodsf.h"

/** PmodSFInit
**
**	Synopsis:
**  Initializes the PmodSF module on the selected SPI channel. 
**  
**  Input: SpiChannel chn  - spi channel initialize
**         uint32_t pbClock - peripheral bus clock in Hz
**         uint32_t bitRate - bit rate desired in Hz
**  Returns: none
**  Description:
**  Opens the desired SPI channel in 8-bit mode as a master, enables the slave select bit,
**  and sets the desired bit rate as a function of pbClock/bitRate.  Examples of peripheral bus
**  bit rate combinations are available in the table labeld "Excerpt from PIC32 Familiy Reference 
**  Manual Chapter 23 section 23.3.7" in pmodsf.h.
**/
void PmodSFInit(SpiChannel chn,uint32_t pbClock,uint32_t bitRate)
{
    SpiChnOpen(chn, SPI_OPEN_MSTEN | SPI_OPEN_SSEN |  SPI_OPEN_MODE8 , pbClock/bitRate);
}

/** PmodSFSetSSLow
**
**	Synopsis:
**  Sets the slave select bit on the selected SPI channel to low
**  
**  Input: SpiChannel chn  - spi channel to set SS bit low on
**  Returns: none
**
**/
void PmodSFSetSSLow(SpiChannel chn)
{
	PORTClearBits(SpiIO[chn].portSS,SpiIO[chn].ssMask);
}

/** PmodSFSetSSHigh
**
**	Synopsis:
**  Sets the slave select bit on the selected SPI channel to high
**  Input: SpiChannel chn  - spi channel to set SS bit high on
**  Returns: none
**	Errors:
**		none
**
**/
void PmodSFSetSSHigh(SpiChannel chn)
{
	PORTSetBits(SpiIO[chn].portSS,SpiIO[chn].ssMask);
}


/** PmodSFReadID
**
**	Synopsis:
**  The Read Identification (RDID) instruction allows
**  the 24-bit device identification to be read
**  into a 32 bit unsigned integer 
**	Input: SpiChannel chn - spi channel to read PmodSF ID from
**  Returns: uin32_t => Bits 0 - 7: Memory Capacity
**                     Bits 8 - 15: Memory Type
**                     Bits 16- 23 : Manufacturer ID
**	Errors: none
** 
** Description from the M25P16/M25P128 reference manual:
** Read Identification (RDID)
** The Read Identification (RDID) instruction allows
** the 8-bit manufacturer identification to be read, followed
** by two bytes of device identification. The
* *manufacturer identification is assigned by JEDEC,
** and has the value 20h for STMicroelectronics. The
** device identification is assigned by the device
** manufacturer, and indicates the memory type in
** the first byte (20h), and the memory capacity of the
** device in the second byte 15h (PmodSF-16) and 18h (PmodSF-18).
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

	BlockWhileWriteInProgress(chn); 

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
/** PmodSFSectorErase
**
**	Synopsis:
**  Erases a sector on the PmodSF on the specified channel
**  containing the 24-bit address passed in
**	Input: SpiChannel chn - spi channel to perform sector erase
**         uint32_t address - 24-bit address contained in sector
**                            to erase
**  Returns: none
**	Errors: none
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
** Description from the M25P16/M25P128 reference manual:
** Sector Erase (SE)
** The Sector Erase (SE) instruction sets to 1 (FFh)
** all bits inside the chosen sector. Before it can be
** accepted, a Write Enable (WREN) instruction
** must previously have been executed. After the
** Write Enable (WREN) instruction has been decoded,
** the device sets the Write Enable Latch (WEL).
** The Sector Erase (SE) instruction is entered by
** driving Chip Select (S) Low, followed by the instruction
** code, and three address bytes on Serial
** Data Input (D). Any address inside the Sector (see
** Table 3.) is a valid address for the Sector Erase
** (SE) instruction. Chip Select (S) must be driven
** Low for the entire duration of the sequence.
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
** A Sector Erase (SE) instruction applied to a page
** which is protected by the Block Protect (BP2, BP1,
** BP0) bits (see Table 2. and Table 3.) is not executed
*/
void PmodSFSectorErase (SpiChannel chn,uint32_t address)
{	
	BlockWhileWriteInProgress(chn); 
	
	PmodSFWriteEnable(chn); //set the WREN bit in status register
	
	PmodSFSetSSLow(chn); //SS to low 

    SpiChnPutC(chn,PMODSF_SECTOR_ERASE); //send read status register command
	SpiChnGetC(chn);
	
	SpiChnPutC(chn,address);
	SpiChnGetC(chn);		

	PmodSFSetSSHigh(chn); //SS to High
}

/** PmodSFWriteStatusRegister
**
**	Synopsis: Writes configuration bits to the status register
**            on the specified SPI channel
**	Input: SpiChannel chn - spi channel to status reg write
**         uint8_t statusReg - status register bits to write
**  Returns: none
**	Errors: none
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
** Description from the M25P16/M25P128 reference manual:
** The Write Status Register (WRSR) instruction allows
** new values to be written to the Status Register.
** Before it can be accepted, a Write Enable
** (WREN) instruction must previously have been executed.
** After the Write Enable (WREN) instruction
** has been decoded and executed, the device sets
** the Write Enable Latch (WEL).
** The Write Status Register (WRSR) instruction is
** entered by driving Chip Select (S) Low, followed
** by the instruction code and the data byte on Serial
** Data Input (D).
** The Write Status Register (WRSR) instruction has
** no effect on b6, b5, b1 and b0 of the Status Register.
** b6 and b5 are always read as 0.
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

	BlockWhileWriteInProgress(chn); 

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
**  Returns: none
**	Errors: none
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
** Description from the M25P16/M25P128 reference manual:
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
}

/** PmodSFReadBytes
**
**	Synopsis:
**  The Read Data bytes  instruction allows bytes to
**  be read from memory into a buffer from the specified
**  24-bit address on the SPI channel selected.
**	Input: SpiChannel chn,uint8_t numBytes,uint8_t *data,uint32_t address
** 
**	chn - spi channel
**  numBytes - number of bytes to read from the PmodSF
**  data - buffer to store data read in from the PmodSF
**  address - 24bit repsresentation of the page address
**  Returns: none
**	Errors:	none
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
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

/** BlockWhileWriteInProgress
**
**	Synopsis:
**  Blocks while a write is in progress
**	Input: SpiChannel chn - channel to poll for write in progress
**  Returns: none
**	Errors: none
**	Description:
**  During a write operation the Write In Progress(WIP) bit
**  is set, operations that write to the PmodSF will be
**  ignored while this bit is set. Calling this function 
**  blocks further operations until the WIP cleared condition
**  is met.	
*/
void BlockWhileWriteInProgress(SpiChannel chn)
{
	uint8_t statusReg = 0;
	//CHECK FOR WRITE IN PROGRESS	
	do
	{
		statusReg = PmodSFReadStatusRegister(chn);	
	}while((statusReg & PMODSF_SR_WIP) == PMODSF_SR_WIP); 
}

/** PmodSFReadStatusRegister
**
**	Synopsis:
**  Reads the value of the status register
**	Input: SpiChannel chn - SPI channel to read status register
**  Returns: uint8_t - 8-bit representation of the status register
             (see "PMODSF Status Register Format")
**	Errors: none
**	Description:
**	The Read Status Register (RDSR) instruction allows
**	the Status Register to be read. The Status
**	Register may be read at any time, even while a
**	Program, Erase or Write Status Register cycle is in
**	progress. When one of these cycles is in progress,
**	it is recommended to check the Write In Progress
**	(WIP) bit before sending a new instruction to the
**	device. It is also possible to read the Status Register
**	continuously.
**	See table "PMODSF Status Register Format" for a
**	description of the status register format.
**	The status and control bits of the Status Register
**	are as follows:
**	WIP bit. The Write In Progress (WIP) bit indicates
**	whether the memory is busy with a Write Status
**	Register, Program or Erase cycle. When set to 1,
**	such a cycle is in progress, when reset to 0 no
**	such cycle is in progress.
**	WEL bit. The Write Enable Latch (WEL) bit indicates
**	the status of the internal Write Enable Latch.
**	When set to 1 the internal Write Enable Latch is
**	set, when set to 0 the internal Write Enable Latch
**	is reset and no Write Status Register, Program or
**	Erase instruction is accepted.
**	BP2, BP1, BP0 bits. The Block Protect (BP2,
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
**	SRWD bit. The Status Register Write Disable
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
}

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

void PmodSFWriteEnable(SpiChannel chn)
{
	fnPmodSFSendCommand(chn,PMODSF_WRITE_ENABLE);
}

void PmodSFWriteDisable(SpiChannel chn)
{
	fnPmodSFSendCommand(chn,PMODSF_WRITE_DISABLE);
}

void PmodSFDeepPowerDown(SpiChannel chn)
{
	fnPmodSFSendCommand(chn,PMODSF_DEEP_POWER_DOWN);
}

uint8_t PmodSFDeepPowerDownRelease(SpiChannel chn)
{
	uint8_t i = 0;
	uint8_t signature = 0;
	
	BlockWhileWriteInProgress(chn); 

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
