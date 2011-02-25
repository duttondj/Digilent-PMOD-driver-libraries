/*
    FreeRTOS V6.0.0 - Copyright (C) 2009 Real Time Engineers Ltd.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    ***NOTE*** The exception to the GPL is included to allow you to distribute
    a combined work that includes FreeRTOS without being obliged to provide the
    source code for proprietary components outside of the FreeRTOS kernel.
    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public 
    License and the FreeRTOS license exception along with FreeRTOS; if not it 
    can be viewed here: http://www.freertos.org/a00114.html and also obtained 
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

/* MPLAB includes. */
#include <p32xxxx.h>
#include <sys/asm.h>
#include <plib.h>
#include <string.h>

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "spi_mx4_7.h"
#include "semphr.h"



/* PIC32 configuration. */
/*#pragma config FPLLODIV = DIV_1, FPLLMUL = MUL_20, FPLLIDIV = DIV_2
#pragma config FWDTEN = OFF, FPBDIV = DIV_2, POSCMOD = XT, FNOSC = PRIPLL, CP = OFF
*/
#pragma config FNOSC = PRIPLL
#pragma config FPLLMUL = MUL_20
#pragma config FPLLIDIV = DIV_2
#pragma config FPBDIV = DIV_2
#pragma config FPLLODIV = DIV_1
#define SYSTEM_CLOCK		80000000

/* Demo includes. */
#include "basic_io.h"

/* Used as a loop counter to create a very crude delay. */
#define mainDELAY_LOOP_COUNT		( 0xfffff )

/* The task function. */
void vTaskFunction( void *pvParameters );

/* Basic hardware and debug interface configuration. */
void vSetupEnvironment( void );
void vSetupTasks(void *pvParameters);
void setupCLS( void );

/* Define the strings that will be passed in as the task parameters.  These are
defined const and off the stack to ensure they remain valid when the tasks are
executing. */
unsigned char *pcTextForTask1 = "Task 1\n\0";
unsigned char *pcTextForTask2 = "Task 2\n\0";
const unsigned char home_cursor[] = {27,'[','j','\0'};
const unsigned char enable_display[] = {27,'[','3','e','\0'};
const unsigned char set_cursor[] = {27,'[','0','c','\0'};
const unsigned char wrap_line[] = {27,'[','0','h','\0'};

xSemaphoreHandle xMutex;

/*-----------------------------------------------------------*/

int main( void )
{
	xMutex = xSemaphoreCreateMutex();
	/* Configure both the hardware and the debug interface. */
	
	vSetupEnvironment();
    setupCLS();
	
	xTaskCreate( vSetupTasks, "Schedule Delayed Tasks", 240, NULL, 1, NULL );

	
	vTaskStartScheduler();	
	

	for( ;; );
	return 0;
}
/*-----------------------------------------------------------*/

void setupCLS(void)
{
    fnSPIConfigureChannelMasterNoFrames(SYSTEM_CLOCK,SPI_C1,SPI_8BIT_MODE,156250);
	SpiChnPutS(SPI_CHANNEL1,enable_display,strlen(enable_display));
	SpiChnPutS(SPI_CHANNEL1,set_cursor,strlen(set_cursor));
	SpiChnPutS(SPI_CHANNEL1,home_cursor,strlen(home_cursor));
	SpiChnPutS(SPI_CHANNEL1,wrap_line,strlen(wrap_line));
}
void vSetupTasks(void *pvParameters)
{
	portTickType xLastWakeTime = xTaskGetTickCount();
	xTaskCreate(vTaskFunction,"Task 1",240,	(void*)pcTextForTask1,1,NULL );				
	vTaskDelayUntil(&xLastWakeTime,(1000/portTICK_RATE_MS));
	xTaskCreate( vTaskFunction, "Task 2", 240, (void*)pcTextForTask2, 1, NULL );
	vTaskDelete(NULL);
}

void vTaskFunction( void *pvParameters )
{
	unsigned char *pcTaskName;
	portTickType xLastWakeTime = xTaskGetTickCount();
	volatile unsigned long ul;

	/* The string to print out is passed in via the parameter.  Cast this to a
	character pointer. */
	pcTaskName = (unsigned char * ) pvParameters;


	for( ;; )
	{
		xSemaphoreTake(xMutex,0);
		{
			SpiChnPutS(SPI_CHANNEL1,home_cursor,strlen(home_cursor));
			SpiChnPutS(SPI_CHANNEL1, pcTaskName,strlen(pcTaskName) -1);
			//Print the task name to the debugger
			vPrintString( pcTaskName );
			vTaskDelayUntil(&xLastWakeTime,(2000/portTICK_RATE_MS));
		}
		xSemaphoreGive(xMutex);
		
	}
}
/*-----------------------------------------------------------*/

void vSetupEnvironment( void )
{
	/* Setup the main clock for maximum performance, and the peripheral clock
	to equal the main clock divided by 2. */
	SYSTEMConfigPerformance( configCPU_CLOCK_HZ );
	mOSCSetPBDIV( OSC_PB_DIV_2 );
	
	/* Enable global interrupt handling. */
	INTEnableSystemMultiVectoredInt();

	/* Initialise the debug utils library to enable strings printed from the
	demo source code to be displayed in the MPLAB IDE. */
	DBINIT();
}
/*-----------------------------------------------------------*/

