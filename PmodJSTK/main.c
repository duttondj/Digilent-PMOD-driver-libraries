/************************************************************************/
/*                                                                      */
/*   main.c test driver functions for SPI,PMODJSTK                      */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  This file drives test functions used in for the Digilent            */
/*  PmodJSTK on the Digilent CEREBOT32MX4 and CEREBOT32MX7              */
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
#include <peripheral/uart.h>
#include <peripheral/ports.h>
#include <peripheral/system.h>

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "pmodJSTK.h"
#include "pmod_jstk_test_driver.h"

#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1  //(8 MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
#pragma config FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL
#pragma config FPBDIV = DIV_2


#define SYSTEM_CLOCK 80000000
#define PB_CLOCK (SYSTEM_CLOCK/2)
#define PMODJSTK_BITRATE 625000
#define NUM_TEST_FUNCTIONS 5



int main()
{
	SpiChannel channel = 2;
	uint8_t procType[128];
	sprintf(procType,"\r\n**Pic32 %d processor detected**",__PIC32_FEATURE_SET__);
	uint8_t *menuItems[NUM_TEST_FUNCTIONS] = {"UNIT_PmodJSTKLed_OFF","UNIT_PmodJSTKLed1_ON",
                "UNIT_PmodJSTKLed2_ON","UNIT_PmodJSTKLed1_Led2_ON","UNIT_PmodJSTKAxisBounds"};
	uint8_t (*testFunc[NUM_TEST_FUNCTIONS])(uint8_t,UART_MODULE) = {UNIT_PmodJSTKLed_OFF,
                UNIT_PmodJSTKLed1_ON,UNIT_PmodJSTKLed2_ON,UNIT_PmodJSTKLed1_Led2_ON,UNIT_PmodJSTKAxisBounds};
	PmodJSTKAxisButton jstkAxisButtons;
	PmodJSTKInit(channel,PB_CLOCK,PMODJSTK_BITRATE,SYSTEM_CLOCK);

	SetupSerialLogging(9600,PB_CLOCK,UART1); //setup serial console IO
	
	UARTPutS(procType,UART1); 	
	
	putsUART1("\r\nPmodJSTK SPI port=>");
	channel =  getIntegerFromConsole(UART1); //SPI port PMODSF is connected to
	
	PmodJSTKInit(channel,PB_CLOCK,PMODJSTK_BITRATE,SYSTEM_CLOCK);


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
