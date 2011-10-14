#include "pmodJSTK.h"

//feature set definitions used in conditional compiling for processor type
#define PIC32_300_400_SERIES ((__PIC32_FEATURE_SET__ >= 300) && (__PIC32_FEATURE_SET__ <= 499))
#define PIC32_500_600_700_SERIES ((__PIC32_FEATURE_SET__ >= 500) && (__PIC32_FEATURE_SET__ <= 799))

static unsigned int NumCycles10us = 0;
static unsigned int NumCycles15us = 0;

typedef struct
{
     uint32_t portSS; //IO Port
     uint32_t ssMask; //Slave Select bit mask
}SpiPortSS;

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

	jystkAxisButtons->xAxis = (jystkOut[1] << 8) | jystkOut[0];
	jystkAxisButtons->yAxis = (jystkOut[3] << 8) | jystkOut[2];
	jystkAxisButtons->buttonStatus = jystkOut[4];
}


void PmodJSTKDelay10us()
{   uint32_t cnt = 0;
    for (cnt = 0; cnt < NumCycles10us; cnt+=12);
}

void PmodJSTKDelay15us()
{   uint32_t cnt = 0;
    for (cnt = 0; cnt < NumCycles15us; cnt+=12);
}
