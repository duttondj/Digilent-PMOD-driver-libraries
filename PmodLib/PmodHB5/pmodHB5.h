/* -------------------------------------------------------------------- */
/*                                                                      */
/*            pmodHB5.h - Header file for manipulating PmodHB5          */
/*                        with DC motors and output comparators         */
/*                        utilizing pulse width modulation              */
/*                                                                      */
/* -------------------------------------------------------------------- */
/*	Author: 	Ryan Hoffman											*/
/*	Copyright (C) 2011 Ryan Hoffman    									*/
/************************************************************************/
/*  Module Description: 												*/
/*            Header file for manipulating PmodHB5                      */
/*            with DC motors and output comparators                     */
/*            utilizing pulse width modulation                          */
/* -------------------------------------------------------------------- */
/*  Revision History:													*/
/*                                                                      */
/*  11/3/2011(RyanH):                                                   */
/*                                                                      */
/* -------------------------------------------------------------------- */

#ifndef _PMOD_HB5_H_
#define _PMOD_HB5_H_

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include <stdint.h>
#include <plib.h>

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */

/* HBRIDGE:
/* Maintains state for an instance of a PmodHB5
/* Initialization example for OC2 on Cerebot32MX4:
/*  
/* hbridge.sensorAport = IOPORT_D; 
/* hbridge.sensorAportBit = BIT_9;
/* hbridge.sensorBport = IOPORT_C;
/* hbridge.sensorBportBit = BIT_1;
/* hbridge.directionPort = IOPORT_D;
/* hbridge.directionPortBit = BIT_7;
/* hbridge.currentDirection = PMOD_HB5_DIR_FWD;
/* hbridge.newDirection = PMOD_HB5_DIR_FWD;
/* hbridge.ocChannel = 2; 
/* hbridge.pulseCount = 0;
/* hbridge.pollCount = 0;
/* hbridge.rpm = 0;
/* hbridge.prevRpm = 0;
/* hbridge.quadPos = 0;
/* 
*/
typedef struct
{
	uint8_t quadPos;          	//quadrature encoding (Gray encoding)
	uint16_t sensorAport;     	//Port mask for sensor A
	uint16_t sensorAportBit;  	//Bit mask for sensor A
	uint16_t sensorBport;     	//Port mask for sensor B
	uint16_t sensorBportBit;  	//Bit mask for sensorB
	uint16_t directionPort;		//Port mask for direction
	uint16_t directionPortBit;  //Bit mask for direction
	uint32_t pulseCount;        //Quadrature encoding state changes/time period detetected
	uint32_t pollCount;         //Number of quadrature encoding state polls taken 
	uint16_t rpm;               //Current motor RPM
	uint16_t prevRpm;           //Previous motor RPM
	uint8_t newDirection;       //New drirection of motor (PMOD_HB5_DIR_CW,PMOD_HB5_DIR_CCW)
	uint8_t currentDirection;   //Current direction of motor (PMOD_HB5_DIR_CW,PMOD_HB5_DIR_CCW)
	uint8_t ocChannel;          //Output comparator channel, (1-5)
	
}HBRIDGE;

#define PMOD_HB5_DIR_CW 0x0  //Motor direction clockwise
#define PMOD_HB5_DIR_CCW 0x1 //motor direction counter clockwise

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
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
uint8_t PmodHB5ChangeDirection(HBRIDGE *hBridge);

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
void PmodHB5SetDCInitialDirection(HBRIDGE *hBridge);

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
void PmodHB5SetDCPWMDutyCycle(uint16_t dutyCycle,uint8_t oc);

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
void PmodHB5getQEncRPM(HBRIDGE *hBridge,uint32_t pollsPerSec,uint32_t pollDivisor);




#endif
