/************************************************************************/
/*                                                                      */
/*                     pmodHB5_test_driver.h                            */
/*              test driver header file for PmodHB5                     */   
/*                Copyright (C) 2011 Ryan Hoffman                       */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  This file contains headers for functions used for use with          */
/*  the  Digilent PMODHB5 on Digilent Pic32 based boards                */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  11/2/2011(RyanH):                                                  */
/*                                                                      */
/************************************************************************/

#ifndef _PMODHB5_TEST_DRIVER_H_
#define _PMODHB5_TEST_DRIVER_H_

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <plib.h>
#include <stdint.h>
#include "./PmodHB5/pmodHB5.h"
#include "./TestHarness/Common/test_harness_common.h"

#define SYSTEM_CLOCK 80000000L  //system clock speed (8 MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
#define PB_DIV                 8
#define PRESCALE               256
#define TOGGLES_PER_SEC        2000
#define PB_CLOCK (SYSTEM_CLOCK/PB_DIV) //peripheral bus clock
#define T1_TICK               (SYSTEM_CLOCK/PB_DIV/PRESCALE/TOGGLES_PER_SEC)

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */
static HBRIDGE hBridge;
static uint8_t isr1Fire = 0;
static uint16_t getRPMSampleCount = 0;
#define Q_0_0 0
#define Q_0_1 1
#define Q_1_1 3
#define Q_1_0 2


/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */
uint8_t UNIT_PmodHB5ChangeDirection(UART_MODULE uartID);
uint8_t UNIT_PmodHB5SetDCPWMDutCycle(UART_MODULE uartID);
static uint8_t fnGetDirFromQenc();
void PmodHB5_INIT(UART_MODULE uartID);


#endif
