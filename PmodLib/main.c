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


/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */
uint8_t UNIT_Exec_All(uint8_t chn, UART_MODULE uartID);

/* ----------------------------------------------------------- */
/*                 Test Driver Function Setup                  */
/*                                                             */
/*	The purpose of this section is to define arrays of test    */
/*  functions for use with the standardized test harness.      */    
/*  Currently this test harness only supports modules that     */
/*  utilized the serial periphal interface and UARTs for       */
/*  driving the serial console menu.                           */
/*                                                             */
/*  Each section must be formatted as follows:                 */
/*                                                             */
/*  Define this value in the "Pmod Selection" section of       */
/*  /TestHarness/config.h                                      */
/*  #define (<MODULE_NAME> == 1)                               */
/*                                                             */
/*  Include the header file for the PMOD test harness          */
/*  #include <PMOD TEST FUNCTION HEADER FILE>                  */
/*                                                             */
/*  Define the number of test functions that will be           */
/*  executed, this value is used in menu item and function     */
/*  array definition.                                          */
/*  #define NUM_TEST_FUNCTIONS <NUMBER OF TEST FUNCTIONS>      */
/*                                                             */
/*  Test function names represented as strings to be displayed */
/*  in the console test menu, ordering must be the same        */
/*  between menuItems and testFunc                             */
/*  uint8_t *menuItems[NUM_TEST_FUNCTIONS] =                   */
/*                                    {<TEST FUNCTION NAMES>}; */
/*                                                             */   
/*  Array of function pointer used by the menu system to       */
/*  execute tests, Test function prototypes must have the      */
/*  following convetions in ortder to execute correctly.       */
/*  uint8_t (*testFunc[NUM_TEST_FUNCTIONS])                    */
/*                    (uint8_t,UART_MODULE) = {TEST FUNCTIONS};*/
/*                                                             */
/*  Sting representatino of the Pmod name displayed in the     */
/*  test menu                                                  */
/*  char * pmodName = "<PMOD NAME>";                           */
/*                                                             */
/*  Pmod initialization macro, an init function should be      */
/*  present in your test driver, this macro is called in main. */
/*  #define INITPMOD(CHN,LOG_UART) <PMOD INIT FUNCTION>        */
/*                                                             */
/*  It is often desireable to exclude specific tests from the  */
/*  UNIT_Exec_All function. This array is parallel to          */
/*  the menuItems and testFunc arrays, place a 1 in the        */
/*  position of the test you wish to exclude.                  */
/*  uint8_t excludeFromExecAll[NUM_TEST_FUNCTIONS]             */
/*                   = {<FILTER_0>,<FILTER_1>,...<FILTER_N>};  */
/* ------------------------------------------------------------*/

/* ------------------------------------------------------------*/
/*                            PmodSF                           */
/*                    Test setup for PmodSF                    */
/* ------------------------------------------------------------*/
#if(PMODSF == 1)
#include "./TestHarness/PmodSF/pmodsf_testDriver.h"
#define NUM_TEST_FUNCTIONS 8  //number of test functions for PmodSF
//Menu Item text pssed into console menu
uint8_t *menuItems[NUM_TEST_FUNCTIONS] = {"UNIT_spfPMOD_ReadID()","UNIT_sfPMODF_ReadStatusReg()",
						"UNIT_sfPMODF_ClearSetReadWriteStatusRegBits","UNIT_sfPMODF_PageProgram()",
						"UNIT_spfPMOD_DPD_Release()","UNIT_sfPMODF_SectorErase","UNIT_sfPMODF_BulkErase","UNIT_Exec_All"};

//Array of function pointers to tests for PmodSF
uint8_t (*testFunc[NUM_TEST_FUNCTIONS])(uint8_t,UART_MODULE) = {UNIT_spfPMOD_ReadID,UNIT_sfPMODF_ReadStatusReg,
						UNIT_sfPMODF_ClearSetReadWriteStatusRegBits,UNIT_sfPMODF_PageProgram,UNIT_spfPMOD_DPD_Release,
						UNIT_sfPMODF_SectorErase,UNIT_sfPMODF_BulkErase,UNIT_Exec_All};

//Name of module to display on console menu
char * pmodName = "PmodSF";
//Pmod initialization macro for PmodSF
#define INITPMOD(CHN,LOG_UART) fnInitPmodSF(CHN,PB_CLOCK,SPI_BITRATE,LOG_UART);

//Filter excluding specific tests from UNIT_Exec_All 
uint8_t excludeFromExecAll[NUM_TEST_FUNCTIONS] = {0,0,0,0,0,0,0,0};

/* ------------------------------------------------------------*/
/*                            PmodJSTK                         */
/*                    Test setup for PmodJSTK                  */
/* ------------------------------------------------------------*/

#elif(PMODJSTK == 1)

//Test function header for PmodJSTK
#include "./TestHarness/PmodJSTK/pmod_jstk_test_driver.h"

#define NUM_TEST_FUNCTIONS 12 //number of test functions for PmodJSTK

//Array of function pointers to tests for PmodJSTK
uint8_t (*testFunc[NUM_TEST_FUNCTIONS])(uint8_t,UART_MODULE) = {UNIT_PmodJSTKLed_OFF,
						UNIT_PmodJSTKLed1_ON,UNIT_PmodJSTKLed2_ON,UNIT_PmodJSTKLed1_Led2_ON,
						UNIT_PmodJSTKAxisBounds,UNIT_PmodJSTKButton_1,UNIT_PmodJSTKButton_2,
						UNIT_PmodJSTKButton_Jstk,UNIT_PmodJSTKButton_None,UNIT_PmodJSTK10usDelay,
						UNIT_PmodJSTK15usDelay,UNIT_Exec_All};

//Menu Item text pssed into console menu
uint8_t *menuItems[NUM_TEST_FUNCTIONS] = {"UNIT_PmodJSTKLed_OFF","UNIT_PmodJSTKLed1_ON",
                "UNIT_PmodJSTKLed2_ON","UNIT_PmodJSTKLed1_Led2_ON","UNIT_PmodJSTKAxisBounds",
				"UNIT_PmodJSTKButton_1","UNIT_PmodJSTKButton_2","UNIT_PmodJSTKButton_Jstk","UNIT_PmodJSTKButton_None",
                "UNIT_PmodJSTK10usDelay","UNIT_PmodJSTK15usDelay","UNIT_Exec_All"};

//Name of module to display on console menu
char * pmodName = "PmodJSTK";

//Pmod initialization macro for PmodJSTK
#define INITPMOD(CHN,LOG_UART) PmodJSTKInit(CHN,PB_CLOCK,SPI_BITRATE,SYSTEM_CLOCK);

//Filter excluding specific tests from UNIT_Exec_All 
uint8_t excludeFromExecAll[NUM_TEST_FUNCTIONS] = {0,0,0,0,0,0,0,0,0,1,1,0};
//**************************************

#endif

#define MENU_UART UART2

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
	
	UARTPutS("\r\nPmodJSTK SPI port=>",MENU_UART);
	channel =  getIntegerFromConsole(MENU_UART); //SPI port PMODSF is connected to
	
	//initialize PMOD, UART is needed for init functions that
    //send output to the console
	INITPMOD(channel,MENU_UART);

	//Main test loop
	while(1)
	{
        //display the console menu, execute the test function returned by the menu
		if((*testFunc[ConsoleMenu(pmodName,menuItems,NUM_TEST_FUNCTIONS,MENU_UART)])(channel,MENU_UART))
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
uint8_t UNIT_Exec_All(uint8_t chn, UART_MODULE uartID)
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
			individualTestResult = (*testFunc[index])(chn,uartID);
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
