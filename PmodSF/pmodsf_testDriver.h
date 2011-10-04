/************************************************************************/
/*                                                                      */
/*   cerebot32mx4_7_test_driver.h test driver function declarations     */
/*   for SPI,PMODSF                                                     */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  This file contains test functions used in for the Digilent          */
/*  PMODSF,SPI on the Digilent CEREBOT32MX4 and CEREBOT32MX7            */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  5/5/2011(RyanH):                                                    */
/*                                                                      */
/************************************************************************/

#ifndef _CEREBOT32MX4_7_SPI_TEST_DRIVER_
#define _CEREBOT32MX4_7_SPI_TEST_DRIVER_
#include <stdio.h>
#include <string.h>
#include <peripheral/spi.h>
#include <peripheral/ports.h>	// Enable port pins for input or output 
#include <peripheral/uart.h>	// Enable UARTs 1 and 2 for reception and transmission of serial data
#include <peripheral/system.h>	// Set up the system and perihperal clocks for best performance
#include "pmodsf.h"

uint8_t UNIT_spfPMOD_DPD_Release(uint8_t chn);
uint8_t UNIT_spfPMOD_ReadID(uint8_t chn);
uint8_t UNIT_sfPMODF_ReadStatusReg(uint8_t chn);
uint8_t  UNIT_sfPMODF_ReadWriteStatusReg(uint8_t chn);
uint8_t UNIT_sfPMODF_PageProgram(uint8_t chn);
uint8_t SetupSerialLogging(uint32_t baud_rate,uint32_t pbClock);
uint8_t ConsoleMenu(char *testNames[],uint32_t numCommands);
uint8_t getIntegerFromConsole();
uint8_t fnGetByteFromUint32(uint32_t value,uint8_t bytePos);


#endif