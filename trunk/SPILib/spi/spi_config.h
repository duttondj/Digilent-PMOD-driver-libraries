#ifndef _SPICONFIG_H_
#define _SPICONFIG_H_
#include <peripheral/int.h>

#define useSPI_INTERRUPTS							0

#define useSPI_CHANNEL_1_INTERRUPT					0
#define useSPI_CHANNEL_1_RX_INTERRUPT				0
#define useSPI_CHANNEL_1_TX_INTERRUPT				0
#define useSPI_CHANNEL_1_ERR_INTERRUPT				0
#define configSPI_CHANNEL_1_INTERRUPT_PRIORITY		INT_PRIORITY_LEVEL_7  //Defined in int.h
#define configSPI_CHANNEL_1_INTERRUPT_SUB_PRIORITY	INT_SUB_PRIORITY_LEVEL_0 //Defined in int.h


#define useSPI_CHANNEL_2_INTERRUPT					0
#define useSPI_CHANNEL_2_RX_INTERRUPT				0
#define useSPI_CHANNEL_2_TX_INTERRUPT				0
#define useSPI_CHANNEL_2_ERR_INTERRUPT				0
#define configSPI_CHANNEL_2_INTERRUPT_PRIORITY		INT_PRIORITY_LEVEL_7 //Defined in int.h
#define configSPI_CHANNEL_2_INTERRUPT_SUB_PRIORITY	INT_SUB_PRIORITY_LEVEL_0 //defined in int.h


#endif