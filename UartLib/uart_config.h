/************************************************************************/
/*                                                                      */
/*   uart_config.h  -- Configuration file that allows the proper setup for the device/s   */
/*					   communicating with the UART.						*/
/*                                                                      */
/************************************************************************/
/*	Author: 	Skylar Graika											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  This file contains configuration variables that allow the correct 
/*    configuration with the device/s communicating with the UARTS		*/                                          */
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*	4/13/2011(SkylarG):
/*                                                                      */
/************************************************************************/

//These defined variables should be set accordingly based on which port
//the uart device is inserted into. (i.e. 0 ==> none, 1 ==> using)
//See the pic32mx Reference Manual for more information on ports and the bits
//associated with those ports.
#define _UART_JE_ 0		//Port JE	-Used by UART1
#define _UART_JH_ 1		//Port JH	-Used by UART2

//Below is a layout of the ports JE and JH and a 
//mapping of the use of the pins associated with them.

	//  UART JE port pins -
	//    JE-01 CN20/U1CTS/RD14 		RD14
	//    JE-02 U1RTS/BCLK1/CN21/RD15   RD15
	//    JE-03 U1RX/RF2 				RF2
	//    JE-04 U1TX/RF8 				RF8 

	//  UART JH port pins - 
	// 	  JH-01 U2CTS/RF12 			RF12
	//	  JH-02 U2RTS/BCLK2/RF13 	RF13
    //    JH-03 PMA9/U2RX/CN17/RF4 	RF4
	//    JH-04 PMA8/U2TX/CN18/RF5 	RF5 
	