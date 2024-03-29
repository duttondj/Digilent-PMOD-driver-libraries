/************************************************************************/
/*                                                                      */
/* pmodJSTK.c -- implementation for common PmodJSTK functions           */
/*                                                                      */
/* -------------------------------------------------------------------- */
/*	Author: 	Ryan Hoffman											*/
/*              Copyright (C) 2011 Ryan Hoffman                         */
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  10/18/2011(RyanH):                                                  */
/*                                                                      */
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include "./PmodJSTK/pmodJSTK.h"
#include "./PmodCommon/utility/pmod_utility.h"

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */
//Values calculed during PmodJSTKInit(), number of cycles to delay
//10us and 15us
static uint32_t NumCycles10us = 0;
static uint32_t NumCycles15us = 0;

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

/*  PmodJSTKInit
**
**	Synopsis:
**  Initializes the PmodJSTK module on the selected SPI channel. 
**  
**  Input: SpiChannel chn  - spi channel initialize
**         uint32_t pbClock - peripheral bus clock in Hz
**         uint32_t bitRate - bit rate desired in Hz
**
**  Returns: none
**
**  Description:
**
**  Opens the desired SPI channel in 8-bit mode as a master, enables the slave select bit,
**  and sets the desired bit rate as a function of pbClock/bitRate.  Maximum recomended 
**  bitrate is 1Mhz.
*/
void PmodJSTKInit(SpiChannel chn,uint32_t pbClock,uint32_t bitRate,uint32_t systemClock)
{
    SpiChnOpen(chn, SPI_OPEN_MSTEN | SPI_OPEN_SSEN |  SPI_OPEN_MODE8 | SPI_OPEN_CKP_HIGH, pbClock/bitRate);
	PmodSPISetSSHigh(chn);
	//calculate number of processor cycles at the current system clock frequence for 10 and 15 microsecond delays
	//(Desired Delay)/(Time Elapsed for 1 System Clock Cycle)
	NumCycles10us = (((systemClock * 10)/4)/1000000);
	NumCycles15us = (((systemClock * 15)/4)/1000000);
}

/*  
**  PmodJSTKSendRecv
**
**	Synopsis:
**	Sends a LED illumination command to the PmodJSTK and recieves the 
**  position of the joystick on its x and y axis and the state of 
**  buttons 1,2,3
**  Input: 
**		SpiChannel chn - spi channel
**		uint8_t cmdIn - button LED command
**      PmodJSTKAxisButton *jystkAxisButtons - pointer to struct containing
**                                             data returned by the joystick.
**
**  Returns: none
**  
**	Errors:	none
**
**  Description:
**  This function will operate on the channel specified in parameter chn.
**  Parameter cmdIn will accept the following values and have the following effect
**  on the PmodJSTK:
**	PMODJSTK_LD1_LD2_OFF: all LEDs off
**	PMODJSTK_LD1_ON: LD1 on
**	PMODJSTK_LD2_ON: LD2 on
**	PMODJSTK_LD1_LD2_ON: LD1 and LD2 on
**  Parameter *jystkAxisButtons is a pointer to a PmodJSTKAxisButton struct which 
**  will be filled as specified below:
**	xAxis: 16 bit field holding the joysitick position on the x axis (between 0 and 1023)
**  yAxis: 16 bit field holding the joysitick position on the y axis (between 0 and 1023)
**  buttonState: 8 bit field holding the state of the buttons, use the following bit masks to
**  determine button status: PMODJSTK_BTN1,PMODJSTK_BTN2,PMODJSTK_BTN_JSTK,PMODJSTK_BTN_NONE 
**              
*/
void PmodJSTKSendRecv(SpiChannel chn,uint8_t cmdIn,PmodJSTKAxisButton *jystkAxisButtons)
{
	int byteNum = 0;
	uint8_t jystkOut[PMODJSTK_BYTES_PER_XFER];
	
	PmodSPISetSSLow(chn);
	PmodJSTKDelay15us();

    for(byteNum = 0;byteNum < PMODJSTK_BYTES_PER_XFER;byteNum++)
	{
		SpiChnPutC(chn,cmdIn);
		jystkOut[byteNum] = SpiChnGetC(chn);	
		PmodJSTKDelay10us();
	}

	PmodSPISetSSHigh(chn);

	jystkAxisButtons->xAxis = (((uint16_t)jystkOut[1] << 8) | (uint16_t)jystkOut[0]);
	jystkAxisButtons->yAxis = (((uint16_t)jystkOut[3] << 8) | (uint16_t)jystkOut[2]);
	jystkAxisButtons->buttonState = jystkOut[4];
	
	PmodJSTKDelay15us();
}

/* 
**	PmodJSTKDelay10us
** 
**	Synopsis:
**	Creates a 10 microsecond delay
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  Creates a 10 microsecond delay by blocking for
**  the number of cycles calculated in PmodJSTKInit. 
*/
void PmodJSTKDelay10us()
{ 
	fnPmodDelayNcycles(NumCycles10us);
}

/*  
**	PmodJSTKDelay15us
**
**	Synopsis:
**	Creates a 15 microsecond delay
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  Creates a 15 microsecond delay by blocking for
**  the number of cycles calculated in PmodJSTKInit.
*/
void PmodJSTKDelay15us()
{ 
	fnPmodDelayNcycles(NumCycles15us);
}
