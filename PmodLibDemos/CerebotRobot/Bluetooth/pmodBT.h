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

#ifndef _PMODBT_H_
#define _PMODBT_H_
/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include <string.h>
#include <stdint.h>
#include <plib.h>
/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */

#define ENTER_COMMAND 						"$$$"	
#define COMMAND_EXIT 						"---" 
#define COMMAND_END_RESPONSE				"END"

#define BLUETOOTH_COMMAND_RESPONSE_ERR_STR		"ERR\r\n"
#define BLUETOOTH_COMMAND_RESPONSE_AOK_STR		"AOK\r\n"
#define BLUETOOTH_COMMAND_RESPONSE_END_STR		"END\r\n"
#define BLUETOOTH_COMMAND_UNRECOGNIZED_STR		"?\r\n"

#define BLUETOOTH_MAX_NUM_COMMAND_RESPONSE		10
#define BLUETOOTH_MAX_COMMAND_LENGTH			128
#define BLUETOOTH_MAX_COMMAND_RESPONSE_LEN		40

#define BLUETOOTH_MAX_PARAMS					3
#define BLUETOOTH_MAX_PARAM_LEN					15
#define BLUETOOTH_MAX_ADDRESS_LEN				18

typedef enum
{
 	BLUETOOTH_COMMAND_RESPONSE_ERR,			
 	BLUETOOTH_COMMAND_RESPONSE_AOK,			
 	BLUETOOTH_COMMAND_RESPONSE_END,			
 	BLUETOOTH_COMMAND_UNRECOGNIZED,			
 	BLUETOOTH_COMMAND_RESPONSE_UNK,  //could not determine response
}BLUETOOTH_COMMAND_RESPONSE;

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */
void UARTPutS(uint8_t *string,UART_MODULE uartID);
void UARTSetup(uint32_t baudRate,uint32_t pbClock,UART_MODULE uartID);
void UARTPutS(uint8_t *string,UART_MODULE uartID);
uint8_t UARTGetOneByte(UART_MODULE uartID);
void UARTPutBytes(uint8_t *bytes,UART_MODULE uartID,uint32_t numBytes);
BLUETOOTH_COMMAND_RESPONSE BTGetSetResponseVal(uint8_t *bytes);
uint32_t BTGetByteSequence(UART_MODULE uartID,uint8_t *bytes,uint32_t numBytes);
void resetBTModule();
uint8_t* BTBuildCommand(uint8_t *fullCommand,uint8_t params[][BLUETOOTH_MAX_PARAM_LEN],uint8_t numParams);
BLUETOOTH_COMMAND_RESPONSE BTSendSetCommand(UART_MODULE uartID,uint8_t params[][BLUETOOTH_MAX_PARAM_LEN],uint8_t numParams);
void UARTPutOneByte(uint8_t oneByte,UART_MODULE uartID);
uint32_t BTGetLine(UART_MODULE uartID,uint8_t *bytes);
void delayN(uint32_t delay);
uint32_t BTGetUpToNLines(UART_MODULE uartID,uint8_t bytes[][BLUETOOTH_MAX_COMMAND_RESPONSE_LEN],uint32_t maxNumLines,uint32_t minNumLines);
uint32_t BTSendCommand(UART_MODULE uartID,uint8_t params[][BLUETOOTH_MAX_PARAM_LEN],uint8_t numParams,uint8_t commandResponse[][BLUETOOTH_MAX_COMMAND_RESPONSE_LEN],
						uint32_t maxLinesReturned,uint32_t minNumLinesReturned);
void BTGetParam(const uint8_t* parameters,uint8_t* oneParameter,uint8_t paramNum);
uint8_t BTGetAddressFromName(const uint8_t addressNamePairs[][BLUETOOTH_MAX_COMMAND_RESPONSE_LEN],const uint8_t *name,uint8_t *address,uint8_t numEntries);
uint8_t BTInquireAndConnect(UART_MODULE uartID,uint8_t *peerName,uint8_t *inquiryTimeOut,uint8_t numInquiryTries,uint8_t *connectResponse);
uint8_t BTRecieveConnect(UART_MODULE uartID,uint8_t* responseString);

#endif