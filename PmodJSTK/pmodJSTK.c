/************************************************************************/
/*                                                                      */
/*  pmodJSTK.c test driver Implimentation file for PMODJSTK             */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  10/13/2011(RyanH):                                                  */
/*                                                                      */
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include "pmodJSTK.h"

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */

//feature set definitions used in conditional compiling for processor type
#define PIC32_300_400_SERIES ((__PIC32_FEATURE_SET__ >= 300) && (__PIC32_FEATURE_SET__ <= 499))
#define PIC32_500_600_700_SERIES ((__PIC32_FEATURE_SET__ >= 500) && (__PIC32_FEATURE_SET__ <= 799))

typedef struct
{
     uint32_t portSS; //IO Port
     uint32_t ssMask; //Slave Select bit mask
}SpiPortSS;


/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

//SpiPortSS contains IO Port/SS bit mask mappings for
//driving SS low and high
static const SpiPortSS SpiIO[] = {
	{0,0},  // SPI 0 is invalid
#if (PIC32_300_400_SERIES) 
	{IOPORT_D,BIT_9}, //SPI1
	{IOPORT_G,BIT_9}  //SPI2
#elif (PIC32_500_600_700_SERIES)
	{IOPORT_D,BIT_9}, //SPI1
	{IOPORT_G,BIT_9},//SPI2/SPI2A
    {IOPORT_D,BIT_14},//SPI1A/SPI3
	{IOPORT_F,BIT_12} //SPI3A/SPI4
#endif
};

static unsigned int NumCycles10us = 0;
static unsigned int NumCycles15us = 0;


/*  PmodJSTKInit
**
**	Synopsis:
**  Initializes the PmodJSTK module on the selected SPI channel. 
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
**  and sets the desired bit rate as a function of pbClock/bitRate.  Maximum recomended 
**  bitrate is 1Mhz.
*/
void PmodJSTKInit(SpiChannel chn,uint32_t pbClock,uint32_t bitRate,uint32_t systemClock)
{
    SpiChnOpen(chn, SPI_OPEN_MSTEN | SPI_OPEN_SSEN |  SPI_OPEN_MODE8 | SPI_OPEN_CKP_HIGH, pbClock/bitRate);
	PmodJSTKSetSSHigh(chn);
	//calculate number of processor cycles at the current system clock frequence for 10 and 15 microsecond delays
	//(Desired Delay)/(Time Elapsed for 1 System Clock Cycle)
	NumCycles10us = ceil((10.0 * .0000001)/((1.0/(double)systemClock)));
	NumCycles15us = ceil((15.0 * .0000001)/((1.0/(double)systemClock)));
}

/*  PmodJSTKSetSSLow
**
**	Synopsis:
**  Sets the slave select bit on the selected SPI channel to low
**  
**  Input: SpiChannel chn  - spi channel to set SS bit low on
**
**  Returns: none
*/
void PmodJSTKSetSSLow(SpiChannel chn)
{
	PORTClearBits(SpiIO[chn].portSS,SpiIO[chn].ssMask);
}

/*  PmodJSTKSetSSHigh
**
**	Synopsis:
**  Sets the slave select bit on the selected SPI channel to high
**
**  Input: SpiChannel chn  - spi channel to set SS bit high on
**
**  Returns: none
**
**	Errors:	none
*/
void PmodJSTKSetSSHigh(SpiChannel chn)
{
	PORTSetBits(SpiIO[chn].portSS,SpiIO[chn].ssMask);
}

/*  
**  PmodJSTKSendRecv
**
**	Synopsis:
**	Sends a LED illumination command to the PmodJSTK and recieves the 
**  position of the joystick on its x and y axis and the state of 
**  buttons 1,2,3
**  Input: 
**		SpiChannel chn - spi channel
**		uint8_t cmdIn - button LED command
**      PmodJSTKAxisButton *jystkAxisButtons - pointer to struct containing
**                                             data returned by the joystick.
**
**  Returns: none
**  
**	Errors:	none
**
**  Description:
**  This function will operate on the channel specified in parameter chn.
**  Parameter cmdIn will accept the following values and have the following effect
**  on the PmodJSTK:
**	PMODJSTK_LED_OFF: all LEDs off
**	PMODJSTK_LED1_ON: LD1 on
**	PMODJSTK_LED2_ON: LD2 on
**	PMODJSTK_LED1_LED2_ON: LD1 and LD2 on
**  Parameter *jystkAxisButtons is a pointer to a PmodJSTKAxisButton struct which 
**  will be filled as specified below:
**	xAxis: 16 bit field holding the joysitick position on the x axis (between 0 and 1023)
**  yAxis: 16 bit field holding the joysitick position on the y axis (between 0 and 1023)
**  buttonState: 8 bit field holding the state of the buttons, use the following bit masks to
**  determine button status: PMODJSTK_BTN1,PMODJSTK_BTN2. 
**              
*/

void PmodJSTKSendRecv(SpiChannel chn,uint8_t cmdIn,PmodJSTKAxisButton *jystkAxisButtons)
{
	int byteNum = 0;
	uint8_t jystkOut[PMODJSTK_BYTES_PER_XFER];

	PmodJSTKSetSSLow(chn);
	PmodJSTKDelay15us();

    for(byteNum = 0;byteNum < PMODJSTK_BYTES_PER_XFER;byteNum++)
	{
		SpiChnPutC(chn,cmdIn);
		jystkOut[byteNum] = SpiChnGetC(chn);	
		PmodJSTKDelay10us();
	}

	PmodJSTKSetSSHigh(chn);

	jystkAxisButtons->xAxis = ((uint16_t)jystkOut[1] << 8) | (uint16_t)jystkOut[0];
	jystkAxisButtons->yAxis = ((uint16_t)jystkOut[3] << 8) | (uint16_t)jystkOut[2];
	jystkAxisButtons->buttonState = jystkOut[4];
}

/*  
**
**	Synopsis:
**
**  Input: 
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
*/
void PmodJSTKDelay10us()
{   uint32_t cnt = 0;
    for (cnt = 0; cnt < NumCycles10us; cnt+=12);
}
/*  
**
**	Synopsis:
**
**  Input: 
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
*/
void PmodJSTKDelay15us()
{   uint32_t cnt = 0;
    for (cnt = 0; cnt < NumCycles15us; cnt+=12);
}
