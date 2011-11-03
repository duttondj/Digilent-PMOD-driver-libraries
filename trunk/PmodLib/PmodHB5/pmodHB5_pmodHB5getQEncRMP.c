/* -------------------------------------------------------------------- */
/*                                                                      */
/* pmodHB5_pmodHB5getQEncRMP.c --implimentation for PmodHB5getQEncRMP   */
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
**  PmodHB5getQEncRPM
**
**	Synopsis:
**	Returns quadrature encoding, calculates and returns RPM
**  Input: 
**		HBRIDGE *hBridge - HBRIDGE struct
**      uint32_t pollsPerSec - number of quadrature 
**                         encoding polls per second
**
**  Returns: rpm, quadrature encoding
**
**	Errors:	none
**
**  Description:
**  The time period used for calculating RPM is pollPerSec/pollDivisor, 
**  Example: Calculate RPM every 200ms: calling PmodHB5getQEncRPM 1000
**  times per second, set pollsPerSec = 1000 and pollDivisor = 5, resulting
**  in a polling period of 200ms, the current number of polls for this time period
**  is stored in HBRIDGE.pollCount and is incremented every time this function is called.
**  When a change in state is detected in quadrature position is detected, HBRIDGE.pulseCount
**  is incremented. Once HBRIDGE.pollCount reaches the determined polling period of
**  200ms (pollCount == 200), Rpm is calculated. Quadrature encoding is stored in 
**  HBRIDGE.quadPos every time this function is called.
*/
void PmodHB5getQEncRPM(HBRIDGE *hBridge,uint32_t pollsPerSec,uint32_t pollDivisor)
{
	//read and calculate the current quadratue encoding
	uint8_t newQuadPos = PORTReadBits(hBridge->sensorAport,hBridge->sensorAportBit) ^ PORTReadBits(hBridge->sensorBport,hBridge->sensorBportBit) ;
	pollsPerSec /= pollDivisor; //determine polling period based on number of polls made in a second
	
	hBridge->pollCount++; //increase the number of polls total
	
	//the position has changed
	if(newQuadPos != hBridge->quadPos)
	{
		hBridge->quadPos = newQuadPos;
		hBridge->pulseCount++;
	}
	//number of polls reached for RPM calculation
	if(hBridge->pollCount == pollsPerSec)
	{
		hBridge->prevRpm = hBridge->rpm;
		hBridge->rpm = (hBridge->pulseCount/4) * pollDivisor;

		hBridge->pulseCount = 0;
		hBridge->pollCount = 0;
	}
	
}
