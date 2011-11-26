/* ------------------------------------------------------------ */
/*				PIC32 Configuration Settings					*/
/* ------------------------------------------------------------ */
//

#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_2

#if(__PIC32_FEATURE_SET__ == 795)
#pragma config ICESEL = ICS_PGx1 //debug mode on Pic32 795
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

#define SYS_FREQ               (80000000L)
#define PB_DIV                 2
#define PB_CLOCK			   SYS_FREQ/PB_DIV
#define PRESCALE_T1            8
#define PRESCALE_T2            64
#define TOGGLES_PER_SEC_T1     1000
#define TOGGLES_PER_SEC_T2     50
#define T1_TICK              (SYS_FREQ/PB_DIV/PRESCALE_T1/TOGGLES_PER_SEC_T1)
#define T2_TICK              (SYS_FREQ/PB_DIV/PRESCALE_T2/TOGGLES_PER_SEC_T2)

#if (__PIC32_FEATURE_SET__ == 460)

#define PORT_BIT_BTN1 		IOPORT_A,BIT_6
#define PORT_BIT_EXT_INT_0 	IOPORT_D,BIT_0
#define PORT_BIT_SERVO 		IOPORT_G,BIT_12
#define PMODACL_SPI			SPI_CHANNEL2
#define CLS_MODULE_I2C		I2C1

#elif(__PIC32_FEATURE_SET__ == 795)

#define PORT_BIT_BTN1 		IOPORT_G,BIT_6
#define PORT_BIT_EXT_INT_0 	IOPORT_D,BIT_0  //JD-02
#define PORT_BIT_SERVO 		IOPORT_E,BIT_0  //PmodCon3 Servo 1, JB-01
#define PMODACL_SPI			SPI_CHANNEL3 //SPI1A JE 01-06
#define CLS_MODULE_I2C		I2C1
#define PMODSF_SPI			SPI_CHANNEL4

#endif


#define CLS_I2C_ADDRESS		0x48   //I2C address of CLS
#define ONE_DEGREE_RAD		.0174  //3.14/180 = .0174, number of radians in one degree
#define UPDATE_CLS 			50     //update cls 20ms * 50 = 1000ms 

#define CLS_DISPLAY_WIDTH 17 //width of CLS + null character

#define PULSE_STATE_HIGH  0x0 //Next servo pulse wil be high
#define PULSE_STATE_LOW   0x1 //next servo pulse will be low

#define PULSE_SERVO_STOP_MIN  1500 //0.5ms pulse
#define PULSE_SERVO_STOP_CENTER  1000  //1.0ms pulse 
#define PULSE_SERVO_STOP_MAX  500  //1.5ms pulse

#define PULSE_ONE_DEGREE 11 //Range 2ms - 1ms = 1ms = 1000us, 1000ms/90deg = 11

#define DEG_180 3.14
#define DEG_90 1.57
#define DEG_360 6.28
#define DEG_270 4.71

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

//PmodCLS commands, see PmodCLS manual for descriptions
uint8_t enableDisplay[] = {27, '[', '3', 'e', '\0'}; //enable display
uint8_t setCursor[] = {27, '[', '0', 'c', '\0'}; //turn off cursor
uint8_t homeRow2[] = {27,'[','1',';','0','H','\0'}; //set cursor to col 0, row 1
uint8_t homeCursor[] = {27, '[', 'j', '\0'}; //set cursor to row 0, col 0, clear display
uint8_t wrapLine[] = {27, '[', '0', 'h', '\0'}; //set display to "wrap"

void initPmodACL();
void configureInterrupts();
void setSurfaceNormal();
void blockUntilBtn1Press();
double getCurrentAngle();
void configurePortIO();
void setPulseWidth();
void calibrate();
void initCLS();
void I2CSetup(uint32_t bitRate,uint32_t pbClock,I2C_MODULE module);
void I2CPutS(uint8_t *string,I2C_MODULE module,uint8_t address);
void updateCls();
uint8_t main(void)
{
   
	pulseState = PULSE_STATE_HIGH;
	configurePortIO();
	initPmodACL();
	initCLS();
	calibrate();
	configureInterrupts();
	setSurfaceNormal();
	

    while(1)
    {
		if(aclDataReady)
		{
			aclDataReady = 0;
			prevAclAngle = aclAngle;
			aclAngle = getCurrentAngle();
			clsUpdateCount++;	
		}
		if(servoPulseComplete)
		{
			if(aclAngle != prevAclAngle)
			{
				setPulseWidth();
			}
			servoPulseComplete = 0;
		}		
		if(clsUpdateCount == UPDATE_CLS)
		{
			clsUpdateCount = 0;	
			updateCls();
		}	
	    
	} 
}

void setPulseWidth()
{
	uint16_t togglesPerSec = (uint16_t) (((surfaceNormal + (surfaceNormal - aclAngle))/ONE_DEGREE_RAD) * PULSE_ONE_DEGREE);
	if(togglesPerSec < PULSE_SERVO_STOP_MIN && togglesPerSec > PULSE_SERVO_STOP_MAX)
	{
		pulseRate = partialTick/togglesPerSec;
	}		
}	


double getCurrentAngle()
{
	PMODACL_AXIS pmodACLAxis;

	double angle = 0.0;
	PmodACLGetAxisData(PMODACL_SPI,&pmodACLAxis);
	
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
	
		angle =atan(pmodACLAxis.xAxis/(double)pmodACLAxis.yAxis);
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
	return (angle);
}	

void blockUntilBtn1Press()
{
	if(!((PORTReadBits(PORT_BIT_BTN1) & BIT_6) >> 6))
	{
		while(!((PORTReadBits(PORT_BIT_BTN1) & BIT_6) >> 6));	
	}	
	while(((PORTReadBits(PORT_BIT_BTN1) & BIT_6) >> 6));
}	

void calibrate()
{
	I2CPutS(homeCursor,CLS_MODULE_I2C,CLS_I2C_ADDRESS);
	I2CPutS("BTN1-Calibrate",CLS_MODULE_I2C,CLS_I2C_ADDRESS);
	blockUntilBtn1Press();
	PmodACLSetIntEnable(PMODACL_SPI,0); //disable PmodACL interrupts
	PmodACLCalibrate(PMODACL_SPI,10,PMODACL_CALIBRATE_X_AXIS);	
}	

void setSurfaceNormal()
{
	I2CPutS(homeCursor,CLS_MODULE_I2C,CLS_I2C_ADDRESS);
	I2CPutS("BTN1-Set Normal",CLS_MODULE_I2C,CLS_I2C_ADDRESS);
	blockUntilBtn1Press();
 	surfaceNormal = getCurrentAngle();
 	aclAngle = surfaceNormal;
 	setPulseWidth();
}	

void configurePortIO()
{
	//setup external interrupt pin for external int 0 to recivieve input
	PORTSetPinsDigitalIn(PORT_BIT_EXT_INT_0); //CEREBOT32MX4 PIN JH-08	
	//Servo S1
	PORTSetPinsDigitalOut(PORT_BIT_SERVO);
    //BTN1
    PORTSetPinsDigitalIn(PORT_BIT_BTN1); 
    
  
}	

void initPmodACL()
{
	uint32_t wait = 0;
    PmodACLInitSpi(PMODACL_SPI,PB_CLOCK,625000);
    	for(wait = 0;wait < 40000;wait++)
	{
		asm("nop");	
	}
    PmodACLSetDataFormat(PMODACL_SPI,PMODACL_BIT_DATA_FORMAT_RANGE_4G|PMODACL_BIT_DATA_FORMAT_INT_INVERT);
	PmodACLSetFIFOCtl(PMODACL_SPI,PMODACL_BIT_FIFO_CTL_BYPASS);
	PmodACLSetBwRate(PMODACL_SPI,PMODACL_BIT_BW_RATE_100HZ);
	PmodACLSetPowerCtl(PMODACL_SPI,PMODACL_BIT_POWER_CTL_MEASURE);

}	

void configureInterrupts()
{
	INTDisableInterrupts();
	INTClearFlag(INT_INT0);//make sure interrupt flag is cleared
	INTClearFlag(INT_T1);
	INTClearFlag(INT_T2);
	//configure multi vector interrupts
	INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);

	//configure INT0 to trigger on rising edge transition with priority 7
	ConfigINT0(EXT_INT_PRI_7 | FALLING_EDGE_INT | EXT_INT_ENABLE); 
	
	OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_8, T1_TICK);
    ConfigIntTimer1(T1_INT_OFF | T1_INT_PRIOR_2);

	OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_64, T2_TICK);
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2);

	PmodACLGetIntSource(PMODACL_SPI); //get interrupt source, clear interrupt
	
	//map all interrupts to INT1
	PmodACLSetIntMap(PMODACL_SPI,0);
	
	//enable interrupts for DataReady, Activity
	PmodACLSetIntEnable(PMODACL_SPI,PMODACL_BIT_INT_MAP_DATA_READY);
	INTEnableInterrupts();
}	

	

void __ISR(_TIMER_1_VECTOR, ipl2)Tmr1Handler_PulseWidth(void)
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

void __ISR(_TIMER_2_VECTOR, ipl2) Tmr2Handler_PulsePeriod(void)
{
	pulseState = PULSE_STATE_HIGH;
	WritePeriod1(pulseRate);
	WriteTimer1(0);
	EnableIntT1;
	clsUpdateCount++;
	
	INTClearFlag(INT_T2);
}	

void __ISR(_EXTERNAL_0_VECTOR, ipl7) Ext0Handler_PmodACLInt1(void)
{	
	aclDataReady = 1;
			
	INTClearFlag(INT_INT0);
}



void I2CSetup(uint32_t bitRate,uint32_t pbClock,I2C_MODULE module)
{
	uint16_t delay = 0;
    I2CEnable(module, FALSE);
    I2CConfigure(module,I2C_STOP_IN_IDLE);
	I2CSetFrequency(module,pbClock,bitRate);

    I2CEnable(module, TRUE);
}

void I2CPutByte(I2C_MODULE module,uint8_t data)
{
	I2C_RESULT result = 0;
	do{
 		do{
	 		I2CClearStatus(module,BIT_7|BIT_10);
	 	
	 		while(!I2CTransmitterIsReady(module));

			result = I2CSendByte(module,data);
			
		}while(result != I2C_SUCCESS);
	 
	 }while(!I2CTransmissionHasCompleted(module) && !I2CByteWasAcknowledged(module));
}	

void I2CPutS(uint8_t *string,I2C_MODULE module,uint8_t address)
{
	I2C_RESULT result = 0;
	
	do{
		result = I2CStart(module);
		if((result & I2C_SUCCESS) != I2C_SUCCESS)
		{
			I2CStop(module);
		}	
	}while((result & I2C_SUCCESS) != I2C_SUCCESS);
	
 	I2CPutByte(module,(address << 1) | I2C_WRITE);
 	
	while(*string != '\0')
	{
		I2CPutByte(module,*string);
		string++;
	}
	
	I2CStop(module);
}

void initCLS()
{
	uint32_t delay = 0;
	I2CSetup(100000,PB_CLOCK,CLS_MODULE_I2C);
	//allow for CLS initialization 
	for(delay = 0;delay < 10000;delay++);
	I2CPutS(enableDisplay,CLS_MODULE_I2C,CLS_I2C_ADDRESS);
	I2CPutS(setCursor,CLS_MODULE_I2C,CLS_I2C_ADDRESS);
	I2CPutS(homeCursor,CLS_MODULE_I2C,CLS_I2C_ADDRESS);
	I2CPutS(wrapLine,CLS_MODULE_I2C,CLS_I2C_ADDRESS);
}


void updateCls()
{
	uint8_t oneLine[CLS_DISPLAY_WIDTH];
	I2CPutS(homeCursor,CLS_MODULE_I2C,CLS_I2C_ADDRESS);
	sprintf(oneLine,"Angle: %0.2f",aclAngle);
	I2CPutS(oneLine,CLS_MODULE_I2C,CLS_I2C_ADDRESS);
	I2CPutS(homeRow2,CLS_MODULE_I2C,CLS_I2C_ADDRESS);
	sprintf(oneLine,"Norm: %0.2f",surfaceNormal);
	I2CPutS(oneLine,CLS_MODULE_I2C,CLS_I2C_ADDRESS);
	
}	