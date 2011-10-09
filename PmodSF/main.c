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
#pragma config FPBDIV = DIV_2

#define SYSTEM_CLOCK		80000000
#define PB_CLOCK (SYSTEM_CLOCK/2)
#define PMODSF_BITRATE 	156250
#define NUM_TEST_FUNCTIONS 7

int main(void)
{
	uint8_t featureSet = __PIC32_FEATURE_SET__;
	uint8_t channel = 0;
	uint8_t procType[128];
	sprintf(procType,"\r\n**Pic32 %d processor detected**",__PIC32_FEATURE_SET__);
	uint8_t *menuItems[NUM_TEST_FUNCTIONS] = {"UNIT_spfPMOD_ReadID()","UNIT_sfPMODF_ReadStatusReg()","UNIT_sfPMODF_ClearSetReadWriteStatusRegBits","UNIT_sfPMODF_PageProgram()","UNIT_spfPMOD_DPD_Release()","UNIT_sfPMODF_SectorErase","UNIT_sfPMODF_BulkErase"};
	uint8_t (*testFunc[NUM_TEST_FUNCTIONS])(uint8_t,UART_MODULE) = {UNIT_spfPMOD_ReadID,UNIT_sfPMODF_ReadStatusReg,UNIT_sfPMODF_ClearSetReadWriteStatusRegBits,UNIT_sfPMODF_PageProgram,UNIT_spfPMOD_DPD_Release,UNIT_sfPMODF_SectorErase,UNIT_sfPMODF_BulkErase};
	SetupSerialLogging(9600,PB_CLOCK,UART1); //setup serial console IO
	
	UARTPutS(procType,UART1); 	
	
	putsUART1("\r\nPmodSF SPI port=>");
	channel =  getIntegerFromConsole(UART1); //SPI port PMODSF is connected to
	
	PmodSFInit(channel,PB_CLOCK,PMODSF_BITRATE);

	if(fnSetPmodFlashCapacity(channel) == PMODSF_128_MBIT)
	{
		UARTPutS("\r\n**PMODSF-128 Detected**",UART1);
	}
	else
	{
		UARTPutS("\r\n**PMODSF-16 Detected**",UART1);
	}
	
	while(1)
	{
		if((*testFunc[ConsoleMenu(menuItems,NUM_TEST_FUNCTIONS,UART1)])(channel,UART1))
		{
			putsUART1("Test Passed\r\n");
		}
		else
		{
			putsUART1("Test Failed\r\n");
		}	
	}
}
