/************************************************************************/
/*	pmodACLCalibrate.c	-- PmodACLCalibrate() implimentation file		*/
/*																		*/
/************************************************************************/
/*	Author: 	Ryan Hoffman 											*/
/*	Copyright (C) 2011 Ryan Hoffman										*/
/************************************************************************/
/*  Module Description: 												*/
/*  PmodACL PmodACLCalibrate implimenation file							*/
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

#include <plib.h>
#include <math.h>
#include "./PmodACL/pmodACL.h"
#include "./PmodCommon/spi/pmod_spi_common.h"

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */
#define PMODACL_SCALE_LSB_2G	 	0x02   //2g per LSB scale
#define PMODACL_SCALE_LSB_4G 		0x04   //4g per LSB scale
#define PMODACL_SCALE_10_BITS 		0x3FF  //Used for determining bit resolution
								           //number of bits in axis register
#define PMODACL_NUM_ACL_AXIS        0x3

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
**  uint8_t oneGaxisOrienatation - 1G axis orientation during calibration
								   Acceptable values are prefixed with 
**                            	   PMODACL_CALIBRATE defined in
**							      "Local Type Definitions:Calibration Orientation"	
**
**  Returns:
** 
**		int32_t - signed integer representing calculated 
**				  offset values <AXIS:byte>:  X:2, Y:1, Z:0
**
**	Errors:	none
**
**  Description:
**
**	Axis value sample are taken and averaged to achieve
**  a baseline reprentation of all axes, the OFFSET register 
**  is then set to automatically adjust axis readings. 
**  PmodACL should placed such that one axis is in a
**  position to read 1g and the others 0g, typically
**  this is the Z axis. The 1G orientation axis is
**  specified using values defined in "Local Type Definitions
**  :Calibration Orientation"	
**
**  Notes:
**
**  For a full description of the calibration proceedure see 
**  "Offset Calibration" in the ADXL345 reference manual.
*/
int32_t PmodACLCalibrate(SpiChannel chn,uint8_t numSamples,uint8_t oneGaxisOrienatation)
{
	PMODACL_AXIS pmodACLAxis;
	int8_t offsetValues[3];
	uint8_t range = PMODACL_SCALE_LSB_4G;  //FULL_RES mode, 4mg/LSB scale
	int16_t xAxis = 0;
	int16_t yAxis = 0;
	int16_t zAxis = 0;
	uint16_t sensitivityLSBg[PMODACL_NUM_ACL_AXIS] = {0,0,0};
	uint8_t sampleCount = 0;
	int32_t offsetRegister = 0;
	uint8_t dataFormat = PmodACLGetDataFormat(chn);

	//If data format is not in FULL_RES mode, set the range based on the Range Bits
	if(!(dataFormat & PMODACL_BIT_DATA_FORMAT_FULL_RES))
	{
		range = PMODACL_SCALE_LSB_2G << (dataFormat & PMODACL_MASK_DATA_FORMAT_RANGE);
	}
	
	//determine LSB/g resolution based on the 10bits / range  (axis registers are 10 bits)
	sensitivityLSBg[oneGaxisOrienatation] = (PMODACL_SCALE_10_BITS/range) + 1;
	
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
	//1g sensitivity is subtracted from the axis that is oriented to 1g
	offsetValues[0] = -ceil(((xAxis/(double)numSamples) - sensitivityLSBg[PMODACL_CALIBRATE_X_AXIS])/range); //x Axis
	offsetValues[1] = -ceil(((yAxis/(double)numSamples) - sensitivityLSBg[PMODACL_CALIBRATE_Y_AXIS])/range); //y Axis
	offsetValues[2] = -ceil(((zAxis/(double)numSamples) - sensitivityLSBg[PMODACL_CALIBRATE_Z_AXIS])/range); //z Axis
	PmodACLSetOffset(chn,(uint8_t*)offsetValues);
	
	offsetRegister = (int32_t)offsetValues[PMODACL_CALIBRATE_X_AXIS] << 16;
	offsetRegister |= (int32_t)offsetValues[PMODACL_CALIBRATE_Y_AXIS] << 8;
	offsetRegister |= (int32_t)offsetValues[PMODACL_CALIBRATE_Z_AXIS];
	
	return offsetRegister;
}
