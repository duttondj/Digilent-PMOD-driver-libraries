/************************************************************************/
/*  pmodBT.c -- implementation file for pmodBT configurator             */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*              Copyright (C) 2011 Ryan Hoffman                         */
/************************************************************************/
/*  Module Description: 												*/
/*  This module contains funtions for configuring, connecting and       */
/*  tranfering data with the PmodBT2 Bluetooth module
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*  11/16/2011(RyanH):                                                  */
/*																		*/
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include "pmodBT.h"


/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */


/*  
**  BTInquireAndConnect()
**
**	Synopsis:
**	Performs a Bluetooth inquiry for the specified name, resolves the 
**	address and attempts to make a connection.
**
**  Input:
**		UART_MODULE uartID - Bluetooth UART module ID
**		uint8_t *peerName - string representing Bluetooth device to inquire for and connect to
**	    uint8_t *inquiryTimeOut - string representing duration in seconds for inquiry
**		uint8_t numInquiryTries - number of attempts at locating and connecting
**		uint8_t *connectResponse - string representing response echoed back on connect success
**
**  Returns: 
**		uint8_t - 0 for connect fail, 1 for connect success
**
**	Errors:	none
**
**  Description:
**	Performs a Bluetooth inquiry for the specified name, resolves the 
**	address and attempts to make a connection. If no connection is made
**  or no devices are found, the inquire and connect process will repeat 
**  numInquiryTries times.
*/
uint8_t BTInquireAndConnect(UART_MODULE uartID,uint8_t *peerName,uint8_t *inquiryTimeOut,uint8_t numInquiryTries,uint8_t *connectResponse)
{
	uint8_t inquiryRetryCount = 0;
	uint8_t connectSuccess = 0;
	uint8_t numLinesForInquiry = 0;
	//string representation of address to connect to
	uint8_t address[BLUETOOTH_MAX_ADDRESS_LEN];
	//response recieved after issueing a command
	uint8_t getCommandResponse[BLUETOOTH_MAX_NUM_COMMAND_RESPONSE][BLUETOOTH_MAX_COMMAND_RESPONSE_LEN];
	//paramaters send into command functions
	uint8_t params1[BLUETOOTH_MAX_PARAMS][BLUETOOTH_MAX_PARAM_LEN];
	uint8_t params2[BLUETOOTH_MAX_PARAMS][BLUETOOTH_MAX_PARAM_LEN];

	//Enter command mode
	strcpy(params1[0],ENTER_COMMAND);
	BTSendSetCommand(uartID,params1,1);
	
	//Set inquiry timeout
	strcpy(params1[0],"I");
	strcpy(params1[1],inquiryTimeOut);
	
	for(inquiryRetryCount = 0; inquiryRetryCount < numInquiryTries;inquiryRetryCount++)
	{
		//Perform inquiry, get the first two response lines
		BTSendCommand(uartID,params1,2,getCommandResponse,2,2);
		//continue only if we found devices
		if(strcmp(getCommandResponse[1],BLUETOOTH_INQUIRY_RESPONSE_NO_DEV) != 0)
		{
			//The second line contains the number of devices found, returns up to 9, grab the
			//ascii representation if the 7th character and convert to decimal, this is the 
			//number of devices found plus trailing inquiry done 
			numLinesForInquiry = (getCommandResponse[1][6] - '0') + 1;
			//Get the remaining lines containing the Name:Address of the device
			BTGetUpToNLines(uartID,getCommandResponse,numLinesForInquiry,numLinesForInquiry);
			//numLinesForInquiry <= 1, we found no visible devices, no need to continue
			if(numLinesForInquiry > 1)
			{
				//Since there was at least one device found, attempt to find the address of the device found, ignore last line ("Inquiry Done")
				if(BTGetAddressFromName(getCommandResponse,peerName,address,numLinesForInquiry - 1))
				{
					strcpy(params2[0],"C");
					strcpy(params2[1],address);
					//attempt to connect to the remote device, expect 2 lines 
					BTSendCommand(uartID,params2,2,getCommandResponse,2,2);
					//check for connect string recieved, if connect response echoed back	
					if(strcmp(connectResponse,getCommandResponse[1]) == 0)
					{
						//connect sucessful, send connectResponse to remote	
						UARTPutS(connectResponse,uartID);
						connectSuccess = 1;
						break;
					}	
						
				}	
			}
		}
	}
	//if connection was unsuccessful exit command mode
	if(!connectSuccess)
	{
		strcpy(params1[0],COMMAND_EXIT);
		BTSendSetCommand(uartID,params1,1);
	}	

	return connectSuccess;
}
	
/*  
** BTRecieveConnect()
**
**	Synopsis:
**		Compares the string passed in to one recieved from the PmodBT2,
**		and determines if the connection was successful	
**
**  Input: 
**		UART_MODULE uartID - UART module 
**		uint8_t* responseString - string to compare to recieved string 
**								  indicating a successful connection
**
**  Returns: 
**		uint8_t - 1 if string matches, 0 otherwise
**
**	Errors:	none
**
**  Description:
**
**	BTInquireAndConnect sends a connection success string to the remote 
**  host, this function is used to check if the data recieved indicated
**	a successful connection.
*/
uint8_t BTRecieveConnect(UART_MODULE uartID,uint8_t* responseString)
{
	uint8_t connectString[20];
	uint8_t numBytes = strlen(responseString);
 	BTGetByteSequence(uartID,connectString,numBytes);
	connectString[numBytes] = '\0'; //add null to the end for string compare
	if(strcmp(connectString,responseString) == 0)
	{
		return 1;
	}
	
	return 0;
}

/*  
**  BTGetAddressFromName()
**
**	Synopsis:
**	Searches array of name address pairs recieved from PmodBT2 inquiry running command "I" for 
**  the name passed in and returns the address. 
**
**  Input: 
**		const uint8_t addressNamePairs[][BLUETOOTH_MAX_COMMAND_RESPONSE_LEN] - array of name address pairs recieved from PmodBT2
**		const uint8_t *name - name to loop up addressNamePairs
**		uint8_t *address - address resolved in lookup
**		uint8_t numEntries - number of entries in addressNamePairs
**
**  Returns: 
**		uint8_t  - 1 if name was resolved to address, 0 otherwise
**
**	Errors:	none
**
**  Description:
**	Searches array of name address pairs recieved from PmodBT2 inquiry running command "I" for 
**  the name passed in *name and returns 1 and the address found in *address, if no match is found
**  the contents of *address remain unchanged and 0 is returned.
*/
uint8_t BTGetAddressFromName(uint8_t addressNamePairs[][BLUETOOTH_MAX_COMMAND_RESPONSE_LEN],uint8_t *name,uint8_t *address,uint8_t numEntries)
{
	uint8_t entryCount = 0;
	uint8_t nameAddress[BLUETOOTH_MAX_COMMAND_RESPONSE_LEN];
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
** 	BTSendSetCommand()
**
**	Synopsis:
**	Use for sending set/change commands only(see RN-42 manual)
**  
**  Input: 
**		UART_MODULE uartID - Bluetooth uart module
**		uint8_t params[][BLUETOOTH_MAX_PARAM_LEN] - command parameters
**		uint8_t numParams -number of paramteters
**
**  Returns: 
**		BLUETOOTH_COMMAND_RESPONSE - response returned from PmodBT2
**
**	Errors:	none
**
**  Description:
**	
**	This function is primarily used for sending set/change commands to the PmodBT2(see RN-42 manual).
**  Each command parameter is assigned to a position in the params array, each parameter has a 
**  maximum length of BLUETOOTH_MAX_PARAM_LEN, and can contain a maximum of BLUETOOTH_MAX_PARAMS.
**  The number of parameters in the array must be passed in as numParams. 
**
**  Ex: set Set class of device to serial port
**	uint8_t params[BLUETOOTH_MAX_PARAMS][BLUETOOTH_MAX_PARAM_LEN]
**	strcpy(params[0],"SD");
**	strcpy(params[1],"1101");
**	response = BTSendSetCommand(UART_BLUETOOTH,params,2);
**
*/
BLUETOOTH_COMMAND_RESPONSE BTSendSetCommand(UART_MODULE uartID,uint8_t params[][BLUETOOTH_MAX_PARAM_LEN],uint8_t numParams)
{
	uint8_t commandResponse[BLUETOOTH_MAX_COMMAND_RESPONSE_LEN];
	uint8_t command[BLUETOOTH_MAX_COMMAND_LENGTH];
	UARTPutS(BTBuildCommand(command,params,numParams),uartID);
	BTGetLine(uartID,commandResponse);
	return BTGetSetResponseVal(commandResponse);
}
/*  
**  BTGetParam()
**
**	Synopsis:
**	Retireives a parameter from a comma delimited string
**
**  Input: 
**		const uint8_t* parameters - comma delimted string of parameters
**		uint8_t* oneParameter - parameter found, returned with null termination
**		uint8_t paramNum - paramter position
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  Retrieves a "parameter" from a comma delimeted string,
**  the parameter number is based on the number of commas in the
**  string starting a 0, for example parameter 2 in the string
**  "test1,test2,test3,test4" would be "test3.
**               ^   ^
**  Only values between commas are copied into *oneParameter, value
**  returned is null terminated.
*/
void BTGetParam(uint8_t* parameters,uint8_t* oneParameter,uint8_t paramNum)
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
** 	BTSendSetCommand()
**
**	Synopsis:
**	Use for sending commands (see RN-42 manual) to PmodBT2, returns response as an 
**  array of strings, number of response lines as uint32_t 
**  
**  Input: 
**		UART_MODULE uartID - Bluetooth uart modules
**		uint8_t params[][BLUETOOTH_MAX_PARAM_LEN] - command parameters
**		uint8_t numParams -number of paramteters
**		uint8_t commandResponse[][BLUETOOTH_MAX_COMMAND_RESPONSE_LEN] - array of string responses returned
**		uint32_t maxLinesReturned - maximum number of lines to return in commandResponse
**		uint32_t minNumLinesReturned - minimum number of lines to return in commandResponse
**
**  Returns: 
**		uint32_t - number of lines returned in command response
**
**	Errors:	none
**
**  Description:
**	
**	This function used for sending commands to the PmodBT2(see RN-42 manual) which return multi-line
**  responses. Lines read in are stored in commandResponse. Parameters for number of lines expected 
**  allow a upper and lower bounds for the number of lines read in. For example if maxLinesReturned = 5
**  and minLinesReturned = 2, any number of lines may be returned between 2 and 5.
**
**  Each command parameter is assigned to a position in the params array, each parameter has a 
**  maximum length of BLUETOOTH_MAX_PARAM_LEN, and can contain a maximum of BLUETOOTH_MAX_PARAMS.
**  The number of parameters in the array must be passed in as numParams. 
**
**  Ex: Perform Bluetooth Inquiry
**	uint8_t params[BLUETOOTH_MAX_PARAMS][BLUETOOTH_MAX_PARAM_LEN]
**	uint8_t getCommandResponse[BLUETOOTH_MAX_NUM_COMMAND_RESPONSE][BLUETOOTH_MAX_COMMAND_RESPONSE_LEN];
**	strcpy(params1[0],"I");
**	strcpy(params1[1],"5");
**  //number of params: 2, get minimum 2 lines, but up to 9 lines
**	BTSendCommand(uartID,params1,2,getCommandResponse,2,9);
**  //response to command is stored in getCommandResponse
*/
uint32_t BTSendCommand(UART_MODULE uartID,uint8_t params[][BLUETOOTH_MAX_PARAM_LEN],uint8_t numParams,uint8_t commandResponse[][BLUETOOTH_MAX_COMMAND_RESPONSE_LEN],uint32_t maxLinesReturned,uint32_t minNumLinesReturned)
{
	uint32_t numLinesRead = 0;
	uint8_t command[BLUETOOTH_MAX_COMMAND_LENGTH];
	UARTPutS(BTBuildCommand(command,params,numParams),uartID);
	numLinesRead = BTGetUpToNLines(uartID,commandResponse,maxLinesReturned,minNumLinesReturned);
	return numLinesRead;
}	
/*  
**  BTBuildCommand
**
**	Synopsis:
**	Builds commands accepted by the PmodBT2 (see RN-42 reference manual)
**
**  Input: 
**		uint8_t *fullCommand - command string built by this function
**		uint8_t params[][BLUETOOTH_MAX_PARAM_LEN] - parameters used to build command string
**		uint8_t numParams - number of parameters in params
**
**  Returns: 
**		uint8_t* - pointer to fullCommand
**
**	Errors:	none
**
**  Description:
**
**	Builds a comma delimited command string with trailing \r\n accepted by the PmodBT2(RN-42),
**  based on string values present in the params array. Command "$$$" will not recieve a 
**  trailing \r\n since this command places the PmodBT2 in command mode.
**
**  Example: Perform Bluetooth inquiry
**
**  uint8_t *fullCommand[BLUETOOTH_MAX_COMMAND_LENGTH];
**	uint8_t params[BLUETOOTH_MAX_PARAMS][BLUETOOTH_MAX_PARAM_LEN];
**  strcpy(params1[0],"I");
**	strcpy(params1[1],"5");
**  BTBuildCommand(fullCommand,params,2)
**  //fullCommand will contain the string "I,5\r\n"
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
**  BTGetSetResponseVal()
**
**	Synopsis:
**	Determines numerical response values for SET commands
**	(see RN-42 user manual)
**
**  Input: 
**		uint8_t *bytes - string response from SET command
**
**  Returns: 
**		BLUETOOTH_COMMAND_RESPONSE - response type
**
**	Errors:	none
**
**  Description:
**	Set commands contain a one line response as defined in the RN-42 user's
**  manual. The response passed in is compared to the know valid response strings,
**  when a match is found, the coressponding BLUETOOTH_COMMAND_RESPONSE
**  value is returned. 
*/
BLUETOOTH_COMMAND_RESPONSE BTGetSetResponseVal(uint8_t *bytes)
{
	if(strcmp(BLUETOOTH_COMMAND_RESPONSE_ERR_STR,bytes) == 0)
	{
		return BLUETOOTH_COMMAND_RESPONSE_ERR;
	}
	else if(strcmp(BLUETOOTH_COMMAND_RESPONSE_AOK_STR,bytes) == 0)
	{
		return BLUETOOTH_COMMAND_RESPONSE_AOK;
	}
	else if(strcmp(BLUETOOTH_COMMAND_RESPONSE_END_STR,bytes) == 0)
	{
		return BLUETOOTH_COMMAND_RESPONSE_END;
	}
	else if(strcmp(BLUETOOTH_COMMAND_UNRECOGNIZED_STR,bytes) == 0)
	{	
		return BLUETOOTH_COMMAND_UNRECOGNIZED;
	}
	else
	{	//could not determine response
		return BLUETOOTH_COMMAND_RESPONSE_UNK;
	}
}	

/*  
**  BTGetByteSequence()
**
**	Synopsis:
**	Returns the number of bytes requested from the PmodBT2
**
**  Input: 
**		UART_MODULE uartID - Bluetooth UART module
**		uint8_t *bytes - byte array to fill
**		uint32_t numBytes - number of bytes to retrieve
**
**  Returns: 
**		uint32_t - actual number of bytes read
**	
**	Errors:	none
**
**  Description:
**	Fills array *bytes with numBytes bytes retrieved from the PmodBT2 
**  at uartID. 
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
**  BTGetUpToNLines
**
**	Synopsis:
**	Reads a bounded number of lines from the PmodBT2
**
**  Input: 
**		UART_MODULE uartID -Bluetooth UART module
**		uint8_t bytes[][BLUETOOTH_MAX_COMMAND_RESPONSE_LEN] - lines read in from PmodBT2
**		uint32_t maxNumLines - maximum number of lines to read
**		uint32_t minNumLines - minimum number of lines to read
**
**  Returns:
**		uint32_t - actual number of lines read into bytes[][]
**
**	Errors:	none
**
**  Description:
**  Parameters for number of lines expected allow a upper and lower bounds for the number 
**  of lines read in. For example if maxNumLines = 5 and minNumLines = 2, any number of 
**  lines may be returned between 2 and 5. 
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
**  BTGetLine()
**
**	Synopsis:
**	Gets one line of bytes from the PmodBT2 terminating with \n
**
**  Input: 
**		UART_MODULE uartID - UART id
**		uint8_t *bytes - array to fill with bytes from UART
**
**  Returns: 
**		uint32_t - number of bytes read into *bytes
**
**	Errors:	none
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
**  delayN()
**
**	Synopsis:
**	Creates delay by performing N number of "nop" commands 
**	
**  Input: 
**		uint32_t delay - number of "nop" commands to perform
**
**  Returns: none
**
**	Errors:	none
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
**  UARTPutBytes()
**
**	Synopsis:
**	Writes a sequence of bytes to the specified UART
**
**  Input: 
**		uint8_t *bytes - bytes to write to UART
**		UART_MODULE uartID - UART id
**		uint32_t numBytes - number of bytes to write to UART
**
**  Returns: none
**
**	Errors:	none
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
**  UARTGetOneByte()
**
**	Synopsis:
**	Gets one byte of data from UART, blocks until available
**
**  Input: 
**		UART_MODULE uartID - UART id
**
**  Returns:
**		uint8_t - byte retrieved from UART
*
**	Errors:	none
*/
uint8_t UARTGetOneByte(UART_MODULE uartID)
{
	while(!UARTReceivedDataIsAvailable(uartID));
	
	return UARTGetDataByte(uartID);
}

/*  
**  UARTPutS()
**
**	Synopsis:
**	Writes a null terminated string to the UART specified
**
**  Input: 
**		uint8_t *string - null terminated string to write to UART
**		UART_MODULE uartID - UART id
**
**  Returns: none
**
**	Errors:	none
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
**  UARTPutOneByte()
**
**	Synopsis:
**	Writes one byte to the UART specified, only writes
**	if transmitter is ready.
**
**  Input: 
**		uint8_t oneByte - byte to write
**		UART_MODULE uartID - UART id
**
**  Returns: none
**
**	Errors:	none
**
*/
void UARTPutOneByte(uint8_t oneByte,UART_MODULE uartID)
{
	if (UARTTransmitterIsReady(uartID))
	{
   	 	UARTSendDataByte(uartID, oneByte);
	}	
}