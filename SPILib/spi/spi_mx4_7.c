/************************************************************************/
/*                                                                      */
/*   spi_mx4_7.c  -- Wrappers for SPI Microchip PIC32 library functions	*/
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  This file contains function definitions used in wrapping the        */
/*  Microchip PIC32 Serial Peripheral interface libraries for use with  */
/*  the Digilent CEREBOT32MX4 and CEREBOT32MX7                          */
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*	2/19/2011(RyanH): 
/*                                                                      */
/************************************************************************/

#include "spi_mx4_7.h"

/* ------------------------------------------------------------ */
/***	fnSPIConfigureChannelMasterNoFrames	
**
**	Synopsis:
**	fnSPIConfigureChannelMasterNoFrame(ulClock,uSpiNum,uBitMode,uSckFreq)
**
**	Input:
**        ulClock - peripheral bus clock frequency in Hz
**        uSpiNum - spi channel number 
**        uBitMode - transfer mode
**        uSckFreq - Sck frequency in MHz, see PIC32 Family Reference Manual, Sect. 23 Serial Peripheral Interface for additional information
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This function configures SPI on the Digilent CEREBOT32MX4 to run in standard, master mode, no framing, 
**      with the clock edge reversed (transmit from active to idle). Digital pin IO is setup according to the 
**      channel selected, see the Digilent CEREBOT32MX4 or CEREBOT32MX7 reference manual for Pmod Port mappings. SCKx, SDIx, 
**      and SDOx,and SSx are included in the digital pin IO configuration. Number of bits transmitter per clock pulse can be set
**      with uBitMode, additioanlly the Sck frequency (baud rate) can be set with uSckFreq. This configuation is typically
**	    used when transmitting to an external device such as an LCD display. 
**      Example config with PmodCLS:  fnSPIConfigureChannelMasterNoFrames(80000000,SPI_C2,SPI_8BIT_MODE,156250);
*/
void fnSPIConfigureChannelMasterNoFrames (unsigned long ulClock,SPICHANNEL spiChannel, SPIOPENMODE spiOpenMode,unsigned int uSckFreq)
{

 	unsigned int uPbClk;

	//master mode, clock edge reversed
    unsigned int uConfig = SPI_CONFIG_MSTEN|SPI_CONFIG_CKE_REV;

	switch(spiOpenMode)
	{
		case SPI_32BIT_MODE:
			uConfig |= SPI_CONFIG_MODE32;
			break;
		case SPI_16BIT_MODE:
			uConfig |= SPI_CONFIG_MODE16;
			break;
		case SPI_8BIT_MODE:
			uConfig |= SPI_CONFIG_MODE8;
			break;
		default:
			uConfig |= SPI_CONFIG_MODE32;
			break;
	}
    
	//get the peripheral bus clack
	uPbClk = SYSTEMConfigPerformance (ulClock); 

	//Enable digital IO for Cerebot32MX4
	fnSPIEnableDigitalPinIO(spiChannel);
	
    if(spiChannel == SPI_C1)//Configure for SPI_C1
	{	
		//disable SPI channel 1
		SpiChnEnable(SPI_CHANNEL1, 0);
		//configure and open SPI channel 1
		SpiChnConfigure(SPI_CHANNEL1, uConfig );
		//set the baud rate for SPI channel 1
		SpiChnSetBrg(SPI_CHANNEL1,SpiBrgVal(uPbClk, uSckFreq));
		//enable SPI channel 1
		SpiChnEnable(SPI_CHANNEL1, 1);
	}
#if defined __CEREBOT32MX4__
	else
#elif defined __CEREBOT32MX7__
	else if (spiChannel == SPI_C2) //Configure for SPI_C2
#endif
	{
		//disable SPI channel 2
		SpiChnEnable(SPI_CHANNEL2, 0);
		//configure and open SPI channel 2
		SpiChnConfigure(SPI_CHANNEL2, uConfig );
		//set the baud rate for SPI channel 2
		SpiChnSetBrg(SPI_CHANNEL2,SpiBrgVal(uPbClk, uSckFreq));
		//enable SPI channel 2
		SpiChnEnable(SPI_CHANNEL2, 1);
	}
#if defined __CEREBOT32MX7__
//TODO: MX7 code goes here
#endif	 

}

/* ------------------------------------------------------------ */
/***	fnSPIEnableDigitalPinIO
**
**	Synopsis:
**  fnSPIEnableDigitalPinIO(uSpiNum)
**
**	Input:
**		uSpiNum - SPI channel number
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
**      Example: fnSPIEnableDigitalPinIO(SPI_C2)
*/
void fnSPIEnableDigitalPinIO(SPICHANNEL spiChannel)
{
#if defined __CEREBOT32MX4__
	if(spiChannel == SPI_C2)
	{
                                               //     | MX4 PmodPort/Pin 
                                               //------------------------
		PORTSetPinsDigitalOut(IOPORT_G,BIT_6); //SCK2 |        (JB04)
		PORTSetPinsDigitalIn(IOPORT_G,BIT_7);  //SDI2 |        (JB03)
		PORTSetPinsDigitalOut(IOPORT_G,BIT_8); //SD02 |        (JB02)
		PORTSetPinsDigitalOut(IOPORT_G,BIT_9); //SS2  |        (JB01)
	}
	else
	{
                                                //     | MX4 PmodPort/Pin 
                                                //------------------------
		PORTSetPinsDigitalOut(IOPORT_D,BIT_10); //SCK1 |       (JD09)
		PORTSetPinsDigitalIn(IOPORT_C,BIT_4);   //SDI1 |       (JK10)
		PORTSetPinsDigitalOut(IOPORT_D,BIT_0);  //SD01 |       (JH08)
		PORTSetPinsDigitalOut(IOPORT_D,BIT_9);  //SS1  |       (JD02)
	}
#endif

#if defined __CEREBOT32MX7__
//TODO: MX7 code goes here

#endif
}

/* ------------------------------------------------------------ */
/***	fnSPIEnableDigitalPinIO
**
**	Synopsis:
**
**	Input:
**
**	Output:
**      none
**
**	Errors:
**		none
**
**	Description:
*/
void fnSPIEnableInterrupts(void)
{

//TODO: Check for multivector interrupts enabled (if possible)
	if(useSPI_CHANNEL_1_INTERRUPT)
	{
		INTEnable(INT_SOURCE_SPI(INT_SPI1), INT_ENABLED);
		INTSetVectorPriority(INT_SOURCE_SPI(INT_SPI1),configSPI_CHANNEL_1_INTERRUPT_PRIORITY);
    	INTSetVectorSubPriority(INT_SOURCE_SPI(INT_SPI1), configSPI_CHANNEL_1_INTERRUPT_SUB_PRIORITY);
	}

	if(useSPI_CHANNEL_2_INTERRUPT)
	{
		INTEnable(INT_SOURCE_SPI(INT_SPI2), INT_ENABLED);
		INTSetVectorPriority(INT_SOURCE_SPI(INT_SPI2),configSPI_CHANNEL_2_INTERRUPT_PRIORITY);
    	INTSetVectorSubPriority(INT_SOURCE_SPI(INT_SPI2), configSPI_CHANNEL_2_INTERRUPT_SUB_PRIORITY);
	}

}