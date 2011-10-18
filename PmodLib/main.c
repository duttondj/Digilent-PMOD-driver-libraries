/************************************************************************/
/*                                                                      */
/*                       main.c                                         */
/*      
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  10/18/2011(RyanH):                                                  */
/*                                                                      */
/************************************************************************/

#include <stdint.h>
#include <peripheral/uart.h>
#include "./TestHarness/config.h"
#include "./TestHarness/Common/test_harness_common.h"

uint32_t systemClock = SYSTEM_CLOCK;

uint8_t UNIT_Exec_All(uint8_t chn, UART_MODULE uartID);

//**************************************
//*              PmodSF                *
//*     test functions for PmodSF      *
//**************************************
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
/
//**************************************
//*            PmodJSTK                *
//*   test functions for PmodJSTK      *
//**************************************
#elif(PMODJSTK == 1)
#include "./TestHarness/PmodJSTK/pmod_jstk_test_driver.h"
#define NUM_TEST_FUNCTIONS 12 //number of test functions for PmodJSTK

//Array of function pointers to tests for PmodJSTK
uint8_t (*testFunc[NUM_TEST_FUNCTIONS])(uint8_t,UART_MODULE) = {UNIT_PmodJSTKLed_OFF,
						UNIT_PmodJSTKLed1_ON,UNIT_PmodJSTKLed2_ON,UNIT_PmodJSTKLed1_Led2_ON,
						UNIT_PmodJSTKAxisBounds,UNIT_PmodJSTKButton_1,UNIT_PmodJSTKButton_2,
						UNIT_PmodJSTKButton_Jstk,UNIT_PmodJSTK10usDelay,UNIT_PmodJSTK15usDelay,UNIT_Exec_All};

//Menu Item text pssed into console menu
uint8_t *menuItems[NUM_TEST_FUNCTIONS] = {"UNIT_PmodJSTKLed_OFF","UNIT_PmodJSTKLed1_ON",
                "UNIT_PmodJSTKLed2_ON","UNIT_PmodJSTKLed1_Led2_ON","UNIT_PmodJSTKAxisBounds",
				"UNIT_PmodJSTKButton_1","UNIT_PmodJSTKButton_2","UNIT_PmodJSTKButton_Jstk","UNIT_PmodJSTK10usDelay","UNIT_PmodJSTK15usDelay","UNIT_Exec_All"};

//Name of module to display on console menu
char * pmodName = "PmodJSTK";

//Pmod initialization macro for PmodJSTK
#define INITPMOD(CHN,LOG_UART) PmodJSTKInit(CHN,PB_CLOCK,SPI_BITRATE,SYSTEM_CLOCK);
//**************************************

#endif



int main()
{
	uint8_t channel = 0;
	uint8_t procType[128];
	sprintf(procType,"\r\n**Pic32 %d processor detected**",__PIC32_FEATURE_SET__);
	SetupSerialLogging(9600,PB_CLOCK,UART1); //setup serial console IO
	UARTPutS(procType,UART1); 	
	UARTPutS("\r\nPmodJSTK SPI port=>",UART1);
	channel =  getIntegerFromConsole(UART1); //SPI port PMODSF is connected to
	INITPMOD(channel,UART1);
	while(1)
	{
		if((*testFunc[ConsoleMenu(pmodName,menuItems,NUM_TEST_FUNCTIONS,UART1)])(channel,UART1))
		{
			UARTPutS("Test Passed\r\n",UART1);
		}
		else
		{
			UARTPutS("Test Failed\r\n",UART1);
		}	
	}
	
	return 0;
}

uint8_t UNIT_Exec_All(uint8_t chn, UART_MODULE uartID)
{
	uint8_t index = 0;
	uint8_t individualTestResult = 0;
	uint8_t overallTestResults = 1;
	for(index = 0;index < NUM_TEST_FUNCTIONS - 1;index++)
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

		overallTestResults &= individualTestResult;
	}

	UARTPutS("Overall Results: ",uartID);

	return overallTestResults;
}
