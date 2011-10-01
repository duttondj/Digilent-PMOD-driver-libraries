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
	char *menuItems[NUM_TEST_FUNCTIONS] = {"UNIT_spfPMOD_ReadID()","UNIT_sfPMODF_ReadStatusReg()","UNIT_sfPMODF_WriteStatusReg()","UNIT_sfPMODF_PageProgram()","UNIT_spfPMOD_DPD_Release()"};
	int (*testFunc[NUM_TEST_FUNCTIONS])(void) = {UNIT_spfPMOD_ReadID,UNIT_sfPMODF_ReadStatusReg,UNIT_sfPMODF_WriteStatusReg,UNIT_sfPMODF_PageProgram,UNIT_spfPMOD_DPD_Release};
	
	PmodSFInit(2,PB_CLOCK,PMODSF_BITRATE);
 	SetupSerialLogging(9600,PB_CLOCK);
	
	while(1)
	{
		(*testFunc[ConsoleMenu(menuItems,NUM_TEST_FUNCTIONS)])();
	}

}
/*
	uint32_t pmodID = 0; 
	uint8_t statusReg = 0;
	PmodSFInit(2,PB_CLOCK,PMODSF_BITRATE);
	pmodID = PmodSFReadID(2);
	PmodSFWriteStatusRegister(2,PMODSF_SR_BP2|PMODSF_SR_BP1|PMODSF_SR_BP0);
	statusReg = PmodSFReadStatusRegister(2);
	PmodSFDisableBlockProtection(2,PMODSF_SR_BP2|PMODSF_SR_BP1|PMODSF_SR_BP0);
    statusReg = PmodSFReadStatusRegister(2);
	PmodSFEnableleBlockProtection(2,PMODSF_SR_BP0);
	statusReg = PmodSFReadStatusRegister(2);
	PmodSFEnableleBlockProtection(2,PMODSF_SR_BP1);
	statusReg = PmodSFReadStatusRegister(2);
	PmodSFEnableleBlockProtection(2,PMODSF_SR_BP2);
	statusReg = PmodSFReadStatusRegister(2);
	PmodSFBulkErase(2);


}

*/