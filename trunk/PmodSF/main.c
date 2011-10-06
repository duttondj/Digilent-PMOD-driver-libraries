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
#include <stdint.h>
#include "pmodsf_testDriver.h"
#include "pmodsf.h"
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2
#pragma config FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL
#pragma config FPBDIV 	 = DIV_2

#define SYSTEM_CLOCK		80000000
#define PB_CLOCK (SYSTEM_CLOCK/2)
#define PMODSF_BITRATE 156250
#define NUM_TEST_FUNCTIONS 5






int main(void)
{
	uint8_t channel = 0;
	char *menuItems[NUM_TEST_FUNCTIONS] = {"UNIT_spfPMOD_ReadID()","UNIT_sfPMODF_ReadStatusReg()","UNIT_sfPMODF_ReadWriteStatusReg","UNIT_sfPMODF_PageProgram()","UNIT_spfPMOD_DPD_Release()"};
	uint8_t (*testFunc[NUM_TEST_FUNCTIONS])(uint8_t) = {UNIT_spfPMOD_ReadID,UNIT_sfPMODF_ReadStatusReg,UNIT_sfPMODF_ReadWriteStatusReg,UNIT_sfPMODF_PageProgram,UNIT_spfPMOD_DPD_Release};
	SetupSerialLogging(9600,PB_CLOCK);
	putsUART1("\r\nPmodSF SPI port=>");
	channel =  getIntegerFromConsole();
	PmodSFInit(channel,PB_CLOCK,PMODSF_BITRATE);
 	fnSetPmodFlashCapacity(channel);
	
	while(1)
	{
		(*testFunc[ConsoleMenu(menuItems,NUM_TEST_FUNCTIONS)])(channel);
	}

}
