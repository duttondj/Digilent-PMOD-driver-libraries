#include "setup.h"

uint8_t RecorderInit(uint8_t BufferCount, uint16_t BufferSize)
{
	// Attempt to allocate buffers
	if(!BufLibInitBuffers(BufferCount, BufferSize))
	{
		// If failed, return 0
		return 0;
	}

	// bugfix for SPI 1, you can remove to test if its needed anymore
	// IMPORTANT! Sound will not record without this at the moment!
	PORTSetPinsDigitalOut(IOPORT_D,BIT_0|BIT_9|BIT_10);

	// Initalize the PmodDA2
	PmodDA2Init(PMOD_DA2_SPI,PB_CLOCK,PMOD_DA2_BITRATE);
	
	// Initialize the PmodMIC
	PmodMicInit(PMOD_MIC_SPI,PB_CLOCK,PMOD_MIC_BITRATE);

	// Enable Interrupt Vectors
	INTEnableSystemMultiVectoredInt ();

	// Setup Timer1 for PmodDA2
	// Period of 4000 gives a 10k sample rate
	OpenTimer1 (T1_ON | T1_IDLE_CON | T1_PS_1_1, 4000);

    // Configure interrupt for Timer1
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_7);

	// Setup Timer2 for PmodMIC
	// Period of 4000 gives a 10k sample rate
	OpenTimer2 (T2_ON | T2_IDLE_CON | T2_PS_1_1, 4000);
    
	// Configure interrupt for Timer2
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_7);

	// Configure BTN1 and BTN2 for use
	PORTSetPinsDigitalIn (IOPORT_A, BIT_6 | BIT_7);
	
	// Start with the timer interrupts disabled
	mT1IntEnable(0);
	mT2IntEnable(0);

	return 1;
}

uint8_t HandleBTN1(uint8_t action)
{
	// If the action is true, disable the timer interrupt
	if(action)
	{
		mT2IntEnable(0);
		return 0;
	}
	else // else, enable the timer interrupt
	{
		mT2IntEnable(1);
		return 1;
	}
}

uint8_t HandleBTN2(uint8_t action)
{
	// If the action is true, disable the timer interrupt
	if(action)
	{
		mT1IntEnable(0);
		return 0;
	}
	else // else, enable the timer interrupt
	{
		mT1IntEnable(1);
		return 1;
	}
}

void __ISR(_TIMER_1_VECTOR, ipl7) fnTimer1Int(void)
{
	// Clear the timer interrupt
	mT1ClearIntFlag ();
	
	// If there is valid sound to play, play it
	if(BufLibReadBuffer((uint16_t*)&OutputSound))
	{
		PmodDA2Send(PMOD_DA2_SPI, OutputSound);
	}
}

void __ISR(_TIMER_2_VECTOR, ipl7) fnTimer2Int(void)
{
	// Clear the timer interrupt
	mT2ClearIntFlag ();
	
	// Poll the PmodMIC for sound data
	InputSound = PmodMicGetData(PMOD_MIC_SPI);
	
	// Put it into the buffer
	BufLibWriteBuffer(InputSound);
}