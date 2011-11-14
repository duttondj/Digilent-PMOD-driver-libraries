/************************************************************************/
/*	pmodACL.h	-- PmodACL header file, public API						*/
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
#ifndef _PMOD_ACL_H_
#define _PMOD_ACL_H_

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <stdint.h>

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */

//Register address definitions
#define PMODACL_REG_DEVID  						0x00 
#define PMODACL_REG_THRESH_TAP 					0x1D 

/************************************/
/*        OFFSET                    */
/************************************/
#define PMODACL_REG_OFSX  						0x1E 
#define PMODACL_REG_OFSY  						0x1F 
#define PMODACL_REG_OFSZ  						0x20 
#define PMODACL_NUM_OFFSET_BYTES                0x03
/************************************/

#define PMODACL_REG_DUR  						0x21 
#define PMODACL_REG_LATENT  					0x22 
#define PMODACL_REG_WINDOW  					0x23 
#define PMODACL_REG_THRESH_ACT 				 	0x24 
#define PMODACL_REG_THRESH_INACT 				0x25 
#define PMODACL_REG_TIME_INACT  				0x26 

/************************************/
/*  PMODACL_REG_ACT_INACT_CTL       */
/************************************/
#define PMODACL_REG_ACT_INACT_CTL			    0x27 
#define PMODACL_BITS_ACT_INACT_CTL_ACT_ACDC	    0x80 
#define PMODACL_BITS_ACT_INACT_CTL_ACT_X	    0x40 
#define PMODACL_BITS_ACT_INACT_CTL_ACT_Y	    0x20 
#define PMODACL_BITS_ACT_INACT_CTL_ACT_Z	    0x10 
#define PMODACL_BITS_ACT_INACT_CTL_INACT_ACDC   0x08 
#define PMODACL_BITS_ACT_INACT_CTL_INACT_X	    0x04 
#define PMODACL_BITS_ACT_INACT_CTL_INACT_Y	    0x02 
#define PMODACL_BITS_ACT_INACT_CTL_INACT_Z	    0x01 
/************************************/

#define PMODACL_REG_THRESH_FF  					0x28 
#define PMODACL_REG_TIME_FF				  		0x29 

/************************************/
/*        TAP_AXES                 */
/************************************/
#define PMODACL_REG_TAP_AXES  					0x2A 
#define PMODACL_BIT_TAP_AXES_SUPRESS			0x08
#define PMODACL_BIT_TAP_AXES_TAP_X				0x04
#define PMODACL_BIT_TAP_AXES_TAP_Y				0x02
#define PMODACL_BIT_TAP_AXES_TAP_Z				0x01

/************************************/
/*        ACT_TAP_STATUS            */
/************************************/
#define PMODACL_REG_ACT_TAP_STATUS 				0x2B 
#define PMODACL_BIT_ACT_TAP_STATUS_ACT_X		0x40
#define PMODACL_BIT_ACT_TAP_STATUS_ACT_Y		0x20
#define PMODACL_BIT_ACT_TAP_STATUS_ACT_Z		0x10
#define PMODACL_BIT_ACT_TAP_STATUS_ASLEEP		0x08
#define PMODACL_BIT_ACT_TAP_STATUS_TAP_X		0x04
#define PMODACL_BIT_ACT_TAP_STATUS_TAP_Y		0x02
#define PMODACL_BIT_ACT_TAP_STATUS_TAP_Z		0x01

/************************************/
/*        BW_RATE                   */
/************************************/
#define PMODACL_REG_BW_RATE  					0x2C 
#define PMODACL_BIT_BW_RATE_LOW_POWER			0x10 
#define PMODACL_BIT_RATE_400HZ					0x0C
#define PMODACL_BIT_RATE_200HZ					0X0B
#define PMODACL_BIT_RATE_100HZ					0x0A	
#define PMODACL_BIT_RATE_50HZ					0x09
#define PMODACL_BIT_RATE_25HZ					0x08
#define PMODACL_BIT_RATE_12_5HZ					0x07
/************************************/
/*        POWER_CTL                 */
/************************************/
#define PMODACL_REG_POWER_CTL  					0x2D  //POWER_CTL register
#define PMODACL_BIT_POWER_CTL_LINK				0x20
#define PMODACL_BIT_POWER_CTL_AUTO_SLEEP		0x10
#define PMODACL_BIT_POWER_CTL_MEASURE			0x08
#define PMODACL_BIT_POWER_CTL_SLEEP				0x04
#define PMODACL_BIT_POWER_CTL_WAKEUP_8HZ		0x00
#define PMODACL_BIT_POWER_CTL_WAKEUP_4HZ		0x01
#define PMODACL_BIT_POWER_CTL_WAKEUP_2HZ		0x02
#define PMODACL_BIT_POWER_CTL_WAKEUP_1HZ		0x03

/************************************/
/*        INT_ENABLE                */
/************************************/
#define PMODACL_REG_INT_ENABLE  				0x2E 
#define PMODACL_BIT_INT_ENABLE_DATA_READY		0x80
#define PMODACL_BIT_INT_ENABLE_SINGLE_TAP		0x40
#define PMODACL_BIT_INT_ENABLE_DOUBLE_TAP		0x20
#define PMODACL_BIT_INT_ENABLE_ACTIVITY			0x10
#define PMODACL_BIT_INT_ENABLE_INACTIVITY		0x08
#define PMODACL_BIT_INT_ENABLE_FREE_FALL		0x04
#define PMODACL_BIT_INT_ENABLE_WATERMARK		0x02
#define PMODACL_BIT_INT_ENABLE_OVERRUN			0x01

/************************************/
/*        INT_MAP                   */
/************************************/
#define PMODACL_REG_INT_MAP  					0x2F 
#define PMODACL_BIT_INT_MAP_DATA_READY			0x80
#define PMODACL_BIT_INT_MAP_SINGLE_TAP			0x40
#define PMODACL_BIT_INT_MAP_DOUBLE_TAP			0x20
#define PMODACL_BIT_INT_MAP_ACTIVITY			0x10
#define PMODACL_BIT_INT_MAP_INACTIVITY			0x08
#define PMODACL_BIT_INT_MAP_FREE_FALL			0x04
#define PMODACL_BIT_INT_MAP_WATERMARK			0x02
#define PMODACL_BIT_INT_MAP_OVERRUN				0x01

/************************************/
/*        INT_SOURCE                */
/************************************/
#define PMODACL_REG_INT_SOURCE  				0x30 
#define PMODACL_BIT_INT_SOURCE_DATA_READY		0x80
#define PMODACL_BIT_INT_SOURCE_DOUBLE_TAP		0x60 //SingleTap OR DoubleTap (0x40 | 0x20) per ADXL345 Reference
#define PMODACL_BIT_INT_SOURCE_SINGLE_TAP		0x40
#define PMODACL_BIT_INT_SOURCE_ACTIVITY			0x10
#define PMODACL_BIT_INT_SOURCE_INACTIVITY		0x08
#define PMODACL_BIT_INT_SOURCE_FREE_FALL		0x04
#define PMODACL_BIT_INT_SOURCE_WATERMARK		0x02
#define PMODACL_BIT_INT_SOURCE_OVERRUN			0x01

/************************************/
/*        DATA_FORMAT               */
/************************************/
#define PMODACL_REG_DATA_FORMAT  				0x31 
#define PMODACL_BIT_DATA_FORMAT_SELF_TEST  		0x80 
#define PMODACL_BIT_DATA_FORMAT_SPI		  		0x40 
#define PMODACL_BIT_DATA_FORMAT_INT_INVERT 		0x20 
#define PMODACL_BIT_DATA_FORMAT_FULL_RES  		0x08 
#define PMODACL_BIT_DATA_FORMAT_JUSTIFY  		0x04 
#define PMODACL_BIT_DATA_FORMAT_RANGE_16G  		0x03 
#define PMODACL_BIT_DATA_FORMAT_RANGE_8G  		0x02 
#define PMODACL_BIT_DATA_FORMAT_RANGE_4G  		0x01 
#define PMODACL_BIT_DATA_FORMAT_RANGE_2G  		0x00
/************************************/

#define PMODACL_REG_DATAX0  					0x32 
#define PMODACL_REG_DATAX1  					0x33 
#define PMODACL_REG_DATAY0  					0x34 
#define PMODACL_REG_DATAY1  					0x35 
#define PMODACL_REG_DATAZ0  					0x36 
#define PMODACL_REG_DATAZ1  					0x37 

/************************************/
/*        FIFO_CTL                  */
/************************************/
#define PMODACL_REG_FIFO_CTL  					0x38 
#define PMODACL_BIT_FIFO_CTL_BYPASS				0x00
#define PMODACL_BIT_FIFO_CTL_FIFO				0x40
#define PMODACL_BIT_FIFO_CTL_STREAM				0x80
#define PMODACL_BIT_FIFO_CTL_TRIGGER			0xC0
#define PMODACL_BIT_FIFO_CTL_TRIGGER_INT2		0x10


/************************************/
/*        FIFO_STATUS               */
/************************************/
#define PMODACL_REG_FIFO_STATUS  				0x39 
#define PMODACL_BIT_FIFO_STATUS_FIFO_TRIG		0x80
#define PMODACL_BIT_MASK_FIFO_STATUS_ENTRIES	0x3F //bit mask for FIFO status entries

/************************************/

#define PMODACL_RW_BIT							0x80
#define PMODACL_MB_BIT              			0x40

#define PMODACL_DEVICE_ID						0xE5


typedef struct
{
	int16_t xAxis;
	int16_t yAxis;
	int16_t zAxis;
}PMODACL_AXIS;

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
void PmodACLInitSpi(SpiChannel chn,uint32_t pbClock,uint32_t bitRate);

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
void PmodACLGetAxisData(SpiChannel chn, PMODACL_AXIS *pmodACLAxis);

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
uint8_t PmodACLReadReg(SpiChannel chn,uint8_t address);

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
void PmodACLReadRegMultiByte(SpiChannel chn,uint8_t startAddress,uint8_t *data,uint8_t numBytes);

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
int32_t PmodACLCalibrate(SpiChannel chn,uint8_t numSamples);

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
void PmodACLWriteRegMultiByte(SpiChannel chn,uint8_t startAddress,uint8_t *data,uint8_t numBytes);

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
void PmodACLWriteReg(SpiChannel chn,uint8_t address,uint8_t dataBits);

/* ------------------------------------------------------------ */
/*					         MACROS	        					*/
/* ------------------------------------------------------------ */

/*  
**  PmodACLSetDataFormat
**
**	Synopsis:
**
**	Sets the resprentation of data to the registers defined in 
**  PMODACL_REG_DATA<AXIS><BIT>, sets SPI 3 or 4 wire mode, 
**  interrupt active high or low, full resolution mode, 
**  bit justification and range bits
**  
**  Input: 
**		SpiChannel CHN -  Spi channel
**      uint8_t DATA_FORMAT - Combination of ORed values prefixed with 
**                            PMODACL_BIT_DATA_FORMAT defined in
**							  "Local Type Definitions"	
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  DATA_FORMAT Register
**  ----------------------------------------------------------------
**  |D7        |D6  |D5         |D4 |D3       |D2      |D1    |   D0|
**  -----------------------------------------------------------------
**  |SELF_TEST |SPI |INT_INVERT |0  |FULL_RES |JUSTIFY |RANGE BITS  |
**  -----------------------------------------------------------------
**
**  (Taken from ADXL345 Reference Manual)
**  The DATA_FORMAT register controls the presentation of data to Register 
**  0x32 through Register 0x37. All data, except that for the ±16 g range, 
**  must be clipped to avoid rollover. 
**  SELF_TEST Bit 
**	A setting of 1 in the SELF_TEST bit applies a self-test force to the 
**	sensor, causing a shift in the output data. A value of 0 disables the 
**  self-test force.
**  SPI Bit
**  A value of 1 in the SPI bit sets the device to 3-wire SPI mode, and a 
**  value of 0 sets the device to 4-wire SPI mode.
**  INT_INVERT Bit
**  A value of 0 in the INT_INVERT bit sets the interrupts to active high, 
**  and a value of 1 sets the interrupts to active low.
**  FULL_RES Bit
**  When this bit is set to a value of 1, the device is in full resolution 
**  mode, where the output resolution increases with the g range set by the 
**  range bits to maintain a 4 mg/LSB scale factor. When the FULL_RES bit is 
**  set to 0, the device is in 10-bit mode, and the range bits determine the 
**  maximum g range and scale factor.
**  JUSTIFY BIT
**  A setting of 1 in the justify bit selects left-justified (MSB) mode, 
**  and a setting of 0 selects right-justified mode with sign extension.
**  RANGE BITS
**  Reference DATA_FORMAT constansts begining with PMODACL_BIT_DATA_FORMAT_RANGE
*/
#define PmodACLSetDataFormat(CHN,DATA_FORMAT) PmodACLWriteReg(CHN,PMODACL_REG_DATA_FORMAT,DATA_FORMAT)


/*  
**  PmodACLGetDataFormat
**
**	Synopsis:
**
**  Input: 
**   	SpiChannel CHN - spiChannel
**
**  Returns: 
**      uint8_t - data format register
**
**	Errors:	none
**
**  Description:
**
**  Returns the DATA_FORMAT register (defined in PMODACL_REG_DATA_FORMAT),
**  for a description of the contents of this register see the ADXL345 refrence
**  manual or the description for PmodACLSetDataFormat
*/
#define PmodACLGetDataFormat(CHN) PmodACLReadReg(CHN,PMODACL_REG_DATA_FORMAT)
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
#define PmodACLSetPowerCtl(CHN,POWER_CTL) PmodACLWriteReg(CHN,PMODACL_REG_POWER_CTL,POWER_CTL)

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
#define PmodACLGetPowerCtl(CHN) PmodACLReadReg(CHN,PMODACL_REG_POWER_CTL)
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
#define PmodACLGetDeviceID(CHN) PmodACLReadReg(CHN,PMODACL_REG_DEVID)

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
#define PmodACLSetFIFOCtl(CHN,FIFO_CTL) PmodACLWriteReg(CHN,PMODACL_REG_FIFO_CTL,FIFO_CTL)


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
#define PmodACLGetFIFOCtl(CHN) PmodACLReadReg(CHN,PMODACL_REG_FIFO_CTL)

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
#define PmodACLSetOffset(CHN,OFFSET_BYTES) PmodACLWriteRegMultiByte(CHN,PMODACL_REG_OFSX,OFFSET_BYTES,PMODACL_NUM_OFFSET_BYTES);
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
#define PmodACLGetOffset(CHN,OFFSET_BYTES) PmodACLReadRegMultiByte(CHN,PMODACL_REG_OFSX,OFFSET_BYTES,PMODACL_NUM_OFFSET_BYTES)

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
#define PmodACLSetTapThresh(CHN,TAP_THRESH) PmodACLWriteReg(CHN,PMODACL_REG_THRESH_TAP,TAP_THRESH)


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
#define PmodACLGetTapThresh(CHN) PmodACLReadReg(CHN,PMODACL_REG_THRESH_TAP)

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
#define PmodACLSetIntEnable(CHN,INT_ENABLE) PmodACLWriteReg(CHN,PMODACL_REG_INT_ENABLE,INT_ENABLE)


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
#define PmodACLGetIntEnable(CHN) PmodACLReadReg(CHN,PMODACL_REG_INT_ENABLE)
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
#define PmodACLSetIntMap(CHN,INT_MAP) PmodACLWriteReg(CHN,PMODACL_REG_INT_MAP,INT_MAP)


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
#define PmodACLGetIntMap(CHN) PmodACLReadReg(CHN,PMODACL_REG_INT_MAP)

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
#define PmodACLGetIntSource(CHN) PmodACLReadReg(CHN,PMODACL_REG_INT_SOURCE)

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
#define PmodACLSetTapAxes(CHN,TAP_AXES) PmodACLWriteReg(CHN,PMODACL_REG_TAP_AXES,TAP_AXES)

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
#define PmodACLGetTapAxes(CHN) PmodACLReadReg(CHN,PMODACL_REG_TAP_AXES)

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
#define PmodACLSetTapDuration(CHN,TAP_DURATION) PmodACLWriteReg(CHN,PMODACL_REG_DUR,TAP_DURATION)

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
#define PmodACLGetTapDuration(CHN) PmodACLReadReg(CHN,PMODACL_REG_DUR)

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
#define PmodACLSetTapLatency(CHN,TAP_LATENCY) PmodACLWriteReg(CHN,PMODACL_REG_LATENT ,TAP_LATENCY)

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
#define PmodACLGetTapLatency(CHN) PmodACLReadReg(CHN,PMODACL_REG_LATENT)

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
#define PmodACLSetTapWindow(CHN,TAP_WINDOW) PmodACLWriteReg(CHN,PMODACL_REG_WINDOW,TAP_WINDOW)

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
#define PmodACLGetTapWindow(CHN) PmodACLReadReg(CHN,PMODACL_REG_WINDOW)

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
#define PmodACLGetActTapStatus(CHN) PmodACLReadReg(CHN,PMODACL_REG_ACT_TAP_STATUS)


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
#define PmodACLSetThreshFF(CHN,THRESH_FF)  PmodACLWriteReg(CHN,PMODACL_REG_THRESH_FF,THRESH_FF)

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
#define PmodACLGetThreshFF(CHN) PmodACLReadReg(CHN,PMODACL_REG_THRESH_FF)


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
#define PmodACLSetBwRate(CHN,BW_RATE) PmodACLWriteReg(CHN,PMODACL_REG_BW_RATE,BW_RATE)

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
#define PmodACLGetBwRate(CHN) PmodACLReadReg(CHN,PMODACL_REG_BW_RATE)

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
#define PmodACLGetFIFOStatus(CHN) PmodACLReadReg(CHN,PMODACL_REG_FIFO_STATUS)

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
#define PmodACLGetTimeInact(CHN) PmodACLReadReg(CHN,PMODACL_REG_TIME_INACT)


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
#define PmodACLSetTimeInact(CHN,TIME_INACT) PmodACLWriteReg(CHN,PMODACL_REG_TIME_INACT,TIME_INACT)

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
#define PmodACLGetThreshAct(CHN) PmodACLReadReg(CHN,PMODACL_REG_THRESH_ACT)

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
#define PmodACLSetThreshAct(CHN,THRESH_ACT) PmodACLWriteReg(CHN,PMODACL_REG_THRESH_ACT,THRESH_ACT)

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
#define PmodACLGetActInactCtl(CHN) PmodACLReadReg(CHN,PMODACL_REG_ACT_INACT_CTL)

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
#define PmodACLSetActInactCtl(CHN,ACT_INACT_CTL) PmodACLWriteReg(CHN,PMODACL_REG_ACT_INACT_CTL,ACT_INACT_CTL)


#endif
