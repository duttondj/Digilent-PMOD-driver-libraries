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
#include <PmodBT.h>
#include <cerebot_robot_remote_types.h>
/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */
#define SYS_FREQ               (80000000L)
#define PB_DIV                 2
#define PB_CLOCK			   SYS_FREQ/PB_DIV
#define PRESCALE_T1            256
#define TOGGLES_PER_SEC_T1     5
#define T1_TICK              (SYS_FREQ/PB_DIV/PRESCALE_T1/TOGGLES_PER_SEC_T1)

//IO PORT DEFINITIONS
#define UART_BLUETOOTH					UART2
#define UART_CLS						UART1
#define SPI_JSTK_FWD_REV				SPI_CHANNEL2
#define SPI_JSTK_LEFT_RIGHT				SPI_CHANNEL1
#define SPI_1_PIC32_460_512L_TRIS		IOPORT_D,BIT_0|BIT_9|BIT_10
#define PORT_BIT_PMODBTN2_STATUS		IOPORT_B,BIT_0
#define PORT_BIT_PMODBTN2_RESET			IOPORT_B,BIT_1
#define PORT_BIT_PMODBTN2_CTS			IOPORT_F,BIT_12
#define PORT_BIT_LED_1					IOPORT_B,BIT_10


#define CLS_UPDATE_COUNT				3

//BIT RATE DEFINITIONS
#define UART_BLUETOOTH_BITRATE			115200
#define UART_CLS_BITRATE				9600
#define SPI_JSTK_BITRATE				156250

//BLUETOOTH PAIRING NAMES
#define BLUETOOTH_NAME					"CEREBOTREMOTE"
#define BLUETOOTH_REMOTE				"CEREBOTROBOT"

#define BLUETOOTH_RESPONSE_CONNECT		"DEV_CONNECT\r\n"

#define BLUETOOTH_CONNECTION_RETRIES	5
#define BLUETOOTH_INQUIRY_TIMEOUT		"10"

#define JSTK_AXIS_ABOVE_CENTER			1
#define JSTK_AXIS_BELOW_CENTER		    0

//MAIN APP TASK STATES
#define STATE_CONNECT					0
#define STATE_DISCONNECTED				1
#define STATE_POLLDEV					2
#define STATE_SEND_MESSAGE				3
#define STATE_RECIEVE_MESSAGE			4
#define STATE_UPDATE_CLS				5
#define STATE_RESEND_MESSAGE			6


#define TASK_LOOP_TIMER_FIRED			1
#define TASK_LOOP_TIMER_RESET			0

#define CLS_DISPLAY_WIDTH 				17 //includes null terminator




/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

//PmodCLS commands, see PmodCLS manual for descriptions
uint8_t enableDisplay[] = {27, '[', '3', 'e', '\0'}; //enable display
uint8_t setCursor[] = {27, '[', '0', 'c', '\0'}; //turn off cursor
uint8_t homeRow2[] = {27,'[','1',';','0','H','\0'}; //set cursor to col 0, row 1
uint8_t homeCursor[] = {27, '[', 'j', '\0'}; //set cursor to row 0, col 0, clear display
uint8_t wrapLine[] = {27, '[', '0', 'h', '\0'}; //set display to "wrap"

uint8_t clsDisplayLine[CLS_DISPLAY_WIDTH];

uint8_t jystkFwdRevLedState;
uint8_t jystkLeftRightLedState;
uint8_t mainLoopState;
uint8_t taskLoopTimerState;

uint8_t clsUpdateCounter = 0;

//Joystick calibration values
JSTK_AXIS_RANGE jstkAxisRange;

//Message sent from remote
CEREBOT_REMOTE_MSG cerebotRemoteMsg;
//Message recieved from robot
CEREBOT_ROBOT_MSG cerebotRobotMsg;


PmodJSTKAxisButton jstkAxisBtnFwdRev;
PmodJSTKAxisButton jstkAxisBtnLeftRight;

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
void resetJstkLedState();
void appTask();
void pollBlueToothConnected();
void pollJstkSetLEDState();
uint8_t longitudinalRedunancyCheck(uint8_t *bytes,uint16_t numBytes);
void sendMessage();
void enableTimers();
void recieveMessage();
void updateCLS();
uint16_t calcAxisDutyCycle(uint16_t axisValue, uint16_t axisMin,uint16_t axisRange);

uint8_t main(void)
{

jystkFwdRevLedState = PMODJSTK_LD1_LD2_OFF;
jystkLeftRightLedState = PMODJSTK_LD1_LD2_OFF;
mainLoopState = STATE_CONNECT;
taskLoopTimerState = TASK_LOOP_TIMER_RESET;

	init();

	return 0;	
}	

void init()
{
	initControllers();
	setPinIO();
	initCLS();
	resetJstkLedState();
	initPmodBTN2();
	calibrateJoySticks();
	connectToRemoteHost();
	enableTimers();
	appTask();
}	



void appTask()
{
	while(1)
	{
		if(taskLoopTimerState == TASK_LOOP_TIMER_FIRED)
		{
			switch(mainLoopState)
			{
					
				 case STATE_CONNECT:
					 connectToRemoteHost();
					 break;
				 case STATE_DISCONNECTED:
				 	mainLoopState = STATE_CONNECT;
				 	break;
				 case STATE_POLLDEV:
					
				 	pollJstkSetLEDState();
				 	mainLoopState = STATE_SEND_MESSAGE;
				 	break;
				 case STATE_SEND_MESSAGE:
				 	sendMessage();
				  	if(mainLoopState != STATE_DISCONNECTED)
				 		mainLoopState = STATE_RECIEVE_MESSAGE;
				 	break;
				 case STATE_RECIEVE_MESSAGE:
				    recieveMessage();
				 	mainLoopState = STATE_UPDATE_CLS;
				 	break;
				 
				 case STATE_UPDATE_CLS:
					if(clsUpdateCounter == CLS_UPDATE_COUNT)
					{
				 		updateCLS();
						clsUpdateCounter = 0;
					}
				 	mainLoopState = STATE_POLLDEV;
				 	taskLoopTimerState = TASK_LOOP_TIMER_RESET;
				 	break;
				 	
				 case STATE_RESEND_MESSAGE:
				 	mainLoopState =  STATE_SEND_MESSAGE;
				 	break;			
			}
		}
	}	
	
}

void enableTimers()
{
	INTDisableInterrupts();
	INTClearFlag(INT_T1);


	//configure multi vector interrupts
	INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
	
	OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, T1_TICK);
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);    
 
    INTEnableInterrupts();	
}	
	

void sendMessage()
{

	uint8_t byteCount = 0;
	uint8_t *msg = (uint8_t*)&cerebotRemoteMsg;
	uint8_t lrc = 0;
	uint8_t numBytes = sizeof(CEREBOT_REMOTE_MSG);
	cerebotRemoteMsg.fwdRevSpeed = calcAxisDutyCycle(jstkAxisBtnFwdRev.yAxis, jstkAxisRange.yAxisMin,jstkAxisRange.yAxisRange);
	cerebotRemoteMsg.leftRightSpeed = calcAxisDutyCycle(jstkAxisBtnLeftRight.xAxis,jstkAxisRange.xAxisMin,jstkAxisRange.xAxisRange);
	cerebotRemoteMsg.vehicleDirectionFwdRev = (jstkAxisBtnFwdRev.yAxis >= jstkAxisRange.yAxisCenter)?JSTK_AXIS_ABOVE_CENTER:JSTK_AXIS_BELOW_CENTER;
	cerebotRemoteMsg.vehicleDirectionLeftRight = (jstkAxisBtnLeftRight.xAxis >= jstkAxisRange.xAxisCenter)?JSTK_AXIS_ABOVE_CENTER:JSTK_AXIS_BELOW_CENTER;
	cerebotRemoteMsg.resetRobot = 0;
	//lrc = longitudinalRedunancyCheck(msg,sizeof(CEREBOT_REMOTE_MSG));
	for(byteCount = 0;byteCount < numBytes;byteCount++)
	{
		while(!UARTTransmitterIsReady(UART_BLUETOOTH) && mainLoopState != STATE_DISCONNECTED);
		UARTSendDataByte(UART_BLUETOOTH, *msg);
		msg++;
	}
//	while(!UARTTransmitterIsReady(UART_BLUETOOTH) && mainLoopState != STATE_DISCONNECTED);
//	UARTSendDataByte(UART_BLUETOOTH, lrc);
}

//Joystick duty cycle based on position
//			   100
//				*
//				*
//     	   100**0**100
//				*
//				*
//			   100

uint16_t calcAxisDutyCycle(uint16_t axisValue, uint16_t axisMin,uint16_t axisRange)
{
	uint16_t axisPos = (uint16_t)(((axisValue - axisMin)/(double)axisRange) * 100);
	if(axisPos > 50)
	{
		return (axisPos % 50) * 2;
	}
	else if(axisPos < 50)
	{
		return (50 - axisPos) * 2;
	}
	//is in center
	return 0;
}

void recieveMessage()
{
	uint8_t byteCount = 0;
	uint8_t oneByte = 0;
	uint8_t *msg = (uint8_t*)&cerebotRobotMsg;
	uint8_t numBytes = sizeof(CEREBOT_ROBOT_MSG);
	while(byteCount < numBytes)
	{
		while(!UARTReceivedDataIsAvailable(UART_BLUETOOTH) && mainLoopState != STATE_DISCONNECTED);
		*msg = UARTGetDataByte(UART_BLUETOOTH);
		msg++;
		byteCount++;

	}
}		



void updateCLS()
{
	UARTPutS(homeCursor,UART_CLS);

	sprintf(clsDisplayLine,"LW: %3d RW: %3d",cerebotRobotMsg.leftWheelRPM,
											cerebotRobotMsg.rightWheelRPM);
	UARTPutS(clsDisplayLine,UART_CLS);
	UARTPutS(homeRow2,UART_CLS);
	sprintf(clsDisplayLine,"V+: %1.2f Dir: %d",cerebotRobotMsg.batteryVoltage * 0.012890625 ,cerebotRobotMsg.vehicleDirection);
	UARTPutS(clsDisplayLine,UART_CLS);
										
}	

 uint8_t longitudinalRedunancyCheck(uint8_t *bytes,uint16_t numBytes)
 {
  	uint8_t lrc = 0;
  	uint16_t byteCount = 0;
    for (byteCount = 0; byteCount < numBytes;byteCount++ )
     {
         lrc ^= bytes[byteCount];
     }
     return lrc;
 }
void pollBlueToothConnected()
{
	if(PORTReadBits(PORT_BIT_PMODBTN2_STATUS) == BIT_0)
	{
		PORTSetBits(PORT_BIT_LED_1); 
	}
	else
	{	
		mainLoopState = STATE_DISCONNECTED;
		PORTClearBits(PORT_BIT_LED_1); 	
	}	
}	

void pollJstkSetLEDState()
{
	PmodJSTKSendRecv(SPI_JSTK_FWD_REV,jystkFwdRevLedState,&jstkAxisBtnFwdRev);	
	PmodJSTKSendRecv(SPI_JSTK_LEFT_RIGHT,jystkLeftRightLedState,&jstkAxisBtnLeftRight);	
}	

void resetJstkLedState()
{
	PmodJSTKAxisButton jstkAxisBtn;
	jystkFwdRevLedState = PMODJSTK_LD1_LD2_OFF;
	jystkLeftRightLedState = PMODJSTK_LD1_LD2_OFF;
	PmodJSTKSendRecv(SPI_JSTK_FWD_REV,jystkFwdRevLedState,&jstkAxisBtn);	
	PmodJSTKSendRecv(SPI_JSTK_LEFT_RIGHT,jystkLeftRightLedState,&jstkAxisBtn);
}	

void connectToRemoteHost()
{
	UARTPutS(homeCursor,UART_CLS);
	UARTPutS("Connecting.....",UART_CLS);
	if(BTInquireAndConnect(UART_BLUETOOTH,BLUETOOTH_REMOTE,BLUETOOTH_INQUIRY_TIMEOUT,BLUETOOTH_CONNECTION_RETRIES,BLUETOOTH_RESPONSE_CONNECT))
	{
			UARTPutS(homeCursor,UART_CLS);
			UARTPutS("Connected!!!",UART_CLS);	
			mainLoopState = STATE_POLLDEV;
			pollBlueToothConnected();
	}
	else
	{
			UARTPutS(homeCursor,UART_CLS);
			UARTPutS("Connect Fail....",UART_CLS);
			UARTPutS(homeRow2,UART_CLS);
			UARTPutS("Reset Needed",UART_CLS);
			mainLoopState = STATE_DISCONNECTED;
			pollBlueToothConnected();
			while(1);//User must reset, can not continue 
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
	
	PmodJSTKAxisButton jstkAxisBtnCalibrate;

	PmodJSTKSendRecv(SPI_JSTK_LEFT_RIGHT,jystkLeftRightLedState,&jstkAxisBtnCalibrate);
	jstkAxisRange.xAxisCenter = jstkAxisBtnCalibrate.xAxis;	
	PmodJSTKDelay15us(0);
	PmodJSTKSendRecv(SPI_JSTK_FWD_REV,jystkFwdRevLedState,&jstkAxisBtnCalibrate);
	jstkAxisRange.yAxisCenter  = jstkAxisBtnCalibrate.yAxis;
	PmodJSTKDelay15us(0);
	
	do
	{
		//Turn on LD1
		jystkFwdRevLedState = PMODJSTK_LD1_ON; //set joystick Fwd/Rev LED state
	
		UARTPutS(homeCursor,UART_CLS);
		UARTPutS("Calibrate Y-",UART_CLS);
		blockWhileBtnDown(SPI_JSTK_FWD_REV,PMODJSTK_BTN1,jystkFwdRevLedState);
		PmodJSTKDelay15us(0);
		getJstkValsOnBTNDown(SPI_JSTK_FWD_REV,&jstkAxisBtnCalibrate,PMODJSTK_BTN1,jystkFwdRevLedState);
		PmodJSTKDelay15us(0);
		jstkAxisRange.yAxisMin = jstkAxisBtnCalibrate.yAxis;
		
		UARTPutS(homeCursor,UART_CLS);
		UARTPutS("Calibrate Y+",UART_CLS);
		blockWhileBtnDown(SPI_JSTK_FWD_REV,PMODJSTK_BTN1,jystkFwdRevLedState);
		PmodJSTKDelay15us(0);
		getJstkValsOnBTNDown(SPI_JSTK_FWD_REV,&jstkAxisBtnCalibrate,PMODJSTK_BTN1,jystkFwdRevLedState);
		PmodJSTKDelay15us(0);
		jstkAxisRange.yAxisMax = jstkAxisBtnCalibrate.yAxis;
		//Turn off LD1
		jystkFwdRevLedState = PMODJSTK_LD1_LD2_OFF; //set joystick Fwd/Rev LED state
		PmodJSTKSendRecv(SPI_JSTK_FWD_REV,jystkFwdRevLedState,&jstkAxisBtnCalibrate);
		
		
		UARTPutS(homeCursor,UART_CLS);
		UARTPutS("Calibrate X-",UART_CLS);
		jystkLeftRightLedState = PMODJSTK_LD1_ON;
		blockWhileBtnDown(SPI_JSTK_LEFT_RIGHT,PMODJSTK_BTN1,jystkLeftRightLedState);
		PmodJSTKDelay15us(0);
		getJstkValsOnBTNDown(SPI_JSTK_LEFT_RIGHT,&jstkAxisBtnCalibrate,PMODJSTK_BTN1,jystkLeftRightLedState);
		PmodJSTKDelay15us(0);
		jstkAxisRange.xAxisMin = 	jstkAxisBtnCalibrate.xAxis;
	
		UARTPutS(homeCursor,UART_CLS);
		UARTPutS("Calibrate X+",UART_CLS);
		blockWhileBtnDown(SPI_JSTK_LEFT_RIGHT,PMODJSTK_BTN1,jystkLeftRightLedState);
		PmodJSTKDelay15us(0);
		getJstkValsOnBTNDown(SPI_JSTK_LEFT_RIGHT,&jstkAxisBtnCalibrate,PMODJSTK_BTN1,jystkLeftRightLedState);
		PmodJSTKDelay15us(0);
		jstkAxisRange.xAxisMax = jstkAxisBtnCalibrate.xAxis;
		
		//Turn off LD1
		jystkLeftRightLedState = PMODJSTK_LD1_LD2_OFF;
		PmodJSTKSendRecv(SPI_JSTK_LEFT_RIGHT,jystkLeftRightLedState,&jstkAxisBtnCalibrate);	
	
		//Calculate axis range
		jstkAxisRange.yAxisRange = (jstkAxisRange.yAxisMax - jstkAxisRange.yAxisMin);
		jstkAxisRange.xAxisRange = (jstkAxisRange.xAxisMax - jstkAxisRange.xAxisMin);
		
	}while(jstkAxisRange.yAxisRange < 500 || jstkAxisRange.xAxisRange < 500);
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
	delayN(2000000);
	//Send reset low to initiate reset
	PORTClearBits(PORT_BIT_PMODBTN2_RESET);
 	while(PORTReadBits(PORT_BIT_PMODBTN2_CTS) == ctsPinState); //wait for CTS to go low, BT off
 
 	delayN(1000000); //delay so PmodBTN2 has to se the state change
 
 	//Send reset high to finish reset
    PORTSetBits(PORT_BIT_PMODBTN2_RESET);
    while(PORTReadBits(PORT_BIT_PMODBTN2_CTS) != ctsPinState);	//wait for CTS to go high, BT on
 
     //wait for module to fully initialize
 	delayN(2000000);
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
	UARTInit(UART_BLUETOOTH_BITRATE,PB_CLOCK,UART_BLUETOOTH,UART_ENABLE_PINS_CTS_RTS );
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
	PORTSetPinsDigitalOut(SPI_1_PIC32_460_512L_TRIS);

	//Allow toggling of board LED 1 for bluetooth connnection status;
	PORTSetPinsDigitalOut(PORT_BIT_LED_1);
	PORTClearBits(PORT_BIT_LED_1); //LED 1initially off
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


	//Optimize bluetooth for latency
	strcpy(params[0],"SQ");
	strcpy(params[1],"16");
	BTSendSetCommand(UART_BLUETOOTH,params,2);

	//Set mode to master
	strcpy(params[0],"SM");
	strcpy(params[1],"0");
	BTSendSetCommand(UART_BLUETOOTH,params,2);

	//Exit local configuration
	strcpy(params[0],COMMAND_EXIT);
	BTSendSetCommand(UART_BLUETOOTH,params,1);
	
	//Reset to allow changes to take effect
	resetBTModule();
}	


void __ISR(_TIMER_1_VECTOR, ipl2)Tmr1Handler_MainTaskLoop(void)
{	
	taskLoopTimerState = TASK_LOOP_TIMER_FIRED;	
	if(clsUpdateCounter < CLS_UPDATE_COUNT)
	{
		clsUpdateCounter++;	
	}
	pollBlueToothConnected();
	INTClearFlag(INT_T1);
}
