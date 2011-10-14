#ifndef _PMODJSTK_TEST_DRIVER_H_
#define _PMODJSTK_TEST_DRIVER_H_
#include <peripheral/spi.h>
#include <peripheral/uart.h>
#include <peripheral/ports.h>
#include <stdint.h>
#include "pmodJSTK.h"

#define JSTK_X_AXIS 0
#define JSTK_Y_AXIS 1

uint8_t UNIT_PmodJSTKAxisBounds(uint8_t chn, UART_MODULE uartID);
uint8_t UNIT_PmodJSTKSendRecv(uint8_t chn, UART_MODULE uartID);
uint8_t UNIT_PmodJSTKLed_OFF(uint8_t chn, UART_MODULE uartID);
uint8_t UNIT_PmodJSTKLed1_ON(uint8_t chn, UART_MODULE uartID);
uint8_t UNIT_PmodJSTKLed2_ON(uint8_t chn, UART_MODULE uartID);
uint8_t UNIT_PmodJSTKLed1_Led2_ON(uint8_t chn, UART_MODULE uartID);

uint8_t UNIT_PmodJSTKButton_1(uint8_t chn, UART_MODULE uartID);
uint8_t UNIT_PmodJSTKButton_2(uint8_t chn, UART_MODULE uartID);
uint8_t UNIT_PmodJSTKButton_Jstk(uint8_t chn, UART_MODULE uartID);


uint8_t execAxisBounds(uint8_t *testString, uint16_t lowerBound,uint16_t upperBound,UART_MODULE uartID,uint8_t whichAxis,uint8_t chn);
uint8_t execLedTest(uint8_t *testString,uint8_t chn,UART_MODULE uartID,uint8_t command);
uint8_t execButtonTest(uint8_t *testString,uint8_t chn,UART_MODULE uartID,uint8_t button);
uint8_t getOneOrZeroFromConsole(UART_MODULE uartID);
void UARTPutS(uint8_t *string,UART_MODULE uartID);
uint8_t SetupSerialLogging(uint32_t baud_rate,uint32_t pbClock,UART_MODULE uartID);
uint8_t ConsoleMenu(uint8_t *testNames[],uint32_t numCommands,UART_MODULE uartID);
uint8_t getIntegerFromConsole(UART_MODULE uartID);
uint8_t UARTGetOneByte(UART_MODULE uartID);
#endif
