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
/*  4/16/2011 (RossK): Added period calculation
/*  5/5/2011(RossK): Added Advanced
/*																		*/
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#ifndef _TIMERLIB_H_
#define _TIMERLIB_H_

#include <peripheral/timer.h>

#if defined TIMER_1

/* ------------------------------------------------------------ */
/***	fnTimer1Setup
**
**	Parameters:
**		unsigned int ulMS - Duration of the timer in milliseconds
**
**	Return Value:
**		0 on failure
**		1 on success
**
**	Errors:
**		none
**
**	Description:
**		Initializes Timer1 and enables Timer1’s interrupt
*/
unsigned char 
fnTimer1Setup (unsigned int ulMS);

/* ------------------------------------------------------------ */
/***	fnTimer1IntHandler
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This is the interrupt handler function for Timer1.
**		THIS MUST BE DEFINED BY USER!
*/
void 
fnTimer1IntHandler(void);

/* ------------------------------------------------------------ */
/***	fnReadTimer1
**
**	Parameters:
**		none
**	Return Value:
**		value of the timer, in ms
**
**	Errors:
**		none
**
**	Description:
**		Read the current time value of the timer.
*/
unsigned int
fnReadTimer1();

/* ------------------------------------------------------------ */
/***	fnSetTimer1
**
**	Parameters:
**		unsigned int ulMS - Duration of the timer in milliseconds
**
**	Return Value:
**		0 on failure
**		1 on success
**
**	Errors:
**		none
**
**	Description:
**		Sets the time value of the timer.
*/
unsigned char
fnSetTimer1 (unsigned int ulMS);

/* ------------------------------------------------------------ */
/***	fnCloseTimer1
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Closes the timer
*/
#define fnCloseTimer1() CloseTimer1();

#endif

#if defined ADVTIMER_1

/* ------------------------------------------------------------ */
/***	fnAdvTimer1Setup
**
**	Parameters:
**		unsigned int ulConfig - bitwise or of configuration constants. 
**						 		See Microchip timer library for more information
**		unsigned int ulPeriod - period of the timer
**
**	Return Value:
**		0 on failure
**		1 on success
**
**	Errors:
**		none
**
**	Description:
**		Initializes Timer1 and enables Timer1’s interrupt
*/
unsigned char
fnAdvTimer1Setup (unsigned int ulConfig, unsigned int ulPeriod);

/* ------------------------------------------------------------ */
/***	fnAdvTimer1IntHandler
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This is the interrupt handler function for Timer1.
**		THIS MUST BE DEFINED BY USER!
*/
void 
fnAdvTimer1IntHandler();

/* ------------------------------------------------------------ */
/***	fnAdvReadTimer1
**
**	Parameters:
**		none
**
**	Return Value:
**		value of the timer
**
**	Errors:
**		none
**
**	Description:
**		gets the current value of the timer.
*/
#define fnAdvReadTimer1() ReadTimer1();

/* ------------------------------------------------------------ */
/***	fnAdvSetTimer1
**
**	Parameters:
**		unsigned int ulValue - the new value for the timer
**
**	Errors:
**		none
**
**	Description:
**		Sets the value of Timer1
*/
#define fnAdvSetTimer1(ulValue) WriteTimer1(ulValue);

/* ------------------------------------------------------------ */
/***	fnAdvReadTimer1Period
**
**	Parameters:
**		none
**
**	Return Value:
**		period of the timer
**
**	Errors:
**		none
**
**	Description:
**		gets the period of the timer.
*/
#define fnAdvReadTimer1Period() ReadPeriod1();

/* ------------------------------------------------------------ */
/***	fnAdvSetTimer1Period
**
**	Parameters:
**		unsigned int ulPeriod - period of the timer
**
**	Return Value:
**		0 on failure
**		1 on success
**
**	Errors:
**		none
**
**	Description:
**		Sets the Timer1 period value
*/
unsigned char
fnAdvSetTimer1Period (unsigned int ulPeriod);

/* ------------------------------------------------------------ */
/***	fnAdvConfigTimer1
**
**	Parameters:
**		unsigned int ulConfig - bitwise or of configuration constants. 
**								See Microchip timer library for more information
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		configures the timer to the passed config
*/
#define fnAdvConfigTimer1(ulConfig) ConfigIntTimer1(ulConfig);

/* ------------------------------------------------------------ */
/***	fnAdvEnableTimer1
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Enables the timer
*/
#define fnAdvEnableTimer1() mT1IntEnable(1);


/* ------------------------------------------------------------ */
/***	fnAdvDisableTimer1
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Disables the timer
*/
#define fnAdvDisableTimer1() mT1IntEnable(0);

/* ------------------------------------------------------------ */
/***	fnAdvTimer1IntPriority(unsigned int ulPriority)
**
**	Parameters:
**		unsigned int ulPriority - a value of T1_INT_PRIOR_XXX
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Sets the interrupt priority of the timer
*/
#define fnAdvTimer1IntPriority(ulPriority) mT1SetIntPriority((ulPriority));

/* ------------------------------------------------------------ */
/***	fnAdvCloseTimer1
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Closes the timer
*/
#define fnAdvCloseTimer1() CloseTimer1();	

#endif

#endif
