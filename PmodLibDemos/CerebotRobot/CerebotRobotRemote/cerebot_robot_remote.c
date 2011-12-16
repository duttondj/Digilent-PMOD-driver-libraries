/************************************************************************/
/*  cerebot_robot_remote.c -- implementation file for the cerebot 		*/
/*                            robot remote controller		            */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*              Copyright (C) 2011 Ryan Hoffman                         */
/************************************************************************/
/*  Module Description: 												*/
/*	See CerebotRemote.pdf for functional description and assembly		*/
/*  instructions.														*/
/*																		*/
/*  ------------------------------------------------------------------- */
/*  HARDWARE SETUP - Cerebot32MX4										*/
/*  ------------------------------------------------------------------- */
/*																		*/
/*  PmodBTN2															*/
/*  ------------------------------------------------------------------- */
/*  UART 2 (Pins 1 - 6) - A UART Crossover cable is required 			*/
/*  JH-01 	RF12	U2CTS												*/
/*  JH-02 	RF13	U2RTS												*/
/*  JH-03 	RF4		U2RX												*/
/*  JH-04 	RF5		U2TX												*/
/*  Pins(7-12)															*/
/*  JJ-01 	RB0     Connection Status									*/
/*  JJ-02 	RB1     Reset												*/
/*  JJ-03 	RB2 														*/
/*  JJ-04 	RB3															*/
/*	(no jumpers installed on PmodBTN2)									*/
/*																		*/
/*  PmodCLS																*/
/*  --------------------------------------------------------------------*/
/*  UART 1																*/
/*  JE-01 	RD14	U1CTS												*/
/*  JE-02 	RD15	U1RTS 												*/
/*  JE-03 	RF2		U1RX  												*/
/*  JE-04 	RF8		U1TX  												*/
/*  See jumper settings in PmodCLS ref. manual for baud rate jumper 	*/
/*  positions (9600 baud)												*/
/*  --------------------------------------------------------------------*/
/*   																	*/
/*  PmodJSTK 															*/
/*  --------------------------------------------------------------------*/
/*  JstkFWD_REV - SPI1 													*/
/*  JD-03 	RD9		SS1 												*/
/*  JH-08 	RD0		SDO1 												*/
/*  JK-10 	RC4		SDI1 												*/
/*  JD-09 	RD10	SCK1 												*/
/*   																	*/
/*  JstkLEFT_RIGHT - SPI2 												*/
/*  JB-01 	RG9		SS2 												*/
/*  JB-02 	RG8		SDO2  												*/
/*  JB-03 	RG7		SDI2 												*/
/*  JB-04 	RG6		SCK2 												*/
/*  --------------------------------------------------------------------*/
/*  LED1 (Bluetooth Connection status) 									*/
/*  RB-10 																*/
/*  																	*/
/*  Pmode8LD (Bluetooth Connection status) 								*/
/*  --------------------------------------------------------------------*/
/*  JC-01 	RG12 	LD0 												*/
/*  JC-02 	RG13 	LD1 												*/
/*  JC-03 	RG14 	LD2 												*/
/*  JC-04 	RG15	LD3 												*/
/*  JC-07 	RG0 	LD4 												*/
/*  JC-08 	RG1 	LD5 												*/
/*  JC-09 	RF0 	LD6 												*/
/*  JC-10 	RF1		LD7 												*/
/* 																		*/
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
#include <pmodBT.h>
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

//IO PORT/CHANNEL DEFINITIONS
#define UART_BLUETOOTH					UART2
#define UART_CLS						UART1
#define SPI_JSTK_FWD_REV				SPI_CHANNEL2
#define SPI_JSTK_LEFT_RIGHT				SPI_CHANNEL1
#define SPI_1_PIC32_460_512L_TRIS		IOPORT_D,BIT_0|BIT_9|BIT_10
#define PORT_BIT_PMODBTN2_STATUS		IOPORT_B,BIT_0
#define PORT_BIT_PMODBTN2_RESET			IOPORT_B,BIT_1
#define PORT_BIT_PMODBTN2_CTS			IOPORT_F,BIT_12
#define PORT_BIT_LD_1					IOPORT_B,BIT_10
#define PORT_BIT_LD8_0_5				IOPORT_G,BIT_0|BIT_1|BIT_12|BIT_13|BIT_14|BIT_15
#define PORT_BIT_LD8_6_7				IOPORT_F,BIT_0|BIT_1

//Minimal range accepted for both axes after joystick calibration
#define JSTK_MIN_AXIS_RANGE				700

//Number of main loop cycles before CLS will update
#define CLS_UPDATE_COUNT				3

//Set to 0 to use board LD1, 1 to use Pmod8LD for bluetooth
//connection status, any other value for no LED status
#define BLUETOOTH_STAT_LED				1

//BIT RATE DEFINITIONS
#define UART_BLUETOOTH_BITRATE			115200
#define UART_CLS_BITRATE				9600
#define SPI_JSTK_BITRATE				156250

//BLUETOOTH PAIRING NAMES
#define BLUETOOTH_REMOTE_NAME			"CEREBOTREMOTE"
#define BLUETOOTH_ROBOT_NAME			"CEREBOTROBOT"

//RESPONSE RECIEVED ON CONNECT
#define BLUETOOTH_RESPONSE_CONNECT		"DEV_CONNECT\r\n"

//NUMBER OF TIMES TO TRY A CONNECTION
#define BLUETOOTH_CONNECTION_RETRIES	5

//DURATION IN SECONDS FOR BLUETOOTH INQUIRY
#define BLUETOOTH_INQUIRY_TIMEOUT		"5"

//BATTERY VOLTAGE SCALE
//3.3/(1024 bits/4(voltage divisor)) = 0.012890625, 6000mv * 0.012890625 ~= 78
#define BATTERY_SCALE  78

//MAIN APP TASK STATES
typedef enum
{
	STATE_CONNECT,
	STATE_DISCONNECTED,				
	STATE_POLLDEV,					
	STATE_SEND_MESSAGE,
	STATE_RECIEVE_MESSAGE,			
	STATE_UPDATE_CLS				
}MAIN_APP_TASK_STATES;

//ISR FIRED, EXECUTE MAIN TASK LOOP
typedef enum
{
	TASK_LOOP_TIMER_FIRED,
	TASK_LOOP_TIMER_RESET
}TASK_LOOP_TIMER_STATE;

#define CLS_DISPLAY_WIDTH 				17 //includes null terminator

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

//PmodCLS commands, see PmodCLS manual for descriptions
uint8_t enableDisplay[] = {27, '[', '3', 'e', '\0'}; //enable display
uint8_t setCursor[] = {27, '[', '0', 'c', '\0'}; 	 //turn off cursor
uint8_t homeRow2[] = {27,'[','1',';','0','H','\0'};  //set cursor to col 0, row 1
uint8_t homeCursor[] = {27, '[', 'j', '\0'}; 		 //set cursor to row 0, col 0, clear display
uint8_t wrapLine[] = {27, '[', '0', 'h', '\0'}; 	 //set display to "wrap"

//one line of display for CLS
uint8_t clsDisplayLine[CLS_DISPLAY_WIDTH];

//maintain stat of joystick LEDs
uint8_t jystkFwdRevLedState = PMODJSTK_LD1_LD2_OFF;
uint8_t jystkLeftRightLedState = PMODJSTK_LD1_LD2_OFF;

//main task loop state
uint8_t mainLoopState = STATE_CONNECT;

//main task loop timer state
uint8_t taskLoopTimerState = TASK_LOOP_TIMER_RESET;

//cls updates when counter reaches CLS_UPDATE_COUNT
uint8_t clsUpdateCounter = 0;

//Joystick calibration values
JSTK_AXIS_RANGE jstkAxisRange;

//Message sent from remote
CEREBOT_REMOTE_MSG cerebotRemoteMsg;
//Message recieved from robot
CEREBOT_ROBOT_MSG cerebotRobotMsg;

//values recieved from joysticks
PmodJSTKAxisButton jstkAxisBtnFwdRev;
PmodJSTKAxisButton jstkAxisBtnLeftRight;


/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */

void init();
void initControllers();
void UARTInit(uint32_t baudRate,uint32_t pbClock,UART_MODULE uartID,UART_CONFIGURATION configParams);
void initCLS();
void setPinIO();
void initPmodBT2();
void calibrateJoySticks();
void getJstkValsOnBTNDown(SpiChannel chn,PmodJSTKAxisButton *jstkAxisBtn,uint8_t btn,uint8_t ledState);
void blockWhileBtnDown(SpiChannel chn,uint8_t button,uint8_t ledState);
void connectToRemoteHost();
void resetJstkLedState();
void appTask();
void pollBlueToothConnected();
void pollJstkSetLEDState();
void sendMessage();
void enableTimers();
void recieveMessage();
void updateCLS();
void calculateAxisRange();
void checkMinMax();
uint8_t calcAxisDutyCycle(uint16_t axisValue, uint16_t axisMin,uint16_t axisRange);

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

/*
** 	main()
**
**	Synopsis:
**	Main for cerebot_robot_remote
**
**  Input: none
**
**  Returns: 0
**
**	Errors:	none
**
**  Description:
**
**  Main function for application, runs init, the launches app task
**  does not return.
*/
uint8_t main(void)
{
	init(); //initialize hardware
	appTask(); //begin primary app loop
	return 0;	
}	

/*  
** 	init()
**
**	Synopsis:
**	Initializes all hardware
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  Initializes all controllers, timers, interrupts and
**  performs joystick calibration.
*/
void init()
{
	initControllers();
	setPinIO();
	initCLS();
	resetJstkLedState();
	initPmodBT2();
	calibrateJoySticks();
	connectToRemoteHost();
	enableTimers();	
}	

/*  
**  appTask()
**
**	Synopsis:
**	Primary application loop, device polling,send/recieve messages
**  update CLS, connect to remote host on disconnect
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  To control the rate at which the task loop executes
**  timer 1 sets the taskerLoopTimerState variable, when
**  it is equal to TASK_LOOP_TIMER_FIRED, one full task
**  sequence executes (see design reference CerebotRemote.pdf
**  for state transition diagram). Once the task loop completes 
**  without a bluetooth disconnect the timer state is set to 
**  TASK_LOOP_TIMER_RESET 
*/
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
					//Disabling and enabling UART flushes FIFO
					UARTEnable(UART_BLUETOOTH, UART_DISABLE);
				 	mainLoopState = STATE_CONNECT;
					UARTEnable(UART_BLUETOOTH, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
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
			}
		}
	}	
}

/*  
**  enableTimers()
**
**	Synopsis:
**	Enables T1 for CLS update counter, polling for Bluetooth connectivity
**  Setting timer fired flag for appTask loop.
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
*/
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
	
/*  
**  sendMessage()
**
**	Synopsis:
**	Sends a message to the Cerebot Robot indicating direction and speed
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  Pulls state from jstkAxisRange,jstkAxisBtnFwdRev,jstkAxisBtnLeftRight 
**  calculates the duty cycle for each axis and its intended direction, 
**  populates a CEREBOT_REMOTE_MSG struct and sends it to the Cerebot Remote
*/
void sendMessage()
{
	uint8_t byteCount = 0;
	uint8_t *msg = (uint8_t*)&cerebotRemoteMsg;
	uint8_t numBytes = sizeof(CEREBOT_REMOTE_MSG);

	//Calculate the percentage of axis throw for Fwd/Rev
	cerebotRemoteMsg.fwdRevSpeed = calcAxisDutyCycle(jstkAxisBtnFwdRev.yAxis, jstkAxisRange.yAxisMin,jstkAxisRange.yAxisRange);
	//Calculate the percentage of axis throw for Left/Right
	cerebotRemoteMsg.leftRightSpeed = calcAxisDutyCycle(jstkAxisBtnLeftRight.xAxis,jstkAxisRange.xAxisMin,jstkAxisRange.xAxisRange);
	//Determine if direction is Fwd or Rev
	cerebotRemoteMsg.vehicleDirectionFwdRev = (jstkAxisBtnFwdRev.yAxis >= jstkAxisRange.yAxisCenter)?ROBOT_DIR_FWD:ROBOT_DIR_REV;
	//Determine if direction is Left or Right
	cerebotRemoteMsg.vehicleDirectionLeftRight = (jstkAxisBtnLeftRight.xAxis >= jstkAxisRange.xAxisCenter)?ROBOT_TURN_RIGHT:ROBOT_TURN_LEFT;

	while(byteCount < numBytes && mainLoopState != STATE_DISCONNECTED)
	{
		while(!UARTTransmitterIsReady(UART_BLUETOOTH) && mainLoopState != STATE_DISCONNECTED);
		if(mainLoopState != STATE_DISCONNECTED)
		{
			UARTSendDataByte(UART_BLUETOOTH, *msg);
			byteCount++;
			msg++;
		}
	}
}

/*  
**  calcAxisDutyCycle()
**
**	Synopsis:
**	Calculates the percentage of deflection from center on a single
**  axis
**  Input: 
**		uint16_t axisValue - axis reading from joystick
**	    uint16_t axisMin - axis minimum detected during calibration
**		uint16_t axisRange - full range of axis deflection (axis max - axis min);
**  Returns:
**		uint8_t percentage of throw past center, used as duty cycle on the robot		
**
**	Errors:	none
**
**  Description:
**
**	Duty cycle is based on position from the center of the axis
**  as shown in the diagram below.
**
**			   100
**				*
**				*
**     	   100**0**100
**				*
**				*
**			   100
*/
uint8_t calcAxisDutyCycle(uint16_t axisValue, uint16_t axisMin,uint16_t axisRange)
{
	uint16_t axisPos = (uint16_t)(((axisValue - axisMin) * 100)/axisRange);
	if(axisPos > 50) //positive axis
	{	//mod 51 keeps 100% from turning into 0%
		return (axisPos % 51) * 2;
	}
	else if(axisPos < 50) //negative axis
	{
		return (50 - axisPos) * 2;
	}
	//is centered
	return 0;
}

/*  
** 	recieveMessage()
**
**	Synopsis:
**	Recieves status updates from the robot
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  Recieves Left Wheel RPM, Right Wheel RPM,
**  Battery Volatge, and FWD/REV direction 
**  from the robot, state is stored in global
**  cerebotRobotMsg
*/
void recieveMessage()
{
	uint8_t byteCount = 0;
	uint8_t *msg = (uint8_t*)&cerebotRobotMsg;
	uint8_t numBytes = sizeof(CEREBOT_ROBOT_MSG);

	while(byteCount < numBytes && mainLoopState != STATE_DISCONNECTED)
	{
		while(!UARTReceivedDataIsAvailable(UART_BLUETOOTH) && mainLoopState != STATE_DISCONNECTED);
		if(mainLoopState != STATE_DISCONNECTED)
		{
			*msg = UARTGetDataByte(UART_BLUETOOTH);
			msg++;
			byteCount++;
		}
	}
}		

/*  
**  updateCLS()
**
**	Synopsis:
**	Updates the PmodCLS display
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**	Updates the PmodCLS with Left Wheel RPM, Right Wheel RPM,
**  robot battery voltage, and robot direction (F/R), values
**  are pulled from global variable cerebotRobotMsg. Battery
**  voltage scale is calculated 3.3v bus/(1024 bits/4(voltage divisor)) = 0.012890625,
**	6000mv * 0.012890625 ~= 78
*/

void updateCLS()
{
	UARTPutS(homeCursor,UART_CLS);

	sprintf(clsDisplayLine,"LW: %3d RW: %3d",cerebotRobotMsg.leftWheelRPM,
											cerebotRobotMsg.rightWheelRPM);
	UARTPutS(clsDisplayLine,UART_CLS);
	UARTPutS(homeRow2,UART_CLS);
	sprintf(clsDisplayLine,"V+: %d.%1d Dir: %c",cerebotRobotMsg.batteryVoltage / BATTERY_SCALE, ((cerebotRobotMsg.batteryVoltage % BATTERY_SCALE) * 10 )/BATTERY_SCALE , 
							 (cerebotRobotMsg.vehicleDirection == ROBOT_DIR_FWD)?'F':'R');
	UARTPutS(clsDisplayLine,UART_CLS);									
}	

/*  
**  pollBlueToothConnected()
**
**	Synopsis:
**	Polls PmodBT2 for connectivity, set state and LEDs
**  according to connect/disconnect state.
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**	Polls PmodBT2 STATUS pin for connectivity, if LOW
**  not connected, turn off status LED, main loop state is 
**  set to STATE_DISCONNECTED, if HIGH state does not change
**  LED's are illuminated.
**
**  LED Options (conditional compiling):
**  BLUETOOTH_STAT_LED: Set to 0 to use Cerebot LD1, 1 to use 
**  Pmod8LD for bluetooth connection status, any other value 
**  for no LED status
*/
void pollBlueToothConnected()
{
	if(PORTReadBits(PORT_BIT_PMODBTN2_STATUS) == BIT_0)
	{
		#if(BLUETOOTH_STAT_LED == 0)
		PORTSetBits(PORT_BIT_LD_1);
		
		#elif(BLUETOOTH_STAT_LED == 1)
		PORTSetBits(PORT_BIT_LD8_0_5);
		PORTSetBits(PORT_BIT_LD8_6_7);
	 	
		#endif
	}
	else
	{	
		mainLoopState = STATE_DISCONNECTED;
		
		#if(BLUETOOTH_STAT_LED == 0)
		PORTClearBits(PORT_BIT_LD_1); 	
		
		#elif(BLUETOOTH_STAT_LED == 1)
		PORTClearBits(PORT_BIT_LD8_0_5);
		PORTClearBits(PORT_BIT_LD8_6_7);
		
		#endif
	}	
}	
/*  
**  pollJstkSetLEDState()
**
**	Synopsis:
**
**	Polls both joysticks, sets global joystick state and checks
**  axis bounds.
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  Polls both joysticks and places their current state in global 
**  variables jstkAxisBtnFwdRev and jstkAxisBtnLeftRight.
**  checkMinMax() is called once polling has completed.
*/
void pollJstkSetLEDState()
{
	PmodJSTKSendRecv(SPI_JSTK_FWD_REV,jystkFwdRevLedState,&jstkAxisBtnFwdRev);	
	PmodJSTKSendRecv(SPI_JSTK_LEFT_RIGHT,jystkLeftRightLedState,&jstkAxisBtnLeftRight);	
 	checkMinMax();
}	

/*  
**  checkMinMax()
**
**	Synopsis:
**
**  Checks bounds of joystick against current state and adjusts
**  if they have been exceeded
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**	Due to innacurate calibration, joystick reading may fall outside
**  of the in/max initially measured introducing errors in duty cycle
**  calculations. This function checks values recieved for both the x 
**  and y axes, if they differ global state is updated with the new 
**  value and axis range is recalculated.
*/
void checkMinMax()
{
	uint8_t rangeChanged = 0;
	
	//Y axis above max or below min
	if(jstkAxisBtnFwdRev.yAxis > jstkAxisRange.yAxisMax)
	{
		jstkAxisRange.yAxisMax = jstkAxisBtnFwdRev.yAxis;
		rangeChanged = 1;
	}
	else if(jstkAxisBtnFwdRev.yAxis < jstkAxisRange.yAxisMin)
	{
		jstkAxisRange.yAxisMin = jstkAxisBtnFwdRev.yAxis;
		rangeChanged = 1;
	}
	
	//X axis above max or below min
	if(jstkAxisBtnLeftRight.xAxis > jstkAxisRange.xAxisMax)
	{
		jstkAxisRange.xAxisMax = jstkAxisBtnLeftRight.xAxis;
		rangeChanged = 1;
	}
	else if(jstkAxisBtnLeftRight.xAxis < jstkAxisRange.xAxisMin)
	{
		jstkAxisRange.xAxisMin = jstkAxisBtnLeftRight.xAxis;
		rangeChanged = 1;
	}
	//one of the axis bounds changed, recalc overall range
	if(rangeChanged)
	{
		calculateAxisRange();
	}
	
}

/*  
**  resetJstkLedState()
**
**	Synopsis:
**	Resets Joystick LED state to both off and turns 
**	all LEDs both joysticks off.
**
**  Input: 
**
**  Returns: none
**
**	Errors:	none
*/
void resetJstkLedState()
{
	PmodJSTKAxisButton jstkAxisBtn;
	jystkFwdRevLedState = PMODJSTK_LD1_LD2_OFF;
	jystkLeftRightLedState = PMODJSTK_LD1_LD2_OFF;
	PmodJSTKSendRecv(SPI_JSTK_FWD_REV,jystkFwdRevLedState,&jstkAxisBtn);	
	PmodJSTKSendRecv(SPI_JSTK_LEFT_RIGHT,jystkLeftRightLedState,&jstkAxisBtn);
}	

/*  
**  connectToRemoteHost()
**
**	Synopsis:
**	Performs a Bluetooth inquire and attempts to connect to the robot
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**	
**	Performs a Bluetooth inquiry for BLUETOOTH_ROBOT_NAME, will retry 
**  BLUETOOTH_CONNECTION_RETRIES, if there is no connection the device
**  will need to be reset, otherwise the state is set to STATE_POLLDEV,
**  connection LEDs are illuminated.
*/
void connectToRemoteHost()
{
	UARTPutS(homeCursor,UART_CLS);
	UARTPutS("Connecting.....",UART_CLS);
	if(BTInquireAndConnect(UART_BLUETOOTH,BLUETOOTH_ROBOT_NAME,BLUETOOTH_INQUIRY_TIMEOUT,BLUETOOTH_CONNECTION_RETRIES,BLUETOOTH_RESPONSE_CONNECT))
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
/*  
**  blockWhileBtnDown()
**
**	Synopsis:
**  Blocks while the specified joystick button is depressed
**
**  Input: 
**		SpiChannel chn - Joystick Spi channel
**		uint8_t button - Joystick button value (see pmodJSTK.h)
**		uint8_t ledState - Joystick LED value (see pmodJSTK.h)
**
**  Returns: none
**
**	Errors:	none
*/
void blockWhileBtnDown(SpiChannel chn,uint8_t button,uint8_t ledState)
{
	PmodJSTKAxisButton jstkAxisBtn;
	do
	{
		PmodJSTKSendRecv(chn,ledState,&jstkAxisBtn);

	}while(jstkAxisBtn.buttonState == button);
}

/*  
**  calibrateJoySticks()
**
**	Synopsis:
**	Performs user assisted joystick calibration
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  Joystick range state is stored in global variable jstkAxisRange.
**  Both joysticks are polled to determine center values(joystick neutral),
**  The user is then prompted to deflect the axis fully in each direction,
**  holding until btn1 on the joystick in calibration has been depressed. 
**  The min and max for each axis is recorded, and the axis deflection range is 
**  determined. This value is used during duty cycle calculation as the divisor
**  for determining percentage of deflection from neutral.  Center values are 
**  used in determining the joysticks position releative to the origin in 
**  a positive or negative direction. Axis range must be greater than 
**  JSTK_MIN_AXIS_RANGE	for both axes, otherwise the calibration sequence will 
**  repeat.
*/
void calibrateJoySticks()
{
	
	PmodJSTKAxisButton jstkAxisBtnCalibrate;

	PmodJSTKSendRecv(SPI_JSTK_LEFT_RIGHT,jystkLeftRightLedState,&jstkAxisBtnCalibrate);
	jstkAxisRange.xAxisCenter = jstkAxisBtnCalibrate.xAxis;	

	PmodJSTKSendRecv(SPI_JSTK_FWD_REV,jystkFwdRevLedState,&jstkAxisBtnCalibrate);
	jstkAxisRange.yAxisCenter  = jstkAxisBtnCalibrate.yAxis;
	
	do
	{
		//Turn on LD1, indcates joystick under calibration
		jystkFwdRevLedState = PMODJSTK_LD1_ON; //set joystick Fwd/Rev LED state
	
		UARTPutS(homeCursor,UART_CLS);		//Clear CLS
		UARTPutS("Calibrate Y-",UART_CLS);	//Calibrate Axis Instruction
		blockWhileBtnDown(SPI_JSTK_FWD_REV,PMODJSTK_BTN1,jystkFwdRevLedState); //in case the button is still down
		//Joystick state will be read while BTN1 is depressed, will exit upon release
		getJstkValsOnBTNDown(SPI_JSTK_FWD_REV,&jstkAxisBtnCalibrate,PMODJSTK_BTN1,jystkFwdRevLedState);
		//This is the yAxis min value
		jstkAxisRange.yAxisMin = jstkAxisBtnCalibrate.yAxis;
		
		UARTPutS(homeCursor,UART_CLS);
		UARTPutS("Calibrate Y+",UART_CLS);
		blockWhileBtnDown(SPI_JSTK_FWD_REV,PMODJSTK_BTN1,jystkFwdRevLedState);
		getJstkValsOnBTNDown(SPI_JSTK_FWD_REV,&jstkAxisBtnCalibrate,PMODJSTK_BTN1,jystkFwdRevLedState);
		jstkAxisRange.yAxisMax = jstkAxisBtnCalibrate.yAxis;
		//Turn off LD1, calibration complete for this joystick
		jystkFwdRevLedState = PMODJSTK_LD1_LD2_OFF; //set joystick Fwd/Rev LED state
		PmodJSTKSendRecv(SPI_JSTK_FWD_REV,jystkFwdRevLedState,&jstkAxisBtnCalibrate);
		
		
		UARTPutS(homeCursor,UART_CLS);
		UARTPutS("Calibrate X-",UART_CLS);
		jystkLeftRightLedState = PMODJSTK_LD1_ON;
		blockWhileBtnDown(SPI_JSTK_LEFT_RIGHT,PMODJSTK_BTN1,jystkLeftRightLedState);
		getJstkValsOnBTNDown(SPI_JSTK_LEFT_RIGHT,&jstkAxisBtnCalibrate,PMODJSTK_BTN1,jystkLeftRightLedState);
		jstkAxisRange.xAxisMin = 	jstkAxisBtnCalibrate.xAxis;
	
		UARTPutS(homeCursor,UART_CLS);
		UARTPutS("Calibrate X+",UART_CLS);
		blockWhileBtnDown(SPI_JSTK_LEFT_RIGHT,PMODJSTK_BTN1,jystkLeftRightLedState);
		getJstkValsOnBTNDown(SPI_JSTK_LEFT_RIGHT,&jstkAxisBtnCalibrate,PMODJSTK_BTN1,jystkLeftRightLedState);
		jstkAxisRange.xAxisMax = jstkAxisBtnCalibrate.xAxis;
		
		//Turn off LD1
		jystkLeftRightLedState = PMODJSTK_LD1_LD2_OFF;
		PmodJSTKSendRecv(SPI_JSTK_LEFT_RIGHT,jystkLeftRightLedState,&jstkAxisBtnCalibrate);	
	
		//Calculate axis range values based on min/max
		calculateAxisRange();
		
	}while(jstkAxisRange.yAxisRange < JSTK_MIN_AXIS_RANGE || jstkAxisRange.xAxisRange < JSTK_MIN_AXIS_RANGE);
}

/*  
**  calculateAxisRange()
**
**	Synopsis:
**	Calculates aaxis defection range for both joysticks
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  Calculates and sets the axis range in global jstkAxisRange
**  by subtracting axis min from axis max for both axes.
**  These values are used during duty cycle calculation as the 
**  divisor for determining percentage of deflection from neutral. 
*/
void calculateAxisRange()
{
		jstkAxisRange.yAxisRange = (jstkAxisRange.yAxisMax - jstkAxisRange.yAxisMin);
		jstkAxisRange.xAxisRange = (jstkAxisRange.xAxisMax - jstkAxisRange.xAxisMin);
}

/*  
**  getJstkValsOnBTNDown()
**
**	Synopsis:
**  Polls joystick while the button specified is depressed
**
**  Input: 
**		SpiChannel chn - Joystick Spi channel
**		PmodJSTKAxisButton *jstkAxisBtn - joystick struct to populate with current state
**		uint8_t button - Joystick button value (see pmodJSTK.h)
**		uint8_t ledState - Joystick LED value (see pmodJSTK.h) 
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  Polls joystick while the button specified is depressed, populates the
**  PmodJSTKAxisButton struct passed in with current joystick state.
**  Function remains in blocking state until btn is released.
*/
void getJstkValsOnBTNDown(SpiChannel chn,PmodJSTKAxisButton *jstkAxisBtn,uint8_t btn,uint8_t ledState)
{
	do
	{
		PmodJSTKSendRecv(chn,ledState,jstkAxisBtn);

	}while(jstkAxisBtn->buttonState != btn);
}
	
/*  
** 	resetBTModule()
**
**	Synopsis:
**	Performs a hardware reset of the PmodBT2
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**	Performs a hardware reset of the PmodBT2 by driving the
**  the reset pin low, waiting for CTS to go low indicating the
**  the module has reset, delay, drive rst high, wait for CTS 
**  to go high indicating the module is powered on, delay to
**  allow for initialization to complete.
*/
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
**  initCLS()
**
**	Synopsis:
**	Initializes the PmodCLS
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  Initializes the PmodCLS by enableing the device,
**  turning the cursor off, setting cursor position
**  to Row 0, Col 0, clearing the display and displaying
**  the text "Initializing..."
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
**  initControllers()
**
**	Synopsis:
**
**	Initialize UART and SPI controllers
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**	Initialize UART and SPI controllers for the following:
**  PmodCLS
**  PmodBT2
**  PmodJSTK - (fwd/rev)
**  PmodJSTK - (left/right)
**  Channel/UART definitions are defined as macros at the top
**  of this file.	
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
/*  
** 	setPinIO()
**
**	Synopsis:
**	Sets pin IO direction
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  Sets pin IO direction for PmodBT2,SPI1, Pmod8LD, LD1
**  SPI1 is set here due to an error in the Microchip spi init
**  improperly setting the TRIS bits.
*/
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
	
	#if(BLUETOOTH_STAT_LED == 0)
	//Allow toggling of board LED 1 for bluetooth connnection status;
	PORTSetPinsDigitalOut(PORT_BIT_LD_1);
	PORTClearBits(PORT_BIT_LD_1); //LED 1initially off
	
	#elif (BLUETOOTH_STAT_LED == 1)
	
	//Allow toggling of PmodLED8 for bluetooth connnection status;
 	PORTSetPinsDigitalOut(PORT_BIT_LD8_0_5);
	PORTSetPinsDigitalOut(PORT_BIT_LD8_6_7);
	PORTClearBits(PORT_BIT_LD8_0_5);
	PORTClearBits(PORT_BIT_LD8_6_7);
	#endif
}		
/*  
**  UARTInit()
**
**	Synopsis:
**	Initialize the specified UART
**
**  Input: 
**		uint32_t baudRate - baud rate in bps
**		uint32_t pbClock - peripheral bus clock in Hz
**		UART_MODULE uartID - uart module ID (see plib docs)
**		UART_CONFIGURATION configParams - UART configuration paramters (see plib docs)
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**	
**	Initializes the specified UARt at the bit rate and configuation parameters specified,
**  Line Control:8 bits, no parity, 1 stop bit
**  Fifo: TX not full, RX not full
**  Enable: RX/TX
*/
void UARTInit(uint32_t baudRate,uint32_t pbClock,UART_MODULE uartID,UART_CONFIGURATION configParams)
{
	UARTConfigure(uartID,configParams);

    UARTSetFifoMode(uartID, UART_INTERRUPT_ON_RX_NOT_EMPTY | UART_INTERRUPT_ON_TX_NOT_FULL );

    UARTSetLineControl(uartID, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);

    UARTSetDataRate(uartID, pbClock, baudRate);

    UARTEnable(uartID, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

}	

/*  
**  initPmodBT2()
**
**	Synopsis:
**
**	Initializes the PmodBT2
**
**  Input: 
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**	The module is initially reset, set to default settings,
**  reset again, then conigurated as a SPP master. See 
**  comments in this section for a description of parameters 
**  configured. All commands and settings for the PmodBTN2 
**	can be found in the Roving Networks RN-42 Advanced User 
**  Manual.
**
**  Notes:
**  Unless otherwise set below, the PmodBTN2 will use its
**  default settings.
*/
void initPmodBT2()
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
	strcpy(params[1],BLUETOOTH_REMOTE_NAME);
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

/*  
**  Tmr1Handler_MainTaskLoop()
**
**	Synopsis:
**
**	Timer handler for Timer1
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**	
**	Set taskLoopTimerState to TASK_LOOP_TIMER_FIRED,
**  increments the CLS update counter and checks
**  if Bluetooth is connected.
*/
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
