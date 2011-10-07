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


uint8_t UNIT_spfPMOD_DPD_Release(uint8_t chn)
{
	char results[128];
	putsUART1("\n\rEXECUING TEST => UNIT_spfPMOD_DPD_Release()\n\r ");
	putsUART1("Entering Deep Power Down\n\r");

	PmodSFDeepPowerDown(chn);
	putsUART1("Exiting Deep Power Down\n\r");
	BlockWhileWriteInProgress(chn);
	sprintf(results,"Exited deep power down with signature => %x\n\r",PmodSFDeepPowerDownRelease(chn));
	putsUART1(results);
}

uint8_t UNIT_spfPMOD_ReadID(uint8_t chn)
{
	uint32_t pmodSFID = 0;
	char results[128];
	putsUART1("\n\rEXECUING TEST =>  UNIT_spfPMOD_ReadID()\n\r ");
    pmodSFID = PmodSFReadID(chn);
	sprintf(results,"MFID: 0x%x\n\r Type: 0x%x\n\r Capacity: 0x%x\n\r",	
             fnGetByteFromUint32_t(pmodSFID,PMODSD_MFID_BYTE),
             fnGetByteFromUint32_t(pmodSFID,PMODSD_MEM_TYPE_BYTE),
             fnGetByteFromUint32_t(pmodSFID,PMODSD_MEM_CAPACITY_BYTE));
	putsUART1(results);

}

uint8_t UNIT_sfPMODF_ReadStatusReg(uint8_t chn)
{
	uint8_t pmodStatusReg = 0;
	char results[128];
	putsUART1("\n\rEXECUING TEST =>  UNIT_sfPMODF_ReadStatusReg()\n\r ");

	pmodStatusReg =  PmodSFReadStatusRegister(chn);
	sprintf(results,"Status Register: 0x%x\n\r",pmodStatusReg);	
	putsUART1(results);
}


uint8_t UNIT_sfPMODF_ReadWriteStatusReg(uint8_t chn)
{
	char results[128];
	putsUART1("\n\rEXECUING TEST => UNIT_sfPMODF_WriteStatusReg()\n\r");
	putsUART1("Setting bits for PMODSF_SR_BP2|PMODSF_SR_BP1|PMODSF_SR_BP0\r\n");

	PmodSFWriteStatusRegister(chn,0);//clear status register

	PmodSFSetStatusRegBits(chn,PMODSF_SR_BP2|PMODSF_SR_BP1|PMODSF_SR_BP0);

	UNIT_sfPMODF_ReadStatusReg(chn);
	putsUART1("Clearing bits for PMODSF_SR_BP2|PMODSF_SR_BP1|PMODSF_SR_BP0\r\n");

	PmodSFClearStatusRegBits(chn,PMODSF_SR_BP2|PMODSF_SR_BP1|PMODSF_SR_BP0);

	UNIT_sfPMODF_ReadStatusReg(chn);
}

uint8_t UNIT_sfPMODF_PageProgram(uint8_t chn)
{
	uint8_t statusReg = 0;
	char results[1024];
	int numBytesToWrite = 100;
	int i = 0;
	uint32_t address= 0x010000;
	unsigned char data[255];
	memset(data,0,255);
	PmodSFClearStatusRegBits(chn,PMODSF_SR_BP2|PMODSF_SR_BP1|PMODSF_SR_BP0);
	PmodSFSectorErase(chn,address);
	putsUART1("\n\rEXECUING TEST => UNIT_sfPMODF_PageProgram()\n\r ");
	sprintf(results,"Writiing %d bytes to PMODSF at %x\n\r",numBytesToWrite,address);
	putsUART1(results);
	for(i = 0; i < numBytesToWrite;i++)
	{
		data[i] =  i + '0';
		sprintf(results,"Byte Written %d = %d\n\r",i,data[i]);
		putsUART1(results);
	}

	PmodSFPageProgram(chn,numBytesToWrite,data,address);

	//READ FROM PMODSF address 0x0

	sprintf(results,"Reading %d bytes from PMODSF from address %x\n\r",numBytesToWrite,address);
	putsUART1(results);
	memset(data,0,255);
    PmodSFReadBytes(chn,numBytesToWrite,data,address);
	for(i = 0;i < numBytesToWrite;i++)
	{
		sprintf(results,"Byte Read %d = %d\n\r",i,data[i]);
		putsUART1(results);
	}

}


uint8_t  UNIT_sfPMODF_SectorErase(uint8_t chn)
{
	uint16_t numPagesPerSector = 0;
	numPagesPerSector = (pmodFlashCapacity == PMODSF_16_MBIT)?255:1024;
	
}


uint8_t SetupSerialLogging(uint32_t baud_rate,uint32_t pbClock)
{
	// UART 1 port pins - connected to PC
	/* JE-01 CN20/U1CTS/RD14 		RD14
	   JE-02 U1RTS/BCLK1/CN21/RD15  RD15
	*/
	PORTSetPinsDigitalIn (IOPORT_F, BIT_2);
	PORTSetPinsDigitalOut (IOPORT_F, BIT_8);

	OpenUART1 (UART_EN | UART_IDLE_CON | UART_RX_TX |UART_NO_PAR_8BIT | UART_1STOPBIT , UART_RX_ENABLE | UART_TX_ENABLE, 
			   mUARTBRG(pbClock, baud_rate));

}

void fnSetPmodFlashCapacity(uint8_t chn)
{
	 uint8_t pmodSFID = PmodSFReadID(chn);
	 pmodFlashCapacity = fnPMODGetByteFromUint32(pmodSFID,PMODSD_MEM_CAPACITY_BYTE);
	 
}

uint8_t getIntegerFromConsole()
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



uint8_t ConsoleMenu(char *testNames[],uint32_t numCommands)
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


uint8_t fnGetByteFromUint32_t(uint32_t value,uint8_t bytePos)
{
	return value >> (bytePos * 8) & 255;
}
