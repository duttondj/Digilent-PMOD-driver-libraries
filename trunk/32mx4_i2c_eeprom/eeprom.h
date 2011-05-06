#ifndef _EEPROM_H
	#define _EEPROM_H
	
	#include "32mx4_i2c.h"
	#define I2C_EEPROM_ADDRESS 0x50

	// positioning
	UINT8 eeprom_set_current(I2C_MODULE id, UINT8 slaveAddr, UINT8 addressHight, UINT8 addressLow);
	
	// read
	UINT8 eeprom_read_address( I2C_MODULE id, UINT8 slaveAddr, UINT8 highAddr, UINT8 lowAddr);
	UINT8 eeprom_read_current(I2C_MODULE id, UINT8 slaveAddr);
	UINT8 eeprom_sequential_read_current(I2C_MODULE id, UINT8 slaveAddr, UINT8 count, UINT8 buffer[]);
	
	// poll
	UINT8 eeprom_wait_4_ready( I2C_MODULE id, UINT8 slaveAddr);
	
	// write
	UINT8 eeprom_write_address( I2C_MODULE id, UINT8 slaveAddr, UINT8 highAddr, UINT8 lowAddr, UINT8 data);
	UINT8 eeprom_sequential_write_page(I2C_MODULE id, UINT8 slaveAddr,UINT8 highAddr, UINT8 lowAddr, UINT8 count, UINT8 buffer[]);
	
	//UINT8 eeprom_squential_write(  // that handles multiple pages
	
#endif
