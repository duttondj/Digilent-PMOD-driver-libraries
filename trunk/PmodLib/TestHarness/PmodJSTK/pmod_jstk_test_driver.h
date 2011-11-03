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
#include "./PmodJSTK/pmodJSTK.h"
#include "./TestHarness/Common/test_harness_common.h"

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */
#define SYSTEM_CLOCK 80000000L  //system clock speed (8 MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
#define PB_CLOCK (SYSTEM_CLOCK/2) //peripheral bus clock
#define SPI_BITRATE 625000 //bit rate for SPI port

#define JSTK_X_AXIS 0
#define JSTK_Y_AXIS 1

static SpiChannel chn;

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */

void PmodJSTK_INIT(UART_MODULE uartID);

uint8_t UNIT_PmodJSTKAxisBounds(UART_MODULE uartID);
uint8_t UNIT_PmodJSTKLed_OFF(UART_MODULE uartID);
uint8_t UNIT_PmodJSTKLed1_ON(UART_MODULE uartID);
uint8_t UNIT_PmodJSTKLed2_ON(UART_MODULE uartID);
uint8_t UNIT_PmodJSTKLed1_Led2_ON(UART_MODULE uartID);

uint8_t UNIT_PmodJSTKButton_1(UART_MODULE uartID);
uint8_t UNIT_PmodJSTKButton_2(UART_MODULE uartID);
uint8_t UNIT_PmodJSTKButton_Jstk(UART_MODULE uartID);
uint8_t UNIT_PmodJSTKButton_None(UART_MODULE uartID);

uint8_t UNIT_PmodJSTK10usDelay(UART_MODULE uartID);
uint8_t UNIT_PmodJSTK15usDelay(UART_MODULE uartID);

uint8_t execAxisBounds(uint8_t *testString, uint16_t lowerBound,uint16_t upperBound,UART_MODULE uartID,uint8_t whichAxis,uint8_t chn);
uint8_t execLedTest(uint8_t *testString,UART_MODULE uartID,uint8_t command);
uint8_t execButtonTest(uint8_t *testString,UART_MODULE uartID,uint8_t button);


#endif
