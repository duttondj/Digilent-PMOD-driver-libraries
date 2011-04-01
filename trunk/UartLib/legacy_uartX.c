/************************************************************************/
/*                                                                      */
/*   uart_32mx.c  -- Wrappers for UART Microchip PIC32 library functions	*/
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
/*	2/21/2011(SkylarG):
/*                                                                      */
/************************************************************************/

#include "buffer.h"
#include "peripherals/uart.h"
#include "uart.h"

#define u08 unsigned char



/* ------------------------------------------------------------ */
/***	fnUARTStandardSetup
**
**	Synopsis:
**  fnUARTStandardSetup(sysClock, uartChannel)
**
**	Input:
 *              sysClock - System clock speed (Hz)
**		uartChannel - UART channel number
**
**	Output:
**      none
**
**	Errors:
**		none
**
**	Description:
**      This function initializes the UART pertaining to the UART channel provided
**      by configuring a pb_clock and then using the OpenUartx function. This is
**      a general use of the specific UART, and if custimization is required then
**      it is recommended to use OpenUARTx with reconfigured parameters.
**      Example: fnUARTStandardSetup(8000000, UART_C1)
*/
void fnUARTStandardSetup(unsigned long sysClock, UARTCHANNEL uartChannel)
{
     unsigned int pb_clock = 0;
     //configure the pb_clock from the system clock parameter
     pb_clock = SYSTEMConfigPerformance (sysClock);



#if defined __CEREBOT32MX4__
    if(uartChannel == UART_C2)
    {
     //Open UART on channel 2 for standard configuration
     OpenUART2 (  UART_EN | UART_IDLE_CON | UART_RX_TX | UART_DIS_WAKE
                | UART_DIS_LOOPBACK | UART_DIS_ABAUD | UART_NO_PAR_8BIT
                | UART_1STOPBIT | UART_IRDA_DIS | UART_MODE_FLOWCTRL
                | UART_DIS_BCLK_CTS_RTS | UART_NORMAL_RX | UART_BRGH_SIXTEEN,
                  UART_TX_PIN_LOW | UART_RX_ENABLE | UART_TX_ENABLE | UART_INT_TX
                | UART_INT_RX_CHAR | UART_ADR_DETECT_DIS | UART_RX_OVERRUN_CLEAR,
	          mUARTBRG(pb_clock, DESIRED_BAUD_RATE));

     // initialize the buffers
     initBuffers_UART(uartChannel);

     // initialize user receive handler
     rxFunc_UART2 = 0;

     // set default baud rate
     setBaudRate_UART(UART_DEFAULT_BAUD_RATE, uartChannel);

     // initialize states
     readyTx_UART2 = TRUE;
     bufferedTx_UART2 = FALSE;

     // clear overflow count
     rxOverflow_UART2 = 0;
    }
    else //Open UART on channel 1 for standard configuration
    {
     OpenUART1 (  UART_EN | UART_IDLE_CON | UART_RX_TX | UART_DIS_WAKE
                | UART_DIS_LOOPBACK | UART_DIS_ABAUD | UART_NO_PAR_8BIT
                | UART_1STOPBIT | UART_IRDA_DIS | UART_MODE_FLOWCTRL
                | UART_DIS_BCLK_CTS_RTS | UART_NORMAL_RX | UART_BRGH_SIXTEEN,
                  UART_TX_PIN_LOW | UART_RX_ENABLE | UART_TX_ENABLE | UART_INT_TX
                | UART_INT_RX_CHAR | UART_ADR_DETECT_DIS | UART_RX_OVERRUN_CLEAR,
	          mUARTBRG(pb_clock, DESIRED_BAUD_RATE));

     // initialize the buffers
     initBuffers_UART(uartChannel);

     // initialize user receive handler
     rxFunc_UART1 = 0;

     // set default baud rate
     setBaudRate_UART(UART_DEFAULT_BAUD_RATE, uartChannel);

     // initialize states
     readyTx_UART1 = TRUE;
     bufferedTx_UART1 = FALSE;

     // clear overflow count
     rxOverflow_UART1 = 0;
    }
#endif
     
}

/* ------------------------------------------------------------ */
/***	fnUARTEnableDigitalPinIO
**
**	Synopsis:
**  fnUARTEnableDigitalPinIO(uartChannel)
**
**	Input:
**		uartChannel - UART channel number
**
**	Output:
**      none
**
**	Errors:
**		none
**
**	Description:
**      Digital pin IO is setup according to the channel selected, see the
**      Digilent CEREBOT32MX4 reference manual for Pmod Port mappings. SCKx, SDIx,
**      and SDOx,and SSx are included in the digital pin IO configuration.
**      Example: fnUARTEnableDigitalPinIO(UART_C1)
*/
void fnUARTEnableDigitalPinIO(UARTCHANNEL uartChannel)
{
#if defined __CEREBOT32MX4__
	if(uartChannel == UART_C2)
	{
            // UART 2 port pins
            PORTSetPinsDigitalIn (IOPORT_F, BIT_4);  //JH-03 PMA9/U2RX/CN17/RF4     RF4
            PORTSetPinsDigitalOut (IOPORT_F, BIT_5); //JH-04 PMA8/U2TX/CN18/RF5     RF5

        }
        else
        {
            // UART 1 port pins
            PORTSetPinsDigitalIn (IOPORT_F, BIT_2);  //JE-03 U1RX/RF2 	RF2
            PORTSetPinsDigitalOut (IOPORT_F, BIT_8); //JE-04 U1TX/RF8   RF8
        }
#endif
}

/* ------------------------------------------------------------ */
/***	fnUARTputs
**
**	Synopsis:
**  fnUARTputs(const char* buffer, UARTCHANNEL uartChannel)
**
**	Input:
**		buffer - NULL terminated string to be transmitted
**              uartChannel - UART channel number
**
**	Output:
**      none
**
**	Errors:
**		none
**
**	Description:
**      Transmit a NULL terminated string via the uart channel number provided
**      in the parameters.
**      Example: fnUARTputs(mystring, UART_C1);
 *               fnUARTputs("Hello World!", UART_C1);
*/
void fnUARTputs(const char* buffer, UARTCHANNEL uartChannel)
{
    
#if defined __CEREBOT32MX4__
    if (uartChannel == UART_C2)
    {
        putsUART2(buffer);
    }
    else
    {
        putsUART1(buffer);
    }
#endif
}

/* ------------------------------------------------------------ */
/***	fnUARTgets
**
**	Synopsis:
**  fnUARTgets(unsigned int length, char* buffer,
 *             unsigned int uart_data_wait, UARTCHANNEL uartChannel)
**
**	Input:
 *              length - This is the length of the string to be received.
**		buffer - A pointer to the location where the data recieved has
 *                       been stored.
 *              uart_data_wait - This is the time-out count for which the module
 *                       has to wait before return.
**              uartChannel - UART channel number
**
**	Output:
**      none
**
**	Errors:
**		none
**
**	Description:
**      Receives data from UART and stores it into the specified buffer location.
**      Example: fnUARTgets(12, myBuffer, 123, UART_C1);
 *               fnUARTgets(5, myBuffer, 123, UART_C2);
*/
void fnUARTgets(unsigned int length, char* buffer,
                unsigned int uart_data_wait, UARTCHANNEL uartChannel)
{

#if defined __CEREBOT32MX4__
    if (uartChannel == UART_C2)
    {
        getsUART2(length, buffer, uart_data_wait);
    }
    else
    {
        getsUART1(length, buffer, uart_data_wait);
    }
#endif
}

/* ------------------------------------------------------------ */
/***	fnUARTbusy
**
**	Synopsis:
**  fnUARTbusy(UARTCHANNEL uartChannel)
**
**	Input:
**              uartChannel - UART channel number
**
**	Output:
**      If '0' is returned then UART transmitter is not busy, any other
 *      non-zero value returned means the transmitter is busy.
**
**	Errors:
**		none
**
**	Description:
**      Checks to see if the UARTs transmitter is empty. The uart channel is 
 *      passed into the function to specify which uart is being checked. This
 *      function works with both pic32MX4 and pic32MX7 boards.
 *      Example: fnUARTbusy(UART_C1);
 *               fnUARTbusy(UART_C2);
*/
int fnUARTbusy(UARTCHANNEL uartChannel)
{
    int busy = 0;

#if defined __CEREBOT32MX4__
    if (uartChannel == UART_C2)
    {
       return BusyUART2();

    }
    else
    {
       return BusyUART1();
    }
#endif
}

/* ------------------------------------------------------------ */
/***	fnUARTdataRdy
**
**	Synopsis:
**  fnUARTdataRdy(UARTCHANNEL uartChannel)
**
**	Input:
**              uartChannel - UART channel number
**
**	Output:
**      If '0' is returned then the receive buffer does not have any new data to
 *      be read. Otherwise is a non-zero value is returned then there is new
 *      data ready to be read.
**
**	Errors:
**		none
**
**	Description:
**      Checks the UART for new data to be read. The uart channel to be checked
 *      is passed into the function. This function works with both pic32MX4 and
 *      pic32MX7 boards.
**      Example: fnUARTdataRdy(UART_C1);
 *               fnUARTdataRdy(UART_C2);
*/
int fnUARTdataRdy(UARTCHANNEL uartChannel)
{

#if defined __CEREBOT32MX4__
    if (uartChannel == UART_C2)
    {
        return DataRdyUART2();
    }
    else
    {
        return DataRdyUART1();
    }
#endif
}

/* ------------------------------------------------------------ */
/***	fnUARTclose
**
**	Synopsis:
**  fnUARTclose(UARTCHANNEL uartChannel)
**
**	Input:
**              uartChannel - UART channel number
**
**	Output:
**              none
**
**	Errors:
**		none
**
**	Description:
**      Closes the UART and disables it's interupt. The uart channel to be closed
 *      is passed into the function. This function works with both pic32MX4 and
 *      pic32MX7 boards.
**      Example: fnUARTclose(UART_C1);
 *               fnUARTclose(UART_C2);
*/
void fnUARTclose(UARTCHANNEL uartChannel)
{

#if defined __CEREBOT32MX4__
    if (uartChannel == UART_C2)
    {
        CloseUART2();
    }
    else
    {
        CloseUART1();
    }
#endif
}

/* ------------------------------------------------------------ */
/***	fnUARTwrite
**
**	Synopsis:
**  fnUARTwrite(unsigned int data,
 *              UARTCHANNEL uartChannel)
**
**	Input:
 *              data        - A character to be transmitted
**              uartChannel - UART channel number
**
**	Output:
**              none
**
**	Errors:
**		none
**
**	Description:
**      Transmits a character via UART. The uart channel to be transmitted on
 *      is passed into the function. This function works with both pic32MX4 and
 *      pic32MX7 boards.
**      Example: fnUARTwrite('a', UART_C1);
 *               fnUARTwrite(mychar, UART_C2);
*/
void fnUARTwrite(unsigned int data, UARTCHANNEL uartChannel)
{

#if defined __CEREBOT32MX4__
    if (uartChannel == UART_C2)
    {
        WriteUART2(data);
    }
    else
    {
        WriteUART1(data);
    }
#endif
}

/* ------------------------------------------------------------ */
/***	fnUARTread
**
**	Synopsis:
**  fnUARTread(UARTCHANNEL uartChannel)
**
**	Input:
**              uartChannel - UART channel number
**
**	Output:
**      Returns a character read from UART receive buffer
**
**	Errors:
**		none
**
**	Description:
**      This function reads a character for the UART receive buffer. The uart
 *      channel to be read from is passed into the function. This function works
 *      with both pic32MX4 and pic32MX7 boards.
**      Example: fnUARTread(UART_C1);
 *               fnUARTread(UART_C2);
*/
char fnUARTread(UARTCHANNEL uartChannel)
{

#if defined __CEREBOT32MX4__
    if (uartChannel == UART_C2)
    {
        return ReadUART2();
    }
    else
    {
        return ReadUART1();
    }
#endif
}

/* ------------------------------------------------------------ */
/***	fnUARTenableAutoAddr
**
**	Synopsis:
**  fnUARTenableAutoAddr(int address, UARTCHANNEL uartChannel)
**
**	Input:
 *              address     - The 9-bit address for this UART
**              uartChannel - UART channel number
**
**	Output:
**              none
**
**	Errors:
**		none
**
**	Description:
**      This function enables auto address matching mode for UART. The uart
 *      channel is passed in to determine which uart to enable auto address
 *      matching on. This function works for both pic32MX4 and
 *      pic32MX7 boards.
**      Example: fnUARTenableAutoAddr(0x18, UART_C1);
 *               
*/
void fnUARTenableAutoAddr(int address, UARTCHANNEL uartChannel)
{

#if defined __CEREBOT32MX4__
    if (uartChannel == UART_C2)
    {
        UART2EnableAutoAddr(address);
    }
    else
    {
        UART1EnableAutoAddr(address);
    }
#endif
}

/* ------------------------------------------------------------ */
/***	fnUARTsendBreak
**
**	Synopsis:
**  fnUARTsendBreak(UARTCHANNEL uartChannel)
**
**	Input:
**              uartChannel - UART channel number
**
**	Output:
**              none
**
**	Errors:
**		none
**
**	Description:
**      This function initiates a break sequence on the UART. The uart channel
 *      to be halted is passed into the function. This function works for both pic32MX4 and
 *      pic32MX7 boards.
**      Example: fnUARTsendBreak(UART_C1);
 *               fnUARTsendBreak(UART_C2);
*/
void fnUARTsendBreak(UARTCHANNEL uartChannel)
{

#if defined __CEREBOT32MX4__
    if (uartChannel == UART_C2)
    {
        UART2SendBreak();
    }
    else
    {
        UART1SendBreak();
    }
#endif
}

/* ------------------------------------------------------------ */
/***	fnUARTgetErrors
**
**	Synopsis:
**  fnUARTgetErrors(UARTCHANNEL uartChannel)
**
**	Input:
**              uartChannel - UART channel number
**
**	Output:
**      Returns a bitmap of various error values.
**
**	Errors:
**      Overflow error - _UxSTA_OERR_MASK
 *      Frame error    - _UxSTA_FERR_MASK
 *      Parity error   - _UxSTA_PERR_MASK
**
**	Description:
**      This function retreives the errors on a UART. The uart channel is passed
 *      as a parameter to determine which uart to retreive errors from. This
 *      function works for both pic32MX4 and pic32MX7 boards.
**      Example: fnUARTgetErrors(UART_C1);
 *               fnUARTgetErrors(UART_C2);
*/
int fnUARTgetErrors(UARTCHANNEL uartChannel)
{

#if defined __CEREBOT32MX4__
    if (uartChannel == UART_C2)
    {
        return UART2GetErrors();
    }
    else
    {
        return UART1GetErrors();
    }
#endif
}

/* ------------------------------------------------------------ */
/***	fnUARTclearError
**
**	Synopsis:
**  fnUARTclearError(int error, UARTCHANNEL uartChannel)
**
**	Input:
 *              error       - bits associated with the error
**              uartChannel - UART channel number
**
**	Output:
**              none
**
**	Errors:
**      Overflow error - _UxSTA_OERR_MASK
 *      Frame error    - _UxSTA_FERR_MASK
 *      Parity error   - _UxSTA_PERR_MASK
**
**	Description:
**      This function clears the error on a UART. The uart channel is passed
 *      as a parameter to determine which uart to clear the error from. This
 *      function works for both pic32MX4 and pic32MX7 boards.
**      Example: fnUARTclearError(_UxSTA_FERR_MASK, UART_C1);
 *               fnUARTclearError(_UxSTA_PERR_MASK, UART_C2);
*/
void fnUARTclearError(int error, UARTCHANNEL uartChannel)
{

#if defined __CEREBOT32MX4__
    if (uartChannel == UART_C2)
    {
        UART2ClearError(error);
    }
    else
    {
        UART1ClearError(error);
    }
#endif
}

/* ------------------------------------------------------------ */
/***	fnUARTclearAllErrors
**
**	Synopsis:
**  fnUARTclearAllErrors(UARTCHANNEL uartChannel)
**
**	Input:
**              uartChannel - UART channel number
**
**	Output:
**              none
**
**	Errors:
**      Overflow error - _UxSTA_OERR_MASK
 *      Frame error    - _UxSTA_FERR_MASK
 *      Parity error   - _UxSTA_PERR_MASK
**
**	Description:
**      This function clears all erros on a UART. The uart channel is passed
 *      as a parameter to determine which uart to clear the errors from. This
 *      function works for both pic32MX4 and pic32MX7 boards.
**      Example: fnUARTclearAllErrors(UART_C1);
 *               fnUARTclearAllErrors(UART_C2);
*/
void fnUARTclearAllErrors(UARTCHANNEL uartChannel)
{

#if defined __CEREBOT32MX4__
    if (uartChannel == UART_C2)
    {
        UART2ClearAllErrors();
    }
    else
    {
        UART1ClearAllErrors();
    }
#endif
}