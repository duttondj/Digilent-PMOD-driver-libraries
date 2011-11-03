/************************************************************************/
/*                                                                      */
/* pmod_jstk_test_driver.c -- test driver implimentation                */
/*                            file for PmodJSTK                         */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*              Copyright (C) 2011 Ryan Hoffman                         */
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  This file contains implementation for functions used for use with   */
/*  the  Digilent PMODJSTK on Digilent Pic32 based boards               */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  10/13/2011(RyanH):                                                  */
/*                                                                      */
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include "./TestHarness/PmodJSTK/pmod_jstk_test_driver.h"

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

void PmodJSTK_INIT(UART_MODULE uartID)
{
	UARTPutS("\r\nPmodJSTK SPI port=>",uartID);
	chn =  getIntegerFromConsole(uartID); //SPI port PMODJSTK is connected to
	PmodJSTKInit(chn,PB_CLOCK,SPI_BITRATE,SYSTEM_CLOCK);
}

/*
UNIT TEST: UNIT_PmodJSTKAxisBounds
An initial baseline measurement of the X and Y axis values is recieved.
Tests are then performed againsts the value of the bounds recieved from 
the joystick, execAxisBounds() is called for full joystick deflection into 
all four quardants of the the joystick, if all four tests are determined 
to be within bounds the test passes.
*/
uint8_t UNIT_PmodJSTKAxisBounds(UART_MODULE uartID)
{
	uint16_t xAxis = 0;
	uint16_t yAxis = 0;
	uint8_t results[128];
	uint8_t testResult = 1;
	PmodJSTKAxisButton jstkAxisButtons;
	
	UARTPutS("UNIT TEST: UNIT_PmodJSTKAxisBounds\r\n Recieving initial joystick position\r\n",uartID);
	
	PmodJSTKSendRecv(chn,PMODJSTK_LD1_LD2_OFF,&jstkAxisButtons);
	
	xAxis =jstkAxisButtons.xAxis;
    yAxis =jstkAxisButtons.yAxis;
	
	sprintf(results,"Joystick baseline: X axis:%d Y axis: %d\r\n",xAxis,yAxis);
	UARTPutS(results,uartID);
	
	testResult &=  execAxisBounds("While holding joystick to full LEFT deflection on X axis press any key\r\n",PMODJSTK_MIN_X_Y_AXIS,xAxis - 200,uartID,JSTK_X_AXIS,chn);
	
	testResult &=  execAxisBounds("While holding joystick to full RIGHT deflection on X axis press any key\r\n",xAxis + 200,PMODJSTK_MAX_X_Y_AXIS,uartID,JSTK_X_AXIS,chn);

	testResult &=  execAxisBounds("While holding joystick to full UPWARD deflection on Y axis press any key\r\n",yAxis + 200,PMODJSTK_MAX_X_Y_AXIS,uartID,JSTK_Y_AXIS,chn);
		
	testResult &=  execAxisBounds("While holding joystick to full DOWNWARD deflection on Y axis press any key\r\n",PMODJSTK_MIN_X_Y_AXIS,yAxis - 200,uartID,JSTK_Y_AXIS,chn);

	return testResult;
}

/*
UNIT TEST:
Turn both LD1 and LD2 off and prompt the tester to verify 
that neither are illuminated, this will ensure 
the #defined value for PMODJSTK_LD1_LD2_OFF is correct.
*/
uint8_t UNIT_PmodJSTKLed_OFF(UART_MODULE uartID)
{
	return execLedTest("UNIT TEST: UNIT_PmodJSTKLed_OFF\r\nAre both LD1 and LD2 OFF? (0: no 1: yes)=>",uartID,PMODJSTK_LD1_LD2_OFF);
}

/*
UNIT TEST:
Turn LD1 on and prompt the tester to verify that 
only LD1 is illuminated, this will ensure the #defined 
value for PMODJSTK_LD1_ON is correct.
*/
uint8_t UNIT_PmodJSTKLed1_ON(UART_MODULE uartID)
{
	return execLedTest("UNIT TEST: UNIT_PmodJSTKLed1_ON\r\nIs only LD1 ON? (0: no 1: yes)=>",uartID,PMODJSTK_LD1_ON);
}

/*
UNIT TEST:
Turn LD2 on and prompt the tester to verify that 
only LD2 is illuminated, this will ensure the #defined 
value for PMODJSTK_LD2_ON is correct.
*/
uint8_t UNIT_PmodJSTKLed2_ON(UART_MODULE uartID)
{
	return execLedTest("UNIT TEST: UNIT_PmodJSTKLed_2_ON\r\nIs only LD2 ON? (0: no 1: yes) =>",uartID,PMODJSTK_LD2_ON);
}

/*
UNIT TEST:
Turn both LD1 and LD2 on and prompt the tester to verify 
that neither are illuminated, this will ensure 
the #defined value for PMODJSTK_LD1_LD2_ON is correct.
*/
uint8_t UNIT_PmodJSTKLed1_Led2_ON(UART_MODULE uartID)
{
	return execLedTest("UNIT TEST: UNIT_PmodJSTKLed1_Led2_ON\r\nAre both LD1 and LD2 on? (0: no 1: yes) =>",uartID,PMODJSTK_LD1_LD2_ON);
}

/*
UNIT TEST:
Poll for the state of BTN1, ensure that it matches PMODJSTK_BTN1,
if it is depressed then the #define for PMODJSTK_BTN1 is correct.
*/
uint8_t UNIT_PmodJSTKButton_1(UART_MODULE uartID)
{
	return execButtonTest("UNIT TEST: UNIT_PmodJSTKButton_1\r\nWhile holding BTN1, press any key\r\n",uartID,PMODJSTK_BTN1);
}

/*
UNIT TEST:
Poll for the state of BTN2, ensure that it matches PMODJSTK_BTN1,
if it is depressed then the #define for PMODJSTK_BTN2 is correct.
*/
uint8_t UNIT_PmodJSTKButton_2(UART_MODULE uartID)
{
	return execButtonTest("UNIT TEST: UNIT_PmodJSTKButton_2\r\nWhile BTN2, press any key\r\n",uartID,PMODJSTK_BTN2);
}

/*
UNIT TEST:
Poll for the state of the joystick button, ensure that it matches PMODJSTK_BTN_JSTK,
if it is depressed then the #define for PMODJSTK_BTN_JSTK is correct.
*/
uint8_t UNIT_PmodJSTKButton_Jstk(UART_MODULE uartID)
{
	return execButtonTest("UNIT TEST: UNIT_PmodJSTKButton_Jstk\r\nWhile holding the Joystick button, press any key\r\n",uartID,PMODJSTK_BTN_JSTK);
}

/*

UNIT TEST:
Poll for the state of the joystick button, ensure that it matches PMODJSTK_BTN_NONE,
if it is depressed then the #define for PMODJSTK_BTN_NONE is correct.
*/
uint8_t UNIT_PmodJSTKButton_None(UART_MODULE uartID)
{
	return execButtonTest("UNIT TEST: UNIT_PmodJSTKButton_Jstk\r\nEnsure no buttons are depress, press any key\r\n",uartID,PMODJSTK_BTN_NONE);
}

/*
UNIT TEST:
This test wil not return, the processor must be reset to run subsequent tests. A delay of 10us is introducted prior to and after setting SS low, 
SS is then driven high again. This process occurs in an infinit loop. An oscilloscope must be connected to the the SS pin of the PmodJSTK,
the tester should observe SS low for approximatley 10us. 
*/
uint8_t UNIT_PmodJSTK10usDelay(UART_MODULE uartID)
{
	UARTPutS("This test requires the use of an oscilloscope,\r\n scale horizontal axis to 10us and set trigger to 1V,\r\nconnect proble to ground and SS pin of PmodJSTK,SS should stay low for ~10us,\r\n this test will not return.\r\n",uartID);
	while(1)
	{
 		 PmodJSTKDelay10us(SYSTEM_CLOCK);
		 PmodSPISetSSLow(chn);
		 PmodJSTKDelay10us(SYSTEM_CLOCK);
		 PmodSPISetSSHigh(chn);
	}
	
	return 1;
}

/*
UNIT TEST:
This test wil not return, the processor must be reset to run subsequent tests. A delay of 15us is introducted prior to and after setting SS low, 
SS is then driven high again. This process occurs in an infinit loop. An oscilloscope must be connected to the the SS pin of the PmodJSTK,
the tester should observe SS low for approximatley 15us. 
*/
uint8_t UNIT_PmodJSTK15usDelay(UART_MODULE uartID)
{
	UARTPutS("This test requires the use of an oscilloscope,\r\n scale horizontal axis to 15us and set trigger to 1V,\r\nconnect proble to ground and SS pin of PmodJSTK,SS should stay low for ~15us,\r\n this test will not return.\r\n",uartID);
	while(1)
	{
 		 PmodJSTKDelay15us(SYSTEM_CLOCK);
  		 PmodSPISetSSLow(chn);
		 PmodJSTKDelay15us(SYSTEM_CLOCK);
		 PmodSPISetSSHigh(chn);
	}

	return 1;
}

/*  
**	execLedTest
**
**	Synopsis:
**	Helper function for executing PmodJSTK LED tests
**
**  Input: 
**  	uint8_t *testString - test description and instructions as a string
**		uint8_t chn - Pmod SPI channel
**      UART_MODULE uartID - serial console UART
**      uint8_t command - LED state bit mask, acceptable values follow:
**                        PMODJSTK_LD1_LD2_OFF,PMODJSTK_LD1_ON, 
**						  PMODJSTK_LD2_ON, PMODJSTK_LD1_LD2_ON
**  Returns: 
**  	uint8_t - 1 test passed, 0 test failed
**
**	Errors:	none
**
**  Description:
**  The text in testString is sent to the specified UART, the LED command
**  is then sent to the PmodJSTK, the tester is prompted on the serial console
**  to enter a 1 for pass or 0 for fail based on visually observing the 
**  state of the LEDs vs. the desired state described in the text output to the
**  serial console.
*/
uint8_t execLedTest(uint8_t *testString,UART_MODULE uartID,uint8_t command)
{
	PmodJSTKAxisButton jstkAxisButtons;
	UARTPutS(testString,uartID);
	PmodJSTKSendRecv(chn,command,&jstkAxisButtons);
	return getOneOrZeroFromConsole(uartID);
}

/*  
**  execButtonTest
**
**	Synopsis:
**	Helper function for executing PmodJSTK button tests
**
**  Input: 
**  	uint8_t *testString - test description and instructions as a string
**		uint8_t chn - Pmod SPI channel
**      UART_MODULE uartID - serial console UART
**      uint8_t button - button state bit masks, acceptable values follow:
**						 PMODJSTK_BTN_NONE,PMODJSTK_BTN_JSTK,
**						 PMODJSTK_BTN1,PMODJSTK_BTN2 
**
**  Returns:
**      uint8_T - 1 test passed, 0 test failed
**
**	Errors:	none
**
**  Description:
**  The text in testString is sent to the specified UART, the tester is
**  asked to place the buttons in state requested in testString, while
**  holding the joystick buttons in this state the tester must strike a key
**  signaling the system to poll the joystick. If the button state returned
**  matches "button" the test passes, otherwise it fails.
*/
uint8_t execButtonTest(uint8_t *testString,UART_MODULE uartID,uint8_t button)
{
	uint8_t results[128];
	PmodJSTKAxisButton jstkAxisButtons;
	//display test text	
	UARTPutS(testString,uartID);

	UARTGetOneByte(uartID);
	
	PmodJSTKSendRecv(chn,PMODJSTK_LD1_LD2_OFF,&jstkAxisButtons);

	sprintf(results, "Expected: %d, Recieved: %d\r\n",button,jstkAxisButtons.buttonState);
	UARTPutS(results,uartID);
	
	return (button == jstkAxisButtons.buttonState)?1:0;
}

/*  
**  execAxisBounds
**
**	Synopsis:
**  Helper function for executing PmodJSTK joystick axis tests.
**  This test Polls the joystick as the tester  moves the joystick 
**  to full deflection on a the specified axis in the specified direction, 
**  the value recieved is tested against the lower and upper bounds
**  passed in.
** 
**  Input: 
**  	uint8_t *testString - test description and instructions as a string
**      uint16_t lowerBound - lower bound for axis value
**      uint16_t upperBound - upper buond for axis value
**      UART_MODULE uartID - serial console UART
**      uint8_t whichAxis - axis to read, acceptable values follow:
**		                   JSTK_X_AXIS,JSTK_Y_AXIS 1
**		uint8_t chn - Pmod SPI channel
**   
**  Returns: uint8_T - 1 test passed, 0 test failed
**
**	Errors:	none
**
**  Description:
**  Test instructions are displayed to the console, the tester deflects the 
**  joystick toward the direction specified then presses a key to poll
**  the joystick. The value recieved is checked againsts the bounds
**  passed in, for values in bounds test passed: 1 is returned, otherwise 
**  test failed: 0.
*/

uint8_t execAxisBounds(uint8_t *testString, uint16_t lowerBound,uint16_t upperBound,UART_MODULE uartID,uint8_t whichAxis,uint8_t chn)
{
	uint8_t results[128];
	uint16_t axisValue = 0;
	PmodJSTKAxisButton jstkAxisButtons;
	
	UARTPutS(testString,uartID);
	UARTGetOneByte(uartID);
	
	//LED bit mask sent does not matter for this test
	PmodJSTKSendRecv(chn,PMODJSTK_LD1_LD2_OFF,&jstkAxisButtons);	
	axisValue = (whichAxis == JSTK_X_AXIS)?jstkAxisButtons.xAxis:jstkAxisButtons.yAxis;
	
	if((axisValue >= lowerBound) && (axisValue <= upperBound))
	{
		sprintf(results,"PASSED: Lower Bound: %d Upper Bound: %d  Measured: %d\r\n",lowerBound,upperBound,axisValue);
		UARTPutS(results,uartID);
		return 1;
	}
	else
	{
		sprintf(results,"FAILED: Lower Bound: %d Upper Bound: %d  Measured: %d\r\n",lowerBound,upperBound,axisValue);
		UARTPutS(results,uartID);
		return 0;
	}
}
