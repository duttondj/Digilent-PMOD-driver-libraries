

//disable legacy code
#define _PLIB_DISABLE_LEGACY
// load in support for all plib functionality
#include <plib.h>
	

#include <stdio.h>
#include "32mx4_i2c.h"


int main(void)
{
	UINT8 data = 0;
	DBPRINTF(" Starting now!!!\n");
	I2C_Initialize( I2C1, 400000, NULL );

	UINT8 buffer1[64] = {0,1,2,3,4,5,6,7,8,9,10}; 
	UINT8 buffer2[64];
	int count = 11;
	int index = 0;
	DBPRINTF("I2c1\n");
	eeprom_write_address( I2C1, 0x50, 0, 0, 45 );
	data = eeprom_read_address( I2C1, 0x50, 0, 0);

	DBPRINTF("******I2c1 eeprom_sequential_write_page");
	eeprom_sequential_write_page(I2C1, EEPROM_slave_addr,0,1, count, buffer1);
	DBPRINTF("******I2c1 eeprom_wait_4_ready");
	eeprom_wait_4_ready(I2C1, EEPROM_slave_addr);
	DBPRINTF("******I2c1 eeprom_set_current");
	eeprom_set_current(I2C1,EEPROM_slave_addr,  0, 1);
	DBPRINTF("******I2c1 eeprom_sequential_read_current");
	eeprom_sequential_read_current( I2C1, EEPROM_slave_addr, count, buffer2);
	while(index < count)
	{
		DBPRINTF("I2c1 read count %d: %d\n",index, buffer2[index]);
		index++;
	}

    DBPRINTF("I2c1 read: %d\n", data);
	DBPRINTF("Program Done\n");
	while(1)
	{	
		
	}
}




UINT8 get( I2C_MODULE id)
{

	UINT8 data;
	I2CReceiverEnable(id, TRUE);
	while(!I2CReceivedDataIsAvailable(id)){}
	
//	I2CAcknowledgeByte(id, FALSE);
	
	data = I2CGetByte(id);
	
	I2CReceiverEnable(id, FALSE);
	
}



