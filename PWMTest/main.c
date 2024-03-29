/* ------------------------------------------------------------ */
/*				PIC32 Configuration Settings					*/
/* ------------------------------------------------------------ */
//
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8
#include <stdint.h>

#include "./PmodJSTK/pmodJSTK.h"
#include "./TestHarness/Common/test_harness_common.h"
#include "hbridgeControl.h"

#include <plib.h>
// Let compile time pre-processor calculate the PR1 (period)
#define SYS_FREQ             (80000000L)
#define PB_DIV                 8
#define PRESCALE               256
#define TOGGLES_PER_SEC        2000
#define T1_TICK               (SYS_FREQ/PB_DIV/PRESCALE/TOGGLES_PER_SEC)

#define CLS_DISPLAY_WIDTH 17
#define CLS_UART UART2

#define PMOD_JSTK_SPI_PORT 2

#define NUM_HB5 2

// define setup parameters for OpenADC10
// Turn module on | ouput in integer | trigger mode auto | enable autosample
#define PARAM1  ADC_MODULE_ON | ADC_FORMAT_INTG | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON
// define setup parameters for OpenADC10
// ADC ref external    | disable offset test    | disable scan mode | perform 2 samples | use dual buffers | use alternate mode
#define PARAM2  ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_OFF | ADC_SAMPLES_PER_INT_2 | ADC_ALT_BUF_ON | ADC_ALT_INPUT_ON
// define setup parameters for OpenADC10
//                   use ADC internal clock | set sample time
#define PARAM3  ADC_CONV_CLK_INTERNAL_RC | ADC_SAMPLE_TIME_15
// define setup parameters for OpenADC10
// do not assign channels to scan
#define PARAM4    SKIP_SCAN_ALL
// define setup parameters for OpenADC10
// set AN4 and AN5 as analog inputs
#define PARAM5    ENABLE_AN8_ANA





uint32_t voltage = 0;

uint8_t ledState = PMODJSTK_LD1_ON;
uint8_t displayLine[CLS_DISPLAY_WIDTH];

PmodJSTKAxisButton jstkAxisBtn;

uint16_t yAxisMin = 0;
uint16_t yAxisMax = 0;
uint16_t xAxisMin = 0;
uint16_t xAxisMax = 0;
uint16_t xAxisRange = 0;
uint16_t yAxisRange = 0;

uint8_t isr1Fire = 0;
uint16_t updateBattery = 0;
uint8_t displayBatteryOnCLS = 0;

HBRIDGE hbridges[NUM_HB5];
uint16_t hbDutyCycleRange[NUM_HB5];

uint8_t enableDisplay[] = {27, '[', '3', 'e', '\0'};
uint8_t setCursor[] = {27, '[', '0', 'c', '\0'};
uint8_t homeRow2[] = {27,'[','1',';','0','H','\0'};
uint8_t homeCursor[] = {27, '[', 'j', '\0'};
uint8_t wrapLine[] = {27, '[', '0', 'h', '\0'};


void initADC10();
void getJstkValsOnBTN1Down(PmodJSTKAxisButton *jstkAxisBtn);
void BlockWhileBtnDown(uint8_t button);
void CalibrateJstk();
void setPortIO();
void configureTimers();
void appTask();
void displayRpm();
void setDutyCycle();
void setLedStateAndDirection();
void initCLS();

int main(void)
{
	appTask();
}

void __ISR(_TIMER_1_VECTOR, ipl2) Timer1Handler(void)
{	
	isr1Fire = 1;
	updateBattery++;
	// clear the interrupt flag
  	INTClearFlag(INT_T1);
}

void displayVoltage()
{
	uint32_t newVoltage = ReadADC10(8 * ((~ReadActiveBufferADC10() & 0x01)));  
	voltage = newVoltage;
	UARTPutS(homeCursor,CLS_UART);
	sprintf(displayLine,"Voltage: %1.2f",voltage * 0.012890625);
	UARTPutS(displayLine,CLS_UART);
}

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
void initCLS()
{
	SetupSerialLogging(9600,SYS_FREQ/PB_DIV,CLS_UART);
	UARTPutS("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",CLS_UART);
	UARTPutS(enableDisplay,CLS_UART);
	UARTPutS(setCursor,CLS_UART);
	UARTPutS(homeCursor,CLS_UART);
	UARTPutS(wrapLine,CLS_UART);
}

void setDutyCycle()
{
	PmodHB5SetDCPWMDutyCycle(PR2  * ((jstkAxisBtn.yAxis - yAxisMin) / (double)hbDutyCycleRange[0]),hbridges[0].ocChannel);
	PmodHB5SetDCPWMDutyCycle(PR2  * ((jstkAxisBtn.xAxis - xAxisMin) / (double)hbDutyCycleRange[1]),hbridges[1].ocChannel);		
}

void displayRpm()
{
	uint8_t hbIndex = 0;

	for(hbIndex = 0;hbIndex < NUM_HB5;hbIndex++)
	{
		hbridges[hbIndex].prevRpm = hbridges[hbIndex].rpm;
		sprintf(displayLine,"RPM%d:%03d %s",hbIndex,hbridges[hbIndex].rpm,(hbIndex,hbridges[hbIndex].currentDirection == PMOD_HB5_DIR_FWD)?"FWD":"REV");

		if(hbIndex == 0)
		{
			UARTPutS(homeCursor,CLS_UART);
		}
		else
		{
			UARTPutS(homeRow2,CLS_UART);
		}

		UARTPutS(displayLine,CLS_UART);		
	}
}

void setLedStateAndDirection()
{
	uint8_t hbIndex = 0;
	if(jstkAxisBtn.buttonState & 0x06) //TODO: Fix change in direction on same button press
	{
		for(hbIndex = 0;hbIndex < NUM_HB5;hbIndex++)
		{
			//toggle direction on all HB5
  			hbridges[hbIndex].newDirection ^= (1 << 0);
		}
	}
	else if(jstkAxisBtn.buttonState & 0x01)
	{
		displayBatteryOnCLS ^= (1 << 0); //toggle battery display on CLS
		if(displayBatteryOnCLS)
		{
			updateBattery = TOGGLES_PER_SEC;
			displayVoltage();
		}
		else
		{
			displayRpm();
		}
	}

	if(jstkAxisBtn.buttonState == PMODJSTK_BTN1)
	{
		ledState = PMODJSTK_LD1_ON;
	}
	else if(jstkAxisBtn.buttonState == PMODJSTK_BTN2)
	{
		ledState = PMODJSTK_LD2_ON;
	}
}



void configureTimers()
{
	//HB5 Y axis  
    //Enable OC | 32 bit Mode  | Timer2 is selected | Continuous O/P   | OC Pin High , S Compare value, Compare value
    OpenOC2(OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0);
    OpenTimer2(T2_ON | T2_PS_1_256, SYS_FREQ/PB_DIV/PRESCALE/(TOGGLES_PER_SEC/2));

	//HB5 X axis  
    //Enable OC | 32 bit Mode  | Timer2 is selected | Continuous O/P   | OC Pin High , S Compare value, Compare value
    OpenOC3(OC_ON | OC_TIMER3_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0);
    OpenTimer3(T3_ON | T3_PS_1_256, SYS_FREQ/PB_DIV/PRESCALE/(TOGGLES_PER_SEC/2));

	//Joystick polling, display, RPM/Quadrature encoding, direction change
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, T1_TICK);
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);

    // enable multi-vector interrupts
    INTEnableSystemMultiVectoredInt();
}

void setPortIO()
{
	//setup HB5 Y axis
	PORTSetPinsDigitalOut(IOPORT_D, BIT_7); //HB5 Direction
	PORTSetPinsDigitalOut(IOPORT_D, BIT_1); //HB5 Enable
	PORTSetPinsDigitalIn(IOPORT_D, BIT_9);  //HB5 Sensor A
	PORTSetPinsDigitalIn(IOPORT_C, BIT_1);  //HB5 Sensor B

	//setup HB5 X axis
	PORTSetPinsDigitalOut(IOPORT_D, BIT_6); //HB5 Direction
	PORTSetPinsDigitalOut(IOPORT_D, BIT_2); //HB5 Enable
	PORTSetPinsDigitalIn(IOPORT_D, BIT_10); //HB5 Sensor A
	PORTSetPinsDigitalIn(IOPORT_C, BIT_2);  //HB5 Sensor B	
}

void getJstkValsOnBTN1Down(PmodJSTKAxisButton *jstkAxisBtn)
{
	do
	{
		PmodJSTKSendRecv(PMOD_JSTK_SPI_PORT,PMODJSTK_LD1_ON,jstkAxisBtn);

	}while(jstkAxisBtn->buttonState != PMODJSTK_BTN1);
}

void BlockWhileBtnDown(uint8_t button)
{
	PmodJSTKAxisButton jstkAxisBtn;
	do
	{
		PmodJSTKSendRecv(PMOD_JSTK_SPI_PORT,ledState,&jstkAxisBtn);

	}while(jstkAxisBtn.buttonState == button);
}

void CalibrateJstk()
{
	UARTPutS(homeCursor,CLS_UART);
	UARTPutS("Calibrate Y-",CLS_UART);
	BlockWhileBtnDown(PMODJSTK_BTN1);
	getJstkValsOnBTN1Down(&jstkAxisBtn);
	yAxisMin = jstkAxisBtn.yAxis;
	
	UARTPutS(homeCursor,CLS_UART);
	UARTPutS("Calibrate Y+",CLS_UART);
	BlockWhileBtnDown(PMODJSTK_BTN1);
	getJstkValsOnBTN1Down(&jstkAxisBtn);
	yAxisMax = jstkAxisBtn.yAxis;
	
	UARTPutS(homeCursor,CLS_UART);
	UARTPutS("Calibrate X-",CLS_UART);
	BlockWhileBtnDown(PMODJSTK_BTN1);
	getJstkValsOnBTN1Down(&jstkAxisBtn);
	xAxisMin = 	jstkAxisBtn.xAxis;

	UARTPutS(homeCursor,CLS_UART);
	UARTPutS("Calibrate X+",CLS_UART);
	BlockWhileBtnDown(PMODJSTK_BTN1);
	getJstkValsOnBTN1Down(&jstkAxisBtn);
	xAxisMax = jstkAxisBtn.xAxis;
	
	BlockWhileBtnDown(PMODJSTK_BTN1);

	hbDutyCycleRange[0] = (yAxisMax - yAxisMin);
	hbDutyCycleRange[1] = (xAxisMax - xAxisMin);

}

void initHbridge()
{
	memset(hbridges,0,sizeof(HBRIDGE) * NUM_HB5);
	hbridges[0].sensorAport = IOPORT_D;
	hbridges[0].sensorAportBit = BIT_9;
	hbridges[0].sensorBport = IOPORT_C;
	hbridges[0].sensorBportBit = BIT_1;
	hbridges[0].directionPort = IOPORT_D;
	hbridges[0].directionPortBit = BIT_7;
	hbridges[0].currentDirection = PMOD_HB5_DIR_FWD;
	hbridges[0].newDirection = PMOD_HB5_DIR_FWD;
	hbridges[0].ocChannel = 2;
	PORTSetBits(IOPORT_D,BIT_7); //set initial direction FWD

	hbridges[1].sensorAport = IOPORT_D;
	hbridges[1].sensorAportBit = BIT_10;
	hbridges[1].sensorBport = IOPORT_C;
	hbridges[1].sensorBportBit = BIT_2;
	hbridges[1].directionPort = IOPORT_D;
	hbridges[1].directionPortBit = BIT_6;
	hbridges[1].currentDirection = PMOD_HB5_DIR_REV;
	hbridges[1].newDirection = PMOD_HB5_DIR_REV;
	hbridges[1].ocChannel = 3;
	PORTClearBits(IOPORT_D,BIT_6);//set inital direction REV
}

void appTask()
{	
	uint8_t hbIndex = 0;
	uint8_t rpmChanged = 0;
	uint8_t directionChangeComplete = 1;
	
	initADC10();

	PmodJSTKInit(PMOD_JSTK_SPI_PORT,SYS_FREQ/PB_DIV,625000,SYS_FREQ);

	initCLS();

	CalibrateJstk();

	setPortIO();
	
	initHbridge();

	//displayRpm();

	configureTimers();

	while(1)
	{
		if(isr1Fire)
		{
			directionChangeComplete = 1;
			isr1Fire = 0;
			
			for(hbIndex = 0;hbIndex < NUM_HB5;hbIndex++)
			{
				PmodHB5getQEncRPM(&(hbridges[hbIndex]),2000);
		
				directionChangeComplete &= PmodHB5ChangeDirection(&(hbridges[hbIndex]));
			
				if(hbridges[hbIndex].rpm != hbridges[hbIndex].prevRpm)
				{
					rpmChanged = 1;
				}
			}
			

			if(displayBatteryOnCLS && updateBattery >= TOGGLES_PER_SEC)
			{

				displayVoltage();
				updateBattery = 0;
			
			}
			else if(rpmChanged && !displayBatteryOnCLS)
			{	
				rpmChanged = 0;
				displayRpm();
			}
			
			if(directionChangeComplete)
			{
				PmodJSTKSendRecv(2,ledState,&jstkAxisBtn);
				BlockWhileBtnDown(0x1);
				setLedStateAndDirection();
				setDutyCycle();

			}
						
		}
	}
}
