/************************************************************************/
/*	pmodACL.c	-- PmodACL implementation file   						*/
/*																		*/
/************************************************************************/
/*	Author: 	Ryan Hoffman 											*/
/*	Copyright (C) 2011 Ryan Hoffman										*/
/************************************************************************/
/*  Module Description: 												*/
/*  PmodACL implimenation file											*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/* <11/14/11>(Ryan H): Initial Release									*/
/*																		*/
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <math.h>
#include "./PmodACL/pmodACL.h"
#include "./PmodCommon/spi/pmod_spi_common.h"

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

/*  
**  PmodACLInitSpi
**
**	Synopsis:
**
**  Initializes the PmodACL for SPI
**
**  Input: 
**		SpiChannel chn - Spi Channel
**		uint32_t pbClock - peripheral bus clock frequency in Hz
**		uint32_t bitRate - bit rate desired in Hz
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  
**  Initalizes the PmodACL for SPI 4 wire in Mode 3 as follows:
**  master, enable slave select, 8 bit mode, CKP high  
**  (SPI Mode 3 -> CKP-1 CKE-0)
**   
**  Notes:
**
**  (Taken from ADXL345 Reference Manual)
**  Use of the 3200 Hz and 1600 Hz output data rates is only recommended
**  with SPI communication rates greater than or equal to 2 MHz. 
**  The 800 Hz output data rate is recommended only for communication speeds 
**  greater than or equal to 400 kHz, and the remaining data rates scale proportionally.
**  For example, the minimum recommended communication speed for a 200 Hz output data 
**  rate is 100 kHz. Operation at an output data rate above the recommended maximum may 
**  result in undesirable effects on the acceleration data, including missing samples 
**  or additional noise.
*/
void PmodACLInitSpi(SpiChannel chn,uint32_t pbClock,uint32_t bitRate)
{
	//Open master, enable slave select, 8 bit, CKP high  (SPI Mode 3 -> CKP-1 CKE-0)
	SpiChnOpen(chn, SPI_OPEN_MSTEN | SPI_OPEN_SSEN |  SPI_OPEN_MODE8 | SPI_OPEN_CKP_HIGH , pbClock/bitRate);
	//Set SPI 4 wire mode
	PmodACLSetDataFormat(chn,(PmodACLGetDataFormat(chn) & ~(PMODACL_BIT_DATA_FORMAT_SPI)));
}

/*  
**  PmodACLGetAxisData
**
**	Synopsis:
**
**	Reads the values from the axis values and writes them
**  to a PMODACL_AXIS struct 
**
**  Input: 
**  SpiChannel chn - Spi Channel
**  PMODACL_AXIS *pmodACLAxis - pointer to data structure 
**                              used to store axis data
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**	A multibyte read of the axis registers is performed, low and high
**  are shifted and combined to create 16 bit signed values then
**  stored in the corresponding axis field in pmodACLAxis.
**  (Taken from ADXL345 Reference Manual)
**  These six bytes (Register 0x32 to Register 0x37) are eight bits 
**  each and hold the output data for each axis. Register 0x32 and 
**  Register 0x33 hold the output data for the x-axis, Register 0x34 
**  and Register 0x35 hold the output data for the y-axis, and Register 
**  0x36 and Register 0x37 hold the output data for the z-axis. 
**  The output data is twos complement, with DATAx0 as the least 
**  significant byte and DATAx1 as the most significant byte, where 
**  x represent X, Y, or Z. The DATA_FORMAT register (Address 0x31)
**  controls the format of the data. It is recommended that a multiple-byte
**  read of all registers be performed to prevent a change in data between 
**  reads of sequential registers.
*/
void PmodACLGetAxisData(SpiChannel chn, PMODACL_AXIS *pmodACLAxis)
{
	uint8_t axisData[6]; //5 bytes in axis data read

	//retrieve axis data in a multi-byte read
	PmodACLReadRegMultiByte(chn,PMODACL_REG_DATAX0,axisData,PMODACL_NUM_AXIS_REGISTERS);
 
	//populate axis values 
	pmodACLAxis->xAxis = (int16_t)axisData[1] << 8 | (uint16_t)axisData[0];

	pmodACLAxis->yAxis = (int16_t)axisData[3] << 8 | (uint16_t)axisData[2];
 
	pmodACLAxis->zAxis = (int16_t)axisData[5] << 8 | (uint16_t)axisData[4];
}

/*  
**  PmodACLReadRegMultiByte
**
**	Synopsis:
**
**	Reads a series of bytes starting at a base register address
**
**  Input: 
**  SpiChannel chn - Spi Channel
**	uint8_t startAddress - register start address
**  uint8_t *data - pointer to array of uint8_t, register values are stored here
**  uint8_t numBytes - number of bytes to read
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  Reads a series of bytes starting at a base register address and
**  stores them in a array. 
**
**  Example: 
**  uint8_t data[PMODACL_NUM_OFSXYZ_OFFSET_BYTES]
**  PmodACLReadRegMultiByte(SPI2,PMODACL_REG_OFSX,data,PMODACL_NUM_OFFSET_BYTES)
**  This preceeding will start at register PMODACL_REG_OFSX and read PMODACL_NUM_OFFSET_BYTES
**  into corresponding indices in data, incrementing register address by 1 each time.
**  This function is best used when multiple contiguous values must be read in an
**  atomically before register values change.
*/
void PmodACLReadRegMultiByte(SpiChannel chn,uint8_t startAddress,uint8_t *data,uint8_t numBytes)
{
	uint8_t byteIndex = 0;
	
	PmodSPISetSSLow(chn);
	
	//Set multibyte bit and start register, get first byte
	SpiChnPutC(chn,PMODACL_READ_BIT | PMODACL_MB_BIT | startAddress); 
	SpiChnGetC(chn);
	
	//Get data bytes from registers
	for(byteIndex = 0;byteIndex < numBytes;byteIndex++)
	{
		SpiChnPutC(chn,0); 
		data[byteIndex] = SpiChnGetC(chn); 
	}

	PmodSPISetSSHigh(chn);
}

/*  
**  PmodACLWriteRegMultiByte
**
**	Synopsis:
**
**  Writes a series of bytes starting at a base register address
**
**  Input: 
**  SpiChannel chn - Spi Channel
**	uint8_t startAddress - register start address
**  uint8_t *data - pointer to array of uint8_t, new register values
**  uint8_t numBytes - number of bytes to write
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  Writes a series of bytes starting at a base register address and
**  from an array.
**
**  Example: 
**  uint8_t data[PMODACL_NUM_OFSXYZ_OFFSET_BYTES] = {1,2,3}
**  PmodACLWriteRegMultiByte(SPI2,PMODACL_REG_OFSX,data,PMODACL_NUM_OFFSET_BYTES)
**  This preceeding will start at register PMODACL_REG_OFSX and write PMODACL_NUM_OFFSET_BYTES
**  from corresponding indices in data to PMODACL_NUM_OFFSET_BYTES registers, the address is increased
**  by 1 each time a byte is writen. This function is best used when multiple contiguous values must be 
**  written atomically for a specific command.
*/
void PmodACLWriteRegMultiByte(SpiChannel chn,uint8_t startAddress,uint8_t *data,uint8_t numBytes)
{
	uint8_t byteIndex = 0;
	
	PmodSPISetSSLow(chn);
	
	//Set multibyte bit and start register, get first byte
	SpiChnPutC(chn,PMODACL_MB_BIT | startAddress); 
	SpiChnGetC(chn);
	
	//Get data bytes from registers
	for(byteIndex = 0;byteIndex < numBytes;byteIndex++)
	{
		SpiChnPutC(chn,data[byteIndex]); 
		SpiChnGetC(chn); 
	}

	PmodSPISetSSHigh(chn);
}

/*  
**  PmodACLWriteReg
**	
**	Synopsis:
**  
**  Writes a value to the specified address(register)
**
**  Input: 
**		SpiChannel chn - Spi Channel
**  	uint8_t address - address of register to write
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**	Writes a value to the register specified in the address parameter,
**  valid register address are located in the "Local Type Declarations" 
**  section of the header and are prefixed with PMODACL_REG
**
*/
void PmodACLWriteReg(SpiChannel chn,uint8_t address,uint8_t dataBits)
{	
	PmodSPISetSSLow(chn);
	SpiChnPutC(chn, address); 
	SpiChnGetC(chn);
	SpiChnPutC(chn,dataBits);
	SpiChnGetC(chn);
	PmodSPISetSSHigh(chn);
}

/*  
**  PmodACLReadReg
**
**	Synopsis:
**
**	Reads the value from the specified address(register) and
**  returns it as an 8 bit unsigned int
**
**  Input: 
**		SpiChannel chn - Spi Channel
**  	uint8_t address - address of register to read
**
**  Returns: 
**		uint8_t - value stored in register
**
**	Errors:	none
**
**  Description:
**
**	Reads and returns a value from the register specified in
**  the address parameter, valid register address are located
**  in the "Local Type Declarations" section of the header
**  and are prefixed with PMODACL_REG
*/
uint8_t PmodACLReadReg(SpiChannel chn,uint8_t address)
{
	uint8_t registerValue = 0;

	PmodSPISetSSLow(chn);

	SpiChnPutC(chn,PMODACL_READ_BIT | address); 
	SpiChnGetC(chn);

	SpiChnPutC(chn,0);
	registerValue = SpiChnGetC(chn);

	PmodSPISetSSHigh(chn);

	return registerValue;
}
