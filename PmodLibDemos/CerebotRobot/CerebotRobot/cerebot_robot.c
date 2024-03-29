/************************************************************************/
/*  cerebot_robot.c -- Cerebot robot implementation file                */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*              Copyright (C) 2011 Ryan Hoffman                         */
/************************************************************************/
/*  Module Description: 												*/
/*	See CerebotRobot.pdf for functional description and assembly		*/
/*  instructions.														*/																		
/*  ------------------------------------------------------------------- */
/*  HARDWARE SETUP	- Cerebot32MX4										*/
/*  ------------------------------------------------------------------- */
/*																		*/
/*  PmodBTN2															*/
/*  ------------------------------------------------------------------- */
/*  UART 2 (Pint 1 - 6) - A UART Crossover cable is required 			*/
/*  JH-01 	RF12	U2CTS												*/
/*  JH-02 	RF13	U2RTS												*/
/*  JH-03 	RF4		U2RX												*/
/*  JH-04 	RF5		U2TX												*/
/*  Pins(7-12)															*/
/*  JJ-01 	RB0     Connection Status									*/
/*  JJ-02 	RB1     Reset												*/
/*  JJ-03 	RB2 														*/
/*  JJ-04 	RB3															*/
/*																		*/
/*  PmodHB5 (Fwd/Rev)													*/
/*  ------------------------------------------------------------------- */
/*  JD-01	RD7		 Direction 											*/
/*  JD-02	RD1		 Enable												*/
/*  JD-03 	RD9	     SensorA											*/
/*  JD-04	RC1	     SensorB											*/
/*																		*/
/*  PmodHB5 (Left/Right)												*/
/*  ------------------------------------------------------------------- */
/*  JD-07 	RD6		 Direction											*/
/*  JD-08   RD2		 Enable												*/
/*  JD-09   RD10	 SensorA											*/
/*  JD-10   RC2		 SensorB											*/
/*																		*/
/*  Voltage Monitor														*/
/*  ------------------------------------------------------------------- */
/*	JP4 - Shorting block installed										*/
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

//SYSTEM CLOCK - PB_CLOCK VALUES
#define SYS_FREQ             			(80000000L)
#define PB_DIV                 		 	2
#define PB_CLOCK					 	SYS_FREQ/PB_DIV

//TIMER SCALE/TICK DEFINITIONS
#define PRESCALE_T1_T2_T3         		8
#define TOGGLES_PER_SEC_T1				6000
#define TOGGLES_PER_SEC_T2_T3       	1000
#define T1_TICK							(SYS_FREQ/PB_DIV/PRESCALE_T1_T2_T3/TOGGLES_PER_SEC_T1)
#define T2_T3_TICK               		(SYS_FREQ/PB_DIV/PRESCALE_T1_T2_T3/TOGGLES_PER_SEC_T2_T3)

//BLUETOOTH UART ISR HANDLER VECTOR
#define BLUETOOTH_UART_ISR _UART_2_VECTOR  //Must be vector for UART_BLUETOOTH

//IO PORT/CHANNEL DEFINITIONS

//BLUETOOTH IO PORT/BIT DEFINITIONS
#define UART_BLUETOOTH					UART2 //Must be UART for BLUETOOTH_UART_ISR
#define PORT_BIT_PMODBTN2_STATUS		IOPORT_B,BIT_0
#define PORT_BIT_PMODBTN2_RESET			IOPORT_B,BIT_1
#define PORT_BIT_PMODBTN2_CTS			IOPORT_F,BIT_12

//BLUETOOTH CONNECTED LED IOPORT/BIT DEFINITION
#define PORT_BIT_LED_WAITING			IOPORT_B,BIT_10 //Waiting for connection
#define PORT_BIT_LED_CONNECTED			IOPORT_B,BIT_10|BIT_11|BIT_12|BIT_13 //Connected

//HBRIDGE IO PORT/BIT DEFINITIONS
#define PORT_HB_LEFT_WHEEL_DIR 			IOPORT_D		//HB5 Direction
#define PORT_HB_LEFT_WHEEL_EN 			IOPORT_D		//HB5 Enable
#define PORT_HB_LEFT_WHEEL_SA  			IOPORT_D		//HB5 Sensor A
#define PORT_HB_LEFT_WHEEL_SB  			IOPORT_C 		//HB5 Sensor B

#define BIT_HB_LEFT_WHEEL_DIR 			BIT_7			//HB5 Direction
#define BIT_HB_LEFT_WHEEL_EN 			BIT_1			//HB5 Enable
#define BIT_HB_LEFT_WHEEL_SA  			BIT_9			//HB5 Sensor A
#define BIT_HB_LEFT_WHEEL_SB  			BIT_1			//HB5 Sensor B

#define PORT_HB_RIGHT_WHEEL_DIR 		IOPORT_D	
#define PORT_HB_RIGHT_WHEEL_EN 			IOPORT_D
#define PORT_HB_RIGHT_WHEEL_SA  		IOPORT_D	
#define PORT_HB_RIGHT_WHEEL_SB  		IOPORT_C

#define BIT_HB_RIGHT_WHEEL_DIR 			BIT_6
#define BIT_HB_RIGHT_WHEEL_EN 			BIT_2
#define BIT_HB_RIGHT_WHEEL_SA  			BIT_10
#define BIT_HB_RIGHT_WHEEL_SB  			BIT_2

//TIMER PERIOD REGISTERS
#define TIMER_PR_LEFT					PR2
#define TIMER_PR_RIGHT					PR3

//BLUETOOTH DEVICE NAME
#define BLUETOOTH_NAME					"CEREBOTROBOT" 

//DC MOTOR REDUCTION RATIO  (divisor)
#define DC_MOTOR_REDUCTION_RATIO		19

//Text recieved from remote on connect 
#define BLUETOOTH_RESPONSE_CONNECT		"DEV_CONNECT\r\n" 

//BIT RATE DEFINITIONS
#define UART_BLUETOOTH_BITRATE			115200

//MAIN APP TASK STATES
typedef enum
{
 	STATE_WAITING_CONNECT,
 	STATE_WAIT_RECIEVE_MESSAGE,
 	STATE_RECIEVE_MESSAGE,		
	STATE_SEND_MESSAGE,				
 	STATE_CHK_CHG_DIR,				
 	STATE_SET_DUTY_CYCLE,
	STATE_DISCONNECTED		
}MAIN_APP_TASK_STATES;

//Scale the wheel scale drift value (cummulativeWheelScaleDrift)
#define WHEELSCALE_DRIFT_DIV			2
//Scale the the input for left/right
#define WHEELSCALE_LEFT_RIGHT_DIV		3
//Axis values less than this value are considered 0 (center)
#define WHEELSCALE_CENTER_AXIS_THRESH	8

//ADC - BATTERY VOLTAGE
// define setup parameters for OpenADC10
// Turn module on | ouput in integer | trigger mode auto | enable autosample
#define PARAM1  ADC_MODULE_ON | ADC_FORMAT_INTG | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON
// define setup parameters for OpenADC10
// ADC ref external    | disable offset test    | disable scan mode | perform 2 samples | use dual buffers | use alternate mode
#define PARAM2  ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_OFF | ADC_SAMPLES_PER_INT_2 | ADC_ALT_BUF_ON | ADC_ALT_INPUT_ON
// define setup parameters for OpenADC10
// use ADC internal clock | set sample time
#define PARAM3  ADC_CONV_CLK_INTERNAL_RC | ADC_SAMPLE_TIME_15
// define setup parameters for OpenADC10
// do not assign channels to scan
#define PARAM4    SKIP_SCAN_ALL
// define setup parameters for OpenADC10
// set AN8 as analog input
#define PARAM5    ENABLE_AN8_ANA

//HB5 array indexes
typedef enum
{
	HB_LEFT_WHEEL = 0, //ensure we start at 0 since these are array index positions
	HB_RIGHT_WHEEL					
}HB_WHEEL_INDEX;

//Number of HB5 modules
#define HB5_NUM_MODULES 				2

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

//PmodHB5 state
HBRIDGE hbridges[HB5_NUM_MODULES];

//Bluetooth UART recieved data flag
uint8_t UART_BT_RxData  = 0;

//HB5 has changed motor direction
uint8_t directionChangeComplete = 0;

//Current FWD/REV direction of robot
CEREBOT_ROBOT_DIR currentDirection = ROBOT_DIR_FWD;

//Processing loop state, initalize as waiting for a connection
uint8_t mainLoopState = STATE_WAITING_CONNECT;

//Message recieved from remote
CEREBOT_REMOTE_MSG cerebotRemoteMsg;
//Message sent from robot
CEREBOT_ROBOT_MSG cerebotRobotMsg;

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */

void setDutyCycle();
void configureTimers();
void initADC10();
void setPortIO();
void resetBTModule();
void initControllers();
void enableInterrupts();
uint8_t pollBlueToothConnected();
void initPmodBTN2();
void UARTInit(uint32_t baudRate,uint32_t pbClock,UART_MODULE uartID,UART_CONFIGURATION configParams);
void recieveMessage();
void sendMessage();
uint8_t recieveConnect();
void appTask();
void initHbridge();
void changeDirection();
void init();

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
	init();
	appTask();
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
**  Initializes all controllers, timers, and interrupts.
**
*/
void init()
{
	initControllers();
	initADC10();
	configureTimers();
	setPortIO();
 	initHbridge();
	initPmodBTN2();
	enableInterrupts();
}

/*  
** 	appTask()
**
**	Synopsis:
**	Primary application loop, device polling,send/recieve messages
**  set motor duty cycle
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  Once a connection is made to the PmodBT2, and data is recieved
**  from the remote, the task sequence begins. When a complete 
**  sequence has been made, message processing will not occur until 
**  data has been recieved and the UART interrupt handler sets the 
**  UART_BT_RxData flag to 1.  (see design reference CerebotRobot.pdf
**  for state transition diagram).
**  
*/
void appTask()
{
	uint8_t hbIndex = 0;
	while(1)
	{
		switch(mainLoopState)
		{
			 case STATE_WAITING_CONNECT:
				break;

			 case STATE_WAIT_RECIEVE_MESSAGE: 

				mainLoopState = (UART_BT_RxData)?STATE_RECIEVE_MESSAGE:STATE_WAIT_RECIEVE_MESSAGE;
			 	break;

			 case STATE_RECIEVE_MESSAGE:

			    UART_BT_RxData = 0;
				if(mainLoopState != STATE_DISCONNECTED)
				{
					recieveMessage();
					if(mainLoopState != STATE_DISCONNECTED)
					{
						mainLoopState = STATE_SEND_MESSAGE;	
					}
				}
				break;

			 case STATE_SEND_MESSAGE:
			 	if(mainLoopState != STATE_DISCONNECTED)
				{
					sendMessage();
					mainLoopState = STATE_CHK_CHG_DIR;	
				}
				break;

			 case STATE_CHK_CHG_DIR:
				//only execute if direction has changed
				if(currentDirection != cerebotRemoteMsg.vehicleDirectionFwdRev && mainLoopState != STATE_DISCONNECTED)
				{
					currentDirection = cerebotRemoteMsg.vehicleDirectionFwdRev;
					changeDirection();
				}

				if(mainLoopState != STATE_DISCONNECTED)
				{
					mainLoopState = STATE_SET_DUTY_CYCLE;
				}
				break;

			 case STATE_SET_DUTY_CYCLE:

				if(directionChangeComplete)
				{
					setDutyCycle();
				}

			  	if(mainLoopState != STATE_DISCONNECTED)
				{
			 		mainLoopState = STATE_WAIT_RECIEVE_MESSAGE;
				}
			 	break;	
			 case STATE_DISCONNECTED:
					//Disabling and enabling UART flushes FIFO
					UARTEnable(UART_BLUETOOTH, UART_DISABLE);
					UARTEnable(UART_BLUETOOTH, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX)); 
					mainLoopState = STATE_WAITING_CONNECT;
				break;
		}
	}		
}

/*  
**  changeDirection()
**
**	Synopsis:
**	Toggles the newDirection field on all hbridges 
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**	Toggles the change direction bit on all hbridges
*/
void changeDirection()
{
	uint8_t hbIndex = 0;
	for(hbIndex = 0;hbIndex < HB5_NUM_MODULES;hbIndex++)
	{
		//toggle direction on all HB5
  		hbridges[hbIndex].newDirection ^= (1 << 0);
	}
}

/*  
**  initHbridge()
**
**	Synopsis:
**	Initialize hbridge struct array
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  Initialize hbridge struct array using macros defined
**  at the top of this file, set initial motor dirction
*/
void initHbridge()
{
	memset(hbridges,0,sizeof(HBRIDGE) * HB5_NUM_MODULES);
	hbridges[HB_LEFT_WHEEL].sensorAport = 		PORT_HB_LEFT_WHEEL_SA;
	hbridges[HB_LEFT_WHEEL].sensorAportBit = 	BIT_HB_LEFT_WHEEL_SA;
	hbridges[HB_LEFT_WHEEL].sensorBport = 		PORT_HB_LEFT_WHEEL_SB;
	hbridges[HB_LEFT_WHEEL].sensorBportBit = 	BIT_HB_LEFT_WHEEL_SB;
	hbridges[HB_LEFT_WHEEL].directionPort = 	PORT_HB_LEFT_WHEEL_DIR;
	hbridges[HB_LEFT_WHEEL].directionPortBit = 	BIT_HB_LEFT_WHEEL_DIR;
	hbridges[HB_LEFT_WHEEL].currentDirection = 	PMOD_HB5_DIR_CW;
	hbridges[HB_LEFT_WHEEL].newDirection = 		PMOD_HB5_DIR_CW;
	hbridges[HB_LEFT_WHEEL].ocChannel = 		2;
	PORTSetBits(hbridges[0].directionPort,hbridges[0].directionPortBit);

	hbridges[HB_RIGHT_WHEEL].sensorAport = 		PORT_HB_RIGHT_WHEEL_SA;
	hbridges[HB_RIGHT_WHEEL].sensorAportBit = 	BIT_HB_RIGHT_WHEEL_SA;
	hbridges[HB_RIGHT_WHEEL].sensorBport = 		PORT_HB_RIGHT_WHEEL_SB;
	hbridges[HB_RIGHT_WHEEL].sensorBportBit = 	BIT_HB_RIGHT_WHEEL_SB;
	hbridges[HB_RIGHT_WHEEL].directionPort = 	PORT_HB_RIGHT_WHEEL_DIR;
	hbridges[HB_RIGHT_WHEEL].directionPortBit =	BIT_HB_RIGHT_WHEEL_DIR;
	hbridges[HB_RIGHT_WHEEL].currentDirection = PMOD_HB5_DIR_CCW;
	hbridges[HB_RIGHT_WHEEL].newDirection = 	PMOD_HB5_DIR_CCW;
	hbridges[HB_RIGHT_WHEEL].ocChannel = 		3;
	PORTClearBits(hbridges[1].directionPort,hbridges[1].directionPortBit);
}

/*  
**  sendMessage()
**
**	Synopsis:
**	Send message to remote
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**	Populates CEREBOT_ROBOT struct with left/right motor shaft RPM,
**	battery voltage and vehicle directions. Transmits to
**  remote using PmodBT2
*/
void sendMessage()
{
	uint8_t byteCount = 0;
	uint8_t *msg = (uint8_t*)&cerebotRobotMsg;
	uint8_t numBytes = sizeof(CEREBOT_ROBOT_MSG);
	//Get left motor RPM, divide by reduction ratio to get shaft RMP
	cerebotRobotMsg.leftWheelRPM = hbridges[HB_LEFT_WHEEL].rpm/DC_MOTOR_REDUCTION_RATIO;
	//Get right motor RPM, divide by reduction ratio to get shaft RMP
	cerebotRobotMsg.rightWheelRPM = hbridges[HB_RIGHT_WHEEL].rpm/DC_MOTOR_REDUCTION_RATIO;
	//Read battery voltage from ADC10
	cerebotRobotMsg.batteryVoltage = ReadADC10(8 * ((~ReadActiveBufferADC10() & 0x01)));
	//Direction the robot is currently traveling in
	cerebotRobotMsg.vehicleDirection = currentDirection;

	while(byteCount < numBytes && mainLoopState != STATE_DISCONNECTED)
	{
		while(!UARTTransmitterIsReady(UART_BLUETOOTH) && mainLoopState != STATE_DISCONNECTED);
		if(mainLoopState != STATE_DISCONNECTED)
		{
			UARTSendDataByte(UART_BLUETOOTH, *msg);
			msg++;
			byteCount++;
		}
	}	
}

/*  
**  recieveMessage()
**
**	Synopsis:
**	
**	Recieives a message from the remote.
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  Recieves a message from the remote, populates
**  global state variable cerebotRemoteMsg.
*/
void recieveMessage()
{
	uint8_t byteCount = 0;
	uint8_t *msg = (uint8_t*)&cerebotRemoteMsg;
	uint8_t numBytes = sizeof(CEREBOT_REMOTE_MSG);
	
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
**  initADC10()
**
**	Synopsis:
**	Initialize the Analog to Digital converter
**
**  Input: 
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**	
**  Initialize the Analog to Digital converter to sample AN8, which is the baterry 
**  voltage monitor on the Cerebot32MX4
*/
void initADC10()
{      
    // configure and enable the ADC
    CloseADC10();    // ensure the ADC is off before setting the configuration
   
     // use ground as neg ref for A | use AN8 for input A  
    // configure to sample AN8
    SetChanADC10( ADC_CH0_NEG_SAMPLEA_NVREF | ADC_CH0_POS_SAMPLEA_AN8); // configure to sample AN8

    OpenADC10( PARAM1, PARAM2, PARAM3, PARAM4, PARAM5 ); // configure ADC using parameter define above
    
	EnableADC10(); // Enable the ADC

    while ( ! mAD1GetIntFlag() ) { } // wait for the first conversion to complete so there will be vaild data in ADC result registers
}

/*  
**  initControllers()
**
**	Synopsis:
** 
**  Initialize peripheral controllers.
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**	Initialize peripheral controllers, currently only the Bluetooth UART needs initializing 
*/
void initControllers()
{
	//Initialize PmodBT2 UART
	UARTInit(UART_BLUETOOTH_BITRATE,PB_CLOCK,UART_BLUETOOTH,UART_ENABLE_PINS_CTS_RTS | UART_RTS_WHEN_RX_NOT_FULL);	
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
**  setDutyCycle()
**
**	Synopsis:
**	Set duty cycle for left and right wheels
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**	Sets the duty cycle for the left and right wheel based
**  on values recieved from the remote. If the if the direction is
**  left, then the duty cycle for the left wheel is reduced by
**  the cerebotRemoteMsg.vehicleDirectionLeftRight/WHEELSCALE_LEFT_RIGHT_DIV,
**  for right turns the right wheel duty cycle is reduced by this value.
**  In an attempt to maintain the same rpm for the left and right wheels
**  the when the left/right axis value is < WHEELSCALE_CENTER_AXIS_THRESH
**  on either side of the axis, a cummulative wheel scale value
**  is kept, adding or subtracting cummulativeWheelScaleDrift/WHEELSCALE_DRIFT_DIV
**  from the right wheel depending the current RPM of the left wheel.
*/
void setDutyCycle()
{
	uint8_t fwdRevDCScaleLeft = 0;
	uint8_t fwdRevDCScaleRight = 0;
	static int8_t cummulativeWheelScaleDrift = 0;

	//If fwd/rev > WHEELSCALE_CENTER_AXIS_THRESH then we should set duty cycle, otherwise we are still stopped
	//this is nessecary since center axis does not always report 0
	if(cerebotRemoteMsg.fwdRevSpeed > WHEELSCALE_CENTER_AXIS_THRESH)
	{ 
		fwdRevDCScaleLeft = cerebotRemoteMsg.fwdRevSpeed;
		fwdRevDCScaleRight = cerebotRemoteMsg.fwdRevSpeed;
		
		//If left/right < WHEELSCALE_CENTER_AXIS_THRESH then we are traveling in a straight line
		//this is nessecary since center axis does not always report 0
		if(cerebotRemoteMsg.leftRightSpeed < WHEELSCALE_CENTER_AXIS_THRESH &&  hbridges[HB_LEFT_WHEEL].rpm > 0) 
		{		
			 if( hbridges[HB_LEFT_WHEEL].rpm > hbridges[HB_RIGHT_WHEEL].rpm)
			{
				cummulativeWheelScaleDrift+=1;
			}
			else if( hbridges[HB_LEFT_WHEEL].rpm < hbridges[HB_RIGHT_WHEEL].rpm)
			{
				cummulativeWheelScaleDrift-=1;
			}
			
 			fwdRevDCScaleRight += (cummulativeWheelScaleDrift/WHEELSCALE_DRIFT_DIV);
		}
		else if(cerebotRemoteMsg.vehicleDirectionLeftRight == ROBOT_TURN_LEFT)
		{
			fwdRevDCScaleLeft -= (cerebotRemoteMsg.leftRightSpeed/WHEELSCALE_LEFT_RIGHT_DIV);
			if(fwdRevDCScaleLeft > 100) //we have rolled the uint8_t over/under, probably tried to set scale to negative value
			{
				fwdRevDCScaleLeft = fwdRevDCScaleRight;	
			}
		}
		else 
		{
			fwdRevDCScaleRight -= (cerebotRemoteMsg.leftRightSpeed/WHEELSCALE_LEFT_RIGHT_DIV);
			if(fwdRevDCScaleRight > 100)  //we have rolled the uint8_t over/under, probably tried to set scale to negative value
			{
				fwdRevDCScaleRight = fwdRevDCScaleLeft;	
			}
		}
	}
	//Set the duty cycle based on (MAXTIMER_VALUE * WHEEL_SCALE)/100 (percentage of max duty cycle)
	PmodHB5SetDCPWMDutyCycle((TIMER_PR_LEFT * fwdRevDCScaleLeft)/100,hbridges[HB_LEFT_WHEEL].ocChannel);	
	PmodHB5SetDCPWMDutyCycle((TIMER_PR_LEFT * fwdRevDCScaleRight)/100,hbridges[HB_RIGHT_WHEEL].ocChannel);			
}

/*  
**  configureTimers()
**
**	Synopsis:
**	Configures timers for interrupts and  OCs
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**	Configures timers, output comparers, sets duty cycle for both wheels to 0%;
*/
void configureTimers()
{

	//RPM/Quadrature encoding, direction change
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_8, T1_TICK);
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);

	//HB5 LEFT
    //Enable OC | 32 bit Mode  | Timer2 is selected | Continuous O/P   | OC Pin High , S Compare value, Compare value
    OpenOC2(OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0);
    OpenTimer2(T2_ON | T2_PS_1_8,T2_T3_TICK);

	//HB5 RIGHT
    //Enable OC | 32 bit Mode  | Timer3 is selected | Continuous O/P   | OC Pin High , S Compare value, Compare value
    OpenOC3(OC_ON | OC_TIMER3_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0);
    OpenTimer3(T3_ON | T3_PS_1_8, T2_T3_TICK);
	PmodHB5SetDCPWMDutyCycle(0,2);
	PmodHB5SetDCPWMDutyCycle(0,3);

}

/*  
**  setPortIO()
**
**	Synopsis:
**
**	Configures digital port IO
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**	Configure digital port IO for PmodBT2,
**	both PmodHB5, and LD 1-4. Set PmodBT2 reset high,
**  turn LD1-4 off. 
*/
void setPortIO()
{
	//Set digital in for PmodBTN2 connection status pin
	PORTSetPinsDigitalIn(PORT_BIT_PMODBTN2_STATUS);

	//Start PmodBT2 RST high
	PORTSetBits(PORT_BIT_PMODBTN2_RESET); 

	//Set digital out for PmodBT2 reset pin
	PORTSetPinsDigitalOut(PORT_BIT_PMODBTN2_RESET);	
	
	
	//setup HB5 PORT IO
	PORTSetPinsDigitalOut(PORT_HB_LEFT_WHEEL_DIR, BIT_HB_LEFT_WHEEL_DIR); //HB5 Direction
	PORTSetPinsDigitalOut(PORT_HB_LEFT_WHEEL_EN, BIT_HB_LEFT_WHEEL_EN); //HB5 Enable
	PORTSetPinsDigitalIn(PORT_HB_LEFT_WHEEL_SA, BIT_HB_LEFT_WHEEL_SA);  //HB5 Sensor A
	PORTSetPinsDigitalIn(PORT_HB_LEFT_WHEEL_SB, BIT_HB_LEFT_WHEEL_SB);  //HB5 Sensor B

	PORTSetPinsDigitalOut(PORT_HB_RIGHT_WHEEL_DIR, BIT_HB_RIGHT_WHEEL_DIR); //HB5 Direction
	PORTSetPinsDigitalOut(PORT_HB_RIGHT_WHEEL_EN, BIT_HB_RIGHT_WHEEL_EN); //HB5 Enable
	PORTSetPinsDigitalIn(PORT_HB_RIGHT_WHEEL_SA, BIT_HB_RIGHT_WHEEL_SA);  //HB5 Sensor A
	PORTSetPinsDigitalIn(PORT_HB_RIGHT_WHEEL_SB, BIT_HB_RIGHT_WHEEL_SB);  //HB5 Sensor B
	
	//Allow toggling of board LED 1-4 for bluetooth connnection status;
	PORTSetPinsDigitalOut(PORT_BIT_LED_CONNECTED);
	PORTClearBits(PORT_BIT_LED_CONNECTED); //All LEDs 1initially off
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
 
 	delayN(1000000); //delay so PmodBT2 has to se the state change
 
 	//Send reset high to finish reset
    PORTSetBits(PORT_BIT_PMODBTN2_RESET);
    while(PORTReadBits(PORT_BIT_PMODBTN2_CTS) != ctsPinState);	//wait for CTS to go high, BT on
 
     //wait for module to fully initialize
 	delayN(2000000);
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
**  reset again, then conigurated as a SPP slave. See 
**  comments in this section for a description of parameters 
**  configured. All commands and settings for the PmodBT2 
**	can be found in the Roving Networks RN-42 Advanced User 
**  Manual.
**
**  Notes:
**  Unless otherwise set below, the PmodBT2 will use its
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

	//Optimize bluetooth for latency
	strcpy(params[0],"SQ");
	strcpy(params[1],"16");
	BTSendSetCommand(UART_BLUETOOTH,params,2);

	//Set mode to slave
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
**  not connected, only LD1 will illuminate, main loop state is 
**  set to STATE_DISCONNECTED, if HIGH state does not change
**  LD1 - LD4 are illuminated.
*/
uint8_t pollBlueToothConnected()
{
	if(PORTReadBits(PORT_BIT_PMODBTN2_STATUS) == BIT_0)
	{
		PORTSetBits(PORT_BIT_LED_CONNECTED);
		return mainLoopState;
	}
	else
	{	
		//Connection lost, stop motors and processing loop

		PmodHB5SetDCPWMDutyCycle(0,hbridges[HB_LEFT_WHEEL].ocChannel);	
		PmodHB5SetDCPWMDutyCycle(0,hbridges[HB_RIGHT_WHEEL].ocChannel);	
		PORTClearBits(PORT_BIT_LED_CONNECTED);
		PORTSetBits(PORT_BIT_LED_WAITING); 	
		return STATE_DISCONNECTED;
	}	

}	

/*  
**  enableInterrupts()
**
**	Synopsis:
**	
**	Enables interrupts.
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  Enables the UART RX interupt for the PmodBT2
**  UART.
*/
void enableInterrupts()
{
	// Configure the interrupt priority, level 5
	INTDisableInterrupts();
	INTClearFlag(INT_SOURCE_UART_RX(UART_BLUETOOTH));//make sure interrupt flag is cleared

	//configure multi vector interrupts
	INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);

	INTSetVectorPriority(INT_VECTOR_UART(UART_BLUETOOTH), INT_PRIORITY_LEVEL_7);

	INTEnable(INT_SOURCE_UART_RX(UART_BLUETOOTH),INT_ENABLED);
	INTEnableInterrupts();
}
/*  
**  getQuadEncodingChangeDir()
**
**	Synopsis:
**
**	Gets the quadrature encoding from both wheels, 
**  and changes direction if needed
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  Checks for direction change completion on the left wheel(RPM went to 0
**  and direction bit toggled), calculates RPM for each wheel in 
**  PmodHB5getQEncRPM, state is maintained in HBRIDGE struct for each wheel.
*/
void getQuadEncodingChangeDir()
{
	uint8_t hbIndex = 0;
	directionChangeComplete = PmodHB5ChangeDirection(&(hbridges[HB_LEFT_WHEEL]));
	for(hbIndex = 0;hbIndex < HB5_NUM_MODULES;hbIndex++)
	{
		PmodHB5getQEncRPM(&(hbridges[hbIndex]),TOGGLES_PER_SEC_T1,5);
		directionChangeComplete &= PmodHB5ChangeDirection(&(hbridges[hbIndex]));
	}	
}
/*  
**  Timer1Handler
**
**	Synopsis:
**
**	Interrupt handler for Timer1	
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  Interrupt handler for Timer1, gets quadrature encoding/RPM
**  check for Bluetooth connectivity
*/
void __ISR(_TIMER_1_VECTOR, ipl2) Timer1Handler(void)
{	
	getQuadEncodingChangeDir();
	mainLoopState = pollBlueToothConnected();
  	INTClearFlag(INT_T1);
}
/*  
**  UARTIntHandler
**
**	Synopsis:
** 
**  Interrupt handler for Bluetooth UART RX
*
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**	Interrupt handler for Bluetooth UART RX, sets flag
**  used in main loop to signify Bluetooth data
**  recieved, sets main loop to STATE_RECIEVE_MESSAGE
**  when the first conection is made.
*/
void __ISR(BLUETOOTH_UART_ISR, ipl7) UARTIntHandler(void)
{
	if(INTGetFlag(INT_SOURCE_UART_RX(UART_BLUETOOTH)))
	{	
		UART_BT_RxData = 1;
		if(mainLoopState == STATE_WAITING_CONNECT)
		{
			mainLoopState = (BTRecieveConnect(UART_BLUETOOTH,BLUETOOTH_RESPONSE_CONNECT))?STATE_WAIT_RECIEVE_MESSAGE:STATE_WAITING_CONNECT;
		}
		INTClearFlag(INT_SOURCE_UART_RX(UART_BLUETOOTH));			
	}	
}
