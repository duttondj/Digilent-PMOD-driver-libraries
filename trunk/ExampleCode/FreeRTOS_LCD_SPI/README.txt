Written By: Ryan Hoffman

This demo shows how to use FreeRTOS mutexes, task delay and demos the beginnings of my SPI library.

Hardware Setup:
1) Connect pins 1-6 on J1 of the PmodCLS to pins 1-6 of Pmod port JB
2) For Rev. E CLS on JP2 pins MDO will have a jumper, MD1 and MD2 will be
   open.

Launch:
1) Open the project file .\FreeRTOS_LCD_SPI\RTOSDEMO\LCD_SPI\RTOSDemo.mcp
2) Run the debugger

The words "Task1" and "Task2" alternate on the CLS and debug output window.
