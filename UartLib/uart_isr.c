/************************************************************************/
/*                                                                      */
/*   uart_isr.c  -- Interupt utility file for uart. Contains necessary functions for
/* 					configuring interupts with processing them once spotted.					*/
/*                                                                      */
/************************************************************************/
/*	Author: 	Skylar Graika											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  This file contains configuration functions for setting up interupts with
/*  uarts.		                                                        */
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*	4/13/2011(SkylarG):
/*                                                                      */
/************************************************************************/
#include <peripheral/ports.h>
#include <peripheral/uart.h>
#include <peripheral/system.h>	// Set up the system and perihperal clocks for best performance
#include <stdint.h>
#include <stdio.h>


void configure_interrupts_UART1RX (void)
{
	// Configure for multi-vectored mode
	INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);

	// Configure UART1 RX Interrupt
	INTEnable(INT_SOURCE_UART_RX(UART1), INT_ENABLED);
	INTSetVectorPriority(INT_VECTOR_UART(UART1), INT_PRIORITY_LEVEL_5);

	// enable interrupts
	INTEnableInterrupts();
}
void configure_interrupts_UART1TX (void)
{
	// Configure for multi-vectored mode
	INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);

	// Configure UART1 TX Interrupt
	INTEnable(INT_SOURCE_UART_TX(UART1), INT_ENABLED);
	INTSetVectorPriority(INT_VECTOR_UART(UART1), INT_PRIORITY_LEVEL_5);

	// enable interrupts
	INTEnableInterrupts();
}
void configure_interrupts_UART2RX (void)
{
	// Configure for multi-vectored mode
	INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);

	// Configure UART2 RX Interrupt
	INTEnable(INT_SOURCE_UART_RX(UART2), INT_ENABLED);
	INTSetVectorPriority(INT_VECTOR_UART(UART2), INT_PRIORITY_LEVEL_5);

	// enable interrupts
	INTEnableInterrupts();
}
void configure_interrupts_UART2TX (void)
{
	// Configure for multi-vectored mode
	INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);

	// Configure UART2 TX Interrupt
	INTEnable(INT_SOURCE_UART_TX(UART2), INT_ENABLED);
	INTSetVectorPriority(INT_VECTOR_UART(UART1), INT_PRIORITY_LEVEL_5);

	// enable interrupts
	INTEnableInterrupts();
}

// UART interrupt service routine. Can be modified
// to handle interupts on UART1 or UART2 by changing
// the IntUart[1-2]Handler(void) function and _UART[1-2]_VECTOR
// accordingly.

void __ISR(_UART1_VECTOR, ipl5) IntUart1Handler(void)
{
	// Check for RX interupt on UART1(can be switched to UART 2)
	if(INTGetFlag(INT_SOURCE_UART_RX(UART1)))
	{
		// Clear the RX interrupt Flag
		INTClearFlag(INT_SOURCE_UART_RX(UART1));
	}

	// Check for TX interupt on UART1(can be switched to UART 2)
	if ( INTGetFlag(INT_SOURCE_UART_TX(UART1)) )
	{
		INTClearFlag(INT_SOURCE_UART_TX(UART1));
	}
}