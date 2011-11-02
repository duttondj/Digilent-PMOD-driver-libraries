/************************************************************************/
/*                                                                      */
/*                     pmod_jstk_test_driver.h                          */
/*              test driver head file for PmodJSTK                      */   
/*                Copyright (C) 2011 Ryan Hoffman                       */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  This file contains headers for functions used for use with          */
/*  the  Digilent PMODJSTK on Digilent Pic32 based boards               */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  10/13/2011(RyanH):                                                  */
/*                                                                      */
/************************************************************************/

#ifndef _PMODJSTK_TEST_DRIVER_H_
#define _PMODJSTK_TEST_DRIVER_H_

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <peripheral/spi.h>
#include <peripheral/uart.h>
#include <peripheral/ports.h>
#include <stdint.h>
#include <sys/clock.h>
#include "./PmodJSTK/pmodJSTK.h"
#include "./TestHarness/Common/test_harness_common.h"

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */

#define JSTK_X_AXIS 0
#define JSTK_Y_AXIS 1

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */

extern uint32_t systemClock; //forward declaration for the system clock 

uint8_t UNIT_PmodJSTKAxisBounds(uint8_t chn, UART_MODULE uartID);
uint8_t UNIT_PmodJSTKLed_OFF(uint8_t chn, UART_MODULE uartID);
uint8_t UNIT_PmodJSTKLed1_ON(uint8_t chn, UART_MODULE uartID);
uint8_t UNIT_PmodJSTKLed2_ON(uint8_t chn, UART_MODULE uartID);
uint8_t UNIT_PmodJSTKLed1_Led2_ON(uint8_t chn, UART_MODULE uartID);

uint8_t UNIT_PmodJSTKButton_1(uint8_t chn, UART_MODULE uartID);
uint8_t UNIT_PmodJSTKButton_2(uint8_t chn, UART_MODULE uartID);
uint8_t UNIT_PmodJSTKButton_Jstk(uint8_t chn, UART_MODULE uartID);
uint8_t UNIT_PmodJSTKButton_None(uint8_t chn, UART_MODULE uartID);

uint8_t UNIT_PmodJSTK10usDelay(uint8_t chn,UART_MODULE uartID);
uint8_t UNIT_PmodJSTK15usDelay(uint8_t chn,UART_MODULE uartID);

uint8_t execAxisBounds(uint8_t *testString, uint16_t lowerBound,uint16_t upperBound,UART_MODULE uartID,uint8_t whichAxis,uint8_t chn);
uint8_t execLedTest(uint8_t *testString,uint8_t chn,UART_MODULE uartID,uint8_t command);
uint8_t execButtonTest(uint8_t *testString,uint8_t chn,UART_MODULE uartID,uint8_t button);


#endif
