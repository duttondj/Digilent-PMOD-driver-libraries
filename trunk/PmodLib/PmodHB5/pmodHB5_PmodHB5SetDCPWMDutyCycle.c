/* -------------------------------------------------------------------- */
/*                                                                      */
/* pmodHB5_PmodHB5SetDCPWMDutyCycle.c --								*/
/*                        implimentation for PmodHB5SetDCPWMDutyCycle   */
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
#include <plib.h>
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
