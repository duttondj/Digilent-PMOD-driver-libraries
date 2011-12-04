#include <stdint.h>
#include <plib.h>
#include "./PmodMic/PmodMic.h"
#include <peripheral/ports.h>


/* ------------------------------------------------------------*/
/*          Main test loop for PMODMIC             */
/* ------------------------------------------------------------*/
int main(void)
{
	uint8_t channel = 0; //spi channel selection
	//setup onboard led
	PORTSetPinsDigitalOut (IOPORT_B, BIT_10 | BIT_11 | BIT_12 | BIT_13);
	PORTClearBits (IOPORT_B,  BIT_10 |  BIT_11 |  BIT_12 |  BIT_13);
	//PmodMicStartRecording();

	//Main test loop
	while(1)
	{
		PORTWrite (IOPORT_B, BIT_10 | BIT_12);
		delay(1000);
		PORTClearBits(IOPORT_B, BIT_10 | BIT_12);
     		
	}
	
	return 0;
}
