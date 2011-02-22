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
/*																		*/
/************************************************************************/

#include "TimerLib.h"

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
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
#if defined TIMER_1
void 
fnTimer1 (unsigned int uMS)
{
	// Open Timer1
    // TODO: Convert uMS to prescalar/period
	OpenTimer1 (T1_ON | T1_IDLE_CON | T1_PS_1_256, uMS);
    
    // Configure interrupt for Timer1
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_7);
}
#endif

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
#if defined TIMER_2
void 
fnTimer2 (unsigned int uMS)
{
	// Open Timer2
    // TODO: Convert uMS to prescalar/period
	OpenTimer2 (T2_ON | T2_IDLE_CON | T2_PS_1_256, uMS);
    
    // Configure interrupt for Timer2
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_7);
}
#endif

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
#if defined TIMER_3
void 
fnTimer3 (unsigned int uMS)
{
	// Open Timer3
    // TODO: Convert uMS to prescalar/period
	OpenTimer3 (T3_ON | T3_IDLE_CON | T3_PS_1_256, uMS);
    
    // Configure interrupt for Timer3
    ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_7);
}
#endif

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
#if defined TIMER_4
void 
fnTimer4 (unsigned int uMS)
{
	// Open Timer4
    // TODO: Convert uMS to prescalar/period
	OpenTimer4 (T4_ON | T4_IDLE_CON | T4_PS_1_256, uMS);
    
    // Configure interrupt for Timer4
    ConfigIntTimer4(T4_INT_ON | T4_INT_PRIOR_7);
}
#endif

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
#if defined TIMER_5
void 
fnTimer5 (unsigned int uMS)
{
	// Open Timer5
    // TODO: Convert uMS to prescalar/period
	OpenTimer5 (T5_ON | T5_IDLE_CON | T5_PS_1_256, uMS);
    
    // Configure interrupt for Timer5
    ConfigIntTimer5(T5_INT_ON | T5_INT_PRIOR_7);
}
#endif

/* ------------------------------------------------------------ */
/***	ProcName
**
**	Parameters:
**
**	Return Value:
**
**	Errors:
**
**	Description:
**
*/

/* ------------------------------------------------------------ */

/************************************************************************/

