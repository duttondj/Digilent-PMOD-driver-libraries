

#define GetPeripheralClock() 8000000
#define PmodCLS_addr 0x48
#define EEPROM 0x50


int I2C_Initialize( I2C_MODULE id, UINT32 i2cClock, UINT16 address );

void print_i2c_status( I2C_MODULE id);

void send(I2C_MODULE id, UINT8 byte);

UINT8 eeprom_write_current_byte(I2C_MODULE id, UINT8 slaveAddr, UINT8 byte);

UINT8  read_I2C(I2C_MODULE id, UINT8 slaveAddr);

UINT8 get( I2C_MODULE id);

void  write_I2C(I2C_MODULE id, UINT8 slaveAddr, UINT8 data );