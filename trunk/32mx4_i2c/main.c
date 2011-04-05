
#include <plib.h>
#include <stdio.h>
#include "32mx4_i2c.h"

int main(void)
{
	UINT8 data = 0;
	DBPRINTF(" Starting now!!!\n");
	I2C_Initialize( I2C1, 400000, NULL );


	
	DBPRINTF("I2c1\n");
	write_I2C( I2C1, 0x50, 45 );
	data = eeprom_read_current_byte( I2C1, 0x50);

	

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