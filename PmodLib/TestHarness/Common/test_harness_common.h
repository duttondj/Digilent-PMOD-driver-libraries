/* -------------------------------------------------------------------- */
/*                                                                      */
/*                    test_harness_common.h                             */
/*                                                                      */
/*                                                                      */
/* -------------------------------------------------------------------- */
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/* -------------------------------------------------------------------- */
/*  Revision History:													*/
/*                                                                      */
/*  10/18/2011(RyanH):                                                  */
/*                                                                      */
/* -------------------------------------------------------------------- */

#ifndef _TEST_HARNESS_COMMON_
#define _TEST_HARNESS_COMMON_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <plib.h>


void SetupSerialLogging(uint32_t baudRate,uint32_t pbClock,UART_MODULE uartID);
uint8_t ConsoleMenu(uint8_t *pmodName,uint8_t *testNames[],uint32_t numCommands,UART_MODULE uartID);
uint8_t getIntegerFromConsole(UART_MODULE uartID);
uint8_t UARTGetOneByte(UART_MODULE uartID);
void UARTPutS(uint8_t *string,UART_MODULE uartID);


#endif
