/************************************************************************/
/*                                                                      */
/* pmodHB5_PmodHB5ChangeDirection.c --                                  */
/*                       implementation for PmodHB5ChangeDirection      */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	Copyright (C) 2011 Ryan Hoffman    									*/
/************************************************************************/
/*  Module Description: 												*/
/*  Toggle direction bit on PmodHB5										*/
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  11/3/2011(RyanH):                                                   */
/*                                                                      */
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include <plib.h>
#include "./PmodHB5/pmodHB5.h"

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */
/*  

**  PmodHB5ChangeDirection
**
**	Synopsis:
**	Changes direction of DC motor based on HBRIDGE.currentDirection 
**  and HBRIDGE.newDirection
**
**	Preconditions: hBridge must be initialized as shown in header
**                 
**  Input:
**		HBRIDGE *hBridge - HBRIDGE struct
**
**  Returns: 
**		uint8_t - 1 if direction changed, 0 otherwise
** 
**	Errors:	none
**
**  Description:
**  The HBRIDGE.currentDirection and HBRIDGE.newDirection
**  fields are compared, if they are different the
**  duty cycle of the OC specified in HBRIDGE.ocChannel
**  is set to 0%, if HBRIDGE.rpm == 0 then the direction
**  bit is toggled, reversing the DC motor direction.
**  Waiting for the DC motor RPM to slow to 0
**  prior to changing direction, prevents current from 
**  flowing back into the HB5, setting the duty cycle to 
**  0% prevents short circuits in the hbridge when 
**  changing directions. 
**  
**  This function should be called repeatedly checking 
**  until returning 1 indicating that conditions for 
**  direction change have been met and the direction 
**  change applied. PmodHB5getQEncRPM should be called
**  prior attempting direction change (except when RPM
**  is known to be 0, for example when the module is
**  first initialized) for a least one RPM polling period
**  as specified in PmodHB5getQEncRPM to ensure an accurate
**  RPM value. If hBridge->newDirection == hBridge->currentDirection
**  no change will be made and a value of one will be returned.
*/
uint8_t PmodHB5ChangeDirection(HBRIDGE *hBridge)
{	
	if(hBridge->newDirection != hBridge->currentDirection)
	{
		PmodHB5SetDCPWMDutyCycle(0,hBridge->ocChannel);
	
		if(hBridge->rpm == 0)
		{
			PORTToggleBits(hBridge->directionPort,hBridge->directionPortBit);
			hBridge->currentDirection = hBridge->newDirection;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return 1;
}

