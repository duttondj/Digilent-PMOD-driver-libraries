// Skylar Graika


#include <peripheral/ports.h>	// Enable port pins for input or output 
#include <peripheral/uart.h>	// Enable UARTs 1 and 2 for reception and transmission of serial data
#include <peripheral/system.h>	// Set up the system and perihperal clocks for best performance

//Local includes
#include <UartLib/uart_config.h>	//uart_config.h file is used to setup the configuration of device/s 
									//communicating with the UARTs.

// SYSCLK = 80 MHz (8 MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 40 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care


#pragma config FNOSC	= PRIPLL // Required
#pragma config FPLLMUL 	= MUL_20
#pragma config FPLLIDIV  = DIV_2
#pragma config FPBDIV 	= DIV_2 // Divide SYSCLK by 2 ==> PBCLK
#pragma config FPLLODIV  = DIV_1 






#define SYSTEM_CLOCK		80000000
#define DESIRED_BAUD_RATE	9600


void setup_ports (void);
//void setup_UART1 (unsigned int pb_clock);
void setup_UART2 (unsigned int pb_clock);
void initialize_CLS (void);


// Globals for setting up pmod CLS
unsigned char enable_display[] = {27, '[', '3', 'e', '\0'};
unsigned char set_cursor[] = {27, '[', '1', 'c', '\0'};
unsigned char home_cursor[] = {27, '[', 'j', '\0'};
unsigned char wrap_line[] = {27, '[', '0', 'h', '\0'};




int main (void)
{
	unsigned int pb_clock = 0;
	unsigned char message[] = "Ready to receive...", data = '\0';


	// extern inline unsigned int __attribute__((always_inline)) SYSTEMConfigPerformance(unsigned int sys_clock)
	pb_clock = SYSTEMConfigPerformance (SYSTEM_CLOCK);


	setup_ports ();


	//Uncomment if using an additional UART to receive from:
	//setup_UART1 (pb_clock);
	setup_UART2( pb_clock);


	
	initialize_CLS ();


	putsUART2 (message);


	while (1)
	{
		//To receive from another UART and display on PmodCLS: 
		//uncomment and setup receiving UART.
		/*
		if (DataRdyUART1 ()) //Checks if data is available to be read from UART
		{
			data = ReadUART1 (); //Reads one character from UART
			putcUART2 (data); //Displays character onto PmodCLS
		}
		*/
	}


	return 0;
}


void setup_ports (void)
{
	if( _UART_JE_ == 1 ) //UART connected to JE pins
	{
	// UART JE port pins -
	/* JE-01 CN20/U1CTS/RD14 		RD14
	  JE-02 U1RTS/BCLK1/CN21/RD15  RD15
	  JE-03 U1RX/RF2 				RF2
	  JE-04 U1TX/RF8 				RF8 */


	PORTSetPinsDigitalIn (IOPORT_F, BIT_2);
	PORTSetPinsDigitalOut (IOPORT_F, BIT_8);
	}
	
	if( _UART_JH_ == 1 ) //UART connected to JH pins
	{ 
	// UART JH port pins - 
	/* JH-01 U2CTS/RF12 			RF12
   	  JH-02 U2RTS/BCLK2/RF13 		RF13
       JH-03 PMA9/U2RX/CN17/RF4 	RF4
	  JH-04 PMA8/U2TX/CN18/RF5 	RF5 */
	
	PORTSetPinsDigitalIn (IOPORT_F, BIT_4);
	PORTSetPinsDigitalOut (IOPORT_F, BIT_5);
	}
}


//To add another UART to receive data from then uncomment 
//setup_UART1. Add the function to main() and functions
//associated with this UART will become available. (i.e.
//ReadUART1()...etc.)
/*
void setup_UART1 (unsigned int pb_clock)
{
	// OpenUART1( config1, config2, ubrg)
	OpenUART1 (UART_EN | UART_IDLE_CON | UART_RX_TX | UART_DIS_WAKE | UART_DIS_LOOPBACK | UART_DIS_ABAUD | UART_NO_PAR_8BIT | UART_1STOPBIT | UART_IRDA_DIS | 
               UART_MODE_FLOWCTRL | UART_DIS_BCLK_CTS_RTS | UART_NORMAL_RX | UART_BRGH_SIXTEEN,
               UART_TX_PIN_LOW | UART_RX_ENABLE | UART_TX_ENABLE | UART_INT_TX | UART_INT_RX_CHAR | UART_ADR_DETECT_DIS	| UART_RX_OVERRUN_CLEAR, 
			  mUARTBRG(pb_clock, DESIRED_BAUD_RATE));
}
*/


void setup_UART2 (unsigned int pb_clock) //UART associated with the PmodCLS
{
	// OpenUART2( config1, config2, ubrg)
	OpenUART2 (UART_EN | UART_IDLE_CON | UART_RX_TX | UART_DIS_WAKE | UART_DIS_LOOPBACK | UART_DIS_ABAUD | UART_NO_PAR_8BIT | UART_1STOPBIT | UART_IRDA_DIS | 
               UART_MODE_FLOWCTRL | UART_DIS_BCLK_CTS_RTS | UART_NORMAL_RX | UART_BRGH_SIXTEEN,
               UART_TX_PIN_LOW | UART_RX_ENABLE | UART_TX_ENABLE | UART_INT_TX | UART_INT_RX_CHAR | UART_ADR_DETECT_DIS	| UART_RX_OVERRUN_CLEAR, 
			  mUARTBRG(pb_clock, DESIRED_BAUD_RATE));




	UARTEnable(UART2, UART_DISABLE_FLAGS(UART_RX ));
}


void initialize_CLS (void)  //Initial PmodCLS setup.
{
	putsUART2 (enable_display);
	putsUART2 (set_cursor);
	putsUART2 (home_cursor);
	putsUART2 (wrap_line);
}