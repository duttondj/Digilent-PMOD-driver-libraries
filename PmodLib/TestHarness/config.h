/************************************************************************/
/*                                                                      */
/*                        config.h                                      */
/*      Configuration header for testing Digilent PmodLib               */
/*               Copyright (C) 2011 Ryan Hoffman                        */                                                                     
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  Processor configuration and contants for use with test driver main  */
/*  this file should be included in the main task loop.                 */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  10/18/2011(RyanH):                                                  */
/*                                                                      */
/************************************************************************/
#ifndef _CONFIG_H_
#define _CONFIG_H_



/* ------------------------------------------------------------ */
/*                     Pmod Selection                           */
/*  This section defines which module to test, 1 to test,       */
/*  0 to exclude, module tests are mutually exclusive           */
/*  conditional compiling will occur in main.c based            */
/*  on these values.                                            */
/* ------------------------------------------------------------ */
#define PMODSF 0    
#define PMODJSTK 1
#define PMODHB5 0




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
/* ------------------------------------------------------------ */
/*				PIC32 Configuration Settings					*/
/* ------------------------------------------------------------ */
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1  //(8 MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
#pragma config FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL
#pragma config FPBDIV = DIV_2

#define SYSTEM_CLOCK 80000000L  //system clock speed (8 MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
#define PB_CLOCK (SYSTEM_CLOCK/2) //peripheral bus clock
#define SPI_BITRATE 625000 //bit rate for SPI port
#include "./TestHarness/PmodSF/pmodsf_testDriver.h"
#define NUM_TEST_FUNCTIONS 8  //number of test functions for PmodSF
//Menu Item text pssed into console menu
uint8_t *menuItems[] = {"UNIT_spfPMOD_ReadID()","UNIT_sfPMODF_ReadStatusReg()",
						"UNIT_sfPMODF_ClearSetReadWriteStatusRegBits","UNIT_sfPMODF_PageProgram()",
						"UNIT_spfPMOD_DPD_Release()","UNIT_sfPMODF_SectorErase","UNIT_sfPMODF_BulkErase","UNIT_Exec_All"};

//Array of function pointers to tests for PmodSF
uint8_t (*testFunc[])(uint8_t,UART_MODULE) = {UNIT_spfPMOD_ReadID,UNIT_sfPMODF_ReadStatusReg,
						UNIT_sfPMODF_ClearSetReadWriteStatusRegBits,UNIT_sfPMODF_PageProgram,UNIT_spfPMOD_DPD_Release,
						UNIT_sfPMODF_SectorErase,UNIT_sfPMODF_BulkErase,UNIT_Exec_All};

//Name of module to display on console menu
char * pmodName = "PmodSF";
//Pmod initialization macro for PmodSF
#define INITPMOD(CHN,LOG_UART) fnInitPmodSF(CHN,PB_CLOCK,SPI_BITRATE,LOG_UART);

//Filter excluding specific tests from UNIT_Exec_All 
uint8_t excludeFromExecAll[] = {0,0,0,0,0,0,0,0};

/* ------------------------------------------------------------*/
/*                            PmodJSTK                         */
/*                    Test setup for PmodJSTK                  */
/* ------------------------------------------------------------*/

#elif(PMODJSTK == 1)

/* ------------------------------------------------------------ */
/*				PIC32 Configuration Settings					*/
/* ------------------------------------------------------------ */
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1  //(8 MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
#pragma config FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL
#pragma config FPBDIV = DIV_2

#define SYSTEM_CLOCK 80000000L  //system clock speed (8 MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
#define PB_CLOCK (SYSTEM_CLOCK/2) //peripheral bus clock
#define SPI_BITRATE 625000 //bit rate for SPI port

//Test function header for PmodJSTK
#include "./TestHarness/PmodJSTK/pmod_jstk_test_driver.h"

#define NUM_TEST_FUNCTIONS 12 //number of test functions for PmodJSTK

//Array of function pointers to tests for PmodJSTK
uint8_t (*testFunc[])(uint8_t,UART_MODULE) = {UNIT_PmodJSTKLed_OFF,
						UNIT_PmodJSTKLed1_ON,UNIT_PmodJSTKLed2_ON,UNIT_PmodJSTKLed1_Led2_ON,
						UNIT_PmodJSTKAxisBounds,UNIT_PmodJSTKButton_1,UNIT_PmodJSTKButton_2,
						UNIT_PmodJSTKButton_Jstk,UNIT_PmodJSTKButton_None,UNIT_PmodJSTK10usDelay,
						UNIT_PmodJSTK15usDelay,UNIT_Exec_All};

//Menu Item text pssed into console menu
uint8_t *menuItems[] = {"UNIT_PmodJSTKLed_OFF","UNIT_PmodJSTKLed1_ON",
                "UNIT_PmodJSTKLed2_ON","UNIT_PmodJSTKLed1_Led2_ON","UNIT_PmodJSTKAxisBounds",
				"UNIT_PmodJSTKButton_1","UNIT_PmodJSTKButton_2","UNIT_PmodJSTKButton_Jstk","UNIT_PmodJSTKButton_None",
                "UNIT_PmodJSTK10usDelay","UNIT_PmodJSTK15usDelay","UNIT_Exec_All"};

//Name of module to display on console menu
char * pmodName = "PmodJSTK";

//Pmod initialization macro for PmodJSTK
#define INITPMOD(CHN,LOG_UART) PmodJSTKInit(CHN,PB_CLOCK,SPI_BITRATE,SYSTEM_CLOCK);

//Filter excluding specific tests from UNIT_Exec_All 
uint8_t excludeFromExecAll[] = {0,0,0,0,0,0,0,0,0,1,1,0};
//**************************************

#elif(PMODHB5 == 1)

/* ------------------------------------------------------------ */
/*				PIC32 Configuration Settings					*/
/* ------------------------------------------------------------ */
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8

#define SYSTEM_CLOCK 80000000L  //system clock speed (8 MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
#define PB_DIV                 8
#define PRESCALE               256
#define TOGGLES_PER_SEC        2000
#define PB_CLOCK (SYSTEM_CLOCK/2) //peripheral bus clock
#define T1_TICK               (SYSTEM_CLOCK/PB_DIV/PRESCALE/TOGGLES_PER_SEC)

//Test function header for PmodJSTK
#include "./TestHarness/PmodJSTK/pmodHB5_test_driver.h"

#define NUM_TEST_FUNCTIONS 1 //number of test functions for PmodJSTK

//Array of function pointers to tests for PmodJSTK
uint8_t (*testFunc[])(uint8_t,UART_MODULE) = {UNIT_Exec_All};

//Menu Item text pssed into console menu
uint8_t *menuItems[] = {,"UNIT_Exec_All"};

//Name of module to display on console menu
char * pmodName = "PmodHB5";

//Pmod initialization macro for PmodJSTK
#define INITPMOD(CHN,LOG_UART) PmodHB5Init();

//Filter excluding specific tests from UNIT_Exec_All 
uint8_t excludeFromExecAll[] = {0};

#endif

#define MENU_UART UART2

//Some test functions will need this value since, since it is not included in the 
//standard function definition accepted by the test loop utilizing the arrays of function 
//pointers. If this value is needed use the extern forward declation in the test module
//for this variable
uint32_t systemClock = SYSTEM_CLOCK;

#endif

