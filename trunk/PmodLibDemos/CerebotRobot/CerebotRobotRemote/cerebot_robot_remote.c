/************************************************************************/
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*              Copyright (C) 2011 Ryan Hoffman                         */
/************************************************************************/
/*  Module Description: 												*/
/*  PmodJSTK (FWD-REV)   - JB(pins 1 - 6)                                           */
/*  PmodJSTK (LEFT_RIGHT)-
/*	PmodCLS -   											*/
/*  PmodBT2 - 
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*  11/16/2011(RyanH):                                                  */
/*																		*/
/************************************************************************/

/* ------------------------------------------------------------ */
/*				PIC32 Configuration Settings					*/
/* ------------------------------------------------------------ */
//
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_2

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include <stdint.h>
#include <plib.h>
#include <pmodlib.h>
#include "PmodBT.h"
/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */
#define SYS_FREQ               (80000000L)
#define PB_DIV                 2
#define PB_CLOCK			   SYS_FREQ/PB_DIV
#define PRESCALE_T1            8
#define TOGGLES_PER_SEC_T1     1000
#define T1_TICK              (SYS_FREQ/PB_DIV/PRESCALE_T1/TOGGLES_PER_SEC_T1)


//IO PORT DEFINITIONS
#define UART_BLUETOOTH					UART2
#define UART_CLS						UART1
#define SPI_JSTK_FWD_REV				SPI_CHANNEL2
#define SPI_JSTK_LEFT_RIGHT				SPI_CHANNEL1
#define PORT_BIT_PMODBTN2_STATUS		IOPORT_B,BIT_0
#define PORT_BIT_PMODBTN2_RESET			IOPORT_B,BIT_1
#define PORT_BIT_PMODBTN2_CTS			IOPORT_F,BIT_12

//BIT RATE DEFINITIONS
#define UART_BLUETOOTH_BITRATE			115200
#define UART_CLS_BITRATE				9600
#define SPI_JSTK_BITRATE				625000

//BLUETOOTH PAIRING NAMES
#define BLUETOOTH_NAME					"CEREBOTREMOTE"
#define BLUETOOTH_REMOTE				"RYAN-LAPTOP-XPS"

#define BLUETOOTH_CONNECTION_RETRIES	5

#define BLUETOOTH_INQUIRY_TIMEOUT		"10"

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

//PmodCLS commands, see PmodCLS manual for descriptions
uint8_t enableDisplay[] = {27, '[', '3', 'e', '\0'}; //enable display
uint8_t setCursor[] = {27, '[', '0', 'c', '\0'}; //turn off cursor
uint8_t homeRow2[] = {27,'[','1',';','0','H','\0'}; //set cursor to col 0, row 1
uint8_t homeCursor[] = {27, '[', 'j', '\0'}; //set cursor to row 0, col 0, clear display
uint8_t wrapLine[] = {27, '[', '0', 'h', '\0'}; //set display to "wrap"


PmodJSTKAxisButton jstkAxisBtnFwdRev;
PmodJSTKAxisButton jstkAxisBtnLeftRight;

//Joystick calibration values
uint16_t yAxisMin = 0;
uint16_t yAxisMax = 0;
uint16_t xAxisMin = 0;
uint16_t xAxisMax = 0;
uint16_t xAxisRange = 0;
uint16_t yAxisRange = 0;


void init();
void initControllers();
void UARTInit(uint32_t baudRate,uint32_t pbClock,UART_MODULE uartID,UART_CONFIGURATION configParams);
void initCLS();
void setPinIO();
void initPmodBTN2();
void calibrateJoySticks();
void getJstkValsOnBTNDown(SpiChannel chn,PmodJSTKAxisButton *jstkAxisBtn,uint8_t btn,uint8_t ledState);
void blockWhileBtnDown(SpiChannel chn,uint8_t button,uint8_t ledState);
void connectToRemoteHost();


uint8_t main(void)
{
	
	init();

	return 0;	
}	

void init()
{
	initControllers();
	setPinIO();
	initCLS();
	initPmodBTN2();
	calibrateJoySticks();
	connectToRemoteHost();
}	

void connectToRemoteHost()
{
	UARTPutS(homeCursor,UART_CLS);
	UARTPutS("Connecting.....",UART_CLS);
	if(BTInquireAndConnect(UART_BLUETOOTH,BLUETOOTH_REMOTE,BLUETOOTH_INQUIRY_TIMEOUT,BLUETOOTH_CONNECTION_RETRIES))
	{
			UARTPutS(homeCursor,UART_CLS);
			UARTPutS("Connected!!!",UART_CLS);	
	}
	else
	{
			UARTPutS(homeCursor,UART_CLS);
			UARTPutS("Connect Fail",UART_CLS);
	}			
}	

void blockWhileBtnDown(SpiChannel chn,uint8_t button,uint8_t ledState)
{
	PmodJSTKAxisButton jstkAxisBtn;
	do
	{
		PmodJSTKSendRecv(chn,ledState,&jstkAxisBtn);

	}while(jstkAxisBtn.buttonState == button);
}


void calibrateJoySticks()
{
	
	PmodJSTKAxisButton jstkAxisBtn;
	
	UARTPutS(homeCursor,UART_CLS);
	UARTPutS("Calibrate Y-",UART_CLS);
	blockWhileBtnDown(SPI_JSTK_FWD_REV,PMODJSTK_BTN1,PMODJSTK_LD1_ON);
	getJstkValsOnBTNDown(SPI_JSTK_FWD_REV,&jstkAxisBtn,PMODJSTK_BTN1,PMODJSTK_LD1_ON);
	yAxisMin = jstkAxisBtn.yAxis;
	
	UARTPutS(homeCursor,UART_CLS);
	UARTPutS("Calibrate Y+",UART_CLS);
	blockWhileBtnDown(SPI_JSTK_FWD_REV,PMODJSTK_BTN1,PMODJSTK_LD1_ON);
	getJstkValsOnBTNDown(SPI_JSTK_FWD_REV,&jstkAxisBtn,PMODJSTK_BTN1,PMODJSTK_LD1_ON);
	yAxisMax = jstkAxisBtn.yAxis;
	//Turn off LD1
	PmodJSTKSendRecv(SPI_JSTK_FWD_REV,PMODJSTK_LD1_LD2_OFF,&jstkAxisBtn);
	
	
	UARTPutS(homeCursor,UART_CLS);
	UARTPutS("Calibrate X-",UART_CLS);
	blockWhileBtnDown(SPI_JSTK_LEFT_RIGHT,PMODJSTK_BTN1,PMODJSTK_LD1_ON);
	getJstkValsOnBTNDown(SPI_JSTK_LEFT_RIGHT,&jstkAxisBtn,PMODJSTK_BTN1,PMODJSTK_LD1_ON);
	xAxisMin = 	jstkAxisBtn.xAxis;

	UARTPutS(homeCursor,UART_CLS);
	UARTPutS("Calibrate X+",UART_CLS);
	blockWhileBtnDown(SPI_JSTK_LEFT_RIGHT,PMODJSTK_BTN1,PMODJSTK_LD1_ON);
	getJstkValsOnBTNDown(SPI_JSTK_LEFT_RIGHT,&jstkAxisBtn,PMODJSTK_BTN1,PMODJSTK_LD1_ON);
	xAxisMax = jstkAxisBtn.xAxis;
	//Turn off LD1
	PmodJSTKSendRecv(SPI_JSTK_LEFT_RIGHT,PMODJSTK_LD1_LD2_OFF,&jstkAxisBtn);	
	
	yAxisRange = (yAxisMax - yAxisMin);
	xAxisRange = (xAxisMax - xAxisMin);
	
}

/*  
** <FUNCTION NAME>
**
**	Synopsis:
**
**  Input: 
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
*/
void getJstkValsOnBTNDown(SpiChannel chn,PmodJSTKAxisButton *jstkAxisBtn,uint8_t btn,uint8_t ledState)
{
	do
	{
		PmodJSTKSendRecv(chn,ledState,jstkAxisBtn);

	}while(jstkAxisBtn->buttonState != btn);
}
	

void resetBTModule()
{
	uint32_t delay = 0;
	//Get current Bluetooth CTS pin state
	uint16_t ctsPinState = PORTReadBits(PORT_BIT_PMODBTN2_CTS);

	//Send reset low to initiate reset
	PORTClearBits(PORT_BIT_PMODBTN2_RESET);
 	while(PORTReadBits(PORT_BIT_PMODBTN2_CTS) == ctsPinState); //wait for CTS to go low, BT off
 
 	delayN(400000); //delay so PmodBTN2 has to se the state change
 
 	//Send reset high to finish reset
    PORTSetBits(PORT_BIT_PMODBTN2_RESET);
    while(PORTReadBits(PORT_BIT_PMODBTN2_CTS) != ctsPinState);	//wait for CTS to go high, BT on
 
     //wait for module to fully initialize
 	delayN(4000000);
}


/*  
** <FUNCTION NAME>
**
**	Synopsis:
**
**  Input: 
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
*/
void initCLS()
{
	//Delay to allow for CLS initialization
	delayN(100000);
	UARTPutS(enableDisplay,UART_CLS);
	UARTPutS(setCursor,UART_CLS);
	UARTPutS(homeCursor,UART_CLS);
	UARTPutS("Initializing...",UART_CLS);
}

/*  
** <FUNCTION NAME>
**
**	Synopsis:
**
**  Input: 
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
*/
void initControllers()
{
	//Initialize PmodBTN2 UART
	UARTInit(UART_BLUETOOTH_BITRATE,PB_CLOCK,UART_BLUETOOTH,UART_ENABLE_PINS_CTS_RTS | UART_RTS_WHEN_RX_NOT_FULL);
	//Initialize PmodCLS UART
	UARTInit(UART_CLS_BITRATE,PB_CLOCK,UART_CLS,UART_ENABLE_PINS_TX_RX_ONLY);
	//Initialize PmodJSTK for forward and reverse
	PmodJSTKInit(SPI_JSTK_FWD_REV,PB_CLOCK,SPI_JSTK_BITRATE,SYS_FREQ);
	//Initialize PmodJSTK for left and right
	PmodJSTKInit(SPI_JSTK_LEFT_RIGHT,PB_CLOCK,SPI_JSTK_BITRATE,SYS_FREQ);

}

void setPinIO()
{
	//Set digital in for PmodBTN2 connection status pin
	PORTSetPinsDigitalIn(PORT_BIT_PMODBTN2_STATUS);
	//Start PmodBTN2 RST high
	PORTSetBits(PORT_BIT_PMODBTN2_RESET); 
	//Set digital out for PmodBTN2 reset pin
	PORTSetPinsDigitalOut(PORT_BIT_PMODBTN2_RESET);	
	
	//fix for SPI1 not initing correctly via Microchip Libraries
	PORTSetPinsDigitalOut(IOPORT_D,BIT_0|BIT_9|BIT_10);
}		

void UARTInit(uint32_t baudRate,uint32_t pbClock,UART_MODULE uartID,UART_CONFIGURATION configParams)
{
	UARTConfigure(uartID,configParams);

    UARTSetFifoMode(uartID, UART_INTERRUPT_ON_RX_NOT_EMPTY | UART_INTERRUPT_ON_TX_NOT_FULL );

    UARTSetLineControl(uartID, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);

    UARTSetDataRate(uartID, pbClock, baudRate);

    UARTEnable(uartID, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

}	

/*  
** <FUNCTION NAME>
**
**	Synopsis:
**
**  Input: 
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  All commands and settings for the PmodBTN2 can be found in the
**  Roving Networks RN-42 Advanced User Manual.
**
**  Notes:
**  Unless otherwise set below, the PmodBTN2 will use its
**  default settings.
*/
void initPmodBTN2()
{
	uint8_t params[BLUETOOTH_MAX_PARAMS][BLUETOOTH_MAX_PARAM_LEN];

	//Reset the module in case it is in an unusable state
	resetBTModule();
	
	//Initiate local configuration
	strcpy(params[0],"$$$");
	BTSendSetCommand(UART_BLUETOOTH,params,1);
	
	//Set all parameters
	strcpy(params[0],"SF");
	strcpy(params[1],"1");
	BTSendSetCommand(UART_BLUETOOTH,params,2);
	
	//Exit local configuration
	strcpy(params[0],"---\r\n");
	BTSendSetCommand(UART_BLUETOOTH,params,1);
	
	//Reset to allow changes to take effect
	resetBTModule();
	
	//Initiate local configuration
	strcpy(params[0],"$$$");
	BTSendSetCommand(UART_BLUETOOTH,params,1);
	
	//Set the name of the device used in discovery
	strcpy(params[0],"SN");
	strcpy(params[1],BLUETOOTH_NAME);
	BTSendSetCommand(UART_BLUETOOTH,params,2);
	
	//Only allow local configuration
	strcpy(params[0],"ST");
	strcpy(params[1],"253");
	BTSendSetCommand(UART_BLUETOOTH,params,2);

	//Set inquiry window to the max setting
	strcpy(params[0],"SI");
	strcpy(params[1],"0800");
	BTSendSetCommand(UART_BLUETOOTH,params,2);
	
	//Set the page scan window to the max setting
	strcpy(params[0],"SJ");
	strcpy(params[1],"0800");
	BTSendSetCommand(UART_BLUETOOTH,params,2);
		//Set the service name 
	strcpy(params[0],"SS");
	strcpy(params[1],"SerialPort");
	BTSendSetCommand(UART_BLUETOOTH,params,2);
	
	//Set Service Class to Bluetooth
	strcpy(params[0],"SC");
	strcpy(params[1],"0001");
	BTSendSetCommand(UART_BLUETOOTH,params,2);
	
	//Set class of device to serial port
	strcpy(params[0],"SD");
	strcpy(params[1],"1101");
	BTSendSetCommand(UART_BLUETOOTH,params,2);
	
	//Set the security pin to 0000
	strcpy(params[0],"SP");
	strcpy(params[1],"0000");
	BTSendSetCommand(UART_BLUETOOTH,params,2);

	//Set connect prefix to DEV_, will send DEV_CONNECT to 
	//remote host on connect
	strcpy(params[0],"SO");
	strcpy(params[1],"DEV_");
	BTSendSetCommand(UART_BLUETOOTH,params,2);

	//Set mode to master
	strcpy(params[0],"SM");
	strcpy(params[1],"1");
	BTSendSetCommand(UART_BLUETOOTH,params,2);

	//Exit local configuration
	strcpy(params[0],COMMAND_EXIT);
	BTSendSetCommand(UART_BLUETOOTH,params,1);
	
	//Reset to allow changes to take effect
	resetBTModule();
}	