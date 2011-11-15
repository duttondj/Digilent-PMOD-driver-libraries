/************************************************************************/
/*                                                                      */
/*   bufflib_test_driver.c -- test driver functions for buffer library  */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ross Kristof											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  This file contains implementation for functions used for use with   */
/*  the n-way buffering library							                */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  11/7/2011(RossK0):                                                  */
/*                                                                      */
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "./TestHarness/BuffLib/bufflib_test_driver.h"

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */
uint8_t UNIT_BufLibRead(UART_MODULE uartID)
{
	uint16_t value;
	uint8_t result = 0;

	if(BufLibWriteBuffer(5))
	{
		UARTPutS("Successfully Wrote to Buffer\r\n", uartID);
	}
	else
	{
		UARTPutS("Write was unsuccessful\r\n", uartID);
	}

	if(BufLibReadBuffer(&value))
	{
		UARTPutS("Successfully read from Buffer\r\n", uartID);
	}
	else
	{
		UARTPutS("Read was unsuccessful\r\n", uartID);
	}

	if(value == 5)
	{
		result = 1;
	}

	return result;
}

uint8_t UNIT_BufLibWrite(UART_MODULE uartID)
{
	return 0;
}

uint8_t UNIT_BufLibReadOverflow(UART_MODULE uartID)
{
	return 0;
}

uint8_t UNIT_BufLibWriteOverflow(UART_MODULE uartID)
{
	return 0;
}

uint8_t UNIT_BufLibInvalidRead(UART_MODULE uartID)
{
	return 0;
}

uint8_t UNIT_BufLibInvalidWrite(UART_MODULE uartID)
{
	return 0;
}

void BufLib_INIT(UART_MODULE uartID)
{
	if(!BufLibInitBuffers(3, 6))
	{
		UARTPutS("Failed to initalize buffers\r\n", uartID);
	}

	UARTPutS("Buffers Initialized\r\n", uartID);
}