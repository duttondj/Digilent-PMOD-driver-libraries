/************************************************************************/
/*																		*/
/*	TimerLib.h - timer library header file						*/
/*																		*/
/************************************************************************/
/*	Author: Ross Kristof 															*/
/*												*/
/************************************************************************/
/*  Module Description: Wrapper library for the timer features of a Cerebot 32MX4		*/
/*																		*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*	2/19/2011(RossK): creation of file	*/
/*																		*/
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#ifndef _32MX4TIMER_H_
#define _32MX4TIMER_H_
#include <p32xxxx.h> // Vector Table Constants
#include <peripheral/timer.h> // Timers 1 - 5
#include <peripheral/ports.h> // PORTSetPinsDigitalIn (), ConfigINT (), etc.
#include <peripheral/int.h> // Enable interrupts - INTEnableInterrupts()
#include <peripheral/system.h>	// Set up the system and perihperal clocks for best performance
#include <sys/attribs.h> // Very important for ISR...


/* ------------------------------------------------------------ */
/***	fnTimer1
**
**	Parameters:
**		unsigned int uMS - milliseconds of the timer
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Initalize timer1 with a millisecond value
*/
void 
fnTimer1 (unsigned int uMS);

/* ------------------------------------------------------------ */
/***	fnTimer2
**
**	Parameters:
**		unsigned int uMS - milliseconds of the timer
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Initalize timer2 with a millisecond value
*/
void 
fnTimer2 (unsigned int uMS);

/* ------------------------------------------------------------ */
/***	fnTimer3
**
**	Parameters:
**		unsigned int uMS - milliseconds of the timer
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Initalize timer3 with a millisecond value
*/
void 
fnTimer3 (unsigned int uMS);

/* ------------------------------------------------------------ */
/***	fnTimer4
**
**	Parameters:
**		unsigned int uMS - milliseconds of the timer
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Initalize timer4 with a millisecond value
*/
void 
fnTimer4 (unsigned int uMS);

/* ------------------------------------------------------------ */
/***	fnTimer5
**
**	Parameters:
**		unsigned int uMS - milliseconds of the timer
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Initalize timer5 with a millisecond value
*/
void 
fnTimer5 (unsigned int uMS);

#endif
