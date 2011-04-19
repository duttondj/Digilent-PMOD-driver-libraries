#include "cerebot32mx4_7_test_driver.h"

int INTEGRATION_spiMasterSlaveNoFramesSameBoardTXRX()
{
	unsigned char myBuffTX[20]= "This is a test"; 
	unsigned char myBuffRX[20]; 
	char* txBuff = myBuffTX;
	char* rxBuff = myBuffRX;
    int delay = 0;
	fnSPIConfigureChannelSlaveNoFrames (SPI_C1, SPI_8BIT_MODE );
	fnSPIConfigureChannelMasterNoFrames (SYSTEM_CLOCK,SPI_C2,  SPI_8BIT_MODE ,156250);

	while(*txBuff != '\0')
	{
		SpiChnPutC(SPI_CHANNEL2,*txBuff);
		for(delay = 0;delay < 40;delay++)
		{
			asm("nop");
		}
		if(SpiChnDataRdy(SPI_CHANNEL1))
		{
			*rxBuff = SpiChnGetC(SPI_CHANNEL1);
    		rxBuff++;
    		txBuff++;
		}
		else
		{
			break;
		}
	}

    return (strcmp(myBuffTX,myBuffRX) == 0) ? 1:0;

}

int INTEGRATION_spiMasterSlaveFramesSameBoardTXRX()
{
	unsigned char myBuffTX[20]= "This is a test"; 
	unsigned char myBuffRX[20]; 
	char* txBuff = myBuffTX;
	char* rxBuff = myBuffRX;
    int delay = 0;
	fnSPIConfigureChannelSlaveWithFrames (SPI_C1, SPI_8BIT_MODE );
	fnSPIConfigureChannelMasterWithFrames (SYSTEM_CLOCK,SPI_C2,  SPI_8BIT_MODE ,156250);

	while(*txBuff != '\0')
	{
		SpiChnPutC(SPI_CHANNEL2,*txBuff);
		*rxBuff = SpiChnGetC(SPI_CHANNEL1);
    	rxBuff++;
    	txBuff++;
	}
    return (strcmp(myBuffTX,myBuffRX) == 0) ? 1:0;

}

int UNIT_spfPMOD_ReadID()
{
  	PMODSFCOMMAND command;
    command.ucID = command.ucMemType = command.ucMemCapacity = 0;
	fnSPIConfigureChannelMasterForPMODSF(SPI_C2,SYSTEM_CLOCK);
	command.ucInstruction = PMODSF_READ_ID;
	command.ucSpiChannel = SPI_C2;
	fnPmodSFsendCommand(&command);
    printf("ID-> %x", command.ucID);
    printf("Type-> %x",command.ucMemType);
    printf("Capacity-> %x",command.ucMemCapacity);

}