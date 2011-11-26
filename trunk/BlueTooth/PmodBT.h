#ifndef _PMODBT_H_
#define _PMODBT_H_

#include <string.h>
#include <stdint.h>
#include <plib.h>

#define ENTER_COMMAND 				"$$$"
#define COMMAND_MODE_RESPONSE 		212 //CMD\r\n (sum of ascii bytes)
#define COMMAND_EXIT 				"---" 
#define COMMAND_VALID_RESPONSE		219 //AOK => 65 + 79 + 75 = 219
#define COMMAND_ERROR_RESPONSE		233 //ERR => 69 + 82 + +82 = 233
#define COMMAND_UNRECOGNIZED		"?\r\n"
#define COMMAND_END_RESPONSE		"END"
#define MAX_COMMAND_RESPONSE_LEN	128	       			
#define RESPONSE_CONNECT			"DEV_CONNECT\r\n"
#define MAX_COMMAND_LEN				40


void UARTPutS(uint8_t *string,UART_MODULE uartID);
void UARTSetup(uint32_t baudRate,uint32_t pbClock,UART_MODULE uartID);
void UARTPutS(uint8_t *string,UART_MODULE uartID);
uint8_t UARTGetOneByte(UART_MODULE uartID);
void UARTPutBytes(uint8_t *bytes,UART_MODULE uartID,uint32_t numBytes);
uint16_t ByteSum(uint8_t *bytes,uint8_t numBytes);
uint32_t BTGetByteSequence(UART_MODULE uartID,uint8_t *bytes,uint32_t numBytes);
void resetBTModule();
uint8_t* BTBuildCommand(uint8_t *fullCommand,uint8_t params[][15],uint8_t numParams);
uint16_t BTSendSetCommand(UART_MODULE uartID,uint8_t params[][15],uint8_t numParams);
void UARTPutOneByte(uint8_t oneByte,UART_MODULE uartID);
uint32_t BTGetLine(UART_MODULE uartID,uint8_t *bytes);
void delayN(uint32_t delay);
uint32_t BTGetUpToNLines(UART_MODULE uartID,uint8_t bytes[][40],uint32_t maxNumLines,uint32_t minNumLines);
uint32_t BTSendGetCommand(UART_MODULE uartID,uint8_t params[][15],uint8_t numParams,uint8_t commandResponse[][40],
						uint32_t maxLinesReturned,uint32_t minNumLinesReturned);
void BTGetParam(const uint8_t* parameters,uint8_t* oneParameter,uint8_t paramNum);
uint8_t BTGetAddressFromName(const uint8_t addressNamePairs[][40],const uint8_t *name,uint8_t *address,uint8_t numEntries);
uint8_t BTInquireAndConnect(UART_MODULE uartID,uint8_t *peerName,uint8_t *inquiryTimeOut,uint8_t numInquiryTries);

#endif