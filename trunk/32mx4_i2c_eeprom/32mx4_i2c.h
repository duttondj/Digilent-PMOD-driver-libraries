#ifndef _32MX4_I2C_H
	#define _32MX4_I2C_H
	
	
	
	
	// The 32mx4 has a 8000000 mhz clock
	// THIS IS NOT GOOD 
	#define GetPeripheralClock() 8000000
	
	
	// EEPROM Slave Address

	
	//todo consider return values
	void I2C_Initialize( I2C_MODULE id, UINT32 i2cClock, UINT16 address );
	BOOL I2C_send(I2C_MODULE id, UINT8 byte);
	BOOL I2C_send_checkAck(I2C_MODULE id, UINT8 byte);
	BOOL I2C_checkAck( I2C_MODULE id);
	BOOL I2C_startBuss(I2C_MODULE id);
	VOID I2C_stopBuss(I2C_MODULE id);
	void print_i2c_status( I2C_MODULE id);
	
	// redo these there is a macro consider ailising that
	UINT8 writeTo(UINT8 addr);   
	UINT8 readTo(UINT8 addr);  
	
//	UINT8 startBuss(I2C_MODULE id);
	//////
//	UINT8 sendWithAck(I2C_MODULE id, UINT8 byte);
//	UINT8 send(I2C_MODULE id, UINT8 byte);
	
//	UINT8 read_I2C(I2C_MODULE id, UINT8 slaveAddr);
//	UINT8 sendWithAck(I2C_MODULE id, UINT8 byte);
//	UINT8 get( I2C_MODULE id);
//	UINT getAck( I2C_MODULE id);
//	void  write_I2C(I2C_MODULE id, UINT8 slaveAddr, UINT8 data );
//	void sendWithNoAck(I2C_MODULE id, UINT8 byte);
	UINT I2C_receive( I2C_MODULE id);
	UINT I2C_receiveAndNack( I2C_MODULE id);
	UINT I2C_receiveAndAck( I2C_MODULE id);
	BOOL send_string( I2C_MODULE id, UINT8 address, char * string);
#endif

