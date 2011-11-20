/* ------------------------------------------------------------ */
/*				PIC32 Configuration Settings					*/
/* ------------------------------------------------------------ */
//

#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_2
#pragma config ICESEL = ICS_PGx1



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
// Let compile time pre-processor calculate the PR1 (period)
#define SYS_FREQ               (80000000L)
#define PB_DIV                 2
#define PB_CLOCK			   SYS_FREQ/PB_DIV
#define PRESCALE_T1            8
#define PRESCALE_T2            64
#define TOGGLES_PER_SEC_T1     1500
#define TOGGLES_PER_SEC_T2     50
#define T1_TICK              (SYS_FREQ/PB_DIV/PRESCALE_T1/TOGGLES_PER_SEC_T1)
#define T2_TICK              (SYS_FREQ/PB_DIV/PRESCALE_T2/TOGGLES_PER_SEC_T2)

#if (__PIC32_FEATURE_SET__ == 460)

#define PORT_BIT_BTN1 		IOPORT_A,BIT_6
#define PORT_BIT_EXT_INT_0 	IOPORT_D,BIT_0
#define PORT_BIT_SERVO 		IOPORT_G,BIT_12
#define CLS_UART 			UART1
#define PMODACL_SPI			2

#elif(__PIC32_FEATURE_SET__ == 795)

#define PORT_BIT_BTN1 		IOPORT_G,BIT_6
#define PORT_BIT_EXT_INT_0 	IOPORT_D,BIT_0
#define PORT_BIT_SERVO 		IOPORT_E,BIT_1
#define PORT_BIT_HB_DIR     IOPORT_E,BIT_0
#define CLS_UART 			UART5
#define PMODACL_SPI			3

#endif

#define ONE_DEGREE_RAD	.0174
#define UPDATE_CLS 50   //update cls 20ms * 50 = 1000ms

#define CLS_DISPLAY_WIDTH 17
#define PULSE_STATE_HIGH  0x0
#define PULSE_STATE_LOW   0x1
#define PULSE_STATE_WAIT  0x2

#define PULSE_SERVO_STOP_MAX  2000 //0.5ms pulse
#define PULSE_SERVO_STOP_CENTER  1500  //1.4ms pulse 
#define PULSE_SERVO_STOP_MIN  500  //2.3ms pulse

#define PULSE_ONE_DEGREE 11  //Range 2ms - 1ms = 1ms = 1000us, 1000ms/90deg = 11

#define DEG_180 3.14
#define DEG_90 1.57
#define DEG_360 6.28
#define DEG_270 4.71


uint8_t isr0Fire = 0;
uint8_t isr1Fire = 0;
uint8_t pulseState = 0;

double surfaceNormal = 0;
const uint32_t partialTick = SYS_FREQ/PB_DIV/PRESCALE_T1;
double aclAngle = 0.0;
uint16_t pulseRate = 0;
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
void UARTSetup(uint32_t baudRate,uint32_t pbClock,UART_MODULE uartID);
void UARTPutS(uint8_t *string,UART_MODULE uartID);
void updateCls();
uint8_t main(void)
{
	
	pulseState = PULSE_STATE_HIGH;
	configurePortIO();
	initPmodACL();
	initCLS();
	calibrate();
	setSurfaceNormal();
	configureInterrupts();
	

    while(1)
    {
		if(isr0Fire)
		{
			isr0Fire = 0;
			aclAngle = getCurrentAngle();
			clsUpdateCount++;	
		}
		if(isr1Fire)
		{
			setPulseWidth();	
			isr1Fire = 0;
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
	if(togglesPerSec > 450 && togglesPerSec < 2300)
	{
		pulseRate = partialTick/togglesPerSec;
	}		
}	


double getCurrentAngle()
{
	PMODACL_AXIS pmodACLAxis;

	double angle = 0;
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
	
		angle = atan(pmodACLAxis.xAxis/(double)pmodACLAxis.yAxis);	
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
	UARTPutS(homeCursor,CLS_UART);
	UARTPutS("BTN1=>Calibrate",CLS_UART);
	blockUntilBtn1Press();
	PmodACLSetIntEnable(PMODACL_SPI,0); //disable PmodACL interrupts
	PmodACLCalibrate(PMODACL_SPI,10,PMODACL_CALIBRATE_X_AXIS);	
}	

void setSurfaceNormal()
{
	UARTPutS(homeCursor,CLS_UART);
	UARTPutS("BTN1=>Set Normal",CLS_UART);
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
    
    #if(__PIC32_FEATURE_SET__ == 795)
	PORTSetPinsDigitalOut(PORT_BIT_HB_DIR);
	PORTClearBits(PORT_BIT_HB_DIR);
	#endif

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

	

void __ISR(_TIMER_1_VECTOR, ipl2) Timer1Handler(void)
{	

	switch(pulseState)
	{
		case PULSE_STATE_HIGH: 
			PORTSetBits(PORT_BIT_SERVO);
			pulseState = PULSE_STATE_LOW;
			break;
		case PULSE_STATE_LOW:
			PORTClearBits(PORT_BIT_SERVO);
			pulseState = PULSE_STATE_WAIT;
			DisableIntT1;
			isr1Fire = 1;
			break;

	}		
	INTClearFlag(INT_T1);
}

void __ISR(_TIMER_2_VECTOR, ipl2) Timer2Handler(void)
{
	pulseState = PULSE_STATE_HIGH;
	WritePeriod1(pulseRate);
	WriteTimer1(0);
	EnableIntT1;
	clsUpdateCount++;
	INTClearFlag(INT_T2);
}	

void __ISR(_EXTERNAL_0_VECTOR, ipl7) ExternalInterrupHandler(void)
{	
	isr0Fire = 1;
			
	INTClearFlag(INT_INT0);
}



void UARTSetup(uint32_t baudRate,uint32_t pbClock,UART_MODULE uartID)
{

    UARTConfigure(uartID, UART_ENABLE_PINS_TX_RX_ONLY);

    UARTSetFifoMode(uartID, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);

    UARTSetLineControl(uartID, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);

    UARTSetDataRate(uartID, pbClock, baudRate);

    UARTEnable(uartID, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
}

void UARTPutS(uint8_t *string,UART_MODULE uartID)
{
	while(*string != '\0')
	{
		while(!UARTTransmitterIsReady(uartID));
		UARTSendDataByte(uartID, *string);

		string++;
	}
}

void initCLS()
{
	uint32_t delay = 0;
	UARTSetup(9600,SYS_FREQ/PB_DIV,CLS_UART);
	//allow for CLS initialization 
	for(delay = 0;delay < 100000;delay++);
	UARTPutS(enableDisplay,CLS_UART);
	UARTPutS(setCursor,CLS_UART);
	UARTPutS(homeCursor,CLS_UART);
	UARTPutS(wrapLine,CLS_UART);
}


void updateCls()
{
	uint8_t oneLine[CLS_DISPLAY_WIDTH];
	UARTPutS(homeCursor,CLS_UART);
	sprintf(oneLine,"Angle: %0.2f",aclAngle);
	UARTPutS(oneLine,CLS_UART);
	UARTPutS(homeRow2,CLS_UART);
	sprintf(oneLine,"Norm: %0.2f",surfaceNormal);
	UARTPutS(oneLine,CLS_UART);
	
}	