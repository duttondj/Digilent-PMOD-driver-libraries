/* ------------------------------------------------------------ */
/*				PIC32 Configuration Settings					*/
/* ------------------------------------------------------------ */
//

#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_2

#include <stdint.h>

#include "PmodBT.h"

#define MASTER	0


#define BLUE_TOOTH_BAUD_RATE		115200


#define UART_BLUETOOTH				UART2
#define SYS_FREQ               (80000000L)
#define PB_DIV                 2
#define PB_CLOCK			   SYS_FREQ/PB_DIV

#if(MASTER == 1)

#define BT_SERIAL_MODE		   		"5"
#define BLUE_TOOTH_NAME				"MASTER"
#define PEER_NAME					"SLAVE"
uint8_t UARTRxData = 0;
void enableInterrupts();

#else

#define BT_SERIAL_MODE		   		"5"
#define BLUE_TOOTH_NAME				"SLAVE"
#define PEER_NAME					"RYAN-LAPTOP-XPS"

#endif

uint8_t buffer[40];



uint8_t main(void)
{
	
	uint8_t params[3][15];

	uint8_t connectSuccess = 0;
	
	UARTSetup(BLUE_TOOTH_BAUD_RATE,PB_CLOCK,UART_BLUETOOTH);
	
	resetBTModule();
	
	strcpy(params[0],ENTER_COMMAND);
	BTSendSetCommand(UART_BLUETOOTH,params,1);
	
	strcpy(params[0],"SF");
	strcpy(params[1],"1");
	BTSendSetCommand(UART_BLUETOOTH,params,2);
	
	strcpy(params[0],COMMAND_EXIT);
	BTSendSetCommand(UART_BLUETOOTH,params,1);
	
	resetBTModule();
	
	strcpy(params[0],ENTER_COMMAND);
	BTSendSetCommand(UART_BLUETOOTH,params,1);
	
	strcpy(params[0],"SN");
	strcpy(params[1],BLUE_TOOTH_NAME);
	BTSendSetCommand(UART_BLUETOOTH,params,2);
	
	strcpy(params[0],"ST");
	strcpy(params[1],"253");
	BTSendSetCommand(UART_BLUETOOTH,params,2);

	strcpy(params[0],"SI");
	strcpy(params[1],"0800");
	BTSendSetCommand(UART_BLUETOOTH,params,2);
	
	strcpy(params[0],"SJ");
	strcpy(params[1],"0800");
	BTSendSetCommand(UART_BLUETOOTH,params,2);
	

	strcpy(params[0],"SS");
	strcpy(params[1],"SerialPort");
	BTSendSetCommand(UART_BLUETOOTH,params,2);
	
	strcpy(params[0],"S~");
	strcpy(params[1],"0");
	BTSendSetCommand(UART_BLUETOOTH,params,2);
	
	
	strcpy(params[0],"SC");
	strcpy(params[1],"0001");
	BTSendSetCommand(UART_BLUETOOTH,params,2);
	
	strcpy(params[0],"SD");
	strcpy(params[1],"1101");
	BTSendSetCommand(UART_BLUETOOTH,params,2);
	
	strcpy(params[0],"SP");
	strcpy(params[1],"0000");
	BTSendSetCommand(UART_BLUETOOTH,params,2);

	strcpy(params[0],"SO");
	strcpy(params[1],"DEV_");
	BTSendSetCommand(UART_BLUETOOTH,params,2);

	strcpy(params[0],"SM");
	strcpy(params[1],BT_SERIAL_MODE);
	BTSendSetCommand(UART_BLUETOOTH,params,2);

	strcpy(params[0],COMMAND_EXIT);
	BTSendSetCommand(UART_BLUETOOTH,params,1);
	
	resetBTModule();
	
	#if(MASTER == 1)
	enableInterrupts();
	
	while(1)
	{
		if(UARTRxData)
		{
			UARTRxData = 0;
			BTGetLine(UART_BLUETOOTH,buffer);
			UARTPutS("CONNECT MASTER\r\n",UART_BLUETOOTH);		
		}			
	}	
	
	#else
	
	connectSuccess = BTInquireAndConnect(UART_BLUETOOTH,PEER_NAME,"10",2);
	
	while(1)
	{
		UARTPutOneByte(UARTGetOneByte(UART_BLUETOOTH),UART_BLUETOOTH);
	}	
	
	#endif
	return 0;	
}	

#if(MASTER == 1)
void enableInterrupts()
{
	// Configure the interrupt priority, level 5
	INTDisableInterrupts();
	INTClearFlag(INT_U2RX);//make sure interrupt flag is cleared

	//configure multi vector interrupts
	INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);

	INTSetVectorPriority(INT_UART_2_VECTOR, INT_PRIORITY_LEVEL_2);

	INTEnable(INT_U2RX,INT_ENABLED);
	INTEnableInterrupts();
}

void __ISR(_UART_2_VECTOR, ipl2) UARTIntHandler(void)
{
	uint8_t oneByte = 0;
	if(INTGetFlag(INT_U2RX))
	{	
		INTClearFlag(INT_U2RX);
	//	UARTPutOneByte(UARTGetOneByte(UART_BLUETOOTH),UART_BLUETOOTH);
		
		UARTRxData = 1;
		
	}	
}

#endif	