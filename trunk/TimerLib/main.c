#include <peripheral/ports.h>
#include <peripheral/int.h>

#include "TimerLib.h"

// Determine clock speed and other system settings
#pragma config FNOSC		= PRIPLL	// Oscillator selection
#pragma config FPLLIDIV 	= DIV_2		// PLL input divider
#pragma config FPLLMUL		= MUL_20	// PLL multiplier
#pragma config FPLLODIV 	= DIV_1		// PLL output divider
#pragma config FPBDIV		= DIV_8 	// Peripheral bus clock divider

// Function Prototypes
void setup_ports (void);
void configure_interrupts (void);

int main (void)
{
	setup_ports ();
	configure_interrupts();

	// Setup a timer to trigger every 500 ms
	fnTimer1Setup(500);

	while (1)
	{
		// Do Nothing
	}

	return 0;
}

void fnTimer1IntHandler(void)
{
	// Toggle LED1 every interrupt
	PORTToggleBits(IOPORT_B,BIT_10);
}

void setup_ports (void)
{
	// Check Cerebot32MX4 ref manual for pin mappings
	// Establish pins connected to LED1 as output pins
	// LD1 ==> PB10
	PORTSetPinsDigitalOut (IOPORT_B, BIT_10);
	PORTClearBits (IOPORT_B,  BIT_10);
}

void configure_interrupts (void)
{
	// Enable interrupts
	INTEnableSystemMultiVectoredInt ();
	INTEnableInterrupts ();
}
