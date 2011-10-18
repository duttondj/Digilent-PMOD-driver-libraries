#include <stdint.h>
#include <peripheral/uart.h>
#include "./TestHarness/config.h"
#include "./TestHarness/Common/test_harness_common.h"

#if(PMODSF)
#include "./TestHarness/PmodSF/pmodsf_testDriver.h"


#elif(PMODJSTK)
#include "./TestHarness/PmodJSTK/pmod_jstk_test_driver.h"
uint8_t (*testFunc[NUM_TEST_FUNCTIONS])(uint8_t,UART_MODULE) = &testFuncPmodJSTK;

#endif



int main()
{
	uint8_t channel = 0;
	uint8_t procType[128];
	sprintf(procType,"\r\n**Pic32 %d processor detected**",__PIC32_FEATURE_SET__);
	SetupSerialLogging(9600,PB_CLOCK,UART1); //setup serial console IO
	UARTPutS(procType,UART1); 	
	UARTPutS("\r\nPmodJSTK SPI port=>",UART1);
	channel =  getIntegerFromConsole(UART1); //SPI port PMODSF is connected to
	
	
	
	return 0;
}
