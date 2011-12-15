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
**  Input: SpiChannel chn  - Spi channel 
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
**	Input: SpiChannel chn - Spi channel
**         uint8_t statusReg - status register bits
** 
**  Returns: none
**
**	Errors: none
**
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
** 
**  Description:
**  See table PMODSF Status Register Format above for register values.
**
**  For a complete description see Write Status Register (WRSR) in the M25P16/M25P128 
**	reference manual 
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
**	Input: SpiChannel chn - Spi channel
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
**	Input: SpiChannel chn - Spi channel
**
**  Returns: uint8_t - 8-bit representation of the status register
**           (see "PMODSF Status Register Format")
**
**	Errors: none
**
**	Description:
**
**  For a complete description see Read Status Register (RDSR) in the M25P16/M25P128 
**	reference manual  
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
**  Enables writing to flash by setting the Write Enable Latch(WEL)
**  bit on the selected chanel
**
**	Input: SpiChannel chn - Spi channel
**
**  Returns: none
**
**	Errors: none
**
**  Description:
**
**  For a complete description see Write Enable (WREN) in the M25P16/M25P128 
**	reference manual 
*/
void PmodSFWriteEnable(SpiChannel chn)
{
	PmodSFSendCommand(chn,PMODSF_WRITE_ENABLE);
}

/* PmodSFSendCommand
**
**	Synopsis:
**
**  Sends a command to the PmodSF on the selected SPI channel,
**  use this fungtion only for commands that do not return values
**
**	Input: SpiChannel chn - Spi channel 
**     	   uint8_t command - 8-bit command from the "PMODSF INSTRUCTION SET"
**
**  Returns: none
**
**	Errors: none
**
**  Description:
**
**  SS is driven low, the 8 bit command is shifted out, 1 dummy byte
**  is shifted in, SS is driven high 
*/
void PmodSFSendCommand(SpiChannel chn,uint8_t command)
{
	PmodSPISetSSLow(chn); //SS to low 
    SpiChnPutC(chn,command); //send read status register command
	SpiChnGetC(chn);
	PmodSPISetSSHigh(chn); //SS to High
}

