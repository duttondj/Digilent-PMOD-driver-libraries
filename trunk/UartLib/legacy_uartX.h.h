/* 
 * File:   uart.h
 * Author: skylar
 *
 * Created on February 16, 2011, 11:24 AM
 */

#ifndef _LEGACY_UARTX_H
#define	_LEGACY_UARTX_H

#include <peripheral/ports.h>
#include <peripheral/ports.h>
#include <peripheral/system.h>

#define FALSE 0
#define TRUE  1
//Buffer size for Receive and Transmit
#define RX_BUFFER_SIZE_UART 0x0040  //size of 40 bytes
#define TX_BUFFER_SIZE_UART 0x0040  //size of 40 bytes
#define UART_DEFAULT_BAUD_RATE 9600 //baud rate of the UART

//32MX4 Global UART variables
#if defined __CEREBOT32MX4__

// flag variables
volatile u08   readyTx_UART1;         ///< ReadyTx_UART1 flag
volatile u08   readyTx_UART2;         ///< ReadyTx_UART2 flag

volatile u08   bufferedTx_UART1;      ///< BufferedTx_UART1 flag
volatile u08   bufferedTx_UART2;      ///< BufferedTx_UART2 flag

// receive and transmit buffers UART1 && UART2
cBuffer rxBuffer_UART1;               ///< uart1 receive buffer
cBuffer rxBuffer_UART2;               ///< uart2 receive buffer
cBuffer txBuffer_UART1;               ///< uart1 transmit buffer
cBuffer txBuffer_UART2;               ///< uart2 transmit buffer

// automatically allocate space in ram for each buffer
static char rxData_UART1[RX_BUFFER_SIZE_UART];
static char rxData_UART2[RX_BUFFER_SIZE_UART];
static char txData_UART1[TX_BUFFER_SIZE_UART];
static char txData_UART2[TX_BUFFER_SIZE_UART];

// overflow counter
unsigned short rxOverflow_UART1;      ///< receive overflow counter UART1
unsigned short rxOverflow_UART2;      ///< receive overflow counter UART2
#endif

//Enumerates the channels available
//Channels 1-2 available on the CEREBOTMX432
typedef enum
{
    UART_C1 = 1,  //Select UART Channel 1
    UART_C2 = 2   //Select UART Channel 2

#if defined __CEREBOT32MX7__
	//UART_C3 = 3  //Select UART Channel 3 (MX7 only)
#endif
}UARTCHANNEL;


void fnUARTgets(unsigned int length, char* buffer,
                unsigned int uart_data_wait, UARTCHANNEL uartChannel);
void fnUARTputs(const char* buffer, UARTCHANNEL uartChannel);
void fnUARTEnableDigitalPinIO(UARTCHANNEL uartChannel);
void fnUARTStandardSetup(unsigned long sysClock, UARTCHANNEL uartChannel);

#endif	/* _UART_H */

