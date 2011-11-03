/* -------------------------------------------------------------------- */
/*                                                                      */
/*            pmodHB5.h - Implimentation file for manipulating PmodHB5  */
/*                        with DC motors and output comparators         */
/*                        utilizing pulse width modulation              */
/*                                                                      */
/* -------------------------------------------------------------------- */
/*	Author: 	Ryan Hoffman											*/
/*	Copyright (C) 2011 Ryan Hoffman    									*/
/************************************************************************/
/*  Module Description: 												*/
/*            Implimentation file for manipulating PmodHB5              */
/*            with DC motors and output comparators                     */
/*            utilizing pulse width modulation                          */
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
/*				Local Variables									*/
/* ------------------------------------------------------------ */

//OCxRS register, used to set output comparator duty cycle, 
//example: set duty cycle to 50% OC1RS: *DCOutPutCompare[1] = PR1/2
static uint32_t *DCOutPutCompare[] = {(uint32_t*)0x0,
#ifdef _OCMP1
				(uint32_t*)(_OCMP1_BASE_ADDRESS + _OC1CON_OC32_MASK), //OCRS1
#endif
#ifdef _OCMP2
				(uint32_t*)(_OCMP2_BASE_ADDRESS + _OC2CON_OC32_MASK), //OCRS2
#endif
#ifdef _OCMP3
				(uint32_t*)(_OCMP3_BASE_ADDRESS + _OC3CON_OC32_MASK), //OCRS3
#endif
#ifdef _OCMP4
				(uint32_t*)(_OCMP4_BASE_ADDRESS + _OC4CON_OC32_MASK), //OCRS4
#endif
#ifdef _OCMP5
				(uint32_t*)(_OCMP5_BASE_ADDRESS + _OC5CON_OC32_MASK)  //OCRS5
#endif
};

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

/*  
**  PmodHB5SetDCPWMDutyCycle
**
**	Synopsis:
**	Sets the duty cycle of the OC specified	
**
**  Input: 
**		uint16_t dutyCycle - OCx duty cycle as a percentage of PRx
**		uint8_t oc - OC channel
** 
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  Sets the PWM duty cycle of the specified output comparator.
**  Duty cycle calculated as percentage of PRx should be passed
**  example OC2 50%: dutyCycle = PR2/2
*/
void PmodHB5SetDCPWMDutyCycle(uint16_t dutyCycle,uint8_t oc)
{
	*(DCOutPutCompare[oc]) = dutyCycle;
}

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

