#include "PmodBT.h"




uint8_t BTInquireAndConnect(UART_MODULE uartID,uint8_t *peerName,uint8_t *inquiryTimeOut,uint8_t numInquiryTries)
{
	uint8_t inquiryRetryCount = 0;
	uint8_t getCommandResponse[10][40];
	uint8_t numLinesForInquiry = 0;
	uint8_t params1[3][15];
	uint8_t params2[3][15];
	uint8_t address[20];
	uint8_t connectSuccess = 0;
	
	strcpy(params1[0],ENTER_COMMAND);
	BTSendSetCommand(uartID,params1,1);
	
	strcpy(params1[0],"I");
	strcpy(params1[1],inquiryTimeOut);
	
	strcpy(params2[0],"C");

	for(inquiryRetryCount = 0; inquiryRetryCount < numInquiryTries;inquiryRetryCount++)
	{
		BTSendGetCommand(uartID,params1,2,getCommandResponse,2,2);
		numLinesForInquiry = (getCommandResponse[1][6] - '0') + 1;
		BTGetUpToNLines(uartID,getCommandResponse,numLinesForInquiry,numLinesForInquiry);
		if(BTGetAddressFromName((const uint8_t(*)[40])getCommandResponse,peerName,address,numLinesForInquiry - 1))
		{
			strcpy(params2[0],"C");
			strcpy(params2[1],address);
			BTSendGetCommand(uartID,params2,2,getCommandResponse,2,2);

			if(strcmp(RESPONSE_CONNECT,getCommandResponse[1]) == 0)
			{
				UARTPutS("CONNECTED\r\n",uartID);
				connectSuccess = 1;
				break;
			}	
				
		}	
	}
	if(!connectSuccess)
	{
		strcpy(params1[0],COMMAND_EXIT);
		BTSendSetCommand(uartID,params1,1);
	}	

	return connectSuccess;
}	

uint8_t BTGetAddressFromName(const uint8_t addressNamePairs[][40],const uint8_t *name,uint8_t *address,uint8_t numEntries)
{
	uint8_t entryCount = 0;
	uint8_t nameAddress[20];
	for(entryCount = 0;entryCount < numEntries;entryCount++)
	{
		BTGetParam(addressNamePairs[entryCount],nameAddress,1);
		if(strcmp(name,nameAddress) == 0)
		{
			BTGetParam(addressNamePairs[entryCount],address,0);	
			return 1;
		}	
		
	}	
	return 0;
}	

uint16_t BTSendSetCommand(UART_MODULE uartID,uint8_t params[][15],uint8_t numParams)
{
	uint32_t numBytesRead = 0;
	uint8_t commandResponse[MAX_COMMAND_RESPONSE_LEN];
	uint8_t command[MAX_COMMAND_LEN];
	
	UARTPutS(BTBuildCommand(command,params,numParams),uartID);
	numBytesRead = BTGetLine(uartID,commandResponse);
	return ByteSum(commandResponse,numBytesRead);
}

void BTGetParam(const uint8_t* parameters,uint8_t* oneParameter,uint8_t paramNum)
{
	uint8_t paramCount = 0;
	
	if(paramNum != 0)
	{
		while(*parameters != ',' && *parameters != '\r' && paramCount < paramNum)
		{
			parameters++;	
			if(*parameters == ',')
			{
				paramCount++;
			}	
		}	
		parameters++;	//advance past the last comma
	}		
	while(*parameters != ',' && *parameters != '\r')
	{
		*oneParameter = *parameters;
		oneParameter++;	
		parameters++;
	}
	*oneParameter = '\0';	
}	 

uint32_t BTSendGetCommand(UART_MODULE uartID,uint8_t params[][15],uint8_t numParams,uint8_t commandResponse[][40],uint32_t maxLinesReturned,uint32_t minNumLinesReturned)
{
	uint32_t bitValue = 0;
	uint32_t numLinesRead = 0;
	uint8_t command[MAX_COMMAND_LEN];
	
	UARTPutS(BTBuildCommand(command,params,numParams),uartID);
	numLinesRead = BTGetUpToNLines(uartID,commandResponse,maxLinesReturned,minNumLinesReturned);
	return numLinesRead;
}	

uint8_t* BTBuildCommand(uint8_t *fullCommand,uint8_t params[][15],uint8_t numParams)
{
	uint8_t paramCount = 0;
	memset(fullCommand,0,sizeof(uint8_t) * MAX_COMMAND_LEN);
	for(paramCount = 0;paramCount < numParams;paramCount++)
	{
		strcat(fullCommand,params[paramCount]);
		//no trailing comma
		if(paramCount < (numParams -1))
		{
			strcat(fullCommand,",");
		}
			
	}
	if(params[0][0] != '$') //no carraige return needed for start command
	{
		strcat(fullCommand,"\r\n");	
	}
	return fullCommand;	
}	 

uint16_t ByteSum(uint8_t *bytes,uint8_t numBytes)
{
	 uint8_t byteCount = 0;
	 uint32_t byteSum = 0;
	 for(byteCount = 0;byteCount < numBytes;byteCount++)
	 {
		 byteSum += *bytes;
		 bytes++;
	 }
	 return byteSum;
}	


uint32_t BTGetByteSequence(UART_MODULE uartID,uint8_t *bytes,uint32_t numBytes)
{
	uint32_t byteCount = 0;
	for(byteCount = 0;byteCount < numBytes;byteCount++)
	{
		*bytes = UARTGetOneByte(uartID);

		bytes++;				

	}	
	return byteCount;
}

uint32_t BTGetUpToNLines(UART_MODULE uartID,uint8_t bytes[][40],uint32_t maxNumLines,uint32_t minNumLines)
{
	uint32_t lineCount = 0;
	
	memset(bytes,0,sizeof(uint8_t) * maxNumLines * 40);
	do
	{
		BTGetLine(uartID,bytes[lineCount]);
		lineCount++;
		
	}while(lineCount < minNumLines || UARTReceivedDataIsAvailable(uartID));
	
	return lineCount;	
}	

uint32_t BTGetLine(UART_MODULE uartID,uint8_t *bytes)	
{
	uint32_t byteCount = 0;
	uint8_t oneByte = 0;
	do
	{
		oneByte = UARTGetOneByte(uartID);
		*bytes = oneByte;
		bytes++;
		byteCount++;
		
	}while(oneByte != '\n');	
	
	return byteCount;
}	

void UARTSetup(uint32_t baudRate,uint32_t pbClock,UART_MODULE uartID)
{
	

    UARTConfigure(uartID,UART_ENABLE_PINS_CTS_RTS | UART_RTS_WHEN_RX_NOT_FULL );

    UARTSetFifoMode(uartID, UART_INTERRUPT_ON_RX_NOT_EMPTY | UART_INTERRUPT_ON_TX_NOT_FULL );

    UARTSetLineControl(uartID, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);

    UARTSetDataRate(uartID, pbClock, baudRate);

    UARTEnable(uartID, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
    
	PORTSetPinsDigitalIn(IOPORT_E, BIT_8);//BT status
	
	PORTSetBits(IOPORT_D, BIT_0); //Start Bluetooth RST high
	PORTSetPinsDigitalOut(IOPORT_D, BIT_0);//BT reset

	delayN(4000000); //wait for module to init
//	resetBTModule();
	
	//wait for bluetooth module to 
	
    
} 

void delayN(uint32_t delay)
{
   uint32_t delayCount = 0;
   for(delayCount = 0;delayCount < delay;delayCount++)
   {
		asm("nop");	
   }	
	
}	

void resetBTModule()
{
	uint32_t delay = 0;
	uint16_t bit12 = PORTReadBits(IOPORT_F, BIT_12);

	PORTClearBits(IOPORT_D, BIT_0);
 	while(PORTReadBits(IOPORT_F, BIT_12) == bit12); //wait for CTS to go low, BT off
 	delayN(400000);
    PORTSetBits(IOPORT_D, BIT_0);
    while(PORTReadBits(IOPORT_F, BIT_12) != bit12);	//wait for CTS to go high, BT on
    
    //wait for module to fully initialize
 	delayN(4000000);
}	


void UARTPutBytes(uint8_t *bytes,UART_MODULE uartID,uint32_t numBytes)
{
	uint32_t byteCount = 0;
	for(byteCount = 0;byteCount < numBytes;byteCount++)
	{
		while(!UARTTransmitterIsReady(uartID));
		UARTSendDataByte(uartID, *bytes);
		bytes++;
	}
}

uint8_t UARTGetOneByte(UART_MODULE uartID)
{
	while(!UARTReceivedDataIsAvailable(uartID));
	
	return UARTGetDataByte(uartID);
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

void UARTPutOneByte(uint8_t oneByte,UART_MODULE uartID)
{
	if (UARTTransmitterIsReady(uartID))
	{
   	 	UARTSendDataByte(uartID, oneByte);
	}	
}