/************************************************************************/
/*                                                                      */
/*  i2c_mx4_7.h  -- Wrappers for i2c Microchip PIC32 library functions  */
/*                                                                      */
/************************************************************************/
/*      Author:         Andrew Koch                                     */
/************************************************************************/
/*  File Description:                                                   */
/*                                                                      */
/*      This file contains interface declarations for the functions and */
/*      data types used in wrapping the Microchip PIC32 i2c             */
/*  interface libraries for use with the Digilent CEREBOT32MX4,         */
/*  CEREBOT32MX7                                                        */                                         
/************************************************************************/
/*  Revision History:                                                   */
/*                                                                      */
/*      2/23/2011 Koch,Andrew: Initial draft                            */
/*                                                                      */
/************************************************************************/

#ifndef _I2C_MX4_7_

	#define _I2C_MX4_7_
	
	//***** Legacy Code *****//
	// Don't use the legacy code in i2c.h
	#define _PLIB_DISABLE_LEGACY
	
	//***** Port Configuration *****//

	// The CEREBOT32MX4 has 2 I2C Ports
	#define _I2C1 
	#define _I2C2
	
	#if defined __CEREBOT32MX7__
	     // additional mx4 ports here
	#endif
	
	//***** Clock Configuration *****//	

	// The I2CSetFrequency needs the clock rate to sync with it.
	// Todo This value may (is) board specific and 
	// needs to be set.
	// TODO Look into this value more
	//#define CEREBOT32MX4_CLOCK = 

	//***** Includes *****//
	// This needs to be included after the defines.
	#include <peripheral/i2c.h>
	
	// 




/*
Function: int I2CInitializeMaster( I2C_MODULE id, I2C_CONFIGURATION flags, UINT32 i2cClock)

Configures, initializes and enables a master I2C port.

Initialize an I2C master port at port id.
I2C_MODULE id: The i2c module to use (defined in i2c.h) 
I2C_CONFIGURATION flags: configuration flags (see i2c.h)
UINT32 i2cClock: The desired clock rate
Returns: The obtaiined clock rate
*/

int I2CInitializeMaster( I2C_MODULE id, I2C_CONFIGURATION flags, UINT32 i2cClock);

/*
Function: int I2CInitializeMaster( I2C_MODULE id, I2C_CONFIGURATION flags, UINT32 i2cClock)

Configures, initializes and enables a slave I2C port.

Initialize an I2C slave port at port id.
I2C_MODULE id: The i2c module to use (defined in i2c.h) 
I2C_CONFIGURATION flags: configuration flags (see i2c.h)
UINT32 i2cClock: The desired clock rate
Returns: The obtaiined clock rate
*/

//TODO work out more information about I2CSetSlaveAddress
//void I2CInitializeSlave( I2C_MODULE id, I2C_CONFIGURATION flags, UINT32 i2cClock....MORE...);
#endif
