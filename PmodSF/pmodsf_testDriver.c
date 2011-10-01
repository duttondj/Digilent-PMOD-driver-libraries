/************************************************************************/
/*                                                                      */
/*   cerebot32mx4_7_test_driver.c test driver functions for SPI,PMODSF  */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  This file contains test functions used in for the Digilent          */
/*  PMODSF,SPI on the Digilent CEREBOT32MX4 and CEREBOT32MX7            */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  5/5/2011(RyanH):                                                    */
/*                                                                      */
/************************************************************************/

#include "pmodsf_testDriver.h"


int UNIT_spfPMOD_DPD_Release()
{
	char results[128];
	putsUART1("\n\rEXECUING TEST => UNIT_spfPMOD_DPD_Release()\n\r ");
	putsUART1("Entering Deep Power Down\n\r");

	PmodSFDeepPowerDown(2);
	putsUART1("Exiting Deep Power Down\n\r");
	sprintf(results,"Exited deep power down with signature => %x\n\r",PmodSFDeepPowerDownRelease(2));
	putsUART1(results);
}

int UNIT_spfPMOD_ReadID()
{
/*
  	PMODSFCOMMAND command;
	char outPut[255];
    command.ucID = command.ucMemType = command.ucMemCapacity = 0;
    fnSPIConfigureChannelMasterForPMODSF(SPI_C2,80000000,156250);
	command.ucInstruction = PMODSF_READ_ID;
	command.ucSpiChannel = SPI_C2;
	fnPmodSFsendCommand(&command);
    putsUART1("\n\rEXECUING TEST => UNIT_spfPMOD_ReadID()\n\r");
	sprintf(outPut,"ID-> %x\n\r", command.ucID);
	putsUART1(outPut);
    sprintf(outPut,"Type-> %x\n\r",command.ucMemType);
	putsUART1(outPut);
    sprintf(outPut,"Capacity-> %x\n\r",command.ucMemCapacity);
	putsUART1(outPut);
*/
}

int UNIT_sfPMODF_ReadStatusReg()
{
/*
  	PMODSFCOMMAND command;
	char results[1024];
    command.ucStatusRegister = 0;
    command.ucSpiChannel = SPI_C2;
	fnSPIConfigureChannelMasterForPMODSF(SPI_C2,80000000,156250);
	command.ucInstruction = PMODSF_WRITE_ENABLE;
	fnPmodSFCommandNoReturn(&command);
	command.ucInstruction = PMODSF_READ_STATUS_REG;
	fnPmodSFsendCommand(&command);
	printf("Register-> %x", command.ucStatusRegister);
	command.ucInstruction = PMODSF_WRITE_DISABLE;
	fnPmodSFCommandNoReturn(&command);
	command.ucInstruction = PMODSF_READ_STATUS_REG;
	fnPmodSFsendCommand(&command);
  	putsUART1("\n\rEXECUING TEST => UNIT_sfPMODF_WriteStatusReg()\n\r ");
 	sprintf(results,"Register-> %x\n\r", command.ucStatusRegister);
	putsUART1(results);

*/
}


int UNIT_sfPMODF_WriteStatusReg()
{
/*
  	PMODSFCOMMAND command;
	char results[1024];
	putsUART1("\n\rEXECUING TEST => UNIT_sfPMODF_WriteStatusReg()\n\r");
    command.ucStatusRegister = 0;
    command.ucSpiChannel = SPI_C2;
	fnSPIConfigureChannelMasterForPMODSF(SPI_C2,80000000,156250);
	command.ucInstruction = PMODSF_WRITE_STATUS_REG;
	command.ucStatusRegister = PMODSF_SR_BP2|PMODSF_SR_BP1|PMODSF_SR_BP0;

	fnPmodSFsendCommand(&command);
	sprintf(results,"Write Register-> %x\n\r", command.ucStatusRegister);
	putsUART1(results);
	command.ucInstruction = PMODSF_READ_STATUS_REG;
    command.ucStatusRegister = 0;
	fnPmodSFsendCommand(&command);

  	sprintf(results,"Read Register-> %x\n\r", command.ucStatusRegister);
	putsUART1(results);
*/

}

int UNIT_sfPMODF_PageProgram()
{
	uint8_t statusReg = 0;
	char results[1024];
	int numBytesToWrite = 100;
	int i = 0;
	unsigned int address= 0x010000;
	unsigned char data[255];
	memset(data,0,255);
	PmodSFDisableBlockProtection(2,PMODSF_SR_BP2|PMODSF_SR_BP1|PMODSF_SR_BP0);
    PmodSFSectorErase(2,address);
	putsUART1("\n\rEXECUING TEST => UNIT_sfPMODF_PageProgram()\n\r ");
	sprintf(results,"Writiing %d bytes to PMODSF at %x\n\r",numBytesToWrite,address);
	putsUART1(results);
	for(i = 0; i < numBytesToWrite;i++)
	{
		data[i] =  i + '0';
		sprintf(results,"Byte Written %d = %d\n\r",i,data[i]);
		putsUART1(results);
	}
	statusReg =  PmodSFReadStatusRegister(2);
	PmodSFPageProgram(2,numBytesToWrite,data,address);

	//READ FROM PMODSF address 0x0

	sprintf(results,"Reading %d bytes from PMODSF from address %x\n\r",numBytesToWrite,address);
	putsUART1(results);
	memset(data,0,255);
   
    PmodSFReadBytes(2,numBytesToWrite,data,address);
	for(i = 0;i < numBytesToWrite;i++)
	{
		sprintf(results,"Byte Read %d = %d\n\r",i,data[i]);
		putsUART1(results);
	}

}




int SetupSerialLogging(unsigned int baud_rate,unsigned int pbClock)
{
	// UART 1 port pins - connected to PC
	/* JE-01 CN20/U1CTS/RD14 		RD14
	   JE-02 U1RTS/BCLK1/CN21/RD15  RD15
	*/
	PORTSetPinsDigitalIn (IOPORT_F, BIT_2);
	PORTSetPinsDigitalOut (IOPORT_F, BIT_8);
	OpenUART1 (UART_EN | UART_IDLE_CON | UART_RX_TX | UART_DIS_WAKE | UART_DIS_LOOPBACK | UART_DIS_ABAUD | UART_NO_PAR_8BIT | UART_1STOPBIT | UART_IRDA_DIS | 
               UART_MODE_FLOWCTRL | UART_DIS_BCLK_CTS_RTS | UART_NORMAL_RX | UART_BRGH_SIXTEEN,
               UART_TX_PIN_LOW | UART_RX_ENABLE | UART_TX_ENABLE | UART_INT_TX | UART_INT_RX_CHAR | UART_ADR_DETECT_DIS	| UART_RX_OVERRUN_CLEAR, 
			   mUARTBRG(pbClock, baud_rate));

}

int getIntegerFromConsole()
{
		char recievedChars[10];
		char oneChar;
		int bufPos = 0;
		do
		{
			while(!UARTReceivedDataIsAvailable(UART1));
			oneChar = UARTGetDataByte(UART1);
			UARTSendDataByte(UART1,oneChar); //echo to console
			if(oneChar != '\r' && oneChar != '\b')	
			{
				recievedChars[bufPos] = oneChar;			
			}			
			else
			{
				UARTSendDataByte(UART1,'\n');
				UARTSendDataByte(UART1,'\r');
			}	
			if(oneChar != '\b') //TODO: fix backspace
				bufPos++;
		}while(bufPos < 9 && oneChar != '\r');

		recievedChars[bufPos] = 0;

		return atoi(recievedChars);
	
}

int ConsoleMenu(char *testNames[],int numCommands)
{
	int selection;
	char menuItem[100];
	int index = 0;
	
	do
	{
		putsUART1("\r\nCerebot32MX4 SPI Test Menu\n\r");
	
		for(index = 0; index < numCommands;index++)
		{	
			sprintf(menuItem,"%d) %s\n\r",index,testNames[index]);
			putsUART1(menuItem);
		}
		putsUART1("Select=>");
		selection = getIntegerFromConsole();
		
		if(selection < 0 || selection > numCommands)
		{
			putsUART1("\n\rInvalid Selection\n\r");
		}
		
	}while(selection < 0 || selection > numCommands);

	return selection;

}


