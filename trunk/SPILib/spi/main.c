/************************************************************************/
/*                                                                      */
/*   main.c test driver functions for SPI,PMODSF                        */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  This file drives test functions used in for the Digilent            */
/*  PMODSF,SPI on the Digilent CEREBOT32MX4 and CEREBOT32MX7            */
/*  SETUP FOR CEREBOT32MX4:                                             */
/*  PMOD CLS - Connected to SPI1 (J1)                                   */
/*  PMODSF - Connected JB  pins JB01  - JB06                            */
/*  PMODRS232 - connected to JE pins JE01 - JE06                        */
/*  PC - connected to PMODRS232 at 9600 baud, 8 data bits, 1 stop bit,  */
/*       parity none, flow control off                                  */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  5/5/2011(RyanH):                                                    */
/*                                                                      */
/************************************************************************/

#include <peripheral/spi.h>
#include <stdio.h>
#include <string.h>
#include "cerebot32mx4_7_test_driver.h"
#include "spi_mx4_7.h"
#include "pmodsf.h"
#define NUM_TEST_FUNCTIONS 5
#pragma config FNOSC = PRIPLL
#pragma config FPLLMUL = MUL_20
#pragma config FPLLIDIV = DIV_2
#pragma config FPBDIV = DIV_2
#pragma config FPLLODIV = DIV_1

int main(void)
{
	char *menuItems[NUM_TEST_FUNCTIONS] = {"UNIT_spfPMOD_ReadID()","UNIT_sfPMODF_ReadStatusReg()","UNIT_sfPMODF_WriteStatusReg()","UNIT_sfPMODF_PageProgram()","UNIT_SPI_MasterNoFrameCKE_Rev()"};
	int (*testFunc[NUM_TEST_FUNCTIONS])(void) = {UNIT_spfPMOD_ReadID,UNIT_sfPMODF_ReadStatusReg,UNIT_sfPMODF_WriteStatusReg,UNIT_sfPMODF_PageProgram,UNIT_SPI_MasterNoFrameCKE_Rev};
	
 	SetupSerialLogging(9600);
	
	while(1)
	{
		(*testFunc[ConsoleMenu(menuItems,NUM_TEST_FUNCTIONS)])();
	}

}