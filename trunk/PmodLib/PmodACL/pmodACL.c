
#include "./PmodACL/pmodACL.h"

void PmodACLInitSpi4Wire(SpiChannel chn,uint32_t pbClock,uint32_t bitRate)
{
    SpiChnOpen(chn, SPI_OPEN_MSTEN | SPI_OPEN_SSEN |  SPI_OPEN_MODE8 | SPI_OPEN_CKP_HIGH, pbClock/bitRate);

}

uint8_t PmodACLGetDeviceID(SpiChannel chn)
{
	uint8_t devID = 0;
	PmodSPISetSSLow(chn);
	SpiChnPutC(chn,PMODACL_REG_DEVID); //read DEVID 
	devID = SpiChnGetC(chn);
	PmodSPISetSSHigh(chn);
	return devID;
}

void PmodACLGetAxisData(SpiChannel chn, PMODACL_AXIS *pmodACLAxis)
{
	memset(pmodACLAxis,0,sizeof(PMODACL_AXIS));

	PmodSPISetSSLow(chn);

	SpiChnPutC(chn,PMODACL_MB_BIT|PMODACL_REG_DATAX0); 
	pmodACLAxis->xAxis = (uint16_t)SpiChnGetC(chn) << 8;

	SpiChnPutC(chn,0); 
	pmodACLAxis->xAxis |= (uint16_t)SpiChnGetC(chn);

	SpiChnPutC(chn,0); 
	pmodACLAxis->yAxis = (uint16_t)SpiChnGetC(chn) << 8;

	SpiChnPutC(chn,0); 
	pmodACLAxis->yAxis |= (uint16_t)SpiChnGetC(chn);

	SpiChnPutC(chn,0); 
	pmodACLAxis->zAxis = (uint16_t)SpiChnGetC(chn) << 8;

	SpiChnPutC(chn,0); 
	pmodACLAxis->zAxis |= (uint16_t)SpiChnGetC(chn);
	
	PmodSPISetSSHigh(chn);
	
}
