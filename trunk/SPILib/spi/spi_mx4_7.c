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

/* ------------------------------------------------------------ 
**	fnSPIConfigureChannelMasterNoFrames	
**
**	Synopsis:
**	fnSPIConfigureChannelMasterNoFrame(ulClock,spiChannel,spiOpenMode,uSckFreq)
**
**	Input:
**        ulClock - peripheral bus clock frequency in Hz
**        spiChannel - spi channel number 
**        spiOpenMode - transfer mode
**        uSckFreq - Sck frequency in MHz, see PIC32 Family Reference Manual, Sect. 23 Serial Peripheral Interface for additional information
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This function configures SPI on the Digilent CEREBOT32MX4 to run in standard, master mode, no framing.
**      Digital pin IO is setup according to the 
**      channel selected, see the Digilent CEREBOT32MX4 or CEREBOT32MX7 reference manual for Pmod Port mappings. SCKx, SDIx, 
**      and SDOx,and SSx are included in the digital pin IO configuration. Number of bits transmitter per clock pulse can be set
**      with spiOpenMode, additioanlly the Sck frequency (baud rate) can be set with uSckFreq. 
**      Example config:  fnSPIConfigureChannelMasterNoFrames(80000000,SPI_C2,SPI_8BIT_MODE,156250);
*/
void fnSPIConfigureChannelMasterNoFrames (uint32_t ulClock,SPICHANNEL spiChannel, SPIOPENMODE spiOpenMode,uint16_t uSckFreq )
{
    uint16_t uConfig = SPI_CONFIG_MSTEN ;
	fnOpenSPI(uConfig,ulClock,spiChannel,spiOpenMode,uSckFreq);
}


/* ------------------------------------------------------------ */
/***	fnSPIConfigureChannelMasterNoFramesCERev	
**
**	Synopsis:
**	fnSPIConfigureChannelMasterNoFrame(ulClock,spiChannel,spiOpenMode,uSckFreq)
**
**	Input:
**        ulClock - peripheral bus clock frequency in Hz
**        spiChannel - spi channel number 
**        spiOpenMode - transfer mode
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
**      with spiOpenMode, additioanlly the Sck frequency (baud rate) can be set with uSckFreq. This configuation is typically
**	    used when transmitting to an external device such as an LCD display. 
**      Example config with PmodCLS:  fnSPIConfigureChannelMasterNoFramesCERev(80000000,SPI_C2,SPI_8BIT_MODE,156250);
*/
void fnSPIConfigureChannelMasterNoFramesCERev (uint32_t ulClock,SPICHANNEL spiChannel, SPIOPENMODE spiOpenMode,uint16_t uSckFreq )
{
    uint16_t uConfig = SPI_CONFIG_MSTEN | SPI_CONFIG_CKE_REV  ;
	fnOpenSPI(uConfig,ulClock,spiChannel,spiOpenMode,uSckFreq);
}

/* ------------------------------------------------------------ 
**  fnSPIConfigureChannelMasterWithFrames
**
**	Synopsis:
**	fnSPIConfigureChannelMasterWithFrames(ulClock,spiChannel,spiOpenMode,uSckFreq)
**
**	Input:
**        ulClock - peripheral bus clock frequency in Hz
**        spiChannel - spi channel number 
**        spiOpenMode - transfer mode
**        uSckFreq - Sck frequency in MHz, see PIC32 Family Reference Manual, Sect. 23 Serial Peripheral Interface for additional information
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This function configures SPI on the Digilent CEREBOT32MX4 to run in standard, master mode, with framing.
**      Digital pin IO is setup according to the 
**      channel selected, see the Digilent CEREBOT32MX4 or CEREBOT32MX7 reference manual for Pmod Port mappings. SCKx, SDIx, 
**      and SDOx,and SSx are included in the digital pin IO configuration. Number of bits transmitter per clock pulse can be set
**      with spiOpenMode, additioanlly the Sck frequency (baud rate) can be set with uSckFreq. 
**      Example config:  fnSPIConfigureChannelMasterWithFrames(80000000,SPI_C2,SPI_8BIT_MODE,156250);
*/
void fnSPIConfigureChannelMasterWithFrames (uint32_t ulClock,SPICHANNEL spiChannel, SPIOPENMODE spiOpenMode,uint16_t uSckFreq )
{
    uint16_t uConfig = SPI_CONFIG_MSTEN | SPI_CONFIG_FRMEN;
	fnOpenSPI(uConfig,ulClock,spiChannel,spiOpenMode,uSckFreq);
}
/* ------------------------------------------------------------ 
**  fnSPIConfigureChannelSlaveWithFrames
**
**	Synopsis:
**	fnSPIConfigureChannelSlaveWithFrames (SPICHANNEL spiChannel, SPIOPENMODE spiOpenMode)
**
**	Input:
**        spiChannel- spi channel number 
**        spiOpenMode - transfer mode
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This function configures SPI on the Digilent CEREBOT32MX4 to run in standard, slave mode, with framing.
**      Digital pin IO is setup according to the channel selected, see the Digilent CEREBOT32MX4 or CEREBOT32MX7 
**      reference manual for Pmod Port mappings. SCKx, SDIx, and SDOx,and SSx are included in the digital pin IO 
**      configuration. Number of bits transmitter per clock pulse can be set with spiOpenMode.
**      Example config:  fnSPIConfigureChannelSlaveWithFrames(SPI_C2,SPI_8BIT_MODE,156250);
*/
void fnSPIConfigureChannelSlaveWithFrames (SPICHANNEL spiChannel, SPIOPENMODE spiOpenMode)
{

    uint16_t uConfig = SPI_CONFIG_SLVEN|SPI_CONFIG_FRMEN | SPI_CONFIG_FSP_IN; 
	fnOpenSPI(uConfig,0,spiChannel,spiOpenMode,0);

}
/* ------------------------------------------------------------ 
**  fnSPIConfigureChannelSlaveNoFrames
**
**	Synopsis:
**	fnSPIConfigureChannelSlaveWithFrames (SPICHANNEL spiChannel, SPIOPENMODE spiOpenMode)
**
**	Input:
**        spiChannel- spi channel number 
**        spiOpenMode - transfer mode
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This function configures SPI on the Digilent CEREBOT32MX4 to run in standard, slave mode, without framing.
**      Digital pin IO is setup according to the channel selected, see the Digilent CEREBOT32MX4 or CEREBOT32MX7 
**      reference manual for Pmod Port mappings. SCKx, SDIx, and SDOx,and SSx are included in the digital pin IO 
**      configuration. Number of bits transmitter per clock pulse can be set with spiOpenMode.
**      Example config:  fnSPIConfigureChannelSlaveNoFrames(SPI_C2,SPI_8BIT_MODE,156250);
** 
*/
void fnSPIConfigureChannelSlaveNoFrames (SPICHANNEL spiChannel, SPIOPENMODE spiOpenMode)
{

    uint16_t uConfig = SPI_CONFIG_SLVEN; 
	fnOpenSPI(uConfig,0,spiChannel,spiOpenMode,0);

}
/* ------------------------------------------------------------ 
**	fnOpenSPI
**
**	Synopsis:
**   fnOpenSPI(uint16_t uConfig,uint32_t ulClock,SPICHANNEL spiChannel, SPIOPENMODE spiOpenMode,uint16_t uSckFreq )
**	Input:
**        uConfig- configuration bits for opening SPI channel, See Microchip PIC32MX Peripheral Library-> SPI->SPI Types->SpiConfigFlagsEnumeration for details
**        ulClock - peripheral bus clock frequency in Hz
**        spiChannel - spi channel number 
**        spiOpenMode - transfer mode
**        uSckFreq - Sck frequency in MHz, see PIC32 Family Reference Manual, Sect. 23 Serial Peripheral Interface for additional information
**
**
**	Output:
**      none
**
**	Errors:
**		none
**
**	Description:
** 		This function configures SPI on the Digilent CEREBOT32MX4 Master/Slave mode based on the options passed in in uConfig
**      as well as uClock,spiChannel,spiOpenMode and uSckFreq.
**      Digital pin IO is setup according to the channel selected, see the Digilent CEREBOT32MX4 or CEREBOT32MX7 
**      reference manual for Pmod Port mappings. SCKx, SDIx, and SDOx,and SSx are included in the digital pin IO 
**      configuration. Number of bits transmitter per clock pulse can be set with spiOpenMode.
**      Example config:  fnOpenSPI(SPI_CONFIG_MSTEN | SPI_CONFIG_FRMEN,periperalBusClockRate,SPI_C2,SPI_8BIT_MODE,156250);
*/
void fnOpenSPI(uint16_t uConfig,uint32_t ulClock,SPICHANNEL spiChannel, SPIOPENMODE spiOpenMode,uint16_t uSckFreq )
{

    uint16_t uPbClk;

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
    if(ulClock > 0) //clock value of zero indicates slave
	{
		//get the peripheral bus clack
		uPbClk = SYSTEMConfigPerformance (ulClock); 
	}
	//Enable digital IO for Cerebot32MX4
	fnSPIEnableDigitalPinIO(spiChannel);
	
    if(spiChannel == SPI_C1)//Configure for SPI_C1
	{	
		//disable SPI channel 1
		SpiChnEnable(SPI_CHANNEL1, 0);
		//configure and open SPI channel 1
		SpiChnConfigure(SPI_CHANNEL1, uConfig );
		//set the baud rate for SPI channel 1
		if(ulClock > 0) //clock value of zero indicates slave
		{
			SpiChnSetBrg(SPI_CHANNEL1,SpiBrgVal(uPbClk, uSckFreq));
		}
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
		if(ulClock > 0) //clock value of zero indicates slave
		{
			SpiChnSetBrg(SPI_CHANNEL2,SpiBrgVal(uPbClk, uSckFreq));
		}
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
**  fnSPIEnableDigitalPinIO(spiChannel)
**
**	Input:
**		spiChannel - SPI channel number
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
/***	fnSPIEnableInterrupts
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
** This functionality is not totally defined yet, TODO in 
** future release
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


/* ------------------------------------------------------------ */
/*** fnSPISetSSLow
**
**	Synopsis:
**   fnSPISetSSLow(SPICHANNEL chn)
**	Input:
**      chn - SPI channel to drive SS low
**	Output:
**      none
**
**	Errors:
**		none
**
**	Description:
** Drives the SS pin low on the requested channel
*/
void fnSPISetSSLow(SPICHANNEL chn)
{
	if(chn == SPI_C1)
	{
		PORTClearBits (IOPORT_D,BIT_9);
	}
	else if(chn == SPI_C2)
	{
		PORTClearBits (IOPORT_G, BIT_9);
	}
#if defined __CEREBOT32MX7__
//TODO: MX7 code goes here

#endif
}
/* ------------------------------------------------------------ */
/*** fnSPISetSSHigh
**
**	Synopsis:
**   fnSPISetSSHigh(SPICHANNEL chn)
**	Input:
**      chn - SPI channel to drive SS high
**	Output:
**      none
**
**	Errors:
**		none
**
**	Description:
** Drives the SS pin high on the requested channel
*/
void fnSPISetSSHigh(SPICHANNEL chn)
{
	if(chn == SPI_C1)
	{
		PORTSetBits (IOPORT_D,BIT_9);
	}
	else if(chn == SPI_C2)
	{
		PORTSetBits (IOPORT_G, BIT_9);
	}
#if defined __CEREBOT32MX7__
//TODO: MX7 code goes here

#endif
}
/* ------------------------------------------------------------ */
/*	getSPIRcvBufStatus
**
**	Synopsis:
**      uint8_t getSPIRcvBufStatus(SPICHANNEL chn)
**	Input:
**       chn -SPI channel to check RX buffer full status on 
**	Output:
**      1 = Receive buffer, SPIxRXB is full
**      0 = Receive buffer, SPIxRXB is not full
**
**	Errors:
**		-1 returned on invlalid channel
**
**	Description:
**  Checks the SPI Receive Buffer Full Status bit on 
**  the specified channel
*/
uint8_t getSPIRcvBufStatus(SPICHANNEL chn)
{
	if(chn == SPI_C1)
	{
		return SPI1STATbits.SPIRBF;
	}
	else if(chn == SPI_C2)
	{
		return SPI2STATbits.SPIRBF;
	}
#if defined __CEREBOT32MX7__
//TODO: MX7 code goes here

#endif
    return -1;
}


/* ------------------------------------------------------------ */
/*	 fnSPIputString
**
**	Synopsis:
**      void fnSPIputString(SPICHANNEL chn,char* string)
**	Input:
**       chn -SPI channel to send the string to
         string - null terminated string to send to SPI channel 
**	Output:
**     none
**
**	Errors:
**		none
**
**	Description:
**  Transmits a null terminated string to the specified SPI Chanel
*/
void fnSPIputString(SPICHANNEL chn,char* string)
{
 	uint8_t ucGarbage;
	uint8_t *spiBuf = SpiChnBuffer(chn);
 	char* stringPtr = string;
     while(*stringPtr != '\0')
     {
		*spiBuf = *stringPtr;
		while(!getSPIRcvBufStatus(chn));   //Wait for transfer complete
    	ucGarbage = *spiBuf;                       //byte will be garbage
		stringPtr++;
	}
}
