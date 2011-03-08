
#include <plib.h>
#include <stdio.h>


 #pragma config  FPLLODIV = DIV_1    // PLL Output Divider Value:
 #pragma config  FPLLMUL  = MUL_18   // PLL Multiplier bits:
 #pragma config  FPLLIDIV = DIV_2    // PLL Input Divider bits:
 #pragma config  FPBDIV   = DIV_2    // Bootup PBCLK divider: SYSCLK/this setting
 #pragma config  POSCMOD  = XT       //Primary Oscillator bits:
 #pragma config  FNOSC    = PRIPLL   // Oscillator Selection bits:
 #pragma config  FCKSM    = CSECMD   // Clock Switching and Monitor Selection bits:
 #pragma config  OSCIOFNC = OFF      //CLKO Enable bit:
 #pragma config  IESO     = OFF      //Internal External Switch Over bit:
 #pragma config  FSOSCEN  = OFF      // Secondary oscillator Enable bit:
 #pragma config  FWDTEN   = OFF      // Watchdog Timer Enable bit:
 #pragma config  WDTPS    = PS1      // Watchdog Timer Postscale Select bits:
 #pragma config  CP       = OFF      // Code Protect Enable bit:
 #pragma config  BWP      = OFF       // Boot Flash Write Protect bit:
 #pragma config  PWP      = OFF      // Program Flash Write Protect:
 #pragma config  ICESEL   = ICS_PGx2 // ICE/ICD Comm Channel Select:
 #pragma config  DEBUG    = ON       //  Background Debugger Enable bit:

#define GetPeripheralClock() 8000000
void __ISR(_I2C_1_VECTOR, IPL2SOFT) I2C1ServiceRoutine(void)
{
	
}

//#define I2C1_Initialize
int I2C_Initialize( I2C_MODULE id, UINT32 i2cClock, UINT16 address )
{
	// Configure Various I2C Options
	I2CConfigure(id, I2C_ENABLE_SLAVE_CLOCK_STRETCHING);
	
	// Set Desired Operation Frequency
	int actualClock = I2CSetFrequency(id, GetPeripheralClock(), i2cClock);
	if ( abs(actualClock-i2cClock) > i2cClock/10 )
	{
	    DBPRINTF("  Clock frequency (%d) error exceeds 10\%\r\n", actualClock);
	}
	else
	{
	    DBPRINTF("  Clock frequency = %ld Hz\n", actualClock);
	}
	
	//  Set the Slave Address
	if( address != NULL)
	{
		I2CSetSlaveAddress(id, address, 0, I2C_USE_7BIT_ADDRESS);
		DBPRINTF("  Slave address = 0x%02X\n", MY_SLAVE_ADDRESS);
	
	}
	else
	{
		DBPRINTF(" I2C%d is master only.", id);
	}	
	// Enable the module
	DBPRINTF("Enabling operations...\n");
	I2CEnable(id, TRUE);
}

int main(void)
{

	// configure for single vectored mode
	INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
	
	// enable interrupts
	INTEnableInterrupts();
	
	// Configure the interrupt priority, level 5
	INTSetVectorPriority( INT_I2C_1_VECTOR,
	                      INT_PRIORITY_LEVEL_5);
	INTSetVectorPriority( INT_I2C_2_VECTOR,
	                      INT_PRIORITY_LEVEL_5);	
	// Configure the interrupt sub-priority, level 2
	INTSetVectorSubPriority( INT_I2C_1_VECTOR,
	                         INT_SUB_PRIORITY_LEVEL_2);
	INTSetVectorSubPriority( INT_I2C_2_VECTOR,
	                         INT_SUB_PRIORITY_LEVEL_2);
	                         
	// Clear the interrupt source flag
	INTClearFlag(INT_I2C1B);
	INTClearFlag(INT_I2C1M);
	
	// Enable the interrupt source
	INTEnable(INT_I2C1B, INT_ENABLED);
	INTEnable(INT_I2C1M, INT_ENABLED);

	I2C_Initialize( I2C1, 10000, NULL );
	
	while(1){
		DBPRINTF("Enabling operations...\n");
	}
}
