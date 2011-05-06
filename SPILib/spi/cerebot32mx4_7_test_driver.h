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
#include "spi_mx4_7.h"
#include "pmodsf.h"


#define SYSTEM_CLOCK		80000000

int UNIT_spfPMOD_ReadID();
int UNIT_SPI_MasterNoFrameCKE_Rev();
int UNIT_sfPMODF_ReadStatusReg();
int UNIT_sfPMODF_WriteStatusReg();
int UNIT_sfPMODF_PageProgram();
int SetupSerialLogging();
int ConsoleMenu(char *testNames[],int numCommands);
int getIntegerFromConsole();


#endif