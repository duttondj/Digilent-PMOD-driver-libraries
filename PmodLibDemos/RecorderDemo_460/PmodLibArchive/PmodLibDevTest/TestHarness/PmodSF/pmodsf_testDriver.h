/************************************************************************/
/*                                                                      */
/*  This file contains functions declaration for use with the  Digilent */
/*  PMODSF on Digilent Pic32 based boards                               */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  This file contains definitions for functions for testing            */
/*  the  Digilent PMODSF on Digilent Pic32 based boards                 */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  5/5/2011(RyanH):                                                    */
/*                                                                      */
/************************************************************************/

#ifndef _PMODSF_TEST_DRIVER_H_
#define _PMODSF_TEST_DRIVER_H_
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <plib.h>
#include "./PmodSF/pmodsf.h"
#include "./TestHarness/Common/test_harness_common.h"

#define SYSTEM_CLOCK 80000000L  //system clock speed (8 MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
#define PB_CLOCK (SYSTEM_CLOCK/2) //peripheral bus clock
#define SPI_BITRATE 625000 //bit rate for SPI port

#define PMODSF_16_SECTOR_SIZE  0x10000
#define PMODSF_128_SECTOR_SIZE 0x40000 

#define PMODSF_128_MFID 0x202018
#define PMODSF_16_MFID 0x202015
#define PMODSF_16_RES_SIG 0x14

static SpiChannel chn;
static uint8_t pmodFlashCapacity = PMODSF_16_MBIT;

uint8_t UNIT_spfPMOD_DPD_Release(UART_MODULE uartID);
uint8_t UNIT_spfPMOD_ReadID(UART_MODULE uartID);
uint8_t UNIT_sfPMODF_ReadStatusReg(UART_MODULE uartID);
uint8_t UNIT_sfPMODF_ClearSetReadWriteStatusRegBits(UART_MODULE uartID);
uint8_t UNIT_sfPMODF_PageProgram(UART_MODULE uartID);
uint8_t UNIT_sfPMODF_BulkErase(UART_MODULE uartID);
uint8_t UNIT_sfPMODF_SectorErase(UART_MODULE uartID);
void fnSetPmodFlashCapacity(UART_MODULE uart);
void fnInitPmodSF(UART_MODULE uart);
#endif

