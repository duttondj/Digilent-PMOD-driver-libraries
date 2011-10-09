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
	UARTPutS("\n\rEXECUING TEST => UNIT_spfPMOD_DPD_Release()\n\r",UART1);
	UARTPutS("Entering Deep Power Down\n\r",UART1);

	PmodSFDeepPowerDown(chn);
	UARTPutS("Exiting Deep Power Down\n\r",UART1);
	BlockWhileWriteInProgress(chn);
	sprintf(results,"Exited deep power down with signature => %x\n\r",PmodSFDeepPowerDownRelease(chn));
	UARTPutS(results,UART1);
}

uint8_t UNIT_spfPMOD_ReadID(uint8_t chn)
{
	uint32_t pmodSFID = 0;
	char results[128];
	UARTPutS("\n\rEXECUING TEST =>  UNIT_spfPMOD_ReadID()\n\r",UART1);
    pmodSFID = PmodSFReadID(chn);
	sprintf(results,"MFID: 0x%x\n\r Type: 0x%x\n\r Capacity: 0x%x\n\r",	
             fnGetByteFromUint32_t(pmodSFID,PMODSD_MFID_BYTE),
             fnGetByteFromUint32_t(pmodSFID,PMODSD_MEM_TYPE_BYTE),
             fnGetByteFromUint32_t(pmodSFID,PMODSD_MEM_CAPACITY_BYTE));
	UARTPutS(results,UART1);

}

uint8_t UNIT_sfPMODF_ReadStatusReg(uint8_t chn)
{
	uint8_t pmodStatusReg = 0;
	char results[128];
	UARTPutS("\n\rEXECUING TEST =>  UNIT_sfPMODF_ReadStatusReg()\n\r ",UART1);

	pmodStatusReg =  PmodSFReadStatusRegister(chn);
	sprintf(results,"Status Register: 0x%x\n\r",pmodStatusReg);	
	UARTPutS(results,UART1);
}

/*
UNIT TEST: PmodSFWriteStatusRegister/PmodSFReadStatusRegister
All block protect bits are set to enabled, the status register 
is read if they match PmodSFSetStatusRegBits passes, all block
protect bits are cleared, if the status register is == 0 then
PmodSFClearStatusRegBits passed. Both tests must achieve a pass
for the entire test to pass.
*/
uint8_t UNIT_sfPMODF_ClearSetStatusRegBits(uint8_t chn)
{
	uint8_t results[128];
	uint8_t testResult = 1;

	UARTPutS("\n\rEXECUING TEST => UNIT_sfPMODF_ClearSetStatusRegBits()\n\r",UART1);
	UARTPutS("Setting bits for PMODSF_SR_BP2|PMODSF_SR_BP1|PMODSF_SR_BP0\r\n",UART1);

	PmodSFWriteStatusRegister(chn,0);//clear status register

	PmodSFSetStatusRegBits(chn,PMODSF_SR_BP2|PMODSF_SR_BP1|PMODSF_SR_BP0);

	BlockWhileWriteInProgress(chn);
	testResult &= (PmodSFReadStatusRegister(chn) == (PMODSF_SR_BP2|PMODSF_SR_BP1|PMODSF_SR_BP0) )?1:0;
	
	if(testResult) UARTPutS("PmodSFSetStatusRegBits passed\r\n",UART1);
	
	
	UARTPutS("Clearing bits for PMODSF_SR_BP2|PMODSF_SR_BP1|PMODSF_SR_BP0\r\n",UART1);

	PmodSFClearStatusRegBits(chn,PMODSF_SR_BP2|PMODSF_SR_BP1|PMODSF_SR_BP0);

	BlockWhileWriteInProgress(chn);
	testResult &= (PmodSFReadStatusRegister(chn) == 0)?1:0;
	
	if(testResult) UARTPutS("PmodSFClearStatusRegBits passed\r\n",UART1);

	return testResult;
}

/*
UNIT TEST: PmodSFSectorErase
All bytes in a 256 byte buffer are set to 0, these bytes are written 
to every page in sector 0, a sector erase is performed and all bits 
in sector 0 are checked, if they are all 1 (sector erase sets all bits to 1)
the test will pass, otherwise the test will fail. 
*/
uint8_t UNIT_sfPMODF_SectorErase(uint8_t chn)
{
	uint8_t test[128];
	uint32_t sectorSize = 0;
	uint32_t address = 0;
	uint8_t data[PMOD_SF_PAGE_LEN];
	uint8_t testResults = 1;
	UARTPutS("\n\rEXECUTING TEST =>UNIT_sfPMODF_SectorErase()\r\n",UART1);
	
	sectorSize = (pmodFlashCapacity == PMODSF_16_MBIT)?PMODSF_16_SECTOR_SIZE:PMODSF_128_SECTOR_SIZE;
	
	for(address = 0;address < sectorSize;address+=PMOD_SF_PAGE_LEN)
	{
		memset(data,0,PMOD_SF_PAGE_LEN);
		PmodSFPageProgram(chn,PMOD_SF_PAGE_LEN,data,address);
	}
	
	PmodSFSectorErase(chn,0);

	for(address = 0;address < sectorSize;address+=PMOD_SF_PAGE_LEN)
	{
		int i = 0;
		PmodSFReadBytes(chn,PMOD_SF_PAGE_LEN,data,address);
		for(i = 0;i < PMOD_SF_PAGE_LEN;i++)
		{
			testResults &= (data[i] == 255)?1:0;
			if(!testResults)
			{
				sprintf(test,"Failed at address: %d offset %d value=>%d\r\n",address,i,data[i]);
				UARTPutS(test,UART1);
			
			}
		}
	}

	return testResults;
}
/*
UNIT TEST: 	PmodSFPageProgram
A sector erase is performed on address 0h. All bytes in a 256 byte buffer 
are set to address 0h, these bytes are written to the first page.
The first page is read from address 0h and compared to the bytes originally
written, if they match the test passes, otherwise it fails.
*/
uint8_t UNIT_sfPMODF_PageProgram(uint8_t chn)
{
	uint8_t results[128];
	uint32_t i = 0;
	uint8_t testResults = 1;
	uint32_t address= 0x000000;
	uint8_t dataIn[PMOD_SF_PAGE_LEN];
	uint8_t dataOut[PMOD_SF_PAGE_LEN];

	memset(dataOut,0,PMOD_SF_PAGE_LEN);

	PmodSFSectorErase(chn,address);

	UARTPutS("\n\rEXECUTING TEST => UNIT_sfPMODF_PageProgram()\r\n",UART1);
	sprintf(results,"Writiing %d bytes to PMODSF at address %x\n\r", PMOD_SF_PAGE_LEN,address);
	UARTPutS(results,UART1);
	for(i = 0; i < PMOD_SF_PAGE_LEN;i++)
	{
		dataOut[i] =  i;
	}

	PmodSFPageProgram(chn,PMOD_SF_PAGE_LEN,dataOut,address);

	//READ FROM PMODSF address 0x0

	sprintf(results,"Reading %d bytes from PMODSF from address %x\n\r",PMOD_SF_PAGE_LEN,address);
	UARTPutS(results,UART1);

    PmodSFReadBytes(chn,PMOD_SF_PAGE_LEN,dataIn,address);
	for(i = 0;i < PMOD_SF_PAGE_LEN;i++)
	{
		testResults &= (dataIn[i] == dataOut[i])?1:0;
		if(!testResults)
		{
			sprintf(results,"Failed at address: %d offset %d value=>%d\r\n",address,i,dataIn[i]);
			UARTPutS(results,UART1);		
		}
	}
	return testResults;
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
		UARTPutS("\r\nCerebot32MX4 SPI Test Menu\n\r",UART1);
	
		for(index = 0; index < numCommands;index++)
		{	
			sprintf(menuItem,"%d) %s\n\r",index,testNames[index]);
			UARTPutS(menuItem,UART1);
		}
		UARTPutS("Select=>",UART1);
		selection = getIntegerFromConsole();
		
		if(selection < 0 || selection > numCommands)
		{
			UARTPutS("\n\rInvalid Selection\n\r",UART1);
		}
		
	}while(selection < 0 || selection > numCommands);

	return selection;

}


uint8_t fnGetByteFromUint32_t(uint32_t value,uint8_t bytePos)
{
	return value >> (bytePos * 8) & 255;
}

void UARTPutS(char *string,UART_MODULE uartID)
{
	while(*string != '\0')
	{
		while(!UARTTransmitterIsReady(uartID));
		UARTSendDataByte(uartID, *string);

		string++;
	}
}
