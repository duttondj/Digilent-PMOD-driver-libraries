// SYSCLK = 80 MHz (8 MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 40 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF

#define TIMER_2
#define TIMER_1
#define SYSTEM_CLOCK		80000000
#define DESIRED_BAUD_RATE	9600
#pragma config FNOSC		= PRIPLL	// Oscillator selection
#pragma config FPLLIDIV 	= DIV_2		// PLL input divider
#pragma config FPLLMUL		= MUL_20	// PLL multiplier
#pragma config FPLLODIV 	= DIV_1		// PLL output divider
#pragma config FPBDIV		= DIV_8		// Peripheral bus clock divider

#include "TimerLib.c"

void setup_ports (void);
void configure_interrupts (void);

int main (void)
{
	unsigned int timer_value = 0;

	setup_ports ();
	configure_interrupts();

	fnTimer1(0xFFFF);
	//fnTimer2(0xFFFF);

	while (1)
	{
		// Do Nothing
	}

	return 0;
}


void setup_ports (void)
{
	// Check Cerebot32MX4 ref manual for pin mappings
	// Establish pins connected to LEDs as output pins
	// LD1 ==> PB10
	// LD2 ==> PB11
	// LD3 ==> PB12
	// LD4 ==> PB13
	PORTSetPinsDigitalOut (IOPORT_B, BIT_10 |  BIT_11 |  BIT_12 |  BIT_13);
	PORTClearBits (IOPORT_B,  BIT_10 |  BIT_11 |  BIT_12 |  BIT_13);

}

void __ISR(_TIMER_1_VECTOR, ipl7) Timer1Handler(void)
{
	// Flash LD1 on each interrupt call
	PORTToggleBits (IOPORT_B, BIT_10);
	mT1ClearIntFlag ();
}

void __ISR(_TIMER_2_VECTOR, ipl7) Timer2Handler(void)
{
	// Flash LD2 on each interrupt call
	PORTToggleBits (IOPORT_B, BIT_11);
	mT1ClearIntFlag ();
}

void configure_interrupts (void)
{
	INTEnableSystemMultiVectoredInt ();
	INTEnableInterrupts ();
}
