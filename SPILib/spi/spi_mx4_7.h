/************************************************************************/
/*                                                                      */
/*  spi_mx4_7.h  -- Wrappers for SPI Microchip PIC32 library functions  */
/*                                                                      */
/************************************************************************/
/*	Author:		Ryan Hoffman                                            */
/************************************************************************/
/*  File Description:                                                   */
/*                                                                      */
/*	This file contains interface declarations for the functions and		*/
/*	data types used in wrapping the Microchip PIC32 Serial Peripheral	*/
/*  interface libraries for use with the Digilent CEREBOT32MX4,			*/
/*  CEREBOT32MX7           												*/
/************************************************************************/
/*  Revision History:                                                   */
/*                                                                      */
/*	2/19/2011 (RyanH):                                                  */ 
/*  5/5/2001  (RyanH);                                                  */
/*                                                                      */
/************************************************************************/

#ifndef _SPI_MX4_7_
#define _SPI_MX4_7_
#include <peripheral/ports.h>
#include <peripheral/spi.h>
#include <peripheral/system.h>
#include<stdint.h>
#include "spi_config.h"


/* ------------------------------------------------------------ */
/*					Type Declarations							*/
/* ------------------------------------------------------------ */

//Enumerates the channels available
//Channels 1-2 available on the CEREBOTMX432
typedef enum
{
	SPI_C1 = 1,  //Select SPI Channel 1
 	SPI_C2 = 2   //Select SPI Channel 2

#if defined __CEREBOT32MX7__
	SPI_C3 = 3  //Select SPI Channel 3 (MX7 only)
#endif
}SPICHANNEL;

//Enumerates the tranfer modes used during open
typedef enum
{
 SPI_32BIT_MODE = 0,  //Open in 32bit transfer mode
 SPI_16BIT_MODE = 1,  //Open in 16bit transfer mode
 SPI_8BIT_MODE = 2    //Open in 8bit transfer mode
}SPIOPENMODE;



/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */
void fnSPIConfigureChannelMasterNoFramesCERev (uint32_t ulClock,SPICHANNEL spiChannel, SPIOPENMODE spiOpenMode,uint16_t uSckFreq );
void fnSPIConfigureChannelMasterNoFrames (uint32_t ulClock,SPICHANNEL spiChannel, SPIOPENMODE spiOpenMode,uint16_t uSckFreq);
void fnSPIConfigureChannelSlaveNoFrames (SPICHANNEL spiChannel, SPIOPENMODE spiOpenMode);
void fnSPIConfigureChannelMasterWithFrames (uint32_t ulClock,SPICHANNEL spiChannel, SPIOPENMODE spiOpenMode,uint16_t uSckFreq);
void fnSPIConfigureChannelSlaveWithFrames (SPICHANNEL spiChannel, SPIOPENMODE spiOpenMode);
void fnSPIEnableDigitalPinIO(SPICHANNEL spiChannel);
void fnSPIEnableInterrupts(void);
void fnOpenSPI(uint16_t uConfig,uint32_t ulClock,SPICHANNEL spiChannel, SPIOPENMODE spiOpenMode,uint16_t uSckFreq );
void fnSPISetSSLow(SPICHANNEL chn);
void fnSPISetSSHigh(SPICHANNEL chn);
#endif