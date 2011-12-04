/************************************************************************/
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*              Copyright (C) 2011 Ryan Hoffman                         */
/************************************************************************/
/*  Module Description: 												*/

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

//BLUETOOTH IO PORT DEFINITIONS
#define UART_BLUETOOTH					UART2
#define PORT_BIT_PMODBTN2_STATUS		IOPORT_B,BIT_0
#define PORT_BIT_PMODBTN2_RESET			IOPORT_B,BIT_1
#define PORT_BIT_PMODBTN2_CTS			IOPORT_F,BIT_12

//BLUETOOTH CONNECTED LED IOPORT DEFITION
#define PORT_BIT_LED_1					IOPORT_B,BIT_10

//HBRIDGE IO PORT DEFINITIONS
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

//BLUETOOTH DEVICE NAME
#define BLUETOOTH_NAME					"CEREBOTROBOT" 

//DC MOTOR REDUCTION RATION  (divisor)
#define DC_MOTOR_REDUCTION_RATIO		19

//Text recieved from remote on connect 
#define BLUETOOTH_RESPONSE_CONNECT		"DEV_CONNECT\r\n" 

//BIT RATE DEFINITIONS
#define UART_BLUETOOTH_BITRATE			115200

//MAIN APP TASK STATES
//Wait for connect, do not receieve message, also serves as disconnected state
#define STATE_WAITING_CONNECT			0 
//Recieve message from remote, UART interrupt has fired
#define STATE_RECIEVE_MESSAGE			1
//connected, but waiting for UART interrupt to fire on data recieved
#define STATE_WAIT_RECIEVE_MESSAGE		2


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
#define HB_LEFT_WHEEL					0
#define HB_RIGHT_WHEEL					1

//Number of HB5 modules
#define HB5_NUM_MODULES 				2

//PmodHB5 state
HBRIDGE hbridges[HB5_NUM_MODULES];

//Bluetooth UART recieved data flag
uint8_t UART_BT_RxData  = 0;

//HB5 has changed motor direction
uint8_t directionChangeComplete = 0;

//Current FWD/REV direction of robot
uint8_t currentDirection = ROBOT_DIR_FWD;

//Processing loop state, initalize as waiting for a connection
uint8_t mainLoopState = STATE_WAITING_CONNECT;



//Message recieved from remote
CEREBOT_REMOTE_MSG cerebotRemoteMsg;
//Message sent from robot
CEREBOT_ROBOT_MSG cerebotRobotMsg;

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
uint8_t main(void)
{
	init();
	appTask();
	return 0;	
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
void init()
{
	initControllers();
	initADC10();
	configureTimers();
 	initHbridge();
	setPortIO();
	initPmodBTN2();
	enableInterrupts();

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
void appTask()
{
	uint8_t hbIndex = 0;
	while(1)
	{

		switch(mainLoopState)
		{
			 case STATE_WAIT_RECIEVE_MESSAGE: 
				mainLoopState = (UART_BT_RxData)?STATE_RECIEVE_MESSAGE:STATE_WAIT_RECIEVE_MESSAGE;
			 	break;
			 case STATE_RECIEVE_MESSAGE:
			    UART_BT_RxData = 0;
				recieveMessage();
			 	sendMessage();
				if(currentDirection != cerebotRemoteMsg.vehicleDirectionFwdRev)
				{
					currentDirection = cerebotRemoteMsg.vehicleDirectionFwdRev;
					changeDirection();
				}
				setDutyCycle();
			  	if(mainLoopState != STATE_WAITING_CONNECT)
			 		mainLoopState = STATE_WAIT_RECIEVE_MESSAGE;
			 	break;
			 
		}
	}
		
	
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
	PORTSetBits(PORT_HB_LEFT_WHEEL_DIR,BIT_HB_LEFT_WHEEL_DIR); //set initial direction CW

	hbridges[HB_RIGHT_WHEEL].sensorAport = 		PORT_HB_RIGHT_WHEEL_SA;
	hbridges[HB_RIGHT_WHEEL].sensorAportBit = 	BIT_HB_RIGHT_WHEEL_SA;
	hbridges[HB_RIGHT_WHEEL].sensorBport = 		PORT_HB_RIGHT_WHEEL_SB;
	hbridges[HB_RIGHT_WHEEL].sensorBportBit = 	BIT_HB_RIGHT_WHEEL_SB;
	hbridges[HB_RIGHT_WHEEL].directionPort = 	PORT_HB_RIGHT_WHEEL_DIR;
	hbridges[HB_RIGHT_WHEEL].directionPortBit =	BIT_HB_RIGHT_WHEEL_DIR;
	hbridges[HB_RIGHT_WHEEL].currentDirection = PMOD_HB5_DIR_CCW;
	hbridges[HB_RIGHT_WHEEL].newDirection = 	PMOD_HB5_DIR_CCW;
	hbridges[HB_RIGHT_WHEEL].ocChannel = 		3;
	PORTClearBits(PORT_HB_RIGHT_WHEEL_DIR,BIT_HB_RIGHT_WHEEL_DIR); //set initial direction CCW
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
void sendMessage()
{
	uint8_t byteCount = 0;
	uint8_t *msg = (uint8_t*)&cerebotRobotMsg;
	uint8_t lrc = 0;
	cerebotRobotMsg.leftWheelRPM = hbridges[HB_LEFT_WHEEL].rpm/DC_MOTOR_REDUCTION_RATIO;
	cerebotRobotMsg.rightWheelRPM = hbridges[HB_RIGHT_WHEEL].rpm/DC_MOTOR_REDUCTION_RATIO;
	cerebotRobotMsg.batteryVoltage = ReadADC10(8 * ((~ReadActiveBufferADC10() & 0x01)));
	cerebotRobotMsg.vehicleDirection = currentDirection;
	for(byteCount = 0;byteCount < sizeof(CEREBOT_ROBOT_MSG);byteCount++)
	{
		while(!UARTTransmitterIsReady(UART_BLUETOOTH) && mainLoopState != STATE_WAITING_CONNECT);
		UARTSendDataByte(UART_BLUETOOTH, *msg);
		msg++;
	}	

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
void recieveMessage()
{
	uint8_t byteCount = 0;
	uint8_t oneByte = 0;
	uint8_t *msg = (uint8_t*)&cerebotRemoteMsg;
	uint8_t numBytes = sizeof(CEREBOT_REMOTE_MSG);
	
	for(byteCount = 0;byteCount < numBytes;byteCount++)
	{
		while(!UARTReceivedDataIsAvailable(UART_BLUETOOTH) && mainLoopState != STATE_WAITING_CONNECT);
		oneByte = UARTGetDataByte(UART_BLUETOOTH);
		*msg = oneByte;
		msg++;
	}
	
	
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
*/
void setDutyCycle()
{
	double fwdRevDCScaleLeft = cerebotRemoteMsg.fwdRevSpeed/100.0;
	double fwdRevDCScaleRight = fwdRevDCScaleLeft;
	static int8_t cummulativeWheelScale = 0;
	if(directionChangeComplete)
	{
		if(cerebotRemoteMsg.leftRightSpeed < 6 &&  hbridges[HB_LEFT_WHEEL].rpm > 0) 
		{
			
			 if( hbridges[HB_LEFT_WHEEL].rpm > hbridges[HB_RIGHT_WHEEL].rpm)
			{
				cummulativeWheelScale+=1;
			}
			else if( hbridges[HB_LEFT_WHEEL].rpm < hbridges[HB_RIGHT_WHEEL].rpm)
			{
				cummulativeWheelScale-=1;
			}
			
 			fwdRevDCScaleRight += (cummulativeWheelScale / 200.0);
	
		}
		else if(cerebotRemoteMsg.vehicleDirectionLeftRight == ROBOT_TURN_LEFT)
		{
			fwdRevDCScaleLeft -= (cerebotRemoteMsg.leftRightSpeed/250.0);
			if(fwdRevDCScaleLeft < 0)
			{
				fwdRevDCScaleLeft = fwdRevDCScaleRight;	
			}

		}
		else 
		{
			fwdRevDCScaleRight -= (cerebotRemoteMsg.leftRightSpeed/250.0);
			if(fwdRevDCScaleRight < 0)
			{
				fwdRevDCScaleRight = fwdRevDCScaleLeft;	
			}

		}
		
		PmodHB5SetDCPWMDutyCycle(PR2 * fwdRevDCScaleLeft,hbridges[HB_LEFT_WHEEL].ocChannel);	
		PmodHB5SetDCPWMDutyCycle(PR3 * fwdRevDCScaleRight,hbridges[HB_RIGHT_WHEEL].ocChannel);		
	}
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
void setPortIO()
{
	//Set digital in for PmodBTN2 connection status pin
	PORTSetPinsDigitalIn(PORT_BIT_PMODBTN2_STATUS);

	//Start PmodBTN2 RST high
	PORTSetBits(PORT_BIT_PMODBTN2_RESET); 

	//Set digital out for PmodBTN2 reset pin
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
	
	//Allow toggling of board LED 1 for bluetooth connnection status;
	PORTSetPinsDigitalOut(PORT_BIT_LED_1);
	PORTClearBits(PORT_BIT_LED_1); //LED 1initially off
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
uint8_t pollBlueToothConnected()
{
	if(PORTReadBits(PORT_BIT_PMODBTN2_STATUS) == BIT_0)
	{
		PORTSetBits(PORT_BIT_LED_1); 
		return mainLoopState;
	}
	else
	{	
		//Connection lost, stop motors and processing loop

		PmodHB5SetDCPWMDutyCycle(0,hbridges[HB_LEFT_WHEEL].ocChannel);	
		PmodHB5SetDCPWMDutyCycle(0,hbridges[HB_RIGHT_WHEEL].ocChannel);	
		PORTClearBits(PORT_BIT_LED_1); 	
		return STATE_WAITING_CONNECT;
	}	

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
void enableInterrupts()
{
	// Configure the interrupt priority, level 5
	INTDisableInterrupts();
	INTClearFlag(INT_U2RX);//make sure interrupt flag is cleared

	//configure multi vector interrupts
	INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);

	INTSetVectorPriority(INT_UART_2_VECTOR, INT_PRIORITY_LEVEL_7);

	INTEnable(INT_U2RX,INT_ENABLED);
	INTEnableInterrupts();
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
void getQuadEncoding()
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
void __ISR(_TIMER_1_VECTOR, ipl2) Timer1Handler(void)
{	
	getQuadEncoding();
	mainLoopState = pollBlueToothConnected();
  	INTClearFlag(INT_T1);
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
void __ISR(_UART_2_VECTOR, ipl7) UARTIntHandler(void)
{

	if(INTGetFlag(INT_U2RX))
	{	
		UART_BT_RxData = 1;
		if(mainLoopState == STATE_WAITING_CONNECT)
		{
			mainLoopState = (BTRecieveConnect(UART_BLUETOOTH,BLUETOOTH_RESPONSE_CONNECT))?STATE_RECIEVE_MESSAGE:STATE_WAITING_CONNECT;
			UART_BT_RxData = 0;
		}
		INTClearFlag(INT_U2RX);
			
	}	
}