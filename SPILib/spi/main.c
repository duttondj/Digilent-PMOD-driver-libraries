#include <peripheral/spi.h>
#include <stdio.h>
#include <string.h>
#include "cerebot32mx4_7_test_driver.h"
#include "spi_mx4_7.h"
#include "pmodsf.h"
#pragma config FNOSC = PRIPLL
#pragma config FPLLMUL = MUL_20
#pragma config FPLLIDIV = DIV_2
#pragma config FPBDIV = DIV_2
#pragma config FPLLODIV = DIV_1
#define NUM_TEST_FUNCTIONS 4


int main(void)
{
	char *menuItems[NUM_TEST_FUNCTIONS] = {"UNIT_spfPMOD_ReadID()","UNIT_sfPMODF_ReadStatusReg()","UNIT_sfPMODF_WriteStatusReg()","UNIT_sfPMODF_PageProgram()"};
	int (*testFunc[NUM_TEST_FUNCTIONS])(void) = {UNIT_spfPMOD_ReadID,UNIT_sfPMODF_ReadStatusReg,UNIT_sfPMODF_WriteStatusReg,UNIT_sfPMODF_PageProgram};
	
 	SetupSerialLogging(9600);
	
	while(1)
	{
		(*testFunc[ConsoleMenu(menuItems,NUM_TEST_FUNCTIONS)])();
	}

}