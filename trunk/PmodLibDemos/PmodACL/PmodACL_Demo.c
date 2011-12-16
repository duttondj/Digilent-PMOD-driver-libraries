
/************************************************************************/
/*																		*/
/*	PmodACL_Demo.c -- implementation file for PmodACL Demo				*/
/*																		*/
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	Copyright (C) 2011 Ryan Hoffman										*/
/************************************************************************/
/*  Module Description: 												*/
/*	See PmodACL_Demo.pdf for functional description and assembly		*/
/*  instructions. Legacy libc library should be used with this project  */
/*  to support floating point string formatting.						*/																		
/*  ------------------------------------------------------------------- */
/*  HARDWARE SETUP	- Cerebot32MX7										*/
/*  ------------------------------------------------------------------- */
/*																		*/
/*  PmodACL																*/
/*  ------------------------------------------------------------------- */
/*  SPI Channel 2	Pins(1-6)											*/
/*  JE-01 	RG9		SS2 												*/
/*  JE-02 	RG8		SDO2  												*/
/*  JE-03 	RG7		SDI2 												*/
/*  JE-04 	RG6		SCK2 												*/
/*  Pins (8)															*/
/*  J8-SCL1 RA14    INT1(pmodACL)		 								*/
/*																		*/
/*  PmodCON3															*/
/*  ------------------------------------------------------------------- */
/*  Pins(1-6) 															*/
/*  JB-01	RE0		J2 													*/
/*  JB-02	RE1		J3 													*/
/*  JB-03	RE2		J3 													*/
/*	JB-04	RE2		J5 													*/
/* 																		*/
/*  PmodCLS																*/
/*  ------------------------------------------------------------------- */
/*  UART3A (UART5) (Pins 1 - 6) 										*/
/*  JF-01 	RF12	U3ACTS												*/
/*  JF-02 	RF5		U3ARTS												*/
/*  JF-03 	RF4		U3ARX												*/
/*  JF-04 	RF13	U3ATX												*/
/* 																		*/
/* 																		*/
/*  ------------------------------------------------------------------- */
/*  HARDWARE SETUP	- Cerebot32MX4										*/
/*  ------------------------------------------------------------------- */
/*																		*/
/*  PmodACL																*/
/*  ------------------------------------------------------------------- */
/*  SPI Channel 2	Pins(1-6)											*/
/*  JB-01 	RG9		SS2 												*/
/*  JB-02 	RG8		SDO2  												*/
/*  JB-03 	RG7		SDI2 												*/
/*  JB-04 	RG6		SCK2 												*/
/*  Pins (8)															*/
/*  J6-SCL1 RA14    INT1(pmodACL)		 								*/
/*																		*/
/*  PmodCLS																*/
/*  ------------------------------------------------------------------- */
/*  UART 2 (Pint 1 - 6)													*/
/*  JH-01 	RF12	U2CTS												*/
/*  JH-02 	RF13	U2RTS												*/
/*  JH-03 	RF4		U2RX												*/
/*  JH-04 	RF5		U2TX												*/
/* 																		*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/* <12/16/11>(RyanH): Initial Release									*/
/*																		*/
/************************************************************************/

/* ------------------------------------------------------------ */
/*				PIC32 Configuration Settings					*/
/* ------------------------------------------------------------ */

#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_2

#if(__PIC32_FEATURE_SET__ == 795)
#pragma config ICESEL = ICS_PGx1 //sets debug mode on CEREBOT32MX7
#endif

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include <stdint.h>
#include <plib.h>
#include <pmodlib.h>
#include <math.h>

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */

//PROCESSOR FREQ
#define SYS_FREQ               (80000000L)   
//PERIPHERAL BUS DIVISOR
#define PB_DIV                 2
//PERIPHERAL BUS FREQ
#define PB_CLOCK			   SYS_FREQ/PB_DIV
//TIMER PRESCALES
#define PRESCALE_T1            8
#define PRESCALE_T2            64
//INITIAL TIMER TOGGLES PER SEC (servo offset 20 degrees from center)
#define TOGGLES_PER_SEC_T1     ((PULSE_SERVO_STOP_CENTER)  - (PULSE_SERVO_20_DEGREES))
#define TOGGLES_PER_SEC_T2     50
//TIMER TICK VALUES
#define T1_TICK              (SYS_FREQ/PB_DIV/PRESCALE_T1/TOGGLES_PER_SEC_T1)
#define T2_TICK              (SYS_FREQ/PB_DIV/PRESCALE_T2/TOGGLES_PER_SEC_T2)

//NUMBER OF SUMPLES TO TAKE DURING CALIBRATION
#define PMOD_ACL_NUM_CALIBRATION_SAMPLES	100

//IO PORT/CHANNEL DEFINITIONS

#if (__PIC32_FEATURE_SET__ == 460) //CEREBOT32MX4
#define UART_CLS						UART2
#define PORT_BIT_BTN1 					IOPORT_A,BIT_6
#define PORT_BIT_SERVO 					IOPORT_G,BIT_12
#define PMODACL_SPI						SPI_CHANNEL2

#elif(__PIC32_FEATURE_SET__ == 795) //CEREBOT32MX7

#define UART_CLS						UART5			//UART3A
#define PORT_BIT_BTN1 					IOPORT_G,BIT_6
#define PORT_BIT_SERVO 					IOPORT_E,BIT_0  //PmodCon3 Servo 1, JB-01
#define PMODACL_SPI						SPI_CHANNEL3 //SPI1A JE 01-06

#endif

#define PORT_BIT_EXT_INT			 	IOPORT_A,BIT_14  //Same for MX4/MX7

#define PMODACL_SPI_BIT_RATE			625000

#define UART_CLS_BITRATE				9600

//NUMBER OF POPINTS COLLED IN ROLLING AVERAGE
#define ROLLING_AVG_NUM_POINTS			20

//I2C address of PmodCLS
#define CLS_I2C_ADDRESS					0x48   

//RADAINS IN ONE DEGREE
#define ONE_DEGREE_RAD					.0174  // PI/180 = .0174, number of radians in one degree

//NUMBER OF MAIN LOOP CYCLES BEFORE CLS UPDATE 
#define UPDATE_CLS 						50     //update cls 20ms * 50 = 1000ms 

//ONE LINE OF TEXT FOR CLS PLUS NULL
#define CLS_DISPLAY_WIDTH 				17 //width of CLS + null character

//SERVO LIMITS
#define PULSE_SERVO_STOP_MIN  			1500 //0.5ms pulse
#define PULSE_SERVO_STOP_CENTER  		1000  //1.0ms pulse 
#define PULSE_SERVO_STOP_MAX  			500  //1.5ms pulse

#define PULSE_SERVO_ONE_DEGREE 			11 //Range 1.5ms - .5ms = 1ms = 1000us, 1000us/90deg = 11

//PULSE 20 DEGREES
#define PULSE_SERVO_20_DEGREES			20 * PULSE_SERVO_ONE_DEGREE 

//DEGREE REPRENSENTATION IN RADIANS
#define DEG_180 						3.14
#define DEG_90 							1.57
#define DEG_360 						6.28
#define DEG_270 						4.71

//SERVO PULSE STATES
typedef enum 
{
	PULSE_STATE_HIGH,  					 //Next servo pulse wil be high
	PULSE_STATE_LOW   				     //Next servo pulse will be low
}PULSE_STATE;

//ACL data ready interrupt fired
uint8_t aclDataReady = 0;
//Pulse width for the servo has completed
uint8_t servoPulseComplete = 0;
//Current state of the servo output (High/Low)
uint8_t pulseState = PULSE_STATE_HIGH;
//Previous angle of PmodACL, used to determine if pulse width should be updated
double prevAclAngle = 0.0;
//Angle to hold, provides reference for adjustemnts to servo
double surfaceNormal = 0.0;
//Partial tick calculation for pusle witdth, divide by number of toggles per
//second desired
const uint32_t partialTick = SYS_FREQ/PB_DIV/PRESCALE_T1;
//current angle of PmodACL
double aclAngle = 0.0;
//Number of pulses per second, determines servo pulse width (SYS_FREQ/PB_DIV/PRESCALE_T1/TOGGLES_PER_SEC_T1)
uint16_t pulseRate = T1_TICK;
//counter for updating the CLS
uint8_t clsUpdateCount = 0;
//Circular buffer for storing data points for rolling average
double rollAverageDataPoints[ROLLING_AVG_NUM_POINTS];

//when the buffer is filled, the rolling average will give an accurate measurement
uint8_t rollingAvgDataPointBufferFilled = 0;

//PmodCLS commands, see PmodCLS manual for descriptions
uint8_t enableDisplay[] = {27, '[', '3', 'e', '\0'}; //enable display
uint8_t setCursor[] = {27, '[', '0', 'c', '\0'}; //turn off cursor
uint8_t homeRow2[] = {27,'[','1',';','0','H','\0'}; //set cursor to col 0, row 1
uint8_t homeCursor[] = {27, '[', 'j', '\0'}; //set cursor to row 0, col 0, clear display


/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */

void init();
void appTask();
void initPmodACL();
void configureInterrupts();
void setSurfaceNormal();
void blockUntilBtn1Press();
double getCurrentAngle();
void configurePortIO();
void setPulseWidth();
void calibrateACL();
void initCLS();
void UARTPutS(uint8_t *string,UART_MODULE uartID);
void UARTInit(uint32_t baudRate,uint32_t pbClock,UART_MODULE uartID,UART_CONFIGURATION configParams);
void updateCls();
double getDataPointRollingAverage(double newDataPoint);

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

/*
** 	main()
**
**	Synopsis:
**	Main for PmodACl_Demo.c
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
**  appTask()
**
**	Synopsis:
**	
**	Main application task loop
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**	Main application loop, get datapoints, 
**	set pulse width, update cls
*/
void appTask()
{
    while(1)
    {
		if(aclDataReady)
		{
			aclDataReady = 0;
			prevAclAngle = aclAngle;
			aclAngle = getDataPointRollingAverage(getCurrentAngle());
			clsUpdateCount++;	
		}
		if(servoPulseComplete)
		{
			setPulseWidth();			
			servoPulseComplete = 0;
		}		
		if(clsUpdateCount == UPDATE_CLS)
		{
			clsUpdateCount = 0;	
			updateCls();
		}	    
	} 
}

/*  
**  init()
**
**	Synopsis:
**	
**	Initializes hardware.
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  Initialize hardware: set port IO,init ACL, init CLS, 
**  calibrate PmodACL, get surface normal configure interrupts.
*/
void init()
{
	configurePortIO();
	initPmodACL();
	initCLS();
	calibrateACL();
	configureInterrupts();
	setSurfaceNormal();

}

/*  
**  setPulseWidth()
**
**	Synopsis:
**
**	Calculates pulse width for servo position
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  Calculates pulse width used in positioning the servo. 
**  Servo position is based on the duty cycle(pulse width) 
**  of the signal recieved, this caclulation is based on 
**  SYS_FREQ/PB_DIV/PRESCALE_T1/<number of toggles per sec>,
**  Surface normal is the angle for the servo(set during
**  startup), all angle offsets reference this angle.
*/
void setPulseWidth()
{
	//Example: Normal = 1.57 rad, Acl = 2.0 rad, (1.57 + (1.57 - 2.0))/ONE_DEGREE_RAD = New Servo Angle in degrees
	//New Servo Angle * PULSE_SERVO_ONE_DEGREE = Toggles for New Angle
    //Toggles for New Angle - 20 deg,  offsets sevo angles by 20deg
	uint16_t togglesPerSec = (uint16_t) (((surfaceNormal + (surfaceNormal - aclAngle))/ONE_DEGREE_RAD) * PULSE_SERVO_ONE_DEGREE) - PULSE_SERVO_20_DEGREES;

	if(togglesPerSec < PULSE_SERVO_STOP_MIN && togglesPerSec > PULSE_SERVO_STOP_MAX)
	{
		pulseRate = partialTick/togglesPerSec;
	}		
}	

/*  
**  getDataPointRollingAverage
**
**	Synopsis:
**	
**	Maintains a rolling average of data points	
**
**  Input: 
**		double newDataPoint - angle value in radians
**
**  Returns: 
**		double - rolling average of angle value in radians
**	Errors:	none
**
**  Description:
**  Rolling average smoothes out the servo movements by
**  averaging the last ROLLING_AVG_NUM_POINTS.
*/
double getDataPointRollingAverage(double newDataPoint)
{
	uint8_t pointIndex = 0;
	//index of current data point in RollAverageDataPoints
	static uint8_t currentDataPoint = 0;
	double rollingAvg = 0;
	
	rollAverageDataPoints[currentDataPoint] = newDataPoint;
	
	currentDataPoint++;
	
	currentDataPoint %= ROLLING_AVG_NUM_POINTS;
	
	for(pointIndex = 0;pointIndex <  ROLLING_AVG_NUM_POINTS;pointIndex++)
	{
		rollingAvg += rollAverageDataPoints[pointIndex];
	}
	
	return rollingAvg / ROLLING_AVG_NUM_POINTS;
}

/*  
**  getCurrentAngle()
**
**	Synopsis:
**	
**	Reads axis values from PmodACL and calculates the angle in radians
**
**  Input: none
**
**  Returns: 
**		double - angle in radians
**
**	Errors:	none
**
**  Description:
**
**	Reads axis values from PmodACL and calculates the angle in radians
**  using the arctan of the X and Y axis values (arctan(x/y))
*/
double getCurrentAngle()
{
	PMODACL_AXIS pmodACLAxis;

	double angle = 0.0;
	
	PmodACLGetAxisData(PMODACL_SPI,&pmodACLAxis);

	//Position is know is Yaxis is 0, we can not divide by zero below
	if(pmodACLAxis.yAxis == 0)
	{
		if(pmodACLAxis.xAxis > 0)
		{
			angle = DEG_90;	
		}	
		else
		{
			angle = DEG_270;
		}	
	}	
	else
	{	 	
		angle = atan(pmodACLAxis.xAxis/(double)pmodACLAxis.yAxis);

		//Adjust angle for quadrant, if conditions below dont match, angle is in Quadrant 1
		if(pmodACLAxis.xAxis >= 0 && pmodACLAxis.yAxis < 0) //Quadrant 2
		{
			angle += DEG_180;	
		}
		else if(pmodACLAxis.xAxis < 0 && pmodACLAxis.yAxis < 0) //Quadrant 3
		{
			angle += DEG_180;
		}	
		else if(pmodACLAxis.xAxis < 0 && pmodACLAxis.yAxis >= 0) //Quadrant 4
		{
			angle+= DEG_360;
		}		
	}
	return angle;
}	

/*  
**  blockUntilBtn1Press()
**
**	Synopsis:
**
**	Blocks intil BTN1 is pressed
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  Blocks until BTN1 is pressed then released.
*/
void blockUntilBtn1Press()
{
	if(!((PORTReadBits(PORT_BIT_BTN1) & BIT_6) >> 6))
	{
		while(!((PORTReadBits(PORT_BIT_BTN1) & BIT_6) >> 6));	
	}	
	while(((PORTReadBits(PORT_BIT_BTN1) & BIT_6) >> 6));
}	

/*  
**  calibrateACL()
**
**	Synopsis:
**	Performs a calibration of the PmodADL
**	
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**	Calibrates the PmodACL for the X axis 
**  at 1g, Y axis 0g, Z axis 0g.
*/
void calibrateACL()
{
	UARTPutS(homeCursor,UART_CLS);
	UARTPutS("BTN1-Calibrate",UART_CLS);
	blockUntilBtn1Press();
	PmodACLSetIntEnable(PMODACL_SPI,0); //disable PmodACL interrupts
	PmodACLCalibrate(PMODACL_SPI,PMOD_ACL_NUM_CALIBRATION_SAMPLES,PMODACL_CALIBRATE_X_AXIS);	
}	

/*  
**  setSurfaceNormal()
**
**	Synopsis:
**	Set the angle to maintain as surface normal
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**	Captures the angle repesenting surface normal (90deg from from baseplate)
**  All servo adjustments will be made relative to this angle.
*/
void setSurfaceNormal()
{
	uint8_t count = 0;
	
	UARTPutS(homeCursor,UART_CLS);
	UARTPutS("BTN1-Set Normal",UART_CLS);
	blockUntilBtn1Press();
	
	//Fill up buffer, surface normal based on rolling average
	for(count = 0; count < ROLLING_AVG_NUM_POINTS; count++)
	{
		surfaceNormal =	getDataPointRollingAverage(getCurrentAngle());
	}
  	aclAngle = surfaceNormal;
 	setPulseWidth();
}	

/*  
**  configurePortIO()
**
**	Synopsis:
**	Configures port IO 
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**  Configures port IO for External Interrupt, Servo and BTN1
*/
void configurePortIO()
{
	//setup external interrupt pin for external int 0 to recivieve input
	PORTSetPinsDigitalIn(PORT_BIT_EXT_INT);
	//Servo 
	PORTSetPinsDigitalOut(PORT_BIT_SERVO);
    //BTN1
    PORTSetPinsDigitalIn(PORT_BIT_BTN1);   
}	

/*  
**  initPmodACL()
**
**	Synopsis:
**
**	Initializes the PmodACL
**
**  Input: none 
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**	Configures and initializes the PmodACL
*/
void initPmodACL()
{
	uint32_t wait = 0;
    PmodACLInitSpi(PMODACL_SPI,PB_CLOCK,PMODACL_SPI_BIT_RATE);
    
	// Wait for initialization
	for(wait = 0;wait < 40000;wait++)
	{
		asm("nop");	
	}
	//G range 4G,Interrupt active low 
    PmodACLSetDataFormat(PMODACL_SPI,PMODACL_BIT_DATA_FORMAT_RANGE_2G|PMODACL_BIT_DATA_FORMAT_INT_INVERT);
	//Bypass FIFO
	PmodACLSetFIFOCtl(PMODACL_SPI,PMODACL_BIT_FIFO_CTL_BYPASS);
	//Normal power mode, Output data rate 100Hz
	PmodACLSetBwRate(PMODACL_SPI,PMODACL_BIT_BW_RATE_100HZ);
	//Place into Measurement mode, 
	PmodACLSetPowerCtl(PMODACL_SPI,PMODACL_BIT_POWER_CTL_MEASURE);

}	

/*  
**  configureInterrupts()
**
**	Synopsis:
**	Configure timer and external interrupts
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**	Configure timer and external interrupts
*/
void configureInterrupts()
{
	INTDisableInterrupts();

	INTClearFlag(INT_INT3);//make sure interrupt flags are cleared
	INTClearFlag(INT_T1);
	INTClearFlag(INT_T2);
	
	//configure for multi vector interrupts
	INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);

	//configure External INT3 to trigger on rising edge transition with priority 7
	//Fires when PmodACL has data
	ConfigINT3(EXT_INT_PRI_7 | FALLING_EDGE_INT | EXT_INT_ENABLE); 
	
	//PWM - Pulse Width Timer
	OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_8, T1_TICK);
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);

	//PWM - Period Timer
	OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_64, T2_TICK);
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2);

	PmodACLGetIntSource(PMODACL_SPI); //get interrupt source, clears interrupt
	
	//map all interrupts to PmodACL INT1
	PmodACLSetIntMap(PMODACL_SPI,0);
	
	//Enable PmodACL interrupts for DataReady, Activity
	PmodACLSetIntEnable(PMODACL_SPI,PMODACL_BIT_INT_MAP_DATA_READY);
	INTEnableInterrupts();
}	

/*  
**  Tmr1Handler_PWM_PulseWidth
**
**	Synopsis:
**  
**	Timer handler controlling pulse width
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**	When timer 1 initially fires the servo
**  pin is driven high, the next time it fires
**  the servo pin is driven low,timer IntT1 is
**  then disabled. This produces the pulse wdith.
*/
void __ISR(_TIMER_1_VECTOR, ipl2)Tmr1Handler_PWM_PulseWidth(void)
{	
	switch(pulseState)
	{
		case PULSE_STATE_HIGH: 
			PORTSetBits(PORT_BIT_SERVO);
			pulseState = PULSE_STATE_LOW;
			break;
		case PULSE_STATE_LOW:
			PORTClearBits(PORT_BIT_SERVO);
			DisableIntT1;
			servoPulseComplete = 1;
			break;

	}		
	INTClearFlag(INT_T1);
}

/*  
**  Tmr2Handler_PWM_PulsePeriod
**
**	Synopsis:
**	Generates pulse period for operating the servo
**   
**  Input: 
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  Pulse period is set at 20ms in timer 2, when this 
**  interrupt fires it signals the beginning of the pulse
**  period, sets the pulse state to HIGH(used by Tmr1Handler_PWM_PulseWidth)
**  writes the new pulse width value to timer 1, resets timer 1's counter
**  then enables it. The combination of the Timer1 and Timer2 generate
**  pulse width modulation with a period of 20ms and pulse width of .5ms to 1.5ms
*/
void __ISR(_TIMER_2_VECTOR, ipl2) Tmr2Handler_PWM_PulsePeriod(void)
{
	pulseState = PULSE_STATE_HIGH;
	WritePeriod1(pulseRate);
	WriteTimer1(0);
	EnableIntT1;
	clsUpdateCount++;
	
	INTClearFlag(INT_T2);
}	

/*  
**  Ext3Handler_PmodACLInt1
**
**	Synopsis:
**
**	Set the aclDataReady flag
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**	Sets the aclDataReady flag when external interrupt 3
**  fires indicating there is data available on the PmodACL
*/
void __ISR(_EXTERNAL_3_VECTOR, ipl7) Ext3Handler_PmodACLInt1(void)
{	
	aclDataReady = 1;			
	INTClearFlag(INT_INT3);
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
**  to Row 0, Col 0, and clearing the display
*/
void initCLS()
{
	uint32_t delay = 0;
	UARTInit(UART_CLS_BITRATE,PB_CLOCK,UART_CLS,UART_ENABLE_PINS_TX_RX_ONLY);
	//allow for CLS initialization 
	for(delay = 0;delay < 100000;delay++);
	UARTPutS(enableDisplay,UART_CLS);
	UARTPutS(setCursor,UART_CLS);
	UARTPutS(homeCursor,UART_CLS);
}

/*  
**  updateCls()
**
**	Synopsis:
**
**	Displays current angle and surface normal on PmodCLS
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**
**	Displays current angle and surface normal on PmodCLS
*/
void updateCls()
{
	uint8_t oneLine[CLS_DISPLAY_WIDTH];
	UARTPutS(homeCursor,UART_CLS);
	sprintf(oneLine,"Angle: %0.2f",aclAngle);
	UARTPutS(oneLine,UART_CLS);
	UARTPutS(homeRow2,UART_CLS);
	sprintf(oneLine,"Norm: %0.2f",surfaceNormal);
	UARTPutS(oneLine,UART_CLS);	
}

/*  
**  UARTPutS()
**
**	Synopsis:
**	Writes a null terminated string to the UART specified
**
**  Input: 
**		uint8_t *string - null terminated string to write to UART
**		UART_MODULE uartID - UART id
**
**  Returns: none
**
**	Errors:	none
*/
void UARTPutS(uint8_t *string,UART_MODULE uartID)
{
	while(*string != '\0')
	{
		while(!UARTTransmitterIsReady(uartID));
		UARTSendDataByte(uartID, *string);

		string++;
	}
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