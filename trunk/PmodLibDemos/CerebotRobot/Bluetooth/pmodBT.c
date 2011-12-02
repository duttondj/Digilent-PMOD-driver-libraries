/************************************************************************/
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*              Copyright (C) 2011 Ryan Hoffman                         */
/************************************************************************/
/*  Module Description: 												*/

/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*  11/16/2011(RyanH):                                                  */
/*																		*/
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include "PmodBT.h"


/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */


/*  
** <FUNCTION NAME>
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

uint8_t BTInquireAndConnect(UART_MODULE uartID,uint8_t *peerName,uint8_t *inquiryTimeOut,uint8_t numInquiryTries,uint8_t *connectResponse)
{
	uint8_t inquiryRetryCount = 0;
	
	uint8_t connectSuccess = 0;
	uint8_t numLinesForInquiry = 0;
	uint8_t address[BLUETOOTH_MAX_ADDRESS_LEN];
	uint8_t getCommandResponse[BLUETOOTH_MAX_NUM_COMMAND_RESPONSE][BLUETOOTH_MAX_COMMAND_RESPONSE_LEN];
	uint8_t params1[BLUETOOTH_MAX_PARAMS][BLUETOOTH_MAX_PARAM_LEN];
	uint8_t params2[BLUETOOTH_MAX_PARAMS][BLUETOOTH_MAX_PARAM_LEN];

	
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
		if(BTGetAddressFromName((const uint8_t(*)[BLUETOOTH_MAX_COMMAND_RESPONSE_LEN])getCommandResponse,peerName,address,numLinesForInquiry - 1))
		{
			strcpy(params2[0],"C");
			strcpy(params2[1],address);
			BTSendGetCommand(uartID,params2,2,getCommandResponse,2,2);
			//check for connect string recieved		
			if(strcmp(connectResponse,getCommandResponse[1]) == 0)
			{
				UARTPutS(connectResponse,uartID);
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
/*  
** <FUNCTION NAME>
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
uint8_t BTRecieveConnect(UART_MODULE uartID,uint8_t* responseString)
{
	uint8_t connectString[20];
	uint8_t numBytes = strlen(responseString);
 	BTGetByteSequence(uartID,connectString,numBytes);
	connectString[numBytes] = '\0';
	if(strcmp(connectString,responseString) == 0)
	{
		return 1;
	}
	
	return 0;
}
/*  
** <FUNCTION NAME>
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
uint8_t BTGetAddressFromName(const uint8_t addressNamePairs[][BLUETOOTH_MAX_COMMAND_RESPONSE_LEN],const uint8_t *name,uint8_t *address,uint8_t numEntries)
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
/*  
** <FUNCTION NAME>
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
uint16_t BTSendSetCommand(UART_MODULE uartID,uint8_t params[][BLUETOOTH_MAX_PARAM_LEN],uint8_t numParams)
{
	uint32_t numBytesRead = 0;
	uint8_t commandResponse[BLUETOOTH_MAX_COMMAND_RESPONSE_LEN];
	uint8_t command[BLUETOOTH_MAX_COMMAND_LENGTH];
	
	UARTPutS(BTBuildCommand(command,params,numParams),uartID);
	numBytesRead = BTGetLine(uartID,commandResponse);
	return ByteSum(commandResponse,numBytesRead);
}
/*  
** <FUNCTION NAME>
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
/*  
** <FUNCTION NAME>
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
uint32_t BTSendGetCommand(UART_MODULE uartID,uint8_t params[][BLUETOOTH_MAX_PARAM_LEN],uint8_t numParams,uint8_t commandResponse[][BLUETOOTH_MAX_COMMAND_RESPONSE_LEN],uint32_t maxLinesReturned,uint32_t minNumLinesReturned)
{
	uint32_t bitValue = 0;
	uint32_t numLinesRead = 0;
	uint8_t command[BLUETOOTH_MAX_COMMAND_LENGTH];
	
	UARTPutS(BTBuildCommand(command,params,numParams),uartID);
	numLinesRead = BTGetUpToNLines(uartID,commandResponse,maxLinesReturned,minNumLinesReturned);
	return numLinesRead;
}	
/*  
** <FUNCTION NAME>
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
uint8_t* BTBuildCommand(uint8_t *fullCommand,uint8_t params[][BLUETOOTH_MAX_PARAM_LEN],uint8_t numParams)
{
	uint8_t paramCount = 0;
	memset(fullCommand,0,sizeof(uint8_t) * BLUETOOTH_MAX_COMMAND_LENGTH);
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
/*  
** <FUNCTION NAME>
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

/*  
** <FUNCTION NAME>
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
/*  
** <FUNCTION NAME>
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
uint32_t BTGetUpToNLines(UART_MODULE uartID,uint8_t bytes[][BLUETOOTH_MAX_COMMAND_RESPONSE_LEN],uint32_t maxNumLines,uint32_t minNumLines)
{
	uint32_t lineCount = 0;
	
	memset(bytes,0,sizeof(uint8_t) * maxNumLines * BLUETOOTH_MAX_COMMAND_RESPONSE_LEN);
	do
	{
		BTGetLine(uartID,bytes[lineCount]);
		lineCount++;
		
	}while(lineCount < minNumLines || UARTReceivedDataIsAvailable(uartID));
	
	return lineCount;	
}	
/*  
** <FUNCTION NAME>
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

/*  
** <FUNCTION NAME>
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
void delayN(uint32_t delay)
{
   uint32_t delayCount = 0;
   for(delayCount = 0;delayCount < delay;delayCount++)
   {
		asm("nop");	
   }	
	
}	

/*  
** <FUNCTION NAME>
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
/*  
** <FUNCTION NAME>
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
/*  
** <FUNCTION NAME>
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
** <FUNCTION NAME>
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
void UARTPutOneByte(uint8_t oneByte,UART_MODULE uartID)
{
	if (UARTTransmitterIsReady(uartID))
	{
   	 	UARTSendDataByte(uartID, oneByte);
	}	
}