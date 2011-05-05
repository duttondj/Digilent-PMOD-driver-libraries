#include "cerebot32mx4_7_test_driver.h"



int UNIT_spfPMOD_ReadID()
{
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
}

int UNIT_sfPMODF_ReadStatusReg()
{
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


}


int UNIT_sfPMODF_WriteStatusReg()
{
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


}

int UNIT_sfPMODF_PageProgram()
{
  	PMODSFCOMMAND pagePGMcommand;
	PMODSFCOMMAND readCommand;
	PMODSFCOMMAND disableBlockProtect;
	char results[1024];
	int numBytesToWrite = 100;
	int i = 0;
	memset(&pagePGMcommand,0,120);
	memset(&readCommand,0,120);
	memset(&disableBlockProtect,0,120);
	fnSPIConfigureChannelMasterForPMODSF(SPI_C2,80000000,156250);
	uint8_t disableBP = ~(PMODSF_SR_BP2|PMODSF_SR_BP1|PMODSF_SR_BP0); //get the bit mask for turning off all block protection
	disableBlockProtect.ucInstruction = PMODSF_READ_STATUS_REG;
	disableBlockProtect.ucSpiChannel = SPI_C2;
	fnPmodSFsendCommand(&disableBlockProtect);
	disableBlockProtect.ucStatusRegister &= disableBP; //flip the BP bits off
	disableBlockProtect.ucInstruction =  PMODSF_WRITE_STATUS_REG;
    fnPmodSFsendCommand(&disableBlockProtect);

	//WRITE TO PMODSF address 0x0
    pagePGMcommand.ucNumBytesReadWrite = numBytesToWrite;
	pagePGMcommand.ulWriteAddress = 0xA0;
	putsUART1("\n\rEXECUING TEST => UNIT_sfPMODF_PageProgram()\n\r ");
	sprintf(results,"Writiing %d bytes to PMODSF at %x\n\r",numBytesToWrite,pagePGMcommand.ulWriteAddress);
	putsUART1(results);
    pagePGMcommand.ucStatusRegister = 0;
    pagePGMcommand.ucSpiChannel = SPI_C2;
	pagePGMcommand.ucInstruction = PMODSF_PAGE_PGM ;
	for(i = 0; i < numBytesToWrite;i++)
	{
		pagePGMcommand.data[i] =  i + 'A';
		sprintf(results,"Byte Written %d = %d\n\r",i,pagePGMcommand.data[i]);
		putsUART1(results);
	}
	

	fnPmodSFsendCommand(&pagePGMcommand);




	//READ FROM PMODSF address 0x0
	readCommand.ucSpiChannel = SPI_C2;
	readCommand.ucInstruction  =PMODSF_READ_DATA_BYTES;
	readCommand.ucNumBytesReadWrite = 100;
	readCommand.ulReadAddress = 0xA0;
	memset(readCommand.data,0,5);
	fnPmodSFsendCommand(&readCommand);
	sprintf(results,"Reading %d bytes from PMODSF from address %x\n\r",numBytesToWrite,readCommand.ulReadAddress);
	putsUART1(results);
	
	for(i = 0;i < numBytesToWrite;i++)
	{
		sprintf(results,"Byte Read %d = %d\n\r",i,readCommand.data[i]);
		putsUART1(results);
	}

}

int SetupSerialLogging(unsigned int baud_rate)
{
	// UART 1 port pins - connected to PC
	/* JE-01 CN20/U1CTS/RD14 		RD14
	   JE-02 U1RTS/BCLK1/CN21/RD15  RD15
	*/
	unsigned int pb_clock = SYSTEMConfigPerformance (SYSTEM_CLOCK); 
	PORTSetPinsDigitalIn (IOPORT_F, BIT_2);
	PORTSetPinsDigitalOut (IOPORT_F, BIT_8);
	OpenUART1 (UART_EN | UART_IDLE_CON | UART_RX_TX | UART_DIS_WAKE | UART_DIS_LOOPBACK | UART_DIS_ABAUD | UART_NO_PAR_8BIT | UART_1STOPBIT | UART_IRDA_DIS | 
               UART_MODE_FLOWCTRL | UART_DIS_BCLK_CTS_RTS | UART_NORMAL_RX | UART_BRGH_SIXTEEN,
               UART_TX_PIN_LOW | UART_RX_ENABLE | UART_TX_ENABLE | UART_INT_TX | UART_INT_RX_CHAR | UART_ADR_DETECT_DIS	| UART_RX_OVERRUN_CLEAR, 
			   mUARTBRG(pb_clock, baud_rate));

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
