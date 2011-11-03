/* -------------------------------------------------------------------- */
/*                                                                      */
/*                     test_harness_common.c                            */
/*                                                                      */
/*                                                                      */
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

#include "./TestHarness/Common/test_harness_common.h"

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

/*  getOneOrZeroFromConsole
**
**	Synopsis:
**  Retrieve a 1 or 0 from the specified UART
**
**  Input: 
**		UART_MODULE uartID - UART to poll 
**
**  Returns:
**      uint8_t - 1 or 0 recieved from UART
**
**	Errors:
**	Error message sent to UART requesting 
**  corrected input
**
**  Description:
**  Poll selected UART for character input of
**  '1' or '0', convert to uint8_t and return.
**  An error message is displayed for input
**  other that 1 or 0. The polling loop will
**  not exit until a 1 or 0 is entered.
*/
uint8_t getOneOrZeroFromConsole(UART_MODULE uartID)
{
	uint8_t value = 0;
	do
	{
		value = getIntegerFromConsole(uartID);
		if(value > 1)
		{
			UARTPutS("Please enter a 0 or 1 =>",uartID);
		}
	}while(value > 1);
	
	return value;
}

/*  SetupSerialLogging
**
**	Synopsis:
**	Configure and open a UART at the specified baud rate
** 
**  Input: 
**	uint32_t baudRate - baud rate for UART
**  uint32_t pbClock - peripheral bus clock in Mhz
**  UART_MODULE uartID - UART module to open
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  Open the UART specified in "uartID" at "baudRate", UART
**  parameters will set be as follows:
**  Pins Enabled: RX and TX only
**  Line Control: Data Bits: 8, Parity: none, Stop Bits: 1 
*/
void SetupSerialLogging(uint32_t baudRate,uint32_t pbClock,UART_MODULE uartID)
{

    UARTConfigure(uartID, UART_ENABLE_PINS_TX_RX_ONLY);

    UARTSetFifoMode(uartID, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);

    UARTSetLineControl(uartID, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);

    UARTSetDataRate(uartID, pbClock, baudRate);

    UARTEnable(uartID, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
}

/*  
**  getIntegerFromConsole
**  
**	Synopsis:
**	Polls specificed UART for character
**  input and returns the 8 bit integer value
**  
**  Input: 
**		UART_MODULE uartID - UART to poll
**
**  Returns: uint8_t - 8 bit integer value character input
**
**	Errors:	none
**
**  Description:
**	Polls specificed UART for character input terminating
**  on \r and returns the 8 bit integer value 
**  of the characters enetered.
*/
uint8_t getIntegerFromConsole(UART_MODULE uartID)
{
	uint8_t recievedChars[10];
	uint8_t oneChar;
	int bufPos = 0;
	memset(recievedChars,0,10);
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
	
	recievedChars[bufPos] = '\0';

	return atoi(recievedChars);
}

/*  
**  ConsoleMenu
** 
**	Synopsis:
**
**  Input: 
**		uint8_t *pmodName - 
**		uint8_t *testNames[] - 
**		uint32_t numCommands - 
**		UART_MODULE uartID - 
**
**  Returns: uint8_t 
**
**	Errors:	none
**
**  Description:
*/
uint8_t ConsoleMenu(uint8_t *pmodName,uint8_t *testNames[],uint32_t numCommands,UART_MODULE uartID)
{
	int selection;
	uint8_t menuItem[100];
	int index = 0;;
	do
	{
		sprintf(menuItem,"\r\n%s Test Menu\n\r",pmodName);
		UARTPutS(menuItem,uartID);
	
		for(index = 0; index < numCommands;index++)
		{	
			sprintf(menuItem,"%d) %s\n\r",index,testNames[index]);
			UARTPutS(menuItem,uartID);
		}

		UARTPutS("Select=>",uartID);
		selection = getIntegerFromConsole(uartID);
		
		if(selection < 0 || selection >= numCommands)
		{
			UARTPutS("\n\rInvalid Selection\n\r",uartID);
		}
		
	}while(selection < 0 || selection >= numCommands);

	return selection;
}

/*  
**
**	Synopsis:
**
**  Input: 
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
*/
void UARTPutS(uint8_t *string,UART_MODULE uartID)
{
	while(*string != '\0')
	{
		while(!UARTTransmitterIsReady(uartID));
		UARTSendDataByte(uartID, *string);

		string++;
	}
}

/*  
**
**	Synopsis:
**
**  Input: 
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
*/
uint8_t UARTGetOneByte(UART_MODULE uartID)
{
	while(!UARTReceivedDataIsAvailable(uartID));
	
	return UARTGetDataByte(uartID);
}

