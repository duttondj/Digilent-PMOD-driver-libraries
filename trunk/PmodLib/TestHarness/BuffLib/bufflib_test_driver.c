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
void BufLib_INIT(UART_MODULE uartID)
{
	if(!BufLibInitBuffers(3, 6))
	{
		UARTPutS("\r\n\r\nFailed to initalize buffers\r\n", uartID);
	}

	UARTPutS("\r\n\r\nBuffers Initialized\r\n", uartID);
}

uint8_t UNIT_BufLibWriteRead(UART_MODULE uartID)
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
		UARTPutS("Successfully Read from Buffer\r\n", uartID);
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

uint8_t UNIT_BufLibWriteReadOverflow(UART_MODULE uartID)
{
	uint8_t result = 0;
	uint16_t value = 0;

	BufLibWriteBuffer(1);
	BufLibWriteBuffer(2);
	BufLibWriteBuffer(3);
	if(BufLibWriteBuffer(4))
	{
		result = 1;
	}

	BufLibReadBuffer(&value);
	BufLibReadBuffer(&value);
	BufLibReadBuffer(&value);
	
	if(!BufLibReadBuffer(&value) || value != 4)
	{
		result = 0;
	}
	
	return result;
}

uint8_t UNIT_BufLibInvalidRead(UART_MODULE uartID)
{
	uint16_t value = 0;
	return !BufLibReadBuffer(&value);
}

uint8_t UNIT_BufLibInvalidWrite(UART_MODULE uartID)
{
	return 0;
}