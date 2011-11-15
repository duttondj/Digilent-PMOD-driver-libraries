/************************************************************************/
/*	pmodACLCalibrate.c	-- PmodACLCalibrate() implimentation file		*/
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
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */
#define SCALE_LSB_2G	 	0x02
#define SCALE_LSB_4G 		0x04
#define SCALE_10_BITS 		0x3FF

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */
/*  
**  PmodACLCalibrate
**
**	Synopsis:
**
**  Performs a calibration of the PmodACL axis
**  by utilizing the OFFSET register
**
**  Input: 
**  SpiChannel chn - Spi Channel
**	uint8_t numSamples -number of samples to take 
**                      during calibration
**
**  Returns: 
**		int32_t - signed integer representing calculated 
**					offset values
**
**	Errors:	none
**
**  Description:
**
**	Axis value sample are taken and averaged to achieve
**  a baseline reprentation of X axis - 0g, Y Axis - 0g
**  Z Axis - 1g. 
**
**  Notes:
**
**  For a full description of the calibration proceedure see 
**  "Offset Calibration" in the ADXL345 reference manual.
*/
int32_t PmodACLCalibrate(SpiChannel chn,uint8_t numSamples)
{
	PMODACL_AXIS pmodACLAxis;
	int8_t axisValues[3];
	uint8_t range = 0;
	int16_t xAxis = 0;
	int16_t yAxis = 0;
	int16_t zAxis = 0;
	uint16_t sensitivityLSBg = SCALE_LSB_4G; //FULL_RES mode, 4mg/LSB scale
	uint8_t sampleCount = 0;
	int32_t offsetRegister = 0;
	uint8_t dataFormat = PmodACLGetDataFormat(chn);
	
	//If data format is not in FULL_RES mode, set the range based on the Range Bits
	if(!(dataFormat & PMODACL_BIT_DATA_FORMAT_FULL_RES))
	{
		range = SCALE_LSB_2G << (dataFormat & PMODACL_MASK_DATA_FORMAT_RANGE);
	}
	
	//determine LSB/g resolution based on the 10bits / range
	sensitivityLSBg = (SCALE_10_BITS/range) + 1;
	
	//accumulate the value of the axis samples 
	for(sampleCount = 0;sampleCount <= numSamples;sampleCount++)
	{
		PmodACLGetAxisData(chn,&pmodACLAxis);
		xAxis += pmodACLAxis.xAxis;
		yAxis += pmodACLAxis.yAxis;
		zAxis += pmodACLAxis.zAxis;
	}

	//take the negative ceiling of each axis average and calculate the 
	//offset based on the range per mg/LSB, ADXL345 states rounding rather than ceil
	axisValues[0] = -ceil((xAxis/(double)numSamples) / range); //x Axis
	axisValues[1] = -ceil((yAxis/(double)numSamples) / range); //y Axis
	//subtract the calculated sensitivity since the Z axis should have 1g 
	axisValues[2] = -ceil (((zAxis/(double)numSamples) - sensitivityLSBg) / range); //z Axis
	PmodACLSetOffset(chn,(uint8_t*)axisValues);
	
	offsetRegister = (int32_t)axisValues[0] << 16;
	offsetRegister |= (int32_t)axisValues[1] << 8;
	offsetRegister |= (int32_t)axisValues[2];
	
	return offsetRegister;
}
