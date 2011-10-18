#include "./TestHarness/Common/test_harness_common.h"

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

