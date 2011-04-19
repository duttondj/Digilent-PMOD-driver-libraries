#ifndef _32MX4_I2C_H
	#define _32MX4_I2C_H
	
	

	
	// The 32mx4 has a 8000000 mhz clock
	#define GetPeripheralClock() 8000000

	
	// EEPROM Slave Address
	#define EEPROM_slave_addr 0x50

	
	int I2C_Initialize( I2C_MODULE id, UINT32 i2cClock, UINT16 address );
	
	void print_i2c_status( I2C_MODULE id);
	
	UINT8 send(I2C_MODULE id, UINT8 byte);
	
	UINT8 eeprom_write_current_byte(I2C_MODULE id, UINT8 slaveAddr, UINT8 byte);
UINT8 eeprom_sequential_write_page(I2C_MODULE id, UINT8 slaveAddr,UINT8 highAddr, UINT8 lowAddr, UINT8 count, UINT8 buffer[]);
	UINT8 eeprom_sequential_read_current(I2C_MODULE id, UINT8 slaveAddr, UINT8 count, UINT8 buffer[]);
	UINT8 read_I2C(I2C_MODULE id, UINT8 slaveAddr);
	UINT8 sendWithAck(I2C_MODULE id, UINT8 byte);
	UINT8 get( I2C_MODULE id);
	UINT8 eeprom_wait_4_ready( I2C_MODULE id, UINT8 slaveAddr);
int eeprom_write_address( I2C_MODULE id, UINT8 slaveAddr, UINT8 highAddr, UINT8 lowAddr, UINT8 data);
	void  write_I2C(I2C_MODULE id, UINT8 slaveAddr, UINT8 data );
#endif
