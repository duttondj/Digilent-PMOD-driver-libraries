/************************************************************************/
/*	pmodACL.h	-- PmodACL header file, public API						*/
/*																		*/
/************************************************************************/
/*	Author: 	Ryan Hoffman 											*/
/*	Copyright (C) 2011 Ryan Hoffman										*/
/************************************************************************/
/*  Module Description: 												*/
/*  Driver library for the Digilent PmodACL based on the ADXL345		*/
/*  accelerometer. Please reference the Analog Devices ADXL345  		*/
/*	reference manual for addition details.								*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/* <11/14/11>(Ryan H): Initial Release									*/
/*																		*/
/************************************************************************/
#ifndef _PMOD_ACL_H_
#define _PMOD_ACL_H_

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <stdint.h>
#include <plib.h>

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */

//************Register address definitions********//

// Descriptions for register values defined in this section
// are available in the Analog Devices ADXL345 reference manual.

#define PMODACL_REG_DEVID  						0x00 
#define PMODACL_REG_THRESH_TAP 					0x1D 

/************************************/
/*        OFFSET                    */
/************************************/
#define PMODACL_REG_OFSX  						0x1E 
#define PMODACL_REG_OFSY  						0x1F 
#define PMODACL_REG_OFSZ  						0x20 
#define PMODACL_NUM_OFFSET_BYTES         		0x03
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
#define PMODACL_BIT_BW_RATE_3200HZ				0x0F
#define PMODACL_BIT_BW_RATE_1600HZ				0x0E
#define PMODACL_BIT_BW_RATE_800HZ				0x0D
/************use in low power mode*****************/
#define PMODACL_BIT_BW_RATE_400HZ				0x0C 
#define PMODACL_BIT_BW_RATE_200HZ				0X0B
#define PMODACL_BIT_BW_RATE_100HZ				0x0A	
#define PMODACL_BIT_BW_RATE_50HZ				0x09
#define PMODACL_BIT_BW_RATE_25HZ				0x08
#define PMODACL_BIT_BW_RATE_12_5HZ				0x07
/**************************************************/
#define PMODACL_BIT_BW_RATE_6_25HZ				0x06
#define PMODACL_BIT_BW_RATE_3_13HZ				0x05
#define PMODACL_BIT_BW_RATE_1_56HZ				0x04
#define PMODACL_BIT_BW_RATE_0_78HZ				0x04
#define PMODACL_BIT_BW_RATE_0_39HZ				0x02
#define PMODACL_BIT_BW_RATE_0_20HZ				0x01
#define PMODACL_BIT_BW_RATE_0_10HZ				0x00

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
#define PMODACL_MASK_DATA_FORMAT_RANGE			0x07
/************************************/

/************************************/
/*        Axis Registers            */
/************************************/
#define PMODACL_REG_DATAX0  					0x32 
#define PMODACL_REG_DATAX1  					0x33 
#define PMODACL_REG_DATAY0  					0x34 
#define PMODACL_REG_DATAY1  					0x35 
#define PMODACL_REG_DATAZ0  					0x36 
#define PMODACL_REG_DATAZ1  					0x37 
#define PMODACL_NUM_AXIS_REGISTERS				0x06

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

#define PMODACL_READ_BIT						0x80
#define PMODACL_MB_BIT              			0x40
#define PMODACL_DEVICE_ID						0xE5

/************************************/
/*    Calibration Orienatation      */
/************************************/
#define PMODACL_CALIBRATE_X_AXIS				0x00
#define PMODACL_CALIBRATE_Y_AXIS				0x01
#define PMODACL_CALIBRATE_Z_AXIS				0x02

//Use with axis read operations
typedef struct
{
	int16_t xAxis; //xAxis register values
	int16_t yAxis; //yAxis register values
	int16_t zAxis; //zAxis register values
}PMODACL_AXIS;

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
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
**	See SERIAL COMMUNICATIONS->SPI in the ADXL345 reference manual for
**  additional information.
*/
void PmodACLInitSpi(SpiChannel chn,uint32_t pbClock,uint32_t bitRate);

/*  
**  PmodACLGetAxisData
**
**	Synopsis:
**
**	Reads the values from the axis values and writes 
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
**  bytes are shifted and combined to create 16 bit signed values then
**  stored in the corresponding axis field in pmodACLAxis.
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x32 to Register 0x37—
**  DATAX0, DATAX1, DATAY0, DATAY1, DATAZ0, DATAZ1 in the ADXL345 reference 
**  manual for additional information.
*/
void PmodACLGetAxisData(SpiChannel chn, PMODACL_AXIS *pmodACLAxis);

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
uint8_t PmodACLReadReg(SpiChannel chn,uint8_t address);

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
void PmodACLReadRegMultiByte(SpiChannel chn,uint8_t startAddress,uint8_t *data,uint8_t numBytes);

/*  
**  PmodACLCalibrate
**
**	Synopsis:
**
**  Performs a calibration of the PmodACL axis utilizing the OFFSET register
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
**  For a full description of the calibration procedure see 
**  "Offset Calibration" in the ADXL345 reference manual.
*/
int32_t PmodACLCalibrate(SpiChannel chn,uint8_t numSamples,uint8_t oneGaxisOrienatation);

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
void PmodACLWriteRegMultiByte(SpiChannel chn,uint8_t startAddress,uint8_t *data,uint8_t numBytes);

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
**  DATA_FORMAT Register (VALUES PREFIXED WITH PMODACL_BIT_DATA_FORMAT)
**  ----------------------------------------------------------------
**  |D7        |D6  |D5         |D4 |D3       |D2      |D1    |   D0|
**  -----------------------------------------------------------------
**  |SELF_TEST |SPI |INT_INVERT |0  |FULL_RES |JUSTIFY |RANGE BITS  |
**  -----------------------------------------------------------------
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x31—DATA_FORMAT (Read/Write) 
**  in the ADXL345 reference manual for additional information.
*/
#define PmodACLSetDataFormat(CHN,DATA_FORMAT) PmodACLWriteReg(CHN,PMODACL_REG_DATA_FORMAT,DATA_FORMAT)

/*  
**  PmodACLGetDataFormat
**
**	Synopsis:
**
**	Gets the contents of the PMODACL_REG_DATA_FORMAT register
**
**  Input: 
**   	SpiChannel CHN - spiChannel
**
**  Returns: 
**      uint8_t - PMODACL_REG_DATA_FORMAT register contents
**
**	Errors:	none
**
**  Description:
**
**  Returns the DATA_FORMAT register (defined in PMODACL_REG_DATA_FORMAT),
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x31—DATA_FORMAT (Read/Write) 
**  in the ADXL345 reference manual for additional information.
*/
#define PmodACLGetDataFormat(CHN) PmodACLReadReg(CHN,PMODACL_REG_DATA_FORMAT)

/*  
**  PmodACLSetPowerCtl
**	
**	Synopsis:
**
**	Sets the PMODACL_REG_POWER_CTL register
**
**  Input: 
**      uint8_t POWER_CTL - Combination of ORed values prefixed with 
**                           PMODACL_BIT_POWER_CTL defined in
**							 "Local Type Definitions"
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  POWER_CTL Register  (VALUES PREFIXED WITH PMODACL_BIT_POWER_CTL)
**  --------------------------------------------------------
**  |D7 |D6 |D5    |D4          |D3       |D2      |D1 |D0 |
**  --------------------------------------------------------
**  | 0 | 0 | LINK | AUTO_SLEEP | MEASURE | SLEEP |WAKEUP  |
**  --------------------------------------------------------
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x2D—POWER_CTL (Read/Write)
**  in the ADXL345 reference manual for additional information.
*/
#define PmodACLSetPowerCtl(CHN,POWER_CTL) PmodACLWriteReg(CHN,PMODACL_REG_POWER_CTL,POWER_CTL)

/*  
**  PmodACLGetPowerCtl
**
**	Synopsis:
**
**	Gets the contents of the PMODACL_REG_POWER_CTL register
**
**  Input: 
**   	SpiChannel CHN - spiChannel
**
**  Returns: 
**      uint8_t - PMODACL_REG_POWER_CTL register contents
**
**	Errors:	none
**
**  Description:
**
**  Returns the POWER_CTL register (defined in PMODACL_REG_POWER_CTL),
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x2D—POWER_CTL (Read/Write)
**  in the ADXL345 reference manual for additional information.
*/
#define PmodACLGetPowerCtl(CHN) PmodACLReadReg(CHN,PMODACL_REG_POWER_CTL)

/*  
** PmodACLGetDeviceID
**
**	Synopsis:
**	Returns the contents of the PMODACL_REG_DEVID register
**
**  Input: 
**		SpiChannel CHN - spiChannel
**
**  Returns: uint8_t - PMODACL_DEVICE_ID (value)
**
**	Errors:	none
**
**  Description:
** 	Returns the contents of DEVID register, which is a fixed
**  value defined in PMODACL_DEVICE_ID
*/
#define PmodACLGetDeviceID(CHN) PmodACLReadReg(CHN,PMODACL_REG_DEVID)

/*  
**  PmodACLSetFIFOCtl
**
**	Synopsis:
**	Sets the PMODACL_REG_FIFO_CTL register
**
**  Input: 
**		SpiChannel CHN -  Spi channel
**      uint8_t FIFO_CTL - Combination of ORed values prefixed with 
**                            PMODACL_BIT_FIFO_CTL defined in
**							  "Local Type Definitions"
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  FIFO_CTL Register (VALUES PREFIXED WITH PMODACL_BIT_FIFO_CTL)
**  -------------------------------------------
**  |D7    |D6  |D5       |D4 |D3 |D2 |D1 |D0 |
**  -------------------------------------------
**  | FIFO_MODE | TRIGGER | Samples           |
**  -------------------------------------------
**  
**  FIFO Modes
**  -------------------------------------------
**  |          Setting            |			  |								  
**  -------------------------------------------
**  |D7           |D6             |Mode       |
**  -------------------------------------------
**  |PMODACL_BIT_FIFO_CTL_BYPASS  |Bypass     |
**  -------------------------------------------
**  |PMODACL_BIT_FIFO_CTL_FIFO    |FIFO       |
**  -------------------------------------------
**  |PMODACL_BIT_FIFO_CTL_STREAM  |Stream     |
**  -------------------------------------------
**  |PMODACL_BIT_FIFO_CTL_TRIGGER |Trigger    | 
**  -------------------------------------------
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x38—FIFO_CTL (Read/Write)
**  in the ADXL345 reference manual for additional information.
 */
#define PmodACLSetFIFOCtl(CHN,FIFO_CTL) PmodACLWriteReg(CHN,PMODACL_REG_FIFO_CTL,FIFO_CTL)

/*  
**  PmodACLGetFIFOCtl
**
**	Synopsis:
**
**	Gets the contents of the PMODACL_REG_FIFO_CTL register
**
**  Input: 
**   	SpiChannel CHN - spiChannel
**
**  Returns: 
**      uint8_t - Combination of ORed values prefixed with 
**                            PMODACL_BIT_FIFO_CTL defined in
**							  "Local Type Definitions"
**
**	Errors:	none
**
**  Description:
**
**  FIFO_CTL Register (VALUES PREFIXED WITH PMODACL_BIT_FIFO_CTL)
**  -------------------------------------------
**  |D7    |D6  |D5       |D4 |D3 |D2 |D1 |D0 |
**  -------------------------------------------
**  | FIFO_MODE | TRIGGER | Samples           |
**  -------------------------------------------
**  
**  FIFO Modes
**  -------------------------------------------
**  |          Setting            |			  |								  
**  -------------------------------------------
**  |D7           |D6             |Mode       |
**  -------------------------------------------
**  |PMODACL_BIT_FIFO_CTL_BYPASS  |Bypass     |
**  -------------------------------------------
**  |PMODACL_BIT_FIFO_CTL_FIFO    |FIFO       |
**  -------------------------------------------
**  |PMODACL_BIT_FIFO_CTL_STREAM  |Stream     |
**  -------------------------------------------
**  |PMODACL_BIT_FIFO_CTL_TRIGGER |Trigger    | 
**  -------------------------------------------
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x38—FIFO_CTL (Read/Write)
**  in the ADXL345 reference manual for additional information.
*/
#define PmodACLGetFIFOCtl(CHN) PmodACLReadReg(CHN,PMODACL_REG_FIFO_CTL)

/*  
**  PmodACLSetOffset
**
**	Synopsis:
**  Set the contents of the PMODACL_REG_OFSX,PMODACL_REG_OFSY, and PMODACL_REG_OFSZ registers
** 
**  Input: 
**      SpiChannel CHN - Spi Channel
**      int8_t OFFSET_BYTES[PMODACL_NUM_OFFSET_BYTES] - 3 byte array containing offset values
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  Sets the offset registers starting at PMODACL_REG_OFSX and ending at 
**  PMODACL_REG_OFSX
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x1E, Register 0x1F, Register 0x20—OFSX, OFSY, OFSZ (Read/Write)
**  in the ADXL345 reference manual for additional information.
**
**	Function PmodACLCalibrate supplied in this library will perform the calibration calculations 
**  and set the OFFSET registers
*/
#define PmodACLSetOffset(CHN,OFFSET_BYTES) PmodACLWriteRegMultiByte(CHN,PMODACL_REG_OFSX,OFFSET_BYTES,PMODACL_NUM_OFFSET_BYTES);

/*  
**  PmodACLGetOffset
**
**	Synopsis:
**
**	Gets the contents of the PMODACL_REG_OFSX,PMODACL_REG_OFSY, and PMODACL_REG_OFSZ registers
**
**  Input: 
**      SpiChannel CHN - Spi Channel
**      int8_t OFFSET_BYTES[PMODACL_NUM_OFFSET_BYTES] - 3 byte array to fill with offset bytes
**
**  Returns:  none
**
**	Errors:	none
**
**  Description:
**
**  Fills a 3 byte array with the contents of the PMODACL_REG_OFSX,PMODACL_REG_OFSY, and PMODACL_REG_OFSZ 
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x1E, Register 0x1F, Register 0x20—OFSX, OFSY, OFSZ (Read/Write)
**  in the ADXL345 reference manual for additional information.
*/
#define PmodACLGetOffset(CHN,OFFSET_BYTES) PmodACLReadRegMultiByte(CHN,PMODACL_REG_OFSX,OFFSET_BYTES,PMODACL_NUM_OFFSET_BYTES)

/*  
**  PmodACLSetThreshTap
**
**	Synopsis:
**		Sets the contents of the PMODACL_REG_THRESH_TAP register
**
**  Input: 
**		SpiChannel CHN -  Spi channel
**      uint8_t THRESH_TAP - tap threshold
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**	Sets threshold value for interrupts.
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x1D—THRESH_TAP (Read/Write)
**  in the ADXL345 reference manual for additional information.
*/
#define PmodACLSetThreshTap(CHN,THRESH_TAP) PmodACLWriteReg(CHN,PMODACL_REG_THRESH_TAP,THRESH_TAP)

/*  
**  PmodACLGetThreshTap
**
**	Synopsis:
**      Gets the contents of the PMODACL_REG_THRESH_TAP register
**
**  Input: 
**   	SpiChannel CHN - spiChannel
**
**  Returns: 
**      uint8_t - PMODACL_REG_THRESH_TAP register contents
**
**	Errors:	none
**
**  Description:
**
**  Returns the THRESH_TAP register (defined in PMODACL_REG_THRESH_TAP)
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x1D—THRESH_TAP (Read/Write)
**  in the ADXL345 reference manual for additional information.
*/
*/
#define PmodACLGetThreshTap(CHN) PmodACLReadReg(CHN,PMODACL_REG_THRESH_TAP)

/*  
**  PmodACLSetIntEnable
**
**	Synopsis:
**  	Sets the contents of the PMODACL_REG_INT_ENABLE register
**
**  Input: 
**		SpiChannel CHN -  Spi channel
**      uint8_t INT_ENABLE - Combination of ORed values prefixed with 
**                            PMODACL_BIT_INT_ENABLE defined in
**							  "Local Type Definitions"	
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  Setting bits in this register enable their responctive functions to generate
**  interrupts, clearing bits disables them.
**
**  INT_ENABLE Register (VALUES PREFIXED WITH MODACL_BIT_INT_ENABLE_)
**  ----------------------------------------------------------------------------------
**  |D7        |D6        |D5        |D4      |D3        |D2        |D1      |D0     |
**  ----------------------------------------------------------------------------------
**  |DATA_READY|SINGLE_TAP|DOUBLE_TAP|ACTIVITY|INACTIVITY|FREE_FALL|WATERMARK|OVERRUN|
**  ----------------------------------------------------------------------------------
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x2E—INT_ENABLE (Read/Write)
**  in the ADXL345 reference manual for additional information.
*/
#define PmodACLSetIntEnable(CHN,INT_ENABLE) PmodACLWriteReg(CHN,PMODACL_REG_INT_ENABLE,INT_ENABLE)

/*  
**  PmodACLGetIntEnable
**
**	Synopsis:
**
**	Gets the contents of the PMODACL_REG_INT_ENABLE register
**
**  Input: 
**   	SpiChannel CHN - spiChannel
**
**  Returns: 
**      uint8_t - PMODACL_REG_INT_ENABLE register contents
**
**	Errors:	none
**
**  Description:
**  Returns the INT_ENABLE register (defined in PMODACL_REG_INT_ENABLE)
**
**  INT_ENABLE Register (VALUES PREFIXED WITH PMODACL_BIT_INT_ENABLE_)
**  ----------------------------------------------------------------------------------
**  |D7        |D6        |D5        |D4      |D3        |D2        |D1      |D0     |
**  ----------------------------------------------------------------------------------
**  |DATA_READY|SINGLE_TAP|DOUBLE_TAP|ACTIVITY|INACTIVITY|FREE_FALL|WATERMARK|OVERRUN|
**  ----------------------------------------------------------------------------------
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x2E—INT_ENABLE (Read/Write)
**  in the ADXL345 reference manual for additional information.
*/
#define PmodACLGetIntEnable(CHN) PmodACLReadReg(CHN,PMODACL_REG_INT_ENABLE)

/*  
**  PmodACLSetIntMap
**
**	Synopsis:
**	Sets the contents of the PMODACL_REG_INT_MAP register
**
**  Input: 
**		SpiChannel CHN -  Spi channel
**      uint8_t INT_MAP - Combination of ORed values prefixed with 
**                            PMODACL_BIT_INT_MAP defined in
**							  "Local Type Definitions"
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  INT_MAP Register (VALUES PREFIXED WITH PMODACL_BIT_INT_MAP_)
**  ----------------------------------------------------------------------------------
**  |D7        |D6        |D5        |D4      |D3        |D2        |D1      |D0     |
**  ----------------------------------------------------------------------------------
**  |DATA_READY|SINGLE_TAP|DOUBLE_TAP|ACTIVITY|INACTIVITY|FREE_FALL|WATERMARK|OVERRUN|
**  ----------------------------------------------------------------------------------
**
**	Bits set to 0 map their interrupt events to the INT1 pin, bits set to 1
**  map their interrupt events to the INT2 pin. Interrupts set for any given
**  pin are OR'ed.
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x2F—INT_MAP (R/W)
**  in the ADXL345 reference manual for additional information.
*/
#define PmodACLSetIntMap(CHN,INT_MAP) PmodACLWriteReg(CHN,PMODACL_REG_INT_MAP,INT_MAP)

/*  
**  PmodACLGetIntMap
**
**	Synopsis:
**
**	Gets the contents of the PMODACL_REG_INT_MAP register
**
**  Input: 
**   	SpiChannel CHN - Spi Channel
**
**  Returns: 
**      uint8_t - PMODACL_REG_INT_MAP register contents
**
**	Errors:	none
**
**  Description:
**  Returns the INT_MAP register (defined in PMODACL_REG_INT_MAP)
**
**  INT_MAP Register (VALUES PREFIXED WITH PMODACL_BIT_INT_MAP_)
**  ----------------------------------------------------------------------------------
**  |D7        |D6        |D5        |D4      |D3        |D2        |D1      |D0     |
**  ----------------------------------------------------------------------------------
**  |DATA_READY|SINGLE_TAP|DOUBLE_TAP|ACTIVITY|INACTIVITY|FREE_FALL|WATERMARK|OVERRUN|
**  ----------------------------------------------------------------------------------
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x2F—INT_MAP (R/W)
**  in the ADXL345 reference manual for additional information.
*/
#define PmodACLGetIntMap(CHN) PmodACLReadReg(CHN,PMODACL_REG_INT_MAP)

/*  
**  PmodACLGetIntSource
**
**	Synopsis:
**	Gets the contents of the PMODACL_REG_INT_SOURCE register
**
**  Input: 
**		SpiChannel CHN -  Spi channel
**
**  Returns: 
**		uint8_t - contents of PMODACL_REG_INT_SOURCE register, which is a combination 
**				  of ORed values prefixed with PMODACL_BIT_INT_SOURCE 
**                defined in "Local Type Definitions"
**	Errors:	
**
**  Description:
**
**  INT_SOURCE Register (VALUES PREFIXED WITH PMODACL_BIT_INT_SOURCE)
**  ----------------------------------------------------------------------------------
**  |D7        |D6        |D5        |D4      |D3        |D2        |D1      |D0     |
**  ----------------------------------------------------------------------------------
**  |DATA_READY|SINGLE_TAP|DOUBLE_TAP|ACTIVITY|INACTIVITY|FREE_FALL|WATERMARK|OVERRUN|
**  ----------------------------------------------------------------------------------
**
**  Bits set to 1 indicate an event of this type occurred. Reading this register will
**  clear the interrupt bits.
**
**  Notes:
**
**  A double tap will set the SINGLE_TAP and DOUBLE_TAP bits, this value is provided
**  as PMODACL_BIT_INT_SOURCE_DOUBLE_TAP defined in "Local Type Definitions"
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x30—INT_SOURCE (Read Only)
**  in the ADXL345 reference manual for additional information.
*/
#define PmodACLGetIntSource(CHN) PmodACLReadReg(CHN,PMODACL_REG_INT_SOURCE)

/*  
**  PmodACLSetTapAxes
**
**	Synopsis:
**
**  Sets the contents of the PMODACL_REG_TAP_AXES register
**
**  Input: 
**		SpiChannel CHN -  Spi channel
**      uint8_t TAP_AXES - Combination of ORed values prefixed with 
**                            PMODACL_BIT_TAP_AXES defined in
**							  "Local Type Definitions"
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  Enables/Disable Axes for tap dectection
**
**  TAP_AXES Register (VALUES PREFIXED WITH PMODACL_BIT_TAP_AXES)
**  ---------------------------------------------------------------------
**  |D7 |D6 |D5 |D4 |D3       |D2           |D1           |D0           |
**  ---------------------------------------------------------------------
**  | 0 | 0 | 0 | 0 |SUPPRESS |TAP_X_ENABLE |TAP_Y_ENABLE |TAP_Z_ENABLE |
**  ---------------------------------------------------------------------
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x2A—TAP_AXES (Read/Write)
**  in the ADXL345 reference manual for additional information.
*/
#define PmodACLSetTapAxes(CHN,TAP_AXES) PmodACLWriteReg(CHN,PMODACL_REG_TAP_AXES,TAP_AXES)
 
/*  
**  PmodACLGetTapAxes
**
**	Synopsis:
**
**	Gets the contents of the PMODACL_REG_TAP_AXES register
**
**  Input: 
**   	SpiChannel CHN - spiChannel
**
**  Returns: 
**      uint8_t - PMODACL_REG_TAP_AXES register contents
**
**	Errors:	none
**
**  Description:
**  Returns the TAP_AXES register (defined in PMODACL_REG_TAP_AXES)
**
**  TAP_AXES Register (VALUES PREFIXED WITH PMODACL_BIT_TAP_AXES)
**  ---------------------------------------------------------------------
**  |D7 |D6 |D5 |D4 |D3       |D2           |D1           |D0           |
**  ---------------------------------------------------------------------
**  | 0 | 0 | 0 | 0 |SUPPRESS |TAP_X_ENABLE |TAP_Y_ENABLE |TAP_Z_ENABLE |
**  ---------------------------------------------------------------------
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x2A—TAP_AXES (Read/Write)
**  in the ADXL345 reference manual for additional information.
*/
#define PmodACLGetTapAxes(CHN) PmodACLReadReg(CHN,PMODACL_REG_TAP_AXES)

/*  
**  PmodACLSetTapDuration
**
**	Synopsis:
**		Sets the contents of the PMODACL_REG_DUR register
*(
**  Input: 
**		SpiChannel CHN -  Spi Channel
**      uint8_t DUR - tap duration (625 microseconds/LSB)
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  Set the amouint of time the an event moust be above THRESH_TAP to 
**  trigger a tap event.
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x21—DUR (Read/Write)
**  in the ADXL345 reference manual for additional information.
*/
#define PmodACLSetTapDuration(CHN,DUR) PmodACLWriteReg(CHN,PMODACL_REG_DUR,DUR)

/*  
**  PmodACLGetTapDuration
**
**	Synopsis:
**
**	Gets the contents of the PMODACL_REG_DUR register
**
**  Input: 
**   	SpiChannel CHN - spiChannel
**
**  Returns: 
**      uint8_t - tap duration (625 microseconds/LSB)
**
**	Errors:	none
**
**  Description:
**  Returns the DUR register (defined in PMODACL_REG_DUR)
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x21—DUR (Read/Write)
**  in the ADXL345 reference manual for additional information.
*/
#define PmodACLGetTapDuration(CHN) PmodACLReadReg(CHN,PMODACL_REG_DUR)

/*  
**  PmodACLSetTapLatency
**
**	Synopsis:
**		Sets the contents of the PMODACL_REG_LATENT register
**  Input: 
**		SpiChannel CHN -  Spi channel
**      uint8_t TAP_LATENCY - time between tap detection and start of the time window (1.25ms/LSB)
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**	Defines the amount of time to wait from a tap detection to the start of the time window
**  as defined in the PMODACL_REG_WINDOW register.
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x22—Latent (Read/Write)
**  in the ADXL345 reference manual for additional information. 
*/
#define PmodACLSetTapLatency(CHN,TAP_LATENCY) PmodACLWriteReg(CHN,PMODACL_REG_LATENT,TAP_LATENCY)

/*  
**  PmodACLGetTapLatency
**
**	Synopsis:
**
**	Gets the contents of the PMODACL_REG_LATENT register
**
**  Input: 
**   	SpiChannel CHN - spiChannel
**
**  Returns: 
**      uint8_t - time between tap detection and start of the time window (1.25ms/LSB)
**
**	Errors:	none
**
**  Description:
**
**	Defines the amount of time to wait from a tap detection to the start of the time window
**  as defined in the PMODACL_REG_WINDOW register.
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x22—Latent (Read/Write)
**  in the ADXL345 reference manual for additional information. 
*/
#define PmodACLGetTapLatency(CHN) PmodACLReadReg(CHN,PMODACL_REG_LATENT)

/*  
**  PmodACLSetTapWindow
**
**	Synopsis:
**		Sets the contents of the PMODACL_REG_WINDOW register
**  Input: 
**		SpiChannel CHN -  Spi channel
**      uint8_t TAP_WINDOW - tap window (1.25ms/LSB)
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  
**  Represents the amount of time after the latency time (PMODACL_REG_LATENT) expires 
**  that a second tap can occur. Setting this register to 0 disables double taps.
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x23—Window (Read/Write)
**  in the ADXL345 reference manual for additional information. 
*/
#define PmodACLSetTapWindow(CHN,TAP_WINDOW) PmodACLWriteReg(CHN,PMODACL_REG_WINDOW,TAP_WINDOW)

/*  
**  PmodACLGetTapWindow
**
**	Synopsis:
**
**	Gets the contents of the PMODACL_REG_WINDOW register
**
**  Input: 
**   	SpiChannel CHN - Spi Channel
**
**  Returns: 
**      uint8_t - tap window (1.25ms/LSB)
**
**	Errors:	none
**
**  Description:
**
**  Represents the amount of time after the latency time (PMODACL_REG_LATENT) expires 
**  that a second tap can occur. Return value of 0 indicates that double taps are 
**  disabled.
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x23—Window (Read/Write)
**  in the ADXL345 reference manual for additional information. 
*/
#define PmodACLGetTapWindow(CHN) PmodACLReadReg(CHN,PMODACL_REG_WINDOW)

/*  
**  PmodACLGetActTapStatus
**
**	Synopsis:
**
**	Gets the contents of the PMODACL_REG_ACT_TAP_STATUS register
**
**  Input: 
**   	SpiChannel CHN - spiChannel
**
**  Returns: 
**      uint8_t - contents of PMODACL_REG_ACT_TAP_STATUS register, which is a combination 
**				  of ORed values prefixed with PMODACL_BIT_ACT_TAP_STATUS
**                defined in "Local Type Definitions"
**
**	Errors:	none
**
**  Description:
**
**  Bits set to 1 indicate the tap or activity source of an event, this
**  register should be read before interrupts are cleared.
**
**  ACT_TAP_STATUS Register (VALUES PREFIXED WITH PMODACL_BIT_ACT_TAP_STATUS)
**  ------------------------------------------------------------------------------------------------
**  |D7 |D6           |D5           |D4           |D3     |D2           |D1           |D0          |
**  ------------------------------------------------------------------------------------------------
**  |0  |ACT_X_SOURCE |ACT_Y_SOURCE |ACT_Z_SOURCE |ALSEEP |TAP_X_SOURCE |TAP_Y_SOURCE |TAP_Z_SOURCE|
**  ------------------------------------------------------------------------------------------------
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x2B—ACT_TAP_STATUS (Read Only)
**  in the ADXL345 reference manual for additional information. 
*/
#define PmodACLGetActTapStatus(CHN) PmodACLReadReg(CHN,PMODACL_REG_ACT_TAP_STATUS)

/*  
**  PmodACLSetThreshFF
**
**	Synopsis:
**		Sets the contents of the PMODACL_REG_THRESH_FF register
**  Input: 
**		SpiChannel CHN -  Spi Channel
**      uint8_t THRESH_FF- free fall threshold (62.5mg/LSB)
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  Sets the threshold for free fall detection, acceleration on all
**  axes is compared to this register, value range should be between
**  300mg and 600mg. 
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x28—THRESH_FF (Read/Write)
**  in the ADXL345 reference manual for additional information. 
*/
#define PmodACLSetThreshFF(CHN,THRESH_FF)  PmodACLWriteReg(CHN,PMODACL_REG_THRESH_FF,THRESH_FF)

/*  
**  PmodACLGetThreshFF
**
**	Synopsis:
**
**	Gets the contents of the PMODACL_REG_THRESH_FF register
**
**  Input: 
**   	SpiChannel CHN - spiChannel
**
**  Returns: 
**      uint8_t - free fall threshold (62.5mg/LSB)
**
**	Errors:	none
**
**  Description:
**  Returns threshold for free fall detection, acceleration on all
**  axes is compared to this register, value range should be between
**  300mg and 600mg. 
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x28—THRESH_FF (Read/Write)
**  in the ADXL345 reference manual for additional information. 
*/
#define PmodACLGetThreshFF(CHN) PmodACLReadReg(CHN,PMODACL_REG_THRESH_FF)

/*  
**  PmodACLSetBwRate
**
**	Synopsis:
**
**	Sets the contents of the PMODACL_BIT_BW_RATE register
**
**  Input: 
**   	SpiChannel CHN - Spi Channel
**
**  Returns: 
**      uint8_t - contents of PMODACL_REG_BW_RATE register, which is a combination 
**				  of ORed values prefixed with PMODACL_BIT_BW_RATE
**                defined in "Local Type Definitions"
**  Errors:	none
**
**  Description:
**
**  Register defines low/normal power modes, device bandwidth and data output rate.
**
**  BW_RATE Register (VALUES PREFIXED WITH PMODACL_BIT_BW_RATE)
**  ---------------------------------------
**  |D7 |D6 |D5 |D4 	   |D3 |D2 |D1 |D0|
**  ---------------------------------------
**  | 0 | 0 | 0 |LOW_POWER | RATE         |
**  ---------------------------------------
**
**	Notes:
**
**  Values for tables 7 and 8 are defined in "Local Type Definitions" and following the
**  following format PMODACL_BIT_BW_RATE_<OUTPUT_BIT_RATE>_HZ, the bandwidth is half
**  of the value defined.
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x2C—BW_RATE (Read/Write)
**  in the ADXL345 reference manual for additional information. 
*/
#define PmodACLSetBwRate(CHN,BW_RATE) PmodACLWriteReg(CHN,PMODACL_REG_BW_RATE,BW_RATE)

/*  
**  PmodACLGetBwRate
**
**	Synopsis:
**
**	Gets the contents of the PMODACL_REG_BW_RATE register
**
**  Input: 
**   	SpiChannel CHN - Spi Channel
**
**  Returns: 
**      uint8_t - PMODACL_REG_BW_RATE register contents
**
**	Errors:	none
**
**  Description:
**
**  Register defines low/normal power modes, device bandwidth and data output rate.
**
**  BW_RATE Register (VALUES PREFIXED WITH PMODACL_BIT_BW_RATE)
**  ---------------------------------------
**  |D7 |D6 |D5 |D4 	   |D3 |D2 |D1 |D0|
**  ---------------------------------------
**  | 0 | 0 | 0 |LOW_POWER | RATE         |
**  ---------------------------------------
**
**	Notes:
**
**  Values for tables 7 and 8 are defined in "Local Type Definitions" and following the
**  following format PMODACL_BIT_BW_RATE_<OUTPUT_BIT_RATE>_HZ, the bandwidth is half
**  of the value defined.
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x2C—BW_RATE (Read/Write)
**  in the ADXL345 reference manual for additional information. 
*/
#define PmodACLGetBwRate(CHN) PmodACLReadReg(CHN,PMODACL_REG_BW_RATE)

/*
**  PmodACLSetTimeInact
**
**	Synopsis:
**		Sets the contents of the PMODACL_REG_TIME_INACT register
**  Input: 
**		SpiChannel CHN -  Spi Channel
**      uint8_t TIME_INACT- amount of time acceleration is less than THRESH_INACT (1sec/LSB)
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  Sets the amount of time acceleration is less than THRESH_INACT (1sec/LSB) before a
**  state of inactivity is detected.
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x26—TIME_INACT (Read/Write)
**  in the ADXL345 reference manual for additional information. 
*/
#define PmodACLSetTimeInact(CHN,TIME_INACT) PmodACLWriteReg(CHN,PMODACL_REG_TIME_INACT,TIME_INACT)

/*  
**  PmodACLGetTimeInact
**
**	Synopsis:
**
**	Gets the contents of the PMODACL_REG_TIME_INACT register
**
**  Input: 
**   	SpiChannel CHN - Spi Channel
**
**  Returns: 
**      uint8_t - mount of time acceleration is less than THRESH_INACT (1sec/LSB)
**
**	Errors:	none
**
**  Description:
**
**  Returns the amount of time acceleration is less than THRESH_INACT (1sec/LSB) before a
**  state of inactivity is detected.
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x26—TIME_INACT (Read/Write)
**  in the ADXL345 reference manual for additional information. 
*/
#define PmodACLGetTimeInact(CHN) PmodACLReadReg(CHN,PMODACL_REG_TIME_INACT)

/*
**  PmodACLGetFIFOStatus
**
**	Synopsis:
**
**	Gets the contents of the PMODACL_REG_FIFO_STATUS register
**
**  Input: 
**   	SpiChannel CHN - spiChannel
**
**  Returns: 
**      uint8_t - PMODACL_REG_FIFO_STATUS register contents
**
**	Errors:	none
**
**  Description:
**
**  Status of FIFO trigger events and number of data values
**  available for collection from the FIFO.
**
**  FIFO_STATUS register 
**  ---------------------------------------
**  |D7       |D6 |D5 |D4 |D3 |D2 |D1 |D0 |
**  ---------------------------------------
**  |FIFO_TRIG| 0 | Entries               |
**  ---------------------------------------
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> 0x39—FIFO_STATUS (Read Only)
**  in the ADXL345 reference manual for additional information. 
*/
#define PmodACLGetFIFOStatus(CHN) PmodACLReadReg(CHN,PMODACL_REG_FIFO_STATUS)

/*
**  PmodACLSetThreshAct
**
**	Synopsis:
**		Sets the contents of the PMODACL_REG_THRESH_ACT register
**  Input: 
**		SpiChannel CHN -  Spi Channel
**      uint8_t THRESH_ACT - Threshold for detecting activity (62.5 mg/LSB)
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  Sets the threshold for detecting activity.
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> THRESH_ACT (Read/Write)
**  in the ADXL345 reference manual for additional information. 
*/
#define PmodACLSetThreshAct(CHN,THRESH_ACT) PmodACLWriteReg(CHN,PMODACL_REG_THRESH_ACT,THRESH_ACT)

/*  
**  PmodACLGetThreshAct
**
**	Synopsis:
**
**	Gets the contents of the PMODACL_REG_THRESH_ACT register
**
**  Input: 
**   	SpiChannel CHN - spiChannel
**
**  Returns: 
**      uint8_t - Threshold for detecting activity (62.5 mg/LSB)
**
**	Errors:	none
**
**  Description:
**
**  Gets the threshold for detecting activity.
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> THRESH_ACT (Read/Write)
**  in the ADXL345 reference manual for additional information. 
*/
#define PmodACLGetThreshAct(CHN) PmodACLReadReg(CHN,PMODACL_REG_THRESH_ACT)

/*  
**  PmodACLSetActInactCtl
**
**	Synopsis:
**		Set the contents of the PMODACL_REG_ACT_INACT_CTL register
**
**  Input: 
**   	SpiChannel CHN - spiChannel
**      uint8_t - contents of PMODACL_REG_ACT_INACT_CTL register, which is a combination 
**				  of ORed values prefixed with PMODACL_BIT_ACT_INACT_CTL
**                defined in "Local Type Definitions"
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  Enables or disables activity or inactivity detection for individual axes,
**  set ac/dc coupled operations. 
**
**  ACT_INACT_CTL register (VALUES PREFIXED WITH PMODACL_BIT_ACT_INACT_CTL)
**  -------------------------------------------------------------
**  |D7 		|D6             |D5             |D4             |
**  -------------------------------------------------------------
**  |ACT ac/dc  |ACT_X enable   |ACT_Y enable   |ACT_Z enable   |
**  -------------------------------------------------------------
**  |D3 		|D2             |D1             |D0             |
**  -------------------------------------------------------------
**  |INACT ac/dc|INACT_X enable |INACT_Y enable |INACT_Z enable |
**  -------------------------------------------------------------
**  
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x27—ACT_INACT_CTL (Read/Write)
**  in the ADXL345 reference manual for additional information. 
*/
#define PmodACLSetActInactCtl(CHN,ACT_INACT_CTL) PmodACLWriteReg(CHN,PMODACL_REG_ACT_INACT_CTL,ACT_INACT_CTL)

/*  
**  PmodACLGetActInactCtl
**
**	Synopsis:
**
**	Gets the contents of the PMODACL_REG_ACT_INACT_CTL register
**
**  Input: 
**   	SpiChannel CHN - spiChannel
**
**  Returns: 
**      uint8_t -PMODACL_REG_ACT_INACT_CTL register contents
**
**	Errors:	none
**
**  Description:
**
**  Resturs state of activity or inactivity detection for individual axes,
**  ac/dc coupled operations. 
**
**  ACT_INACT_CTL register (VALUES PREFIXED WITH PMODACL_BIT_ACT_INACT_CTL)
**  -------------------------------------------------------------
**  |D7 		|D6             |D5             |D4             |
**  -------------------------------------------------------------
**  |ACT ac/dc  |ACT_X enable   |ACT_Y enable   |ACT_Z enable   |
**  -------------------------------------------------------------
**  |D3 		|D2             |D1             |D0             |
**  -------------------------------------------------------------
**  |INACT ac/dc|INACT_X enable |INACT_Y enable |INACT_Z enable |
**  -------------------------------------------------------------
**  
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x27—ACT_INACT_CTL (Read/Write)
**  in the ADXL345 reference manual for additional information. 
*/
#define PmodACLGetActInactCtl(CHN) PmodACLReadReg(CHN,PMODACL_REG_ACT_INACT_CTL)

/*
**  PmodACLSetThreshInact
**
**	Synopsis:
**		Sets the contents of the PMODACL_REG_THRESH_INACT register
**  Input: 
**		SpiChannel CHN -  Spi channel
**      uint8_t THRESH_INACT - Threshold for detecting inactivity (62.5 mg/LSB)
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  Threshold for detecting inactivity. 
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x25—THRESH_INACT (Read/Write)
**  in the ADXL345 reference manual for additional information. 
*/
#define PmodACLSetThreshInact(CHN,THRESH_INACT) PmodACLWriteReg(CHN,PMODACL_REG_THRESH_INACT,THRESH_INACT)

/*  
**  PmodACLGetThreshInact
**
**	Synopsis:
**
**	Gets the contents of the PMODACL_REG_THRESH_INACT register
**
**  Input: 
**   	SpiChannel CHN - spiChannel
**
**  Returns: 
**      uint8_t - Threshold for detecting inactivity (62.5 mg/LSB)
**
**	Errors:	none
**
**  Description:
**
**  Threshold for detecting inactivity. 
**
**  Notes:
**
**	See REGISTER MAP->REGISTER DEFINITIONS-> Register 0x25—THRESH_INACT (Read/Write)
**  in the ADXL345 reference manual for additional information. 
*/
#define PmodACLGetThreshInact(CHN) PmodACLReadReg(CHN,PMODACL_REG_THRESH_INACT)

#endif
