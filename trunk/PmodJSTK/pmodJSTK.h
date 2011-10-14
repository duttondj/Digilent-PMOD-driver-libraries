#ifndef _PMODJSTK_H_
#define _PMODJSTK_H_

#include <peripheral/spi.h>
#include <peripheral/ports.h>
#include <math.h>
#include <stdint.h>

#define PMODJSTK_LED_OFF 0x80
#define PMODJSTK_LED1_ON 0x81
#define PMODJSTK_LED2_ON 0x82
#define PMODJSTK_LED1_LED2_ON 0x83

#define PMODJSTK_BUTTON_JSTK 0x1
#define PMODJSTK_BUTTON_1 0x2
#define PMODJSTK_BUTTON_2 0x4

#define PMODJSTK_MAX_X_Y_AXIS 1023
#define PMODJSTK_MIN_X_Y_AXIS 0

#define PMODJSTK_BYTES_PER_XFER 5


typedef struct 
{
	uint16_t xAxis;
	uint16_t yAxis;
	uint8_t buttonStatus;
}PmodJSTKAxisButton;


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
void PmodJSTKInit(SpiChannel chn,uint32_t pbClock,uint32_t bitRate,uint32_t systemClock);

/*  PmodJSTKSetSSLow
**
**	Synopsis:
**  Sets the slave select bit on the selected SPI channel to low
**  
**  Input: SpiChannel chn  - spi channel to set SS bit low on
**
**  Returns: none
*/
void PmodJSTKSetSSLow(SpiChannel chn);

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
void PmodJSTKSetSSHigh(SpiChannel chn);


void PmodJSTKSendRecv(SpiChannel chn,uint8_t cmdIn,PmodJSTKAxisButton *jystkAxisButtons);

void PmodJSTKDelay10us();
void PmodJSTKDelay15us();

#endif
