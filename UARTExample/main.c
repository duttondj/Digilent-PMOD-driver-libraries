/********************************************************************************
 * Programmer: Peter Olson														*
 * Class: CptS 466/566,															*
 * Take-home Exam 1, problem 1													*
 * Date: March 7, 2010															*
 * 																				*
 * Description: This program is an external calculator. It connects over serial	*
 *					with the RS232 pmod on the top part of port JE				*
 * 																				*
 *******************************************************************************/


#include <peripheral/ports.h>
#include <peripheral/uart.h>
#include <peripheral/system.h>	// Set up the system and perihperal clocks for best performance
#include <stdint.h>
#include <stdio.h>

// *****************************************************************************
// *****************************************************************************
// Section: Configuration bits
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 40 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
// *****************************************************************************
// *****************************************************************************
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1

// *****************************************************************************
// *****************************************************************************
// Section: System Macros
// *****************************************************************************
// *****************************************************************************
#define	GetSystemClock() 			(80000000ul)
#define	GetPeripheralClock()		(GetSystemClock()/(1 << OSCCONbits.PBDIV))
#define	GetInstructionClock()		(GetSystemClock())

#define DESIRED_BAUDRATE    	(9600)      //The desired BaudRate

// *****************************************************************************
// *****************************************************************************


double solve_rhs(double lhs);
void setup_UART (void);
void WriteString1(const char *string);
void configure_interrupts (void);

// Global flag for previous char recieved (used only in UART ISR)
volatile char f_prev = 0;
// Global flag for input ready
volatile char f_ready = 0;
	// '#' for number, '/*+-' for operator, '=' for end of problem
volatile double in_num = 0;
	// if f_ready == '#', then in_num has the value of the inputted #
volatile double div_amnt = 0;

int main (void)
{ 
	double ret = 0;
	char ret_str[32] = {0};
	
	// extern inline unsigned int __attribute__((always_inline)) SYSTEMConfig(unsigned int sys_clock, unsigned int flags)
	SYSTEMConfig(GetSystemClock(), SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

	//Initialization routines
	setup_UART();
	configure_interrupts();


	WriteString1("*** Enter an equation followed by = *** ");

 
	while (1)
	{
		if(f_ready == '=')
		{
			WriteString1(" 0 ");
			f_ready = 0;
		}
		else if(f_ready == '#')
		{
			f_ready = 0;
			ret = solve_rhs(in_num);
			sprintf(ret_str,"%f ",ret);
			WriteString1(ret_str);
			f_ready = 0;
		}
	}

	return 0;
}

double solve_rhs(double lhs)
{
	while(!f_ready) ;
	if(f_ready == '=')
		return lhs;
	if(f_ready == '*')
	{
		f_ready = 0;
		while(!f_ready) ;
		if(f_ready != '#')
			WriteString1("ERROR: Must enter a number");
		lhs *= in_num;
		f_ready = 0;
		return solve_rhs(lhs);
	}
	if(f_ready == '/')
	{
		f_ready = 0;
		while(!f_ready) ;
		if(f_ready != '#')
			WriteString1("ERROR: Must enter a number");
		lhs /= in_num;
		f_ready = 0;
		return solve_rhs(lhs);
	}
	if(f_ready == '+')
	{
		f_ready = 0;
		while(!f_ready) ;
		if(f_ready != '#')
			WriteString1("ERROR: Must enter a number");
		f_ready = 0;
		return lhs + solve_rhs(in_num);
	}
	if(f_ready == '-')
	{
		f_ready = 0;
		while(!f_ready) ;
		if(f_ready != '#')
			WriteString1("ERROR: Must enter a number");
		f_ready = 0;
		return lhs - solve_rhs(in_num);
	}
	//If not caught by any of the above, user error
	WriteString1("ERROR: Can't enter two numbers in a row");
	return 0;
}


void setup_UART (void)
{
	//UART1 is read from computer
	UARTConfigure(UART1, UART_ENABLE_PINS_TX_RX_ONLY);
	UARTSetFifoMode(UART1, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
	UARTSetLineControl(UART1, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
	UARTSetDataRate(UART1, GetPeripheralClock(), DESIRED_BAUDRATE);
	UARTEnable(UART1, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

//	//UART2 is write to display
//	UARTConfigure(UART2, UART_ENABLE_PINS_TX_RX_ONLY);
//	UARTSetFifoMode(UART2, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
//	UARTSetLineControl(UART2, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
//	UARTSetDataRate(UART2, GetPeripheralClock(), DESIRED_BAUDRATE);
//	UARTEnable(UART2, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
}


void configure_interrupts (void)
{
	// Configure for multi-vectored mode
	INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);

	// Configure UART1 RX Interrupt
	INTEnable(INT_SOURCE_UART_RX(UART1), INT_ENABLED);
	INTSetVectorPriority(INT_VECTOR_UART(UART1), INT_PRIORITY_LEVEL_5);

	// enable interrupts
	INTEnableInterrupts();
}

//Write a string to the UART
void WriteString1(const char *string)
{
	while(*string != '\0')
	{
		while(!UARTTransmitterIsReady(UART1))	;

		UARTSendDataByte(UART1, *string);

		string++;

		while(!UARTTransmissionHasCompleted(UART1))	;
	}
}

// UART 1 interrupt handler
// it is set at priority level 5
void __ISR(_UART1_VECTOR, ipl5) IntUart1Handler(void)
{
	// Is this an RX interrupt?
	if(INTGetFlag(INT_SOURCE_UART_RX(UART1)))
	{
		int cur = UARTGetDataByte(UART1);
		// Clear the RX interrupt Flag
		INTClearFlag(INT_SOURCE_UART_RX(UART1));

		
		while(!UARTTransmitterIsReady(UART1))	;
		UARTSendDataByte(UART1, cur);
		while(!UARTTransmissionHasCompleted(UART1))	;
		
 		if(f_prev == 0) //Last charcter was a space
		{
			if(cur == '-')
				f_prev = '-';
			else if(cur == '+')
				f_prev = '+';
			else if(cur == '*')
				f_prev = '*';
			else if(cur == '/')
				f_prev = '/';
			else if(cur == '=')
				f_prev = '=';
			else if((cur >= '0')&&(cur <= '9'))
			{
				f_prev = '#';
				in_num = cur-'0';
			}
			else
				WriteString1("ERROR: Invalid Input");
		}
		else if(f_prev == '#')
		{
			if(cur == ' ')
			{
				f_ready = '#';
				f_prev = 0;
				div_amnt = 0;
			}
			else if((cur >= '0')&&(cur <= '9'))
			{
				if(div_amnt == 0)
					in_num = 10*in_num + (cur - '0');
				else
				{
					in_num += div_amnt * (cur - '0');
					div_amnt *= 0.1;
				}
			}
			else if(cur == '.')
				div_amnt = 0.1;
			else
				WriteString1("ERROR: Invalid input");
		}
		else if(f_prev == '@') //  Negative number
		{
			if(cur == ' ')
			{
				f_ready = '#';
				f_prev = 0;
				div_amnt = 0;
				in_num = 0 - in_num;
			}
			else if((cur >= '0')&&(cur <= '9'))
			{
				if(div_amnt == 0)
					in_num = 10*in_num + (cur - '0');
				else
				{
					in_num += div_amnt * (cur - '0');
					div_amnt *= 0.1;
				}
			}
			else if(cur == '.')
				div_amnt = 0.1;
			else
				WriteString1("ERROR: Invalid input");
		}
		else if(f_prev == '*')
		{
			if(cur != ' ')
				WriteString1("ERROR: Invalid input");
			else
				f_ready = '*';
			f_prev = 0;
		}
		else if(f_prev == '/')
		{
			if(cur != ' ')
				WriteString1("ERROR: Invalid input");
			else
				f_ready = '/';
			f_prev = 0;
		}
		else if(f_prev == '=')
		{
			if(cur != ' ')
				WriteString1("ERROR: Invalid input");
			else
				f_ready = '=';
			f_prev = 0;
		}
		else if(f_prev == '-')
		{
			if(cur == ' ')
			{
				f_ready = '-';
				f_prev = 0;
			}
			else if((cur >= '0')&&(cur <= '9'))
			{
				f_prev = '@'; //'#' - one key = '@'
				in_num = cur - '0';
			}
			else
				WriteString1("ERROR: Invalid input");
		}
		else if(f_prev == '+')
		{
			if(cur == ' ')
			{
				f_ready = '+';
				f_prev = 0;
			}
			else if((cur >= '0')&&(cur <= '9'))
			{
				f_prev = '#';
				in_num = cur - '0';
			}
			else
				WriteString1("ERROR: Invalid input");
		}
	}

	// We don't care about TX interrupt
	if ( INTGetFlag(INT_SOURCE_UART_TX(UART1)) )
	{
		INTClearFlag(INT_SOURCE_UART_TX(UART1));
	}
}
