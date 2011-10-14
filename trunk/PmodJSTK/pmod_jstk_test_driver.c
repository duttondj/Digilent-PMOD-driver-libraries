/************************************************************************/
/*                                                                      */
/*   cerebot32mx4_7_test_driver.c test driver functions for PmodJSTK    */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
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

#include "pmod_jstk_test_driver.h"

uint8_t UNIT_PmodJSTKAxisBounds(uint8_t chn, UART_MODULE uartID)
{
	uint16_t xAxis = 0;
	uint16_t yAxis = 0;
	uint8_t results[128];
	uint8_t testResult = 1;
	PmodJSTKAxisButton jstkAxisButtons;
	UARTPutS("UNIT TEST: UNIT_PmodJSTKAxisBounds\r\n Recieving initial joystick position\r\n",uartID);
	PmodJSTKSendRecv(chn,PMODJSTK_LED_OFF,&jstkAxisButtons);
	xAxis =jstkAxisButtons.xAxis;
    yAxis =jstkAxisButtons.yAxis;
	sprintf(results,"Joystick baseline: X axis:%d Y axis: %d\r\n",xAxis,yAxis);
	UARTPutS(results,uartID);
	
	UARTPutS("While holding joystick to full LEFT deflection on X axis press any key\r\n",uartID);
	UARTGetOneByte(uartID);
	PmodJSTKSendRecv(chn,PMODJSTK_LED_OFF,&jstkAxisButtons);
	testResult &= IsAxisInBounds(PMODJSTK_MIN_X_Y_AXIS,xAxis - 1,jstkAxisButtons.xAxis,uartID);

	UARTPutS("While holding joystick to full RIGHT deflection on X axis press any key\r\n",uartID);
	UARTGetOneByte(uartID);
	PmodJSTKSendRecv(chn,PMODJSTK_LED_OFF,&jstkAxisButtons);
	testResult &= IsAxisInBounds(xAxis + 1,PMODJSTK_MAX_X_Y_AXIS,jstkAxisButtons.xAxis,uartID);
	
	UARTPutS("While holding joystick to full UPWARD deflection on Y axis press any key\r\n",uartID);
	UARTGetOneByte(uartID);
	PmodJSTKSendRecv(chn,PMODJSTK_LED_OFF,&jstkAxisButtons);
	testResult &= IsAxisInBounds(yAxis + 1,PMODJSTK_MAX_X_Y_AXIS,jstkAxisButtons.yAxis,uartID);
	
	UARTPutS("While holding joystick to full DOWNWARD deflection on Y axis press any key\r\n",uartID);
	UARTGetOneByte(uartID);
	PmodJSTKSendRecv(chn,PMODJSTK_LED_OFF,&jstkAxisButtons);
	testResult &= IsAxisInBounds(PMODJSTK_MIN_X_Y_AXIS,yAxis - 1,jstkAxisButtons.yAxis,uartID);	

	return testResult;

}

uint8_t UNIT_PmodJSTKLed_OFF(uint8_t chn, UART_MODULE uartID)
{
	return execLedTest("UNIT TEST: UNIT_PmodJSTKLed_OFF\r\nAre both LED lights OFF? (0: no 1: yes)=>",chn,uartID,PMODJSTK_LED_OFF);
}

uint8_t UNIT_PmodJSTKLed1_ON(uint8_t chn, UART_MODULE uartID)
{
	return execLedTest("UNIT TEST: UNIT_PmodJSTKLed1_ON\r\nIs LED 1 ON? (0: no 1: yes)=>",chn,uartID,PMODJSTK_LED1_ON);
}

uint8_t UNIT_PmodJSTKLed2_ON(uint8_t chn, UART_MODULE uartID)
{
	return execLedTest("UNIT TEST: UNIT_PmodJSTKLed_2_ON\r\nIs LED 2 ON? (0: no 1: yes) =>",chn,uartID,PMODJSTK_LED2_ON);
}

uint8_t UNIT_PmodJSTKLed1_Led2_ON(uint8_t chn, UART_MODULE uartID)
{
	return execLedTest("UNIT TEST: UNIT_PmodJSTKLed1_Led2_ON\r\nAre both LED lights on? (0: no 1: yes) =>",chn,uartID,PMODJSTK_LED1_LED2_ON);
}

uint8_t execLedTest(uint8_t *testString,uint8_t chn,UART_MODULE uartID,uint8_t command)
{
	PmodJSTKAxisButton jstkAxisButtons;
	PmodJSTKSendRecv(chn,command,&jstkAxisButtons);
	UARTPutS(testString,uartID);
	return getOneOrZeroFromConsole(uartID);

}

uint8_t IsAxisInBounds(uint16_t lowerBound,uint16_t upperBound,uint16_t axisValue,UART_MODULE uartID)
{
	uint8_t results[128];
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

uint8_t getOneOrZeroFromConsole(UART_MODULE uartID)
{
	uint8_t value = 0;
	do
	{
		value = getIntegerFromConsole(uartID);
		if(value > 1)
		{
			UARTPutS("Please enter a 0 or 1 =>",UART1);
		}
	}while(value > 1);
	
	return value;
}


uint8_t SetupSerialLogging(uint32_t baud_rate,uint32_t pbClock,UART_MODULE uartID)
{

    UARTConfigure(uartID, UART_ENABLE_PINS_TX_RX_ONLY);

    UARTSetFifoMode(uartID, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);

    UARTSetLineControl(uartID, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);

    UARTSetDataRate(uartID, pbClock, 9600);

    UARTEnable(uartID, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

}

uint8_t getIntegerFromConsole(UART_MODULE uartID)
{
		uint8_t recievedChars[10];
		uint8_t oneChar;
		int bufPos = 0;
		do
		{
			oneChar = UARTGetOneByte(uartID);

			UARTSendDataByte(uartID,oneChar); //echo to console
			if(oneChar != '\r' && oneChar != '\b')	
			{
				recievedChars[bufPos] = oneChar;			
			}			
			else
			{
				UARTSendDataByte(uartID,'\n');
				UARTSendDataByte(uartID,'\r');
			}	
			if(oneChar != '\b') //TODO: fix backspace
				bufPos++;
		}while(bufPos < 9 && oneChar != '\r');

		recievedChars[bufPos] = 0;

		return atoi(recievedChars);
	
}


uint8_t ConsoleMenu(uint8_t *testNames[],uint32_t numCommands,UART_MODULE uartID)
{
	int selection;
	uint8_t menuItem[100];
	int index = 0;
	
	do
	{
		UARTPutS("\r\nPmodJSTK Test Menu\n\r",uartID);
	
		for(index = 0; index < numCommands;index++)
		{	
			sprintf(menuItem,"%d) %s\n\r",index,testNames[index]);
			UARTPutS(menuItem,uartID);
		}
		UARTPutS("Select=>",uartID);
		selection = getIntegerFromConsole(uartID);
		
		if(selection < 0 || selection > numCommands)
		{
			UARTPutS("\n\rInvalid Selection\n\r",uartID);
		}
		
	}while(selection < 0 || selection > numCommands);

	return selection;

}

void UARTPutS(uint8_t *string,UART_MODULE uartID)
{
	while(*string != '\0')
	{
		while(!UARTTransmitterIsReady(uartID));
		UARTSendDataByte(uartID, *string);

		string++;
	}
}

uint8_t UARTGetOneByte(UART_MODULE uartID)
{
	while(!UARTReceivedDataIsAvailable(uartID));
	
	return UARTGetDataByte(uartID);
}
