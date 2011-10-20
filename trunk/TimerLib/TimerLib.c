/************************************************************************/
/*																		*/
/*	TimerLib.c - timer library source file				*/
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
/*	3/15/2011(RossK): changed intterupt handler	*/
/*																		*/
/************************************************************************/
#include <p32xxxx.h> // Vector Table Constants
#include <sys/attribs.h> // Very important for ISR...
#include "TimerLib.h"
#include "TimerHelper.h"

#define TICKRATE 1000
#define PRESCALAR 256

/* ------------------------------------------------------------ */
/*				                Globals                         			*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

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
fnTimer1Setup (unsigned int ulMS)
{
    unsigned int ulPeriod;
    
    // calculate a period
    ulPeriod = ((GetPeripheralClock() / TICKRATE) * ulMS) / PRESCALAR;
    
    // If final value is larger than 2 bytes, return error
    if(ulPeriod > 0xFFFF)
    {
        return 0;
    }

	// Open Timer1
	OpenTimer1 (T1_ON | T1_IDLE_CON | T1_PS_1_256, ulPeriod);
    
    // Configure interrupt for Timer1
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_7);
    
    return 1;
}

/* ------------------------------------------------------------ */
/***	fnTimer1Int
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
**		The interrupt wrapper function
**		user must define the fnTimer1Handler
**		DO NOT MODIFY!
*/
#ifndef ADVTIMER_1
void __ISR(_TIMER_1_VECTOR, ipl7) fnTimer1Int(void)
{
	// Clear the timer interrupt and call our handler function
	mT1ClearIntFlag ();
	fnTimer1IntHandler();
}
#endif

/* ------------------------------------------------------------ */
/***	fnReadTimer1
**
**	Parameters:
**		none
**
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
fnReadTimer1()
{
    // prescalar * timer value / (peripheral bus clock / tick rate) = time in milliseconds
    return ((PRESCALAR * ReadTimer1()) / (GetPeripheralClock() / TICKRATE));    
}

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
**		This function will sets the time value of the timer.
*/
unsigned char
fnSetTimer1 (unsigned int ulMS)
{
    unsigned int ulPeriod;
    
    // calculate a period
    ulPeriod = ((GetPeripheralClock() / TICKRATE) * ulMS) / PRESCALAR;
    
    // If final value is larger than 2 bytes, return error
    if(ulPeriod > 0xFFFF)
    {
        return 0;
    }
    
    // Write this period to our timer
    WritePeriod1(ulPeriod);
    
    return 1;
}

#endif

#if ADVTIMER_1 == 1

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
fnAdvTimer1Setup (unsigned int ulConfig, unsigned int ulPeriod)
{   
    
    // If final value is larger than 2 bytes, return error
    if(ulPeriod > 0xFFFF)
    {
        return 0;
    }

	// Open Timer1
	OpenTimer1 (ulConfig, ulPeriod);
    
    // Configure interrupt for Timer1
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_7);
    
    return 1;
}

/* ------------------------------------------------------------ */
/***	fnAdvTimer1Int
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
**		The interrupt wrapper function.
**		user must define the fnAdvTimer1Handler
**		DO NOT MODIFY!
*/
void __ISR(_TIMER_1_VECTOR, ipl7) fnAdvTimer1Int(void)
{
	// Clear the timer interrupt and call our handler function
	mT1ClearIntFlag ();
	fnAdvTimer1IntHandler();
}

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
fnAdvSetTimer1Period (unsigned int ulPeriod)
{
    // If final value is larger than 2 bytes, return error
    if(ulPeriod > 0xFFFF)
    {
        return 0;
    }
    
    // Write this period to our timer
    WritePeriod1(ulPeriod);
    
    return 1;
}

#endif
