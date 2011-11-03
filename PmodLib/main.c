/* -------------------------------------------------------------------- */
/*                                                                      */
/*                       main.c                                         */
/*      Main test driver loop for testing Digilent Pmods                */
/*              Copyright (C) 2011 Ryan Hoffman                         */
/* -------------------------------------------------------------------- */
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/* -------------------------------------------------------------------- */
/*  Revision History:													*/
/*                                                                      */
/*  10/18/2011(RyanH):                                                  */
/*                                                                      */
/* -------------------------------------------------------------------- */


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include <stdint.h>
#include <peripheral/uart.h>
#include "./TestHarness/config.h"
#include "./TestHarness/Common/test_harness_common.h"


extern uint8_t *menuItems[];
extern uint8_t (*testFunc[])(UART_MODULE);
extern char * pmodName;
extern uint8_t excludeFromExecAll[];

/* ------------------------------------------------------------*/
/*          Main test loop for PMOD test harness               */
/* ------------------------------------------------------------*/
int main(void)
{
	uint8_t channel = 0; //spi channel selection
	uint8_t procType[128]; //Text representaion of processor model
	
	sprintf(procType,"\r\n**Pic32 %d processor detected**",__PIC32_FEATURE_SET__);
	
	//setup serial console logging 
	SetupSerialLogging(9600,PB_CLOCK,MENU_UART); //setup serial console IO

	//Display processor type to console
	UARTPutS(procType,MENU_UART); 	
	
	//initialize PMOD, UART is needed for init functions that
    //send output to the console
	INITPMOD(MENU_UART);

	//Main test loop
	while(1)
	{
        //display the console menu, execute the test function returned by the menu
		if((*testFunc[ConsoleMenu(pmodName,menuItems,NUM_TEST_FUNCTIONS,MENU_UART)])(MENU_UART))
		{
			UARTPutS("Test Passed\r\n",MENU_UART);
		}
		else
		{
			UARTPutS("Test Failed\r\n",MENU_UART);
		}	
	}
	
	return 0;
}

/*  UNIT TEST: UNIT_Exec_All
**
**	Synopsis:
**	Executes all unit tests in sequence returning the
**  overall test result as a pass or faill.
**
**  Input:
**		uint8_t chn - spi channel to perform tests on
**		UART_MODULE uartID - UART for test result output
**
**  Returns: 
**      uint8_t - 1 for all tests pass, 0 otherwise
**
**	Errors:	none
**
**  Description:
**  Executes all test functions in sequence for 0
**  to NUM_TEST_FUNCTION, sending each result
**  to the specified UART. This function utilizes
**  globally defined arrays which are selected 
**  at compile time based on the module selected
**  for test in /TestHarness/config.h. 
**
**  excludeFromExecAll: prevents tests with 1 in 
**  the corresponding test position from executing
**
**  menuItems: text denoting the unit test name
**             
**  testFunc: functino pointers to unit tests
**  
**  The number of elements in each array must be 
**  identical, the correspondin the menu item,filter, 
**  and function pointer for each function must 
**  have the same indice.
*/
uint8_t UNIT_Exec_All(UART_MODULE uartID)
{
	uint8_t index = 0;
	uint8_t individualTestResult = 0;
	uint8_t overallTestResults = 1;
	uint8_t textOut[128];
	for(index = 0;index < NUM_TEST_FUNCTIONS - 1;index++)
	{
		if(excludeFromExecAll[index])
		{
			sprintf(textOut,"Test %s will not be executed due to filter.\r\n",menuItems[index]);
			UARTPutS(textOut,uartID);
		}
		else
		{
			individualTestResult = (*testFunc[index])(uartID);
			if(individualTestResult)
			{
				UARTPutS("Test Passed\r\n",uartID);
			}
			else
			{
				UARTPutS("Test Failed\r\n",uartID);
			}
		}
		overallTestResults &= individualTestResult;
	}

	UARTPutS("Overall Results: ",uartID);

	return overallTestResults;
}
