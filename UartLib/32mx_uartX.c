/************************************************************************/
/*                                                                      */
/*   32mx_uartX.c  -- Wrappers for UART Microchip PIC32 library functions	*/
/*                                                                      */
/************************************************************************/
/*	Author: 	Skylar Graika											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  This file contains function definitions used in wrapping the        */
/*  Microchip PIC32 universal asynchronous receiver/transmitter libraries for use with  */
/*  the Digilent CEREBOT32MX4 (*TODO* & 32MX7)                                          */
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*	3/25/2011(SkylarG):
/*                                                                      */
/************************************************************************/





/* ------------------------------------------------------------ */
/***	fnUARTOpen_basicRxTx
**
**	Synopsis:
**              fnUARTOpen_basicRxTx(id)
**
**	Input:
**		id          -   Uart module number
**
**	Output:
**              Returns 1 on success or -1 on failure to open.
**
**	Errors:
**		none
**
**	Description:
**              This function enables the uart for basic read and write capabilities.
 *      The function requires the uart module id which specifies which uart it
 *      will setup. UART_ENABLE_FLAGS (located in <peripheral/uart.c>) is a macro
 *      in which creates the appropriate flags for enabling operating modes in
 *      the uart module.
 *
**      Example: 
 *              fnUARTOpen_basicRxTx(UART1)
 *              fnUARTOpen_basicRxTx(UART2)
*/
int fnUARTOpen_basicRxTx(UART_MODULE id)
{
    #if defined __CEREBOT32MX4__

        if(id != UART1 && id != UART2)
        {
            return -1; //Invalid uart module id.
        }


        UARTEnable(id, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

        return 1;

    #endif
}


/* ------------------------------------------------------------ */
/***	fnUARTOpen_basicRx
**
**	Synopsis:
**              fnUARTOpen_basicRx(id)
**
**	Input:
**		id          -   Uart module number
**
**	Output:
**              Returns 1 on success or -1 on failure to open.
**
**	Errors:
**		none
**
**	Description:
**              This function enables the uart for basic read capabilities.
 *      The function requires the uart module id which specifies which uart it
 *      will setup. UART_ENABLE_FLAGS (located in <peripheral/uart.c>) is a macro
 *      in which creates the appropriate flags for enabling operating modes in
 *      the uart module.
 *
**      Example:
 *              fnUARTOpen_basicRx(UART1)
 *              fnUARTOpen_basicRx(UART2)
*/


int fnUARTOpen_basicRx(UART_MODULE id)
{
    #if defined __CEREBOT32MX4__

        if(id != UART1 && id != UART2)
        {
            return -1; //Invalid uart module id.
        }


        UARTEnable(id, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX ));

        return 1;

    #endif
}


/* ------------------------------------------------------------ */
/***	fnUARTOpen_basicTx
**
**	Synopsis:
**              fnUARTOpen_basicTx(id)
**
**	Input:
**		id          -   Uart module number
**
**	Output:
**              Returns 1 on success or -1 on failure to open.
**
**	Errors:
**		none
**
**	Description:
**              This function enables the uart for basic write capabilities.
 *      The function requires the uart module id which specifies which uart it
 *      will setup. UART_ENABLE_FLAGS (located in <peripheral/uart.c>) is a macro
 *      in which creates the appropriate flags for enabling operating modes in
 *      the uart module.
 *
**      Example:
 *              fnUARTOpen_basicTx(UART1)
 *              fnUARTOpen_basicTx(UART2)
*/
int fnUARTOpen_basicTx(UART_MODULE id)
{
    #if defined __CEREBOT32MX4__

        if(id != UART1 && id != UART2)
        {
            return -1; //Invalid uart module id.
        }


        UARTEnable(id, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_TX ));

        return 1;

    #endif
}