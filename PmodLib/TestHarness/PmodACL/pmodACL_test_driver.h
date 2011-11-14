/************************************************************************/
/*																		*/
/*	<FILE NAME>	--	<BRIEF DESCRIPTION>         						*/
/*																		*/
/************************************************************************/
/*	Author: 	<AUTHOR NAME> 											*/
/*	Copyright (C) <YEAR> <ENTITY>										*/
/************************************************************************/
/*  Module Description: 												*/
/*  <MODULE DESCRIPTION>												*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/* <MM/DD/YY>(<FIRST NAME><LAST INITIAL): <NOTES>						*/
/*																		*/
/************************************************************************/
#ifndef _PMODACL_TEST_DRIVER_H_
#define _PMODACL_TEST_DRIVER_H_
/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <plib.h>
#include <stdint.h>
#include <string.h>

#include "./PmodACL/pmodACL.h"
#include "./TestHarness/Common/test_harness_common.h"

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */
#define SYSTEM_CLOCK 80000000L  //system clock speed (8 MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
#define PB_DIV                 8
#define PRESCALE               256
#define TOGGLES_PER_SEC        2000
#define PB_CLOCK (SYSTEM_CLOCK/PB_DIV) //peripheral bus clock
#define T1_TICK               (SYSTEM_CLOCK/PB_DIV/PRESCALE/TOGGLES_PER_SEC)
#define SPI_BITRATE 625000 //bit rate for SPI port



/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */
uint8_t isrFired;
SpiChannel chn;

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */

void fnPmodACL_INIT(UART_MODULE uartID);
void fnSetupPmodACLForInterruptsInt1(uint8_t interruptMask,uint8_t tapAxes,uint8_t fifoCtl);

uint8_t UNIT_PmodACLGetDeviceID(UART_MODULE uartID);
uint8_t UNIT_PmodACLGetAxisData(UART_MODULE uartID);
uint8_t UNIT_PmodACLGetAxisData(UART_MODULE uartID);
uint8_t UNIT_PmodACLCalibrate(UART_MODULE uartID);
uint8_t UNIT_PmodACLSetGetTapThresh(UART_MODULE uartID);
uint8_t UNIT_PmodACLInterupt(UART_MODULE uartID);
uint8_t UNIT_PmodACLSetGetDataFormat(UART_MODULE uartID);
uint8_t UNIT_PmodACLSetGetPowerCtl(UART_MODULE uartID);
uint8_t UNIT_PmodACLGSetGetFIFOCtl(UART_MODULE uartID);
uint8_t UNIT_PmodACLSetGetOffset(UART_MODULE uartID);
uint8_t UNIT_PmodACLSetGetIntEnable(UART_MODULE uartID);
uint8_t UNIT_PmodACLSetGetIntMap(UART_MODULE uartID);
uint8_t UNIT_PmodACLSetGetIntSource(UART_MODULE uartID);
uint8_t UNIT_PmodACLSetGetTapAxes(UART_MODULE uartID);
uint8_t UNIT_PmodACLSetGetTapDuration(UART_MODULE uartID);
uint8_t UNIT_PmodACLSetGetTapLatency(UART_MODULE uartID);
uint8_t UNIT_PmodACLSetGetTapWindow(UART_MODULE uartID);
uint8_t UNIT_PmodACLGetActTapStatus(UART_MODULE uartID);
uint8_t UNIT_PmodACLSetGetThreshFF(UART_MODULE uartID);
uint8_t UNIT_PmodACLSetGetBwRate(UART_MODULE uartID);
uint8_t UNIT_PmodACLGetFIFOStatus(UART_MODULE uartID);
uint8_t UNIT_PmodACLSetGetTimeInact(UART_MODULE uartID);
uint8_t UNIT_PmodACLSetGetThreshAct(UART_MODULE uartID);
uint8_t UNIT_PmodACLSetGetActInactCtl(UART_MODULE uartID);

#endif

