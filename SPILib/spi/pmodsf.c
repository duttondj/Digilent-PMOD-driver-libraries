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
/*  Table 2 Protected Area Sizes                                        */
/* -----------------------------------------------------------------------------------------------------------------
/* |Status Register Content|                                     Memory Content                                    |
/* -----------------------------------------------------------------------------------------------------------------
/* |  BP2  |  BP1  |  BP0  |              Protected Area              |            Unprotected Area                |
/* |----------------------------------------------------------------------------------------------------------------
/* |   0   |   0   |   0   | none                                     | All sectors1 (32 sectors: 0 to 31)         |
/* |   0   |   0   |   1   | Upper 32nd (Sector 31)                   | Lower 31/32nds (31 sectors: 0 to 30)       |
/* |   0   |   1   |   0   | Upper sixteenth (two sectors: 30 and 31) | Lower 15/16ths (30 sectors: 0 to 29)       |
/* |   0   |   1   |   1   | Upper eighth (four sectors: 28 to 31)    | Lower seven-eighths (28 sectors: 0 to 27)  |
/* |   1   |   0   |   0   | Upper quarter (eight sectors: 24 to 31)  | Lower three-quarters (24 sectors: 0 to 23) |
/* |   1   |   0   |   1   | Upper half (sixteen sectors: 16 to 31)   | Lower half (16 sectors: 0 to 15)           |
/* |   1   |   1   |   0   | All sectors (32 sectors: 0 to 31)        | none                                       |
/* |   1   |   1   |   1   | All sectors (32 sectors: 0 to 31)        | none                                       |
/* -----------------------------------------------------------------------------------------------------------------
/* Table 3. Memory Organization
/* ------------------------------
/* | Sector | Address |  Range  |
/* ------------------------------
/* |   31   | 1F0000h | 1FFFFFh |
/* |   30   | 1E0000h | 1EFFFFh |
/* |   29   | 1D0000h | 1DFFFFh |
/* |   28   | 1C0000h | 1CFFFFh |
/* |   27   | 1B0000h | 1BFFFFh |
/* |   26   | 1A0000h | 1AFFFFh |
/* |   25   | 190000h | 19FFFFh |
/* |   24   | 180000h | 18FFFFh |
/* |   23   | 170000h | 17FFFFh |
/* |   22   | 160000h | 16FFFFh |
/* |   21   | 150000h | 15FFFFh |
/* |   20   | 140000h | 14FFFFh |
/* |   19   | 130000h | 13FFFFh |
/* |   18   | 120000h | 12FFFFh |
/* |   17   | 110000h | 11FFFFh |
/* |   16   | 100000h | 10FFFFh |
/* |   15   | 0F0000h | 0FFFFFh |
/* |   14   | 0E0000h | 0EFFFFh |
/* |   13   | 0D0000h | 0DFFFFh |
/* |   12   | 0C0000h | 0CFFFFh |
/* |   11   | 0B0000h | 0BFFFFh |
/* |   10   | 0A0000h | 0AFFFFh |
/* |   9    | 090000h | 09FFFFh |
/* |   8    | 080000h | 08FFFFh |
/* |   7    | 070000h | 07FFFFh |
/* |   6    | 060000h | 06FFFFh |
/* |   5    | 050000h | 05FFFFh |
/* |   4    | 040000h | 04FFFFh |
/* |   3    | 030000h | 03FFFFh |
/* |   2    | 020000h | 02FFFFh |
/* |   1    | 010000h | 01FFFFh |
/* |   0    | 000000h | 00FFFFh |
/* ------------------------------
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  5/5/2011(RyanH):                                                    */
/*                                                                      */
/************************************************************************/

#include "pmodsf.h"


/* ------------------------------------------------------------ */
/** fnPmodSFsendCommand
**
**	Synopsis:
**  Sends a command to PMOSF to be executed based on the 
**  PMODSFCOMMAND struct, relevant fields and value returned
**  are detailed below:
**	Input: PMODSFCOMMAND *command
**	Description:
**  The following commands have been implemented for the
**  PMODSF
** 
**  Required for all commands:
**  	PMODFCOMMAND.ucSpiChannel: SPICHANNEL enum
**
**  Commands with reqiured fields and values returned:
**		IN=> PMODSFCOMMAND.ucInstruction: PMODSF_READ_ID
**				OUT=> PMODSFCOMMAND.ucID  
**				OUT=> PMODSFCOMMAND.ucMemType
**				OUT=> PMODSFCOMMAND.ucMemCapacity 
**		IN=> PMODSFCOMMAND.ucInstruction: PMODSF_WRITE_ENABLE
**			 OUT=> NONE
**		IN=> PMODSFCOMMAND.ucInstruction: PMODSF_WRITE_DISABLE
**			 OUT=> NONE
**		IN=> PMODSFCOMMAND.ucInstruction: PMODSF_READ_STATUS_REG
**			 OUT=> PMODSFCOMMAND.ucStatusRegister
**		IN=> PMODSFCOMMAND.ucInstruction: PMODSF_WRITE_STATUS_REG
**			 IN=> PMODSFCOMMAND.ucStatusRegister
**			 OUT=> NONE
**		IN=> PMODSFCOMMAND.ucInstruction: PMODSF_READ_DATA_BYTES
**			 IN=> PMODSFCOMMAND.ulReadAddress : 24 bit address value, begin read from here
**			 IN=> PMODSFCOMMAND.ucNumBytesReadWrite: number of bytes to read
**			 OUT=> PMODSFCOMMAND.data: bytes read from PMODSF
**		IN=> PMODSFCOMMAND.ucInstruction: PMODSF_PAGE_PGM
**			 IN=> PMODSFCOMMAND.ulWriteAddress: 24 byte address, begin write from here
**			 IN=> PMODSFCOMMAND.ucNumBytesReadWrite: number of bytes to write
**			 IN=> PMODSFCOMMAND.data: data to write to PMODSF
**	Errors:
**		none
**
*/
void fnPmodSFsendCommand(PMODSFCOMMAND *command)
{
	uint8_t tempInstruction;
	switch(command->ucInstruction)
	{
		case PMODSF_READ_ID:
			fnPmodSFReadID(command);
			break;
		case PMODSF_WRITE_ENABLE:
			fnPmodSFCommandNoReturn(command);
			break;
		case PMODSF_WRITE_DISABLE:
			fnPmodSFCommandNoReturn(command);
			break;
		case PMODSF_READ_STATUS_REG:
			fnPmodSFReadStatusRegister(command);
			break;
		case PMODSF_WRITE_STATUS_REG:
			tempInstruction = command->ucInstruction;
			command->ucInstruction = PMODSF_WRITE_ENABLE;
			fnPmodSFCommandNoReturn(command); //enable write to status register
 			command->ucInstruction = tempInstruction;
			fnPmodSFWriteStatusRegister(command); //write to status register
			break;
		case PMODSF_READ_DATA_BYTES:
			fnPmodSFReadBytes(command);
			break;
		case PMODSF_READ_DATA_BYTES_HS:
			break;
		case PMODSF_PAGE_PGM:
			 fnPmodSFPageProgram(command);
			break;
		case PMODSF_SECTOR_ERASE:
			break;
		case PMODSF_BULK_ERASE:
			break;
		case PMODSF_DEEP_POWER_DOWN:
			break;
		case PMODSF_RELEASE_FROM_DPD:
			break;
	}
}

/* ------------------------------------------------------------ */
/** fnPmodSFReadStatusRegister
**
**	Synopsis:
**  Sends a command to a PMODSF that reads the status register and 
**  returns its value in the PMODSFCOMMAND struct passed in
**	Input: PMODSFCOMMAND *command
** 
**  Required fields:
**  	IN=> PMODFCOMMAND.ucSpiChannel: SPICHANNEL enum		   
**		IN=> PMODSFCOMMAND.ucInstruction: PMODSF_READ_STATUS_REG
**  	OUT=> PMODSFCOMMAND.ucStatusRegister
**	Errors:
**		none
** Description from the M25P16 reference manual:
** Read Status Register (RDSR)
** The Read Status Register (RDSR) instruction allows
** the Status Register to be read. The Status
** Register may be read at any time, even while a
** Program, Erase or Write Status Register cycle is in
** progress. When one of these cycles is in progress,
** it is recommended to check the Write In Progress
** (WIP) bit before sending a new instruction to the
** device. It is also possible to read the Status Register
** continuously, as shown in Figure 12..
** Table 6. Status Register Format
**   b7                                    b0
** |SRWD| 0 | 0 | BP2 | BP1 | BP0 | WEL | WIP |
** The status and control bits of the Status Register
** are as follows:
** WIP bit. The Write In Progress (WIP) bit indicates
** whether the memory is busy with a Write Status
** Register, Program or Erase cycle. When set to 1,
** such a cycle is in progress, when reset to 0 no
** such cycle is in progress.
** WEL bit. The Write Enable Latch (WEL) bit indicates
** the status of the internal Write Enable Latch.
** When set to 1 the internal Write Enable Latch is
** set, when set to 0 the internal Write Enable Latch
** is reset and no Write Status Register, Program or
** Erase instruction is accepted.
** BP2, BP1, BP0 bits. The Block Protect (BP2,
** BP1, BP0) bits are non-volatile. They define the
** size of the area to be software protected against
** Program and Erase instructions. These bits are
** written with the Write Status Register (WRSR) instruction.
** When one or both of the Block Protect
** (BP2, BP1, BP0) bits is set to 1, the relevant memory
** area (as defined in Table 2.) becomes protected
** against Page Program (PP) and Sector Erase
** (SE) instructions. The Block Protect (BP2, BP1,
** BP0) bits can be written provided that the Hardware
** Protected mode has not been set. The Bulk
** Erase (BE) instruction is executed if, and only if,
** both Block Protect (BP2, BP1, BP0) bits are 0.
** SRWD bit. The Status Register Write Disable
** (SRWD) bit is operated in conjunction with the
** Write Protect (W) signal. The Status Register
** Write Disable (SRWD) bit and Write Protect (W)
** signal allow the device to be put in the Hardware
** Protected mode (when the Status Register Write
** Disable (SRWD) bit is set to 1, and Write Protect
** (W) is driven Low). In this mode, the non-volatile
** bits of the Status Register (SRWD, BP2, BP1,
** BP0) become read-only bits and the Write Status
** Register (WRSR) instruction is no longer accepted
** for execution.
*/
void fnPmodSFReadStatusRegister(PMODSFCOMMAND *command)
{
	uint8_t *spiBuf = SpiChnBuffer(command->ucSpiChannel);
    uint8_t ucGarbage;
	fnSPISetSSLow(command->ucSpiChannel); //SS to low
 
   *spiBuf = command->ucInstruction;
    while(!getSPIRcvBufStatus(command->ucSpiChannel));   //Wait for transfer complete
    ucGarbage = *spiBuf;                       //first byte will be garbage
    
	*spiBuf=0x00; //triger xfer
    while(!getSPIRcvBufStatus(command->ucSpiChannel));
	command->ucStatusRegister = *spiBuf;   

    fnSPISetSSHigh(command->ucSpiChannel); //SS to High
}
/* ------------------------------------------------------------ */
/** fnPmodSFWriteStatusRegister
**
**	Synopsis:
**  Sends a command to a PMODSF that writes to the status register 
**	Input: PMODSFCOMMAND *command
** 
**  Required fields:
**  	IN=> PMODFCOMMAND.ucSpiChannel: SPICHANNEL enum		   
**		IN=> PMODSFCOMMAND.ucInstruction: PMODSF_WRITE_STATUS_REG
**  	IN=> PMODSFCOMMAND.ucStatusRegister
**	Errors:
**		none
** Description from the M25P16 reference manual:
** Write Status Register (WRSR)
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
** The instruction sequence is shown in Figure 13..
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
**
*/
void fnPmodSFWriteStatusRegister(PMODSFCOMMAND *command)
{
    PMODSFCOMMAND enableWrite;
   	uint8_t *spiBuf = SpiChnBuffer(command->ucSpiChannel);
    uint8_t ucGarbage;
 
	enableWrite.ucInstruction = PMODSF_WRITE_ENABLE;

	fnPmodSFsendCommand(&enableWrite); //enable writing for Write Status Register

	fnSPISetSSLow(command->ucSpiChannel); //SS to low
 
   *spiBuf = command->ucInstruction;
    while(!getSPIRcvBufStatus(command->ucSpiChannel));   //Wait for transfer complete
    ucGarbage = *spiBuf;                       //first byte will be garbage
    
	*spiBuf= command->ucStatusRegister; //triger xfer
    while(!getSPIRcvBufStatus(command->ucSpiChannel));
	ucGarbage = *spiBuf;   

    fnSPISetSSHigh(command->ucSpiChannel); //SS to High
}

/** fnPmodSFCommandNoReturn
**
**	Synopsis:
**  Generic send command function that sends a command to a 
**	PMODSF that provides no return values 
**	Input: PMODSFCOMMAND *command
** 
**  Required fields:
**  	IN=> PMODFCOMMAND.ucSpiChannel: SPICHANNEL enum		   
**		IN=> PMODSFCOMMAND.ucInstruction: PMODSF_WRITE_STATUS_REG
**	Errors:
**		none
**
*/
void fnPmodSFCommandNoReturn(PMODSFCOMMAND *command)
{
	uint8_t *spiBuf = SpiChnBuffer(command->ucSpiChannel);
    uint8_t ucGarbage;
	fnSPISetSSLow(command->ucSpiChannel); //SS to low
 
   *spiBuf = command->ucInstruction;
    while(!getSPIRcvBufStatus(command->ucSpiChannel));   //Wait for transfer complete
    ucGarbage = *spiBuf;                       //first byte will be garbage

    fnSPISetSSHigh(command->ucSpiChannel); //SS to High
}

/** fnPmodSFReadID
**
**	Synopsis:
** The Read Identification (RDID) instruction allows
** the 8-bit manufacturer identification to be read
**	PMODSF that provides no return values 
**	Input: PMODSFCOMMAND *command
** 
**		IN=> PMODSFCOMMAND.ucInstruction: PMODSF_READ_ID
**				OUT=> PMODSFCOMMAND.ucID  
**				OUT=> PMODSFCOMMAND.ucMemType
**				OUT=> PMODSFCOMMAND.ucMemCapacity 
**	Errors:
**		none
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
void fnPmodSFReadID(PMODSFCOMMAND *command)
{
	uint8_t *spiBuf = SpiChnBuffer(command->ucSpiChannel);
    uint8_t ucGarbage;
	fnSPISetSSLow(command->ucSpiChannel); //SS to low
 
   *spiBuf = command->ucInstruction;
    while(!getSPIRcvBufStatus(command->ucSpiChannel));   //Wait for transfer complete
    ucGarbage = *spiBuf;                       //first byte will be garbage
    
	*spiBuf=0x00; //triger xfer
    while(!getSPIRcvBufStatus(command->ucSpiChannel));
	command->ucID= *spiBuf;   
	
    *spiBuf=0x00; //triger xfer
    while(!getSPIRcvBufStatus(command->ucSpiChannel));
	command->ucMemType = *spiBuf;

    *spiBuf=0x00; //triger xfer
    while(!getSPIRcvBufStatus(command->ucSpiChannel));
	command->ucMemCapacity = *spiBuf;
 
    fnSPISetSSHigh(command->ucSpiChannel); //SS to High
}

/** fnSPIConfigureChannelMasterForPMODSF
** Synopsis:
**   Enabled digial IO  and Sets up the SPI master on the channel specified at the desired baud 
**   rate in framed, 8 bit  mode. Baud rate must not exceed 50Mhz for PMODSF, See PIC32 Familiy
**   Reference Manual Chapter 23 section 23.3.7 for more information, an exerpt from this section 
**   is provided below.
** Input: 
**  SPICHANNEL spiChannel - spi chanel to setup for PMODSF
**  uint32_t ulClock - System clock in Hz
**  uint32_t ulBaud  - baud in kHz
**
** --------------------------------------------------------------------------------------------------
** | Excperpts from PIC32 Familiy Reference Manual Chapter 23 section 23.3.7                        |
** --------------------------------------------------------------------------------------------------
** | The following equation defines the SCKx clock frequency as a function of SPIxBRG settings      |
** |  Fsck = (Fpb) / 2 * (SPIxBRK + 1)                                                              |
** |                                                                                                |
** --------------------------------------------------------------------------------------------------
** |                                       Sample SCKx Frequencies                                  |
** --------------------------------------------------------------------------------------------------
** | SPIxBRG Setting   |    0      |    15      |     31     |     63     |     85     |     127    |
** --------------------------------------------------------------------------------------------------
** | FPB = 50 MHz      | 25.00 MHz | 1.56 MHz   | 781.25 kHz | 390.63 kHz | 290.7 kHz  | 195.31 kHz |
** | FPB = 40 MHz      | 20.00 MHz | 1.25 MHz   | 625.00 kHz | 312.50 kHz | 232.56 kHz | 156.25 kHz |
** | FPB = 25 MHz      | 12.50 MHz | 781.25 kHz | 390.63 kHz | 195.31 kHz | 145.35 kHz | 97.66 kHz  |
** | FPB = 20 MHz      | 10.00 MHz | 625.00 kHz | 312.50 kHz | 156.25 kHz | 116.28 kHz | 78.13 kHz  |
** | FPB = 10 MHZ      | 5.00 MHz  | 312.50 kHz | 156.25 kHz | 78.13 kHz  | 58.14 kHz  | 39.06 kHz  |
** --------------------------------------------------------------------------------------------------
*/
void fnSPIConfigureChannelMasterForPMODSF(SPICHANNEL spiChannel,uint32_t ulClock,uint32_t ulBaud)
{
    fnSPIEnableDigitalPinIO(spiChannel);
    fnSPIConfigureChannelMasterWithFrames(ulClock,spiChannel,SPI_8BIT_MODE,ulBaud);
}


/** fnPmodSFPageProgram
**
**	Synopsis:
**  The Page Program (PP) instruction allows bytes to
**  be programmed in the memory (changing bits from
**  1 to 0).
**	Input: PMODSFCOMMAND *command
** 
**		IN=> PMODSFCOMMAND.ucInstruction: PMODSF_PAGE_PGM
**			 IN=> PMODSFCOMMAND.ulWriteAddress: 24 byte address, begin write from here
**			 IN=> PMODSFCOMMAND.ucNumBytesReadWrite: number of bytes to write
**			 IN=> PMODSFCOMMAND.data: data to write to PMODSF
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
void fnPmodSFPageProgram(PMODSFCOMMAND *command)
{
	PMODSFCOMMAND setupForPage;
	uint8_t ucGarbage;
	uint8_t *spiBuf = SpiChnBuffer(command->ucSpiChannel);
	uint8_t byteCounter = 0;

	setupForPage.ucInstruction = PMODSF_READ_STATUS_REG;
    setupForPage.ucSpiChannel = command->ucSpiChannel;
	fnPmodSFReadStatusRegister(&setupForPage);
	//CHECK FOR WRITE IN PROGRESS	
	while((setupForPage.ucStatusRegister & PMODSF_SR_WIP) == PMODSF_SR_WIP) 
	{
		fnPmodSFReadStatusRegister(&setupForPage);	
	}
	//SET WRITE ENABLE BIT
	setupForPage.ucInstruction = PMODSF_WRITE_ENABLE; 
	fnPmodSFsendCommand(&setupForPage);	

	setupForPage.ucInstruction = PMODSF_READ_STATUS_REG;
	setupForPage.ucStatusRegister = 0;
	fnPmodSFReadStatusRegister(&setupForPage);//WEL BIT

	//SEND COMMAND FOR PAGE PROGRAM OPERATION
	fnSPISetSSLow(command->ucSpiChannel); //SS to low
	
   *spiBuf = command->ucInstruction;
    while(!getSPIRcvBufStatus(command->ucSpiChannel));   //Wait for transfer complete
    ucGarbage = *spiBuf;                       // byte will be garbage
	
	//SEND IN THE 24 BIT ADDRESS
	for(byteCounter = 0;byteCounter < 3;byteCounter++)    
	{
		*spiBuf = fnGetByteFromUint32(command->ulWriteAddress,byteCounter);
		while(!getSPIRcvBufStatus(command->ucSpiChannel));   //Wait for transfer complete
    	ucGarbage = *spiBuf;                       //byte will be garbage
	}
	
	//TRANSFER BYTES TO BE WRITTEN TO PMODSF

	for(byteCounter = 0;byteCounter < command->ucNumBytesReadWrite ;byteCounter++)    
	{
		*spiBuf = command->data[byteCounter];
		while(!getSPIRcvBufStatus(command->ucSpiChannel));   //Wait for transfer complete
    	ucGarbage = *spiBuf;                       //byte will be garbage
	}
	
	fnSPISetSSHigh(command->ucSpiChannel); //SS to High

}

/** fnPmodSFReadBytes
**
**	Synopsis:
**  The Read Data bytes  instruction allows bytes to
**  be read from memory into the data field of the PMODSFCOMMAND.data
**	Input: PMODSFCOMMAND *command
** 
**		IN=> PMODSFCOMMAND.ucInstruction: PMODSF_READ_DATA_BYTES
**			 IN=> PMODSFCOMMAND.ulReadAddress : 24 bit address value, begin read from here
**			 IN=> PMODSFCOMMAND.ucNumBytesReadWrite: number of bytes to read
**			 OUT=> PMODSFCOMMAND.data: bytes read from PMODSF
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
void fnPmodSFReadBytes(PMODSFCOMMAND *command)
{
	PMODSFCOMMAND setupForPage;
	uint8_t ucGarbage;
	uint8_t *spiBuf = SpiChnBuffer(command->ucSpiChannel);
	uint8_t byteCounter = 0;

	setupForPage.ucInstruction = PMODSF_READ_STATUS_REG;
    setupForPage.ucSpiChannel = command->ucSpiChannel;
	fnPmodSFReadStatusRegister(&setupForPage);
	//CHECK FOR WRITE IN PROGRESS	
	while((setupForPage.ucStatusRegister & PMODSF_SR_WIP) == PMODSF_SR_WIP) 
	{
		fnPmodSFReadStatusRegister(&setupForPage);	
	}

	//SEND COMMAND FOR PAGE PROGRAM OPERATION
	fnSPISetSSLow(command->ucSpiChannel); //SS to low
	
   *spiBuf = command->ucInstruction;
    while(!getSPIRcvBufStatus(command->ucSpiChannel));   //Wait for transfer complete
    ucGarbage = *spiBuf;                       //byte will be garbage
	
	//SEND IN THE 24 BIT ADDRESS
	for(byteCounter = 0;byteCounter < 3;byteCounter++)    
	{
		*spiBuf = fnGetByteFromUint32(command->ulReadAddress,byteCounter);
		while(!getSPIRcvBufStatus(command->ucSpiChannel));   //Wait for transfer complete
    	ucGarbage = *spiBuf;                       //byte will be garbage
	}
	
	//GET BYTES FROM PMODSF

	for(byteCounter = 0;byteCounter < command->ucNumBytesReadWrite ;byteCounter++)    
	{
		*spiBuf = 0;
		while(!getSPIRcvBufStatus(command->ucSpiChannel));   //Wait for transfer complete
    	command->data[byteCounter] = *spiBuf;                       //byte will be garbage
	}
	
	fnSPISetSSHigh(command->ucSpiChannel); //SS to High

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
uint8_t fnGetByteFromUint32(uint32_t value,uint8_t bytePos)
{
	return value >> (bytePos * 8) & 255;
}