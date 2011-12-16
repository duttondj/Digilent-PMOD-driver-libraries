/************************************************************************/
/*                                                                      */
/*  This file contains functions declaration for use with the n-way     */
/*  buffering library					                                */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ross Kristof											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  This file contains functions declaration for use with the n-way     */
/*  buffering library									                */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  11/7/2011(RossK):                                                    */
/*                                                                      */
/************************************************************************/

#ifndef _BUFFLIB_TEST_DRIVER_H_
#define _BUFFLIB_TEST_DRIVER_H_

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "./PmodCommon/bufferlib/bufferlib.h"
#include "./TestHarness/Common/test_harness_common.h"

#define SYSTEM_CLOCK 80000000L  //system clock speed (8 MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
#define PB_DIV 2
#define PB_CLOCK (SYSTEM_CLOCK/PB_DIV) //peripheral bus clock

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */
void BufLib_INIT(UART_MODULE uartID);
uint8_t UNIT_BufLibWriteRead(UART_MODULE uartID);
uint8_t UNIT_BufLibWriteReadOverflow(UART_MODULE uartID);
uint8_t UNIT_BufLibInvalidRead(UART_MODULE uartID);
uint8_t UNIT_BufLibInvalidWrite(UART_MODULE uartID);

#endif