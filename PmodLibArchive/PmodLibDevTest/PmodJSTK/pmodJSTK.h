/************************************************************************/
/* pmodJSTK.h -- header file for Digilent PmodJSTK	manipulation        */
/*               public API                                             */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	Copyright (C) 2011 Ryan Hoffman    									*/
/************************************************************************/
/*  Module Description: 												*/
/*  Header file for Digilent PmodJSTK manimputation, see PmodJSTK       */
/*  data sheet for details.												*/
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  10/18/2011(RyanH):                                                  */
/*                                                                      */
/************************************************************************/
#ifndef _PMODJSTK_H_
#define _PMODJSTK_H_

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <plib.h>
#include <stdint.h>

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */

//LED state bit masks
#define PMODJSTK_LD1_LD2_OFF 0x80 //LD1 and LD2 OFF
#define PMODJSTK_LD1_ON 0x81 //LD 1 ON
#define PMODJSTK_LD2_ON 0x82 //LD 2 ON
#define PMODJSTK_LD1_LD2_ON 0x83 //LD1 and LD 2 ON

//Button state bit masks
#define PMODJSTK_BTN_NONE 0x0 //no buttones are depressed
#define PMODJSTK_BTN_JSTK 0x1 //joystick button depressed
#define PMODJSTK_BTN1 0x2    //BTN1 depressed
#define PMODJSTK_BTN2 0x4    //BTN2 depressed

#define PMODJSTK_MAX_X_Y_AXIS 1023 //maximum deflection of X or Y axis
#define PMODJSTK_MIN_X_Y_AXIS 0 //minimum deflection of X or Y axis

#define PMODJSTK_BYTES_PER_XFER 5 //number of bytes transfered in and out
                                  //for one poll of the PmodJSTK

//Structure representing values recieved from PmodJSTK
typedef struct 
{
	uint16_t xAxis; //X axis position
	uint16_t yAxis; //Y axis position
	uint8_t buttonState; //current state of buttons 
}PmodJSTKAxisButton;

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
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
void PmodJSTKInit(SpiChannel chn,uint32_t pbClock,uint32_t bitRate,uint32_t systemClock);

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
**  determine button status: PMODJSTK_BTN1,PMODJSTK_BTN2.               
*/
void PmodJSTKSendRecv(SpiChannel chn,uint8_t cmdIn,PmodJSTKAxisButton *jystkAxisButtons);

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
void PmodJSTKDelay10us();

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
void PmodJSTKDelay15us();

#endif
