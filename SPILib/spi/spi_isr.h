#ifndef _SPI_ISR_H_
#define _SPI_ISR_H_

#include "spi_config.h"


/* ------------------------------------------------------------ */
/***	ISR_SPI_CHANNEL_1_Handler
**
**	Synopsis:
**
**	Input:
**	Output:
**	Errors:
**		none
**
**		none
**
**	Description:
*/
#if useSPI_CHANNEL_1_INTERRUPT == 1
void __ISR(INT_SPI_1_VECTOR, configSPI_CHANNEL_1_INTERRUPT_PRIORITY) ISR_SPI_CHANNEL_1_Handler(void)
{

	#if useSPI_CHANNEL_1_TX_INTERRUPT == 1
	if(INTGetFlag(INT_SOURCE_SPI_TX(INT_SPI1)))
	{
		INTClearFlag(INT_SOURCE_SPI_TX(INT_SPI1);
		fnSpi1ISRTx();
	}
	#endif	

	#if useSPI_CHANNEL_1_RX_INTERRUPT == 1
	if(INTGetFlag(INT_SOURCE_SPI_RX(INT_SPI1)))
	{
		INTClearFlag(INT_SOURCE_SPI_RX(INT_SPI1);
		fnSpi1ISRRx();
	}
	#endif

	#if useSPI_CHANNEL_1_ERR_INTERRUPT == 1
	if(INTGetFlag(INT_SOURCE_SPI_ERR(INT_SPI1)))
	{
		INTClearFlag(INT_SOURCE_SPI_ERR(INT_SPI1));
		fnSpi1ISRErr();
	}
	#endif
}

#endif

/* ------------------------------------------------------------ */
/***	ISR_SPI_CHANNEL_2_Handler
**
**	Synopsis:
**
**	Input:
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
*/
#if useSPI_CHANNEL_2_INTERRUPT == 1
void __ISR(INT_SPI_2_VECTOR, configSPI_CHANNEL_2_INTERRUPT_PRIORITY) ISR_SPI_CHANNEL_2_Handler(void)
{

	#if useSPI_CHANNEL_2_TX_INTERRUPT == 1
	if(INTGetFlag(INT_SOURCE_SPI_TX(INT_SPI2)))
	{
		INTGetFlag(INT_SOURCE_SPI_TX(INT_SPI2));
		fnSpi2ISRTx();
		
	}
	#endif

	#if useSPI_CHANNEL_2_RX_INTERRUPT == 1
	if(INTGetFlag(INT_SOURCE_SPI_RX(INT_SPI2)))
	{
		INTClearFlag(INT_SOURCE_SPI_RX(INT_SPI2));
		fnSpi2ISRRx();

	}
	#endif

	#if useSPI_CHANNEL_2_ERR_INTERRUPT == 1
	if(INTGetFlag(INT_SOURCE_SPI_ERR(INT_SPI2)))
	{
		INTClearFlag(INT_SOURCE_SPI_ERR(INT_SPI2));
		fnSpi2ISRErr();
	}
	#endif
}
#endif


