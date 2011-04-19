#ifndef _CEREBOT32MX4_7_SPI_TEST_DRIVER_
#define _CEREBOT32MX4_7_SPI_TEST_DRIVER_
#include <stdio.h>
#include <string.h>
#include <peripheral/spi.h>
#include "spi_mx4_7.h"
#include "pmodsf.h"

#define SYSTEM_CLOCK		80000000


int INTEGRATION_spiMasterSlaveNoFramesSameBoardTXRX();
int INTEGRATION_spiMasterSlaveFramesSameBoardTXRX();
int UNIT_spfPMOD_ReadID();


#endif