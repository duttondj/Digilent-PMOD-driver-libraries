/* -------------------------------------------------------------------- */
/*                                                                      */
/* pmodHB5_PmodHB5SetInitialDirection.c --                              */
/*                       implimentation for PmodHB5SetInitialDirection  */
/*                                                                      */
/* -------------------------------------------------------------------- */
/*	Author: 	Ryan Hoffman											*/
/*	Copyright (C) 2011 Ryan Hoffman    									*/
/************************************************************************/
/*  Module Description: 												*/
/* -------------------------------------------------------------------- */
/*  Revision History:													*/
/*                                                                      */
/*  11/3/2011(RyanH):                                                   */
/*                                                                      */
/* -------------------------------------------------------------------- */

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include "./PmodHB5/pmodHB5.h"

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */
/*  
**  PmodHB5SetInitialDirection
**
**	Synopsis:
**	Sets the duty cycle to 0% and the initial direction of the
**  DC motor
** 
**	Preconditions: hBridge must be initialized as shown in header
**
**  Input: 
**		HBRIDGE *hBridge - HBRIDGE struct
**
**  Returns: On direction change new direction is stored in 
**           hBridge->currentDirection
**
**	Errors:	none
**
**  Description:
**  Sets the initial duty cycle to 0% and direction of the DC motor 
**  based on HBRIDGE.newDirection. This function should be used
**  only when the RPM is known to be 0 (example: setup and initalization)
**  otherwise the PmodHB5 could be damaged.
*/
void PmodHB5SetDCInitialDirection(HBRIDGE *hBridge)
{
	PmodHB5SetDCPWMDutyCycle(0,hBridge->ocChannel);

	if(hBridge->newDirection == PMOD_HB5_DIR_CW)
	{
		PORTToggleBits(hBridge->directionPort,hBridge->directionPortBit);
	}
	else
	{
		PORTClearBits(hBridge->directionPort,hBridge->directionPortBit);
	}

	hBridge->currentDirection = hBridge->newDirection;
}


