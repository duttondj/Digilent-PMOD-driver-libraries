#include "pmodsf.h"

void fnPmodSFsendCommand(PMODSFCOMMAND *command)
{
	uint8_t tempInstruction;
	switch(command->ucInstruction)
	{
		case PMODSF_READ_ID:
			fnPmodSFReadID(command);
			break;
		case PMODSF_WRITE_ENABLE:
			fnPmodSFCommandNoReturn(command);
			break;
		case PMODSF_WRITE_DISABLE:
			fnPmodSFCommandNoReturn(command);
			break;
		case PMODSF_READ_STATUS_REG:
			fnPmodSFReadStatusRegister(command);
			break;
		case PMODSF_WRITE_STATUS_REG:
			tempInstruction = command->ucInstruction;
			command->ucInstruction = PMODSF_WRITE_ENABLE;
			fnPmodSFCommandNoReturn(command); //enable write to status register
 			command->ucInstruction = tempInstruction;
			fnPmodSFWriteStatusRegister(command); 
			break;
		case PMODSF_READ_DATA_BYTES:
			break;
		case PMODSF_READ_DATA_BYTES_HS:
			break;
		case PMODSF_PAGE_PGM:
			break;
		case PMODSF_SECTOR_ERASE:
			break;
		case PMODSF_BULK_ERASE:
			break;
		case PMODSF_DEEP_POWER_DOWN:
			break;
		case PMODSF_RELEASE_FROM_DPD:
			break;
	}
}

void fnPmodSFReadStatusRegister(PMODSFCOMMAND *command)
{
	uint8_t *spiBuf = SpiChnBuffer(command->ucSpiChannel);
    uint8_t ucGarbage;
	fnSPISetSSLow(command->ucSpiChannel); //SS to low
 
   *spiBuf = command->ucInstruction;
    while(!getSPIRcvBufStatus(command->ucSpiChannel));   //Wait for transfer complete
    ucGarbage = *spiBuf;                       //first byte will be garbage
    
	*spiBuf=0x00; //triger xfer
    while(!getSPIRcvBufStatus(command->ucSpiChannel));
	command->ucStatusRegister = *spiBuf;   

    fnSPISetSSHigh(command->ucSpiChannel); //SS to High
}

void fnPmodSFWriteStatusRegister(PMODSFCOMMAND *command)
{
    PMODSFCOMMAND enableWrite;
   	uint8_t *spiBuf = SpiChnBuffer(command->ucSpiChannel);
    uint8_t ucGarbage;
 
	enableWrite.ucInstruction = PMODSF_WRITE_ENABLE;

	fnPmodSFsendCommand(&enableWrite); //enable writing for Write Status Register

	fnSPISetSSLow(command->ucSpiChannel); //SS to low
 
   *spiBuf = command->ucInstruction;
    while(!getSPIRcvBufStatus(command->ucSpiChannel));   //Wait for transfer complete
    ucGarbage = *spiBuf;                       //first byte will be garbage
    
	*spiBuf= command->ucStatusRegister; //triger xfer
    while(!getSPIRcvBufStatus(command->ucSpiChannel));
	ucGarbage = *spiBuf;   

    fnSPISetSSHigh(command->ucSpiChannel); //SS to High
}

void fnPmodSFCommandNoReturn(PMODSFCOMMAND *command)
{
	uint8_t *spiBuf = SpiChnBuffer(command->ucSpiChannel);
    uint8_t ucGarbage;
	fnSPISetSSLow(command->ucSpiChannel); //SS to low
 
   *spiBuf = command->ucInstruction;
    while(!getSPIRcvBufStatus(command->ucSpiChannel));   //Wait for transfer complete
    ucGarbage = *spiBuf;                       //first byte will be garbage

    fnSPISetSSHigh(command->ucSpiChannel); //SS to High
}

void fnPmodSFReadID(PMODSFCOMMAND *command)
{
	uint8_t *spiBuf = SpiChnBuffer(command->ucSpiChannel);
    uint8_t ucGarbage;
	fnSPISetSSLow(command->ucSpiChannel); //SS to low
 
   *spiBuf = command->ucInstruction;
    while(!getSPIRcvBufStatus(command->ucSpiChannel));   //Wait for transfer complete
    ucGarbage = *spiBuf;                       //first byte will be garbage
    
	*spiBuf=0x00; //triger xfer
    while(!getSPIRcvBufStatus(command->ucSpiChannel));
	command->ucID= *spiBuf;   
	
    *spiBuf=0x00; //triger xfer
    while(!getSPIRcvBufStatus(command->ucSpiChannel));
	command->ucMemType = *spiBuf;

    *spiBuf=0x00; //triger xfer
    while(!getSPIRcvBufStatus(command->ucSpiChannel));
	command->ucMemCapacity = *spiBuf;
 
    fnSPISetSSHigh(command->ucSpiChannel); //SS to High
}

uint8_t fnPmodSFReadByte(SPICHANNEL channel)
{
	return SpiChnReadC(channel);
}

void fnSPIConfigureChannelMasterForPMODSF(SPICHANNEL spiChannel,uint32_t ulClock)
{
    fnSPIEnableDigitalPinIO(spiChannel);
    fnSPIConfigureChannelMasterWithFrames(ulClock,spiChannel,SPI_8BIT_MODE,156250);
}