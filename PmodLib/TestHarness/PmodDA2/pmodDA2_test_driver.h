/************************************************************************/
/*                                                                      */
/*  This file contains functions declaration for use with the  Digilent */
/*  PMODDA2 on Digilent Pic32 based boards                              */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ross Kristof											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  This file contains definitions for functions for testing            */
/*  the  Digilent PMODDA2 on Digilent Pic32 based boards                */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  12/3/2011(RossK):                                                    */
/*                                                                      */
/************************************************************************/

#ifndef _PMODDA2_TEST_DRIVER_H_
#define _PMODDA2_TEST_DRIVER_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <plib.h>
#include "./PmodSF/pmodsf.h"
#include "./PmodDA2/pmodDA2.h"
#include "./TestHarness/Common/test_harness_common.h"
#include "./PmodCommon/bufferlib/bufferlib.h"

#define SYSTEM_CLOCK 80000000L  //system clock speed (8 MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
#define PB_CLOCK (SYSTEM_CLOCK/2) //peripheral bus clock
#define SPI_BITRATE 20000000 //bit rate for SPI port

volatile UART_MODULE PmodDA2UartID;
volatile SpiChannel chn;
volatile uint16_t PmodDA2Sound;

void PmodDA2_INIT(UART_MODULE uartID);
uint8_t UNIT_PmodDA2PlaySound(UART_MODULE uartID);

#endif