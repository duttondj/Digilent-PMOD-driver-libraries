/************************************************************************/
/*                                                                      */
/*   pmodDA2_test_driver.c -- test driver functions for PmodDA2         */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ross Kristof											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  This file contains implementation for functions used for use with   */
/*  the  Digilent PMODDA2 on Digilent Pic32 based boards                */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  12/3/2011(RossK):                                                   */
/*                                                                      */
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "./TestHarness/PmodDA2/pmodDA2_test_driver.h"

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */


void PmodDA2_INIT(UART_MODULE uartID)
{
	PmodDA2UartID = uartID;
	INTEnableSystemMultiVectoredInt ();

	PmodDA2Sound = 0xEFF;

	PORTSetPinsDigitalOut (IOPORT_B, BIT_10 |  BIT_11 |  BIT_12 |  BIT_13);
	PORTClearBits (IOPORT_B,  BIT_10 |  BIT_11 |  BIT_12 |  BIT_13);

	UARTPutS("\r\nPmodDA2 SPI port=>",uartID);
	chn =  getIntegerFromConsole(uartID); //SPI port PMODSF is connected to
	PmodDA2Init(chn,PB_CLOCK,SPI_BITRATE);
}

uint8_t UNIT_PmodDA2PlaySound(UART_MODULE uartID)
{
	uint8_t result;
	uint32_t i = 0, j = 0, k = 0;

	OpenTimer2 (T2_ON | T2_IDLE_CON | T2_PS_1_1, 5000);
    // Configure interrupt for Timer1
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_7);

	mT2IntEnable(1);

	UARTPutS("Is a sound playing? (Y/N)\r\n",uartID);
	result = UARTGetOneByte(uartID);
	
	if(result == 'Y')
	{
		result = 1;
	}
	else
	{
		result = 0;
	}

	mT2IntEnable(0);
	PORTClearBits (IOPORT_B,  BIT_10 |  BIT_11 |  BIT_12 |  BIT_13);
	
	return result;
}

void __ISR(_TIMER_2_VECTOR, ipl7) fnTimer1Int(void)
{
	// Clear the timer interrupt and call our handler function
	mT2ClearIntFlag ();
	PmodDA2Send(chn, PmodDA2Sound);
	//PORTToggleBits (IOPORT_B, BIT_10 | BIT_11 | BIT_12 | BIT_13 );
}