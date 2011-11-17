/************************************************************************/
/*	pmodACL.h	-- PmodACL header file, public API						*/
/*																		*/
/************************************************************************/
/*	Author: 	Ryan Hoffman 											*/
/*	Copyright (C) 2011 Ryan Hoffman										*/
/************************************************************************/
/*  Module Description: 												*/
/*  Driver library for the Digilent PmodACL based on the ADXL345		*/
/*  accelerometer.														*/
/*																		*/
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
#define PMODACL_MASK_DATA_FORMAT_RANGE			0x0F
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

typedef struct
{
	int16_t xAxis;
	int16_t yAxis;
	int16_t zAxis;
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
void PmodACLInitSpi(SpiChannel chn,uint32_t pbClock,uint32_t bitRate);

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
**  for a description of the contents of this register see the ADXL345 refrence
**  manual or the description for PmodACLSetDataFormat
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
**  POWER_CTL Register
**  --------------------------------------------------------
**  |D7 |D6 |D5    |D4          |D3       |D2      |D1 |D0 |
**  --------------------------------------------------------
**  | 0 | 0 | LINK | AUTO_SLEEP | MEASURE | SLEEP |WAKEUP  |
**  --------------------------------------------------------
**  (Taken from ADXL345 Reference Manual)
**  LINK
**  A setting of 1 in the link bit with both the activity and inactivity 
**  functions enabled delays the start of the activity function until 
**  inactivity is detected. After activity is detected, inactivity 
**  detection begins, preventing the detection of activity. This bit 
**  serially links the activity and inactivity functions. When this bit 
**  is set to 0, the inactivity and activity functions are concurrent.
**  Additional information can be found in the Link Mode section.
**  When clearing the link bit, it is recommended that the part be placed 
**  into standby mode and then set back to measurement mode with a subsequent
**  write. This is done to ensure that the device is properly biased if sleep
**  mode is manually disabled; otherwise, the first few samples of data after
**  the link bit is cleared may have additional noise, especially if the 
**  device was asleep when the bit was cleared.
**
**  AUTO_SLEEP Bit
**  If the link bit is set, a setting of 1 in the AUTO_SLEEP bit enables the 
**  auto-sleep functionality. In this mode, the ADXL345 auto-matically switches 
**  to sleep mode if the inactivity function is enabled and inactivity is 
**  detected (that is, when acceleration is below the THRESH_INACT value for
**  at least the time indicated by TIME_INACT). If activity is also enabled,
**  the ADXL345 automatically wakes up from sleep after detecting activity
**  and returns to operation at the output data rate set in the BW_RATE register.
**  A setting of 0 in the AUTO_SLEEP bit disables automatic switching to sleep 
**  mode. See the description of the Sleep Bit in this section for more 
**  information on sleep mode.
**  If the link bit is not set, the AUTO_SLEEP feature is disabled and setting 
**  the AUTO_SLEEP bit does not have an impact on device operation. Refer to the 
**  Link Bit section or the Link Mode section for more information on utilization
**  of the link feature.
**  When clearing the AUTO_SLEEP bit, it is recommended that the part be placed 
**  into standby mode and then set back to measure-ment mode with a subsequent 
**  write. This is done to ensure that the device is properly biased if sleep
**  mode is manually disabled; otherwise, the first few samples of data after 
**  the AUTO_SLEEP bit is cleared may have additional noise, especially if the 
**  device was asleep when the bit was cleared.
**
**  MEASURE Bit
**  A setting of 0 in the measure bit places the part into standby mode, and a 
**  setting of 1 places the part into measurement mode. The ADXL345 powers up 
**  in standby mode with minimum power consumption.
**
**  SLEEP Bit
**  A setting of 0 in the sleep bit puts the part into the normal mode of operation,
**  and a setting of 1 places the part into sleep mode. Sleep mode suppresses 
**  DATA_READY, stops transmission of data to FIFO, and switches the sampling rate 
**  to one specified by the wakeup bits. In sleep mode, only the activity function 
**  can be used. When the DATA_READY interrupt is suppressed, the output data 
**  registers (Register 0x32 to Register 0x37) are still updated at the sampling 
**  rate set by the wakeup bits (D1:D0).
**  When clearing the sleep bit, it is recommended that the part be placed into 
**  standby mode and then set back to measurement mode with a subsequent write. 
**  This is done to ensure that the device is properly biased if sleep mode is
**  manually disabled; otherwise, the first few samples of data after the sleep 
**  bit is cleared may have additional noise, especially if the device was asleep 
**  when the bit was cleared.
**
**  WAKEUP Bits
**  These bits control the frequency of readings in sleep mode and are prefixed with
**  PMODACL_BIT_POWER_CTL_WAKEUP
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
**  for a description of the contents of this register see the ADXL345 refrence
**  manual or the description for PmodACLSetPowerCtl
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
**  (Taken from ADXL345 Reference Manual)
**  FIFO_CTL Register
**  -------------------------------------------
**  |D7    |D6  |D5       |D4 |D3 |D2 |D1 |D0 |
**  -------------------------------------------
**  | FIFO_MODE | TRIGGER | Samples           |
**  -------------------------------------------
**  
**  FIFO Modes
**  ---------------------------------------------------------------------------
**  |          Setting            |											  |
**  ---------------------------------------------------------------------------
**  |D7           |D6             |Mode       |Function						  |
**  ---------------------------------------------------------------------------
**  |PMODACL_BIT_FIFO_CTL_BYPASS  |Bypass     |FIFO is bypassed.              |
**  ---------------------------------------------------------------------------
**  |PMODACL_BIT_FIFO_CTL_FIFO    |FIFO       |FIFO collects up to 32 values  |
**  |							  |           |and then stops collecting data,|
**  |                             |           |collecting new data only when  |
**  |                             |           |FIFO is not full.              |
**  ---------------------------------------------------------------------------
**  |PMODACL_BIT_FIFO_CTL_STREAM  |Stream     |FIFO holds the last 32 data    |
**  |                             |           |values. When FIFO is full, the |
**  |                             |           |oldest data is overwritten with|
**  |                             |           |newer data.                    |
**  ---------------------------------------------------------------------------
**  |PMODACL_BIT_FIFO_CTL_TRIGGER |Trigger    |When triggered by the trigger  |
**  |                             |           |bit, FIFO holds the last data  |
**  |                             |           |samples before the trigger     |
**  |                             |           |event and then continues to    |
**  |                             |           |collect data until full. New   |
**  |                             |           |data is collected only when    |
**  |                             |           |FIFO is not full.              |
**  ---------------------------------------------------------------------------
**  Trigger Bit
**  A value of 0 in the trigger bit links the trigger event of trigger mode to 
**  INT1, and a value of 1 links the trigger event to INT2.

**  Samples Bits
**  The function of these bits depends on the FIFO mode selected  
**  Entering a value of 0 in the samples bits immediately sets the watermark 
**  status bit in the INT_SOURCE register, regardless of which FIFO mode is 
**  selected. Undesirable operation may occur if a value of 0 is used for the 
**  samples bits when trigger mode is used.
**
**  Samples Bits Functions
**  --------------------------------------------------------------
**  |FIFO Mode  | Samples Bits Funtions 				         |
**  --------------------------------------------------------------
**  |Bypass     |None                                            |
**  --------------------------------------------------------------
**  |FIFO       |Specifies how many FIFO entries are needed to   |
**  |           |trigger a watermark interrupt.                  |
**  --------------------------------------------------------------
**  |Stream     |Specifies how many FIFO entries are needed to   |
**  |           |trigger a watermark interrupt.                  |
**  --------------------------------------------------------------
**  |Trigger    |Specifies how many FIFO samples are retained in |
**  |           |the FIFO buffer before a trigger event.         |
**  --------------------------------------------------------------
** 
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
**      uint8_t - PMODACL_REG_FIFO_CTL register contents
**
**	Errors:	none
**
**  Description:
**
**  Returns the FIFO_CTL register (defined in PMODACL_REG_FIFO_CTL),
**  for a description of the contents of this register see the ADXL345 refrence
**  manual or the description for PmodACLSetFIFOCtl
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
**  (Taken from ADXL345 Reference Manual)
**  The OFSX, OFSY, and OFSZ registers are each eight bits and offer 
**  user-set offset adjustments in twos complement format with a 
**  scale factor of 15.6 mg/LSB (that is, 0x7F = 2 g). The value stored 
**  in the offset registers is automatically added to the acceleration data,
**  and the resulting value is stored in the output data registers. For 
**  additional information regarding offset calibration and the use of the 
**  offset registers, refer to the Offset Calibration section in the ADXL345
**  reference manual. Function PmodACLCalibrate supplied in this library 
**  will perform the calibration caclculations and set the OFFSET registers
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
**  (Taken from ADXL345 Reference Manual)
**  The THRESH_TAP register is eight bits and holds the threshold value for tap interrupts. 
**  The data format is unsigned, therefore, the magnitude of the tap event is compared with 
**  the value in THRESH_TAP for normal tap detection. The scale factor is 62.5 mg/LSB 
**  (that is, 0xFF = 16 g). A value of 0 may result in undesirable behavior if single tap/double 
**  tap interrupts are enabled.
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
**  Returns the THRESH_TAP register (defined in PMODACL_REG_THRESH_TAP),
**  for a description of the contents of this register see the ADXL345 refrence
**  manual or the description for PmodACLSetThreshTap
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
**  (Taken from ADXL345 Reference Manual)
**  INT_ENABLE Register
**  ----------------------------------------------------------------------------------
**  |D7        |D6        |D5        |D4      |D3        |D2        |D1      |D0     |
**  ----------------------------------------------------------------------------------
**  |DATA_READY|SINGLE_TAP|DOUBLE_TAP|ACTIVITY|INACTIVITY|FREE_FALL|WATERMARK|OVERRUN|
**  ----------------------------------------------------------------------------------
**  Setting bits in this register to a value of 1 enables their respective 
**  functions to generate interrupts, whereas a value of 0 prevents the 
**  functions from generating interrupts. The DATA_READY, watermark, and
**  overrun bits enable only the interrupt output; the functions are always
**  enabled. It is recommended that interrupts be configured before enabling
**  their outputs.
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
**  Returns the INT_ENABLE register (defined in PMODACL_REG_INT_ENABLE),
**  for a description of the contents of this register see the ADXL345 refrence
**  manual or the description for PmodACLSetIntEnable
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
**  (Taken from ADXL345 Reference Manual)
**  INT_MAP Register
**  ----------------------------------------------------------------------------------
**  |D7        |D6        |D5        |D4      |D3        |D2        |D1      |D0     |
**  ----------------------------------------------------------------------------------
**  |DATA_READY|SINGLE_TAP|DOUBLE_TAP|ACTIVITY|INACTIVITY|FREE_FALL|WATERMARK|OVERRUN|
**  ----------------------------------------------------------------------------------
**  Any bits set to 0 in this register send their respective interrupts to the INT1
**  pin, whereas bits set to 1 send their respective interrupts to the INT2 pin. All 
**  selected interrupts for a given pin are OR’ed.
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
**   	SpiChannel CHN - spiChannel
**
**  Returns: 
**      uint8_t - PMODACL_REG_INT_MAP register contents
**
**	Errors:	none
**
**  Description:
**  Returns the INT_MAP register (defined in PMODACL_REG_INT_MAP),
**  for a description of the contents of this register see the ADXL345 refrence
**  manual or the description for PmodACLSetIntMap
*/
#define PmodACLGetIntMap(CHN) PmodACLReadReg(CHN,PMODACL_REG_INT_MAP)

/*  
**  PmodACLGetIntSource
**
**	Synopsis:
**	Gets the contents of the PMODACL_REG_INT_SOURCEregister
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
**  (Taken from ADXL345 Reference Manual)
**  INT_SOURCE Register
**  ----------------------------------------------------------------------------------
**  |D7        |D6        |D5        |D4      |D3        |D2        |D1      |D0     |
**  ----------------------------------------------------------------------------------
**  |DATA_READY|SINGLE_TAP|DOUBLE_TAP|ACTIVITY|INACTIVITY|FREE_FALL|WATERMARK|OVERRUN|
**  ----------------------------------------------------------------------------------
**  Bits set to 1 in this register indicate that their respective functions have 
**  triggered an event, whereas a value of 0 indicates that the corresponding event has
**  not occurred. The DATA_READY, watermark, and overrun bits are always set if the 
**  corresponding events occur, regardless of the INT_ENABLE register settings, and 
**  are cleared by reading data from the DATAX, DATAY, and DATAZ registers. The 
**  DATA_READY and watermark bits may require multiple reads, as indicated in the FIFO 
**  mode descriptions in the FIFO section. Other bits, and the corresponding interrupts,
**  are cleared by reading the INT_SOURCE register.
**
**  Notes:
**
**  A double tap will set the SINGLE_TAP and DOUBLE_TAP bits, this value is provided
**  as PMODACL_BIT_INT_SOURCE_DOUBLE_TAP defined in "Local Type Definitions"
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
**  (Taken from ADXL345 Reference Manual)
**  TAP_AXES Register
**  ---------------------------------------------------------------------
**  |D7 |D6 |D5 |D4 |D3       |D2           |D1           |D0           |
**  ---------------------------------------------------------------------
**  | 0 | 0 | 0 | 0 |SUPPRESS |TAP_X_ENABLE |TAP_Y_ENABLE |TAP_Z_ENABLE |
**  ---------------------------------------------------------------------
**  SUPRESS Bit
**  Setting the suppress bit suppresses double tap detection if acceleration 
**  greater than the value in THRESH_TAP is present between taps. See the Tap 
**  Detection section for more details.
**  TAP_x Enable Bits
**  A setting of 1 in the TAP_X enable, TAP_Y enable, or TAP_Z enable bit 
**  enables x-, y-, or z-axis participation in tap detection. A setting of 0 
**  excludes the selected axis from participation in tap detection.
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
**  Returns the TAP_AXES register (defined in PMODACL_REG_TAP_AXES),
**  for a description of the contents of this register see the ADXL345 refrence
**  manual or the description for PmodACLSetTapAxes
*/
#define PmodACLGetTapAxes(CHN) PmodACLReadReg(CHN,PMODACL_REG_TAP_AXES)

/*  
**  PmodACLSetTapDuration
**
**	Synopsis:
**		Sets the contents of the PMODACL_REG_DUR register
**  Input: 
**		SpiChannel CHN -  Spi channel
**      uint8_t DUR - tap duration
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  (Taken from ADXL345 Reference Manual)
**  The DUR register is eight bits and contains an unsigned time value representing 
**  the maximum time that an event must be above the THRESH_TAP threshold to qualify as
**  a tap event. The scale factor is 625 µs/LSB. A value of 0 disables the single tap/ 
**  double tap functions.
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
**      uint8_t - PMODACL_REG_DUR register contents
**
**	Errors:	none
**
**  Description:
**  Returns the DUR register (defined in PMODACL_REG_DUR),
**  for a description of the contents of this register see the ADXL345 refrence
**  manual or the description for PmodACLSetTapDuration
*/
#define PmodACLGetTapDuration(CHN) PmodACLReadReg(CHN,PMODACL_REG_DUR)

/*  
**  PmodACLSetTapLatency
**
**	Synopsis:
**		Sets the contents of the PMODACL_REG_LATENT register
**  Input: 
**		SpiChannel CHN -  Spi channel
**      uint8_t TAP_LATENCY - time between tap detection and start of the time window 
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  (Taken from ADXL345 Reference Manual)
**  The latent register is eight bits and contains an unsigned time value representing 
**  the wait time from the detection of a tap event to the start of the time window 
**  (defined by the PMODACL_REG_WINDOW register) during which a possible second tap 
**  event can be **  detected. The scale factor is 1.25 ms/LSB. A value of 0 disables 
**  the double tap function.
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
**      uint8_t - PMODACL_REG_LATENT register contents
**
**	Errors:	none
**
**  Description:
**  Returns the LATENT register (defined in PMODACL_REG_LATENT),
**  for a description of the contents of this register see the ADXL345 refrence
**  manual or the description for PmodACLSetTapLatency
*/
#define PmodACLGetTapLatency(CHN) PmodACLReadReg(CHN,PMODACL_REG_LATENT)

/*  
**  PmodACLSetTapWindow
**
**	Synopsis:
**		Sets the contents of the PMODACL_REG_WINDOW register
**  Input: 
**		SpiChannel CHN -  Spi channel
**      uint8_t TAP_WINDOW - tap window
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  (Taken from ADXL345 Reference Manual)
**  The window register is eight bits and contains an unsigned time value representing
**  the amount of time after the expiration of the latency time (determined by the latent 
**  register) during which a second valid tap can begin. The scale factor is 1.25 ms/LSB.
**  A value of 0 disables the double tap function.
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
**   	SpiChannel CHN - spiChannel
**
**  Returns: 
**      uint8_t - PMODACL_REG_WINDOW register contents
**
**	Errors:	none
**
**  Description:
**  Returns the PMODACL_REG_WINDOW register for a description of the contents 
**  of this register see the ADXL345 refrence manual or the description for 
**  PmodACLSetTapWindow
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
**  (Taken from ADXL345 Reference Manual)
**  ACT_TAP_STATUS Register
**  ------------------------------------------------------------------------------------------------
**  |D7 |D6           |D5           |D4           |D3     |D2           |D1           |D0          |
**  ------------------------------------------------------------------------------------------------
**  |0  |ACT_X_SOURCE |ACT_Y_SOURCE |ACT_Z_SOURCE |ALSEEP |TAP_X_SOURCE |TAP_Y_SOURCE |TAP_Z_SOURCE|
**  ------------------------------------------------------------------------------------------------
**  ACT_x Source and TAP_x Source Bits
**  These bits indicate the first axis involved in a tap or activity event. A setting 
**  of 1 corresponds to involvement in the event, and a setting of 0 corresponds to no 
**  involvement. When new data is available, these bits are not cleared but are overwritten 
**  by the new data. The ACT_TAP_STATUS register should be read before clearing the interrupt.
**  Disabling an axis from participation clears the corresponding source bit when the next 
**  activity or single tap/double tap event occurs.
**  Asleep Bit
**  A setting of 1 in the asleep bit indicates that the part is asleep, and a setting of 0 
**  indicates that the part is not asleep. This bit toggles only if the device is configured
**  for auto sleep. See the AUTO_SLEEP Bit section for more information on autosleep mode.
*/
#define PmodACLGetActTapStatus(CHN) PmodACLReadReg(CHN,PMODACL_REG_ACT_TAP_STATUS)

/*  
**  PmodACLSetThreshFF
**
**	Synopsis:
**		Sets the contents of the PMODACL_REG_THRESH_FF register
**  Input: 
**		SpiChannel CHN -  Spi channel
**      uint8_t THRESH_FF- free fall threshold
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  (Taken from ADXL345 Reference Manual)
**  The THRESH_FF register is eight bits and holds the threshold value, in unsigned format, 
**  for free-fall detection. The acceleration on all axes is compared with the value in 
**  THRESH_FF to determine if a free-fall event occurred. The scale factor is 62.5 mg/LSB. 
**  Note that a value of 0 mg may result in undesirable behavior if the free-fall interrupt 
**  is enabled. Values between 300 mg and 600 mg (0x05 to 0x09) are recommended.
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
**      uint8_t - PMODACL_REG_THRESH_FF register contents
**
**	Errors:	none
**
**  Description:
**  Returns the PMODACL_REG_THRESH_FF register for a description of the contents 
**  of this register see the ADXL345 refrence manual or the description for 
**  PmodACLSetThreshFF
*/
#define PmodACLGetThreshFF(CHN) PmodACLReadReg(CHN,PMODACL_REG_THRESH_FF)

/*  
**  PmodACLSetBwRate
**
**	Synopsis:
**
**	Gets the contents of the PMODACL_REG_ACT_TAP_STATUS register
**
**  Input: 
**   	SpiChannel CHN - spiChannel
**
**  Returns: 
**      uint8_t - contents of PMODACL_REG_BW_RATE register, which is a combination 
**				  of ORed values prefixed with PMODACL_BIT_BW_RATE
**                defined in "Local Type Definitions"
**  Errors:	none
**
**  Description:
**
**  (Taken from ADXL345 Reference Manual)
**  BW_RATE Register
**  ---------------------------------------
**  |D7 |D6 |D5 |D4 	   |D3 |D2 |D1 |D0|
**  ---------------------------------------
**  | 0 | 0 | 0 |LOW_POWER | RATE         |
**  ---------------------------------------
**  LOW_POWER Bit
**  A setting of 0 in the LOW_POWER bit selects normal operation, and a setting of 1 selects 
**  reduced power operation, which has somewhat higher noise (see the Power Modes section 
**  for details).
**  
**  Rate Bits
**  These bits select the device bandwidth and output data rate (see Table 7 and Table 8 for 
**  details). The default value is 0x0A, which translates to a 100 Hz output data rate. An 
**  output data rate should be selected that is appropriate for the communication protocol and
**  frequency selected. Selecting too high of an output data rate with a low communication speed
**  results in samples being discarded.
**
**	Notes:
**
**  Values for tables 7 and 8 are defined in "Local Type Definitions" and following the
**  following format PMODACL_BIT_BW_RATE_<OUTPUT_BIT_RATE>_HZ, the bandwidth is half
**  of the value defined.
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
**   	SpiChannel CHN - spiChannel
**
**  Returns: 
**      uint8_t - PMODACL_REG_BW_RATE register contents
**
**	Errors:	none
**
**  Description:
**  Returns the PMODACL_REG_BW_RATE register for a description of the contents 
**  of this register see the ADXL345 refrence manual or the description for 
**  PmodACLSetBwRate
*/
#define PmodACLGetBwRate(CHN) PmodACLReadReg(CHN,PMODACL_REG_BW_RATE)

/*
**  PmodACLSetTimeInact
**
**	Synopsis:
**		Sets the contents of the PMODACL_REG_TIME_INACT register
**  Input: 
**		SpiChannel CHN -  Spi channel
**      uint8_t TIME_INACT- amount of time acceleration is less than THRESH_INACT
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  (Taken from ADXL345 Reference Manual)
**  The TIME_INACT register is eight bits and contains an unsigned time value representing 
**  the amount of time that acceleration must be less than the value in the THRESH_INACT 
**  register for inactivity to be declared. The scale factor is 1 sec/LSB. Unlike the other
**  interrupt functions, which use unfiltered data (see the Threshold section), the 
**  inactivity function uses filtered output data. At least one output sample must be generated
**  for the inactivity interrupt to be triggered. This results in the function appearing 
**  unresponsive if the TIME_INACT register is set to a value less than the time constant of 
**  the output data rate. A value of 0 results in an interrupt when the output data is less 
**  than the value in the THRESH_INACT register.
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
**   	SpiChannel CHN - spiChannel
**
**  Returns: 
**      uint8_t - PMODACL_REG_TIME_INACT register contents
**
**	Errors:	none
**
**  Description:
**  Returns the PMODACL_REG_TIME_INACT register for a description of the contents 
**  of this register see the ADXL345 refrence manual or the description for 
**  PmodACLSetTimeInact
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
**  (Taken from ADXL345 Reference Manual)
**  ---------------------------------------
**  |D7       |D6 |D5 |D4 |D3 |D2 |D1 |D0 |
**  ---------------------------------------
**  |FIFO_TRIG| 0 | Entries               |
**  ---------------------------------------
**
**  FIFO_TRIG Bit
**  A 1 in the FIFO_TRIG bit corresponds to a trigger event occurring, and a 0 means
**  that a FIFO trigger event has not occurred. 
**  
**  Entries Bits
**  These bits report how many data values are stored in FIFO. Access to collect the 
**  data from FIFO is provided through the DATAX, DATAY, and DATAZ registers. FIFO 
**  reads must be done in burst or multiple-byte mode because each FIFO level is 
**  cleared after any read (single- or multiple-byte) of FIFO. FIFO stores a maximum
**  of 32 entries, which equates to a maximum of 33 entries available at any given 
**  time because an additional entry is available at the output filter of the device
*/
#define PmodACLGetFIFOStatus(CHN) PmodACLReadReg(CHN,PMODACL_REG_FIFO_STATUS)

/*
**  PmodACLSetThreshAct
**
**	Synopsis:
**		Sets the contents of the PMODACL_REG_THRESH_ACT register
**  Input: 
**		SpiChannel CHN -  Spi channel
**      uint8_t THRESH_ACT - Threshold for detecting activity
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  (Taken from ADXL345 Reference Manual)
**  The TIME_INACT register is eight bits and contains an unsigned time value representing 
**  the amount of time that acceleration must be less than the value in the THRESH_INACT 
**  register for inactivity to be declared. The scale factor is 1 sec/LSB. Unlike the other
**  interrupt functions, which use unfiltered data (see the Threshold section), the 
**  inactivity function uses filtered output data. At least one output sample must be generated
**  for the inactivity interrupt to be triggered. This results in the function appearing 
**  unresponsive if the TIME_INACT register is set to a value less than the time constant of 
**  the output data rate. A value of 0 results in an interrupt when the output data is less 
**  than the value in the THRESH_INACT register.
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
**      uint8_t -PMODACL_REG_THRESH_ACT register contents
**
**	Errors:	none
**
**  Description:
**
**  Returns the PMODACL_REG_THRESH_ACT register for a description of the contents 
**  of this register see the ADXL345 refrence manual or the description for 
**  PmodACLSetThreshAct
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
**  (Taken from ADXL345 Reference Manual)
**  ACT_INACT_CTL register
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
**  ACT AC/DC and INACT AC/DC Bits
**
**  A setting of 0 selects dc-coupled operation, and a setting of 1 enables ac-coupled
**  operation. In dc-coupled operation, the current acceleration magnitude is compared 
**  directly with THRESH_ACT and THRESH_INACT to determine whether activity or inactivity
**  is detected. 
**  
**  In ac-coupled operation for activity detection, the acceleration value 
**  at the start of activity detection is taken as a reference value. New samples of 
**  acceleration are then compared to this reference value, and if the magnitude of the 
**  difference exceeds the THRESH_ACT value, the device triggers an activity interrupt.
**  
**  Similarly, in ac-coupled operation for inactivity detection, a reference value is used
**  for comparison and is updated whenever the device exceeds the inactivity threshold. 
**  After the reference value is selected, the device compares the magnitude of the 
**  difference between the reference value and the current acceleration with THRESH_INACT.
**  If the difference is less than the value in THRESH_INACT for the time in TIME_INACT, 
**  the device is considered inactive and the inactivity interrupt is triggered.
**
**  ACT_x Enable Bits and INACT_x Enable Bits
**
**  A setting of 1 enables x-, y-, or z-axis participation in detecting activity or 
**  inactivity. A setting of 0 excludes the selected axis from participation. If all axes
**  are excluded, the function is disabled. For activity detection, all participating axes 
**  are logically OR’ed, causing the activity function to trigger when any of the partici-pating 
**  axes exceeds the threshold. For inactivity detection, all participating axes are logically 
**  AND’ed, causing the inactivity function to trigger only if all participating axes are below 
**  the threshold for the specified time.
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
**  Returns the PMODACL_REG_ACT_INACT_CTL register for a description of the contents 
**  of this register see the ADXL345 refrence manual or the description for 
**  PmodACLSetActInactCtl
*/
#define PmodACLGetActInactCtl(CHN) PmodACLReadReg(CHN,PMODACL_REG_ACT_INACT_CTL)

#endif
