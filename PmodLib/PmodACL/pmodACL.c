/************************************************************************/
/*	pmodACL.c	-- PmodACL implimentation file   						*/
/*																		*/
/************************************************************************/
/*	Author: 	Ryan Hoffman 											*/
/*	Copyright (C) 2011 Ryan Hoffman										*/
/************************************************************************/
/*  Module Description: 												*/
/*  <MODULE DESCRIPTION>												*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/* <MM/DD/YY>(<FIRST NAME><LAST INITIAL): <NOTES>						*/
/*																		*/
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <plib.h>
#include <math.h>
#include "./PmodACL/pmodACL.h"
#include "./PmodCommon/spi/pmod_spi_common.h"

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

/*  
** <FUNCTION NAME>
**
**	Synopsis:
**
**  Input: 
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
*/
void PmodACLInitSpi4Wire(SpiChannel chn,uint32_t pbClock,uint32_t bitRate)
{
    SpiChnOpen(chn, SPI_OPEN_MSTEN | SPI_OPEN_SSEN |  SPI_OPEN_MODE8 | SPI_OPEN_CKP_HIGH , pbClock/bitRate);
	
}
/*  
** <FUNCTION NAME>
**
**	Synopsis:
**
**  Input: 
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
*/
void PmodACLGetAxisData(SpiChannel chn, PMODACL_AXIS *pmodACLAxis)
{
	uint8_t axisData[6]; //5 bytes in axis data read

	//retrieve axis data in a multi-byte read
	PmodACLReadRegMultiByte(chn,PMODACL_REG_DATAX0,axisData,6);
 
	//populate axis values 
	pmodACLAxis->xAxis = (int16_t)axisData[1] << 8 | (uint16_t)axisData[0];

	pmodACLAxis->yAxis = (int16_t)axisData[3] << 8 | (uint16_t)axisData[2];
 
	pmodACLAxis->zAxis = (int16_t)axisData[5] << 8 | (uint16_t)axisData[4];
}
/*  
** <FUNCTION NAME>
**
**	Synopsis:
**
**  Input: 
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
*/
void PmodACLReadRegMultiByte(SpiChannel chn,uint8_t startAddress,uint8_t *data,uint8_t numBytes)
{
	uint8_t byteIndex = 0;
	
	PmodSPISetSSLow(chn);
	
	//Set multibyte bit and start register, get first byte
	SpiChnPutC(chn,PMODACL_RW_BIT | PMODACL_MB_BIT | startAddress); 
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
** <FUNCTION NAME>
**
**	Synopsis:
**
**  Input: 
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
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
** <FUNCTION NAME>
**
**	Synopsis:
**
**  Input: 
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
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
** <FUNCTION NAME>
**
**	Synopsis:
**
**  Input: 
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
*/
uint8_t PmodACLReadReg(SpiChannel chn,uint8_t address)
{
	uint8_t registerValue = 0;

	PmodSPISetSSLow(chn);

	SpiChnPutC(chn,PMODACL_RW_BIT | address); 
	SpiChnGetC(chn);

	SpiChnPutC(chn,0);
	registerValue = SpiChnGetC(chn);

	PmodSPISetSSHigh(chn);

	return registerValue;
}
/*  
** <FUNCTION NAME>
**
**	Synopsis:
**
**  Input: 
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
*/
int32_t PmodACLCalibrate(SpiChannel chn,uint8_t numSamples)
{
	PMODACL_AXIS pmodACLAxis;
	int8_t axisValues[3];
	int16_t xAxis = 0;
	int16_t yAxis = 0;
	int16_t zAxis = 0;
	uint8_t sampleCount = 0;
	int32_t offsetRegister = 0;

	for(sampleCount = 0;sampleCount <= numSamples;sampleCount++)
	{
		PmodACLGetAxisData(chn,&pmodACLAxis);
		xAxis += pmodACLAxis.xAxis;
		yAxis += pmodACLAxis.yAxis;
		zAxis += pmodACLAxis.zAxis;
	}

	axisValues[0] = -ceil((xAxis/(double)numSamples) / 4);
	axisValues[1] = -ceil((yAxis/(double)numSamples) / 4);
	axisValues[2] = -ceil (((zAxis/(double)numSamples) - 256) / 4);
	PmodACLSetOffset(chn,(uint8_t*)axisValues);
	
	offsetRegister = (int32_t)axisValues[0] << 16;
	offsetRegister |= (int32_t)axisValues[1] << 8;
	offsetRegister |= (int32_t)axisValues[2];
	
	return offsetRegister;
}
