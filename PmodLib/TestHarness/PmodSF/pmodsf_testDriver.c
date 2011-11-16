/************************************************************************/
/*                                                                      */
/*   pmodsf_test_driver.c -- test driver functions for PmodSF           */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  This file contains implementation for functions used for use with   */
/*  the  Digilent PMODSF on Digilent Pic32 based boards                 */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  10/20/2011(RyanH):                                                  */
/*                                                                      */
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "./TestHarness/PmodSF/pmodsf_testDriver.h"

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */



/*UNIT TEST: PmodSFDeepPowerDown/PmodSFDeepPowerDownRelease
This feature is only supported on the PMODSF-16. This
test places the PMODSF in deep power down mode, then releases
and reads the electronic signature, if a value of 14h will return
a passing result.
*/
uint8_t UNIT_spfPMOD_DPD_Release(UART_MODULE uartID)
{
	uint8_t results[128];
	uint8_t signature = 0;
	UARTPutS("\n\rEXECUING TEST => UNIT_spfPMOD_DPD_Release()\n\r",uartID);
	if(pmodFlashCapacity == PMODSF_128_MBIT)
	{
		UARTPutS("Not Supported on PMODSF-128\r\n",uartID);
		return 0;
	}
	UARTPutS("Entering Deep Power Down\n\r",uartID);
	PmodSFDeepPowerDown(chn);

	UARTPutS("Exiting Deep Power Down\n\r",uartID);
	PmodSFBlockWhileWriteInProgress(chn);

	signature = PmodSFDeepPowerDownRelease(chn);

	sprintf(results,"Exited deep power down with signature => %x\n\r",signature);
	UARTPutS(results,uartID);

	return (signature == PMODSF_16_RES_SIG)?1:0;
}
/*
UNIT TEST: PmodSFReadID
Reads the manufacturer's ID, for the PMODSF-16 and PMODSF-128
if the correct value is returned the test passes.
*/
uint8_t UNIT_spfPMOD_ReadID(UART_MODULE uartID)
{
	uint32_t pmodSFID = 0;
	uint8_t results[128];
	UARTPutS("\n\rEXECUING TEST =>  UNIT_spfPMOD_ReadID()\n\r",uartID);
    pmodSFID = PmodSFReadID(chn);
	sprintf(results,"MFID: 0x%x\n\rType: 0x%x\n\rCapacity: 0x%x\n\r",	
            fnPmodGetByteFromUint32(pmodSFID,PMODSD_MFID_BYTE),
             fnPmodGetByteFromUint32(pmodSFID,PMODSD_MEM_TYPE_BYTE),
             fnPmodGetByteFromUint32(pmodSFID,PMODSD_MEM_CAPACITY_BYTE),pmodSFID);
	UARTPutS(results,uartID);
	if(pmodFlashCapacity == PMODSF_128_MBIT)
	{
		return (pmodSFID == PMODSF_128_MFID)?1:0;
	}
	else
	{
		return (pmodSFID == PMODSF_16_MFID)?1:0;
	}

}
/*
UNIT TEST: PmodSFReadStatusRegister/PmodSFWriteStatusRegister

Prompts for a bitmask, sets the status register to that 
value, the status register is then read and the values compared.
If the values are the same the test returns a pass.
*/
uint8_t UNIT_sfPMODF_ReadStatusReg(UART_MODULE uartID)
{
	uint8_t results[128];
	uint8_t statusRegIn = 0;
	uint8_t statusRegOut = 0;

	UARTPutS("\n\rEXECUING TEST =>  UNIT_sfPMODF_ReadStatusReg()\n\r ",uartID);
	UARTPutS("Enter an 8-bit register value (base 10)=>",uartID);

	statusRegIn = getIntegerFromConsole(uartID);
	PmodSFWriteStatusRegister(chn,statusRegIn);
	
	PmodSFBlockWhileWriteInProgress(chn);//wait for WEL/WIP bit to reset
	statusRegOut =  PmodSFReadStatusRegister(chn);
	
	sprintf(results,"Status Register: 0x%x\n\r",statusRegOut);	
	UARTPutS(results,uartID);
	
	PmodSFWriteStatusRegister(chn,0); //clear status register
	
	return (statusRegIn == statusRegOut)?1:0;	
}

/*
UNIT TEST: PmodSFWriteStatusRegister/PmodSFReadStatusRegister
PmodSFSetStatusRegBits/PmodSFClearStatusRegBits
All block protect bits are set to enabled, the status register 
is read if they match PmodSFSetStatusRegBits passes, all block
protect bits are cleared, if the status register is == 0 then
PmodSFClearStatusRegBits passed. Both tests must achieve a pass
for the entire test to pass.
*/
uint8_t UNIT_sfPMODF_ClearSetReadWriteStatusRegBits(UART_MODULE uartID)
{
	uint8_t results[128];
	uint8_t testResult = 1;

	UARTPutS("\n\rEXECUING TEST => UNIT_sfPMODF_ClearSetStatusRegBits()\n\r",uartID);
	UARTPutS("Setting bits for PMODSF_SR_BP2|PMODSF_SR_BP1|PMODSF_SR_BP0\r\n",uartID);

	PmodSFWriteStatusRegister(chn,0);//clear status register

	PmodSFSetStatusRegBits(chn,PMODSF_SR_BP2|PMODSF_SR_BP1|PMODSF_SR_BP0);

	PmodSFBlockWhileWriteInProgress(chn);
	testResult &= (PmodSFReadStatusRegister(chn) == (PMODSF_SR_BP2|PMODSF_SR_BP1|PMODSF_SR_BP0) )?1:0;
	
	if(testResult) UARTPutS("PmodSFSetStatusRegBits passed\r\n",uartID);
	
	UARTPutS("Clearing bits for PMODSF_SR_BP2|PMODSF_SR_BP1|PMODSF_SR_BP0\r\n",uartID);

	PmodSFClearStatusRegBits(chn,PMODSF_SR_BP2|PMODSF_SR_BP1|PMODSF_SR_BP0);

	PmodSFBlockWhileWriteInProgress(chn);
	testResult &= (PmodSFReadStatusRegister(chn) == 0)?1:0;
	
	if(testResult) UARTPutS("PmodSFClearStatusRegBits passed\r\n",uartID);

	return testResult;
}

/*
UNIT TEST: PmodSFSectorErase
All bytes in a 256 byte buffer are set to 0, these bytes are written 
to every page in sector 0, a sector erase is performed and all bits 
in sector 0 are checked, if they are all 1 (sector erase sets all bits to 1)
the test will pass, otherwise the test will fail. 
*/
uint8_t UNIT_sfPMODF_SectorErase(UART_MODULE uartID)
{
	uint8_t test[128];
	uint32_t sectorSize = 0;
	uint32_t address = 0;
	uint8_t data[PMODSF_PAGE_LEN];
	uint8_t testResults = 1;
	UARTPutS("\n\rEXECUTING TEST =>UNIT_sfPMODF_SectorErase()\r\n",uartID);
	
	sectorSize = (pmodFlashCapacity == PMODSF_16_MBIT)?PMODSF_16_SECTOR_SIZE:PMODSF_128_SECTOR_SIZE;
	
	for(address = 0;address < sectorSize;address+=PMODSF_PAGE_LEN)
	{
		memset(data,0,PMODSF_PAGE_LEN);
		PmodSFPageProgram(chn,PMODSF_PAGE_LEN,data,address);
	}
	
	PmodSFSectorErase(chn,0);

	for(address = 0;address < sectorSize;address+=PMODSF_PAGE_LEN)
	{
		int i = 0;
		PmodSFReadBytes(chn,PMODSF_PAGE_LEN,data,address);
		for(i = 0;i < PMODSF_PAGE_LEN;i++)
		{
			testResults &= (data[i] == 255)?1:0;
			if(!testResults)
			{
				sprintf(test,"Failed at address: %d offset %d value=>%d\r\n",address,i,data[i]);
				UARTPutS(test,uartID);
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
uint8_t UNIT_sfPMODF_PageProgram(UART_MODULE uartID)
{
	uint8_t results[128];
	uint32_t i = 0;
	uint8_t testResults = 1;
	uint32_t address= 0x000000;
	uint8_t dataIn[PMODSF_PAGE_LEN];
	uint8_t dataOut[PMODSF_PAGE_LEN];

	memset(dataOut,0,PMODSF_PAGE_LEN);

	PmodSFSectorErase(chn,address);

	UARTPutS("\n\rEXECUTING TEST => UNIT_sfPMODF_PageProgram()\r\n",uartID);
	sprintf(results,"Writiing %d bytes to PMODSF at address %x\n\r", PMODSF_PAGE_LEN,address);
	UARTPutS(results,uartID);
	for(i = 0; i < PMODSF_PAGE_LEN;i++)
	{
		dataOut[i] =  i;
	}

	PmodSFPageProgram(chn,PMODSF_PAGE_LEN,dataOut,address);

	//READ FROM PMODSF address 0x0

	sprintf(results,"Reading %d bytes from PMODSF from address %x\n\r",PMODSF_PAGE_LEN,address);
	UARTPutS(results,uartID);

    PmodSFReadBytes(chn,PMODSF_PAGE_LEN,dataIn,address);
	for(i = 0;i < PMODSF_PAGE_LEN;i++)
	{
		testResults &= (dataIn[i] == dataOut[i])?1:0;
		if(!testResults)
		{
			sprintf(results,"Failed at address: %d offset %d value=>%d\r\n",address,i,dataIn[i]);
			UARTPutS(results,uartID);		
		}
	}
	return testResults;
}
/*UNIT TEST: PmodSFBulkErase
The first three pages starting at address 0h are written with
0s in all bits. A bulk erase is performed setting all bits to 1,
the same three pages are read, if all bits read are set to 1
the test returns a pass.
*/
uint8_t UNIT_sfPMODF_BulkErase(UART_MODULE uartID)
{
	uint8_t dataOut[PMODSF_PAGE_LEN];
	uint8_t dataIn[PMODSF_PAGE_LEN*3];
	uint8_t testResult = 1;
	uint32_t byteCount = 0;

	UARTPutS("\n\rEXECUTING TEST => UNIT_sfPMODF_BulkErase()\r\n",uartID);
	memset(dataOut,0,PMODSF_PAGE_LEN);
	//set all bytes of first 3 pages to all 0
	PmodSFPageProgram(chn,PMODSF_PAGE_LEN,dataOut,0);
	PmodSFPageProgram(chn,PMODSF_PAGE_LEN,dataOut,PMODSF_PAGE_LEN);
	PmodSFPageProgram(chn,PMODSF_PAGE_LEN,dataOut,PMODSF_PAGE_LEN *2);
	PmodSFBulkErase(chn);
	PmodSFReadBytes(chn,PMODSF_PAGE_LEN,dataIn,0);
	for(byteCount=0; byteCount < PMODSF_PAGE_LEN; byteCount++)
	{
		testResult &= (dataIn[byteCount] == 255)?1:0;
	}
	return testResult;
}

/*  
** fnSetPmodFlashCapacity
**
**	Synopsis:
**  Polls PmodSF for capacity and sets pmodFlashCapacity
**  accordingly
**
**  Input: 
**		uint8_t chn - Pmod SPI channel
**      UART_MODULE uart - serial console UART
** 
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  The manufucture's ID is read from the PmodSF and 
**  pmodFlashCapacity is set to the value in the
**  byte position returned determiend by PMODSD_MEM_CAPACITY_BYTE.
**  A string is sent to the serial console detailing Pmod 
**  detected. pmmodFlashCapacity is use during unit tests to 
**  exclude tests unsopported by the detected module.
*/
void fnSetPmodFlashCapacity(UART_MODULE uart)
{
	 uint8_t pmodSFID = PmodSFReadID(chn);
	 pmodFlashCapacity = fnPmodGetByteFromUint32(pmodSFID,PMODSD_MEM_CAPACITY_BYTE);
	 if(pmodFlashCapacity == PMODSF_128_MBIT)
	{
		UARTPutS("\r\n**PMODSF-128 Detected**",uart);
	}
	else
	{
		UARTPutS("\r\n**PMODSF-16 Detected**",uart);
	}
}

/*  
**  fnInitPmodSF
**
**	Synopsis:
**  Initializes the SPI port for the PmodSF and detemines 
**  its flash capacity.
**
**  Input: 
**  	uint8_t chn - Pmod SPI channel
**      uint32_t pbClock - peripheral bus clock rate in Hz
**      uint32_t bitRate - bitrate in Hz
**      UART_MODULE uart - serial console UART
**
**  Returns: none
**
**	Errors:	none
**
**  Description:
**  Initializes the SPI module at the specified bitrate for the PmodSF,
**  calls fnSetPmodFlashCapacity to set the global variable pmmodFlashCapacity
*/
void fnInitPmodSF(UART_MODULE uartID)
{
	UARTPutS("\r\nPmodSF SPI port=>",uartID);
	chn =  getIntegerFromConsole(uartID); //SPI port PMODSF is connected to
	PmodSFInit(chn,PB_CLOCK,SPI_BITRATE);
	fnSetPmodFlashCapacity(uartID);
}
