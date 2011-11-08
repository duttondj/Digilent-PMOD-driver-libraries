/************************************************************************/
/*                                                                      */
/*                           pmodMic.c                                 */
/*                                                                      */
/*                                                                      */
/* -------------------------------------------------------------------- */
/*	Author: 	Skylar Graika											*/
/*              Copyright (C) 2011 Skylar Graika                         */
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  11/07/2011(SkylarG):                                                  */
/*                                                                      */
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include "./PmodMic/pmodMic.h"
#include "./PmodCommon/bufferlib/bufferlib.h"
#include "./PmodSF/pmodsf.h"
#include "./PmodCommon/spi/pmod_spi_common.h"

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

#define TICKRATE 8192
SpiChannel chn;

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

/*  
**  fnInitPmodMic
**
**	Synopsis:
**  Initializes the SPI port for the PmodSF and detemines 
**  its flash capacity.
**
**  Input: 
**  	uint8_t chn - Pmod SPI channel
**      uint32_t pbClock - peripheral bus clock rate in Hz
**      uint32_t bitRate - bitrate in Hz
**      UART_MODULE uart - serial console UART
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  Initializes the SPI module at the specified bitrate for the PmodSF,
**  calls fnSetPmodFlashCapacity to set the global variable pmmodFlashCapacity
*/
void fnInitPmodMic(UART_MODULE uartID)
{
	UARTPutS("\r\nPmodMic SPI port=>",uartID);
	chn =  getIntegerFromConsole(uartID); //SPI port PMODSF is connected to
	PmodMicInit(chn,PB_CLOCK,SPI_BITRATE);

}

/*  PmodMicInit
**
**	Synopsis:
**  Initializes the PmodMic module on the selected SPI channel. 
**  
**  Input: SpiChannel chn  - spi channel initialize
**         uint32_t pbClock - peripheral bus clock in Hz
**         uint32_t bitRate - bit rate desired in Hz
**
**  Returns: none
**
**  Description:
**
**  Opens the desired SPI channel in 8-bit mode as a master, enables the slave select bit,
**  and sets the desired bit rate as a function of pbClock/bitRate.  Examples of peripheral bus
**  bit rate combinations are available in the table labeld "Excerpt from PIC32 Familiy Reference 
**  Manual Chapter 23 section 23.3.7" in pmodsf.h.
*/
void PmodMicInit(uint32_t pbClock,uint32_t bitRate)
{
    SpiChnOpen(chn, SPI_OPEN_MSTEN | SPI_OPEN_SSEN |  SPI_OPEN_MODE8 , pbClock/bitRate);
}

void PmodMicStartRecording()
{
	//TODO:
	//Insert UARTID
	fnInitPmodMic(1);
	fnTimer1Setup();
	configure_interrupts();
}

void PmodMicStopRecording()
{

}

void PmodMicTakeSample()
{
	PmodSPISetSSLow(chn); //SS to low 
	
	//GET 2 BYTES FROM PMODMIC

	uint8_t oneByte = 0;
	SpiChnPutC(chn,0);
	oneByte = SpiChnGetC(chn);
	oneByte << 8;
	SpiChnPutC(chn,0);
	oneByte & SpiChnGetC(chn);
	
	PmodSPISetSSHigh(chn); //SS to High
	BufflibWriteBuffer(oneByte);
	
}
unsigned char fnTimer1Setup ()
{
	// Open Timer1
	OpenTimer1 (T1_ON | T1_IDLE_CON | T1_PS_1_1, (pbClock/TICKRATE));
    
    // Configure interrupt for Timer1
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_7);
    
    return 1;
}


void __ISR(_TIMER_1_VECTOR, ipl7) fnTimer1Int(void)
{
	PmodMicTakeSample();
	// Clear the timer interrupt and call our handler function
	mT1ClearIntFlag ();
	
}

void configure_interrupts (void)
{
	// Enable interrupts
	INTEnableSystemMultiVectoredInt ();
	INTEnableInterrupts ();
}