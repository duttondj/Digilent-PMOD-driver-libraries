
#include <plib.h>
#include <stdio.h>
#include "32mx4_i2c.h"
//#define I2C_Initialize
int I2C_Initialize( I2C_MODULE id, UINT32 i2cClock, UINT16 address )
{
	// Configure Various I2C Options
	I2CConfigure(id, I2C_ENABLE_SLAVE_CLOCK_STRETCHING | I2C_ENABLE_HIGH_SPEED );
	
	// Set Desired Operation Frequency
	UINT32 actualClock = I2CSetFrequency(id, GetPeripheralClock(), i2cClock);
	if ( abs(actualClock-i2cClock) > i2cClock/10 )
	{
	    DBPRINTF("  Clock frequency (%d) error exceeds 10\%\r\n", actualClock);
	}
	else
	{
	    DBPRINTF("  Clock frequency = %ld Hz\n", actualClock);
	}
	
	//  Set the Slave Address
	// This won't work, maybe use a neg number for this?
		// If the slave interupt works the way I think this would be a good place to init that too
	if( address != NULL)
	{
		I2CSetSlaveAddress(id, address, 0, I2C_USE_7BIT_ADDRESS);
		DBPRINTF("  Slave address = 0x%02X\n", address);
	
	}
	else
	{	

		DBPRINTF(" I2C%d is master only.", id +1);
	}	
	// Enable the module
	DBPRINTF("Enabling operations...\n");
	I2CEnable(id, TRUE);
}

void  write_I2C(I2C_MODULE id, UINT8 slaveAddr, UINT8 data )
{
	UINT8  byte;

	 
	while (!I2CBusIsIdle(id)){ DBPRINTF("Waiting on bus idle \n");}

    DBPRINTF(" !writing Starting\n");

    if (I2CStart(id) == I2C_SUCCESS )
    {	// transmiter is ready to send
	    DBPRINTF("Start sent \n");
	    print_i2c_status(id);
	    
	    // sent write to slave
	 	byte = (slaveAddr << 1)|I2C_WRITE;
	    send(id, byte );
	    // 24cl256 internal addr high
	    byte = 0x00;
	 	send(id, byte);	    
	    // 24cl256 internal addr low
	    byte = 0x00;
	 	send(id, byte);	    
	    //data
	    byte = data;
	 	send(id, byte);

	    DBPRINTF("stop\n");
	    I2CStop(id);
 	}      	
	else 
		DBPRINTF("i2c%d failed to start", id + 1 );
}


void print_i2c_status( I2C_MODULE id)
{
	   DBPRINTF("* start checking i2c%d * ", id +1);
	 //I2CClearStatus(I2C1, I2C_ARBITRATION_LOSS);
	I2C_STATUS status = I2CGetStatus(id);
	if(I2C_ARBITRATION_LOSS & status)
	{
	     DBPRINTF("I2C_ARBITRATION_LOSS ");
	}
	if( I2C_DATA_AVAILABLE & status)
	{
	     DBPRINTF(" I2C_DATA_AVAILABLE ");
	}
		if(I2C_SLAVE_READ & status)
	{
	     DBPRINTF("I2C_SLAVE_READ ");
	}
		if( I2C_START & status)
	{
	     DBPRINTF(" I2C_START ");
	}
		if(I2C_STOP & status)
	{
	     DBPRINTF("I2C_STOP ");
	}
		if(I2C_SLAVE_DATA & status)
	{
	     DBPRINTF("I2C_SLAVE_DATA ");
	}
		if(I2C_RECEIVER_OVERFLOW & status)
	{
	     DBPRINTF("I2C_RECEIVER_OVERFLOW ");
	}
 		if(I2C_TRANSMITTER_OVERFLOW & status)
	{
	     DBPRINTF("I2C_TRANSMITTER_OVERFLOW ");
	}
			if(I2C_10BIT_ADDRESS & status)
	{
	     DBPRINTF("I2C_10BIT_ADDRESS ");
	}
			if( I2C_GENERAL_CALL & status)
	{
	     DBPRINTF(" I2C_GENERAL_CALL ");
	}
			if( I2C_TRANSMITTER_BUSY & status)
	{
	     DBPRINTF(" I2C_TRANSMITTER_BUSY ");
	}
			if(I2C_BYTE_ACKNOWLEDGED & status)
	{
	     DBPRINTF("I2C_BYTE_ACKNOWLEDGED ");
	}
	  DBPRINTF("* stop checking i2c%d\n", id +1);
	
}

void send(I2C_MODULE id, UINT8 byte)
{
		while (!I2CTransmitterIsReady(id)){DBPRINTF("Waiting for transmitter to be ready\n");}
				// send address and write to slave
				I2CSendByte(id, (byte << 1)|I2C_WRITE);
				print_i2c_status(id);
				while (!I2CTransmissionHasCompleted(I2C1)){DBPRINTF("Waiting for trans to complete\n");}

			 	if (I2CByteWasAcknowledged(id))
			    {
				   print_i2c_status(id);
			       DBPRINTF("ack - slave responds to call\n");
			    }
}

UINT8  eeprom_read_current_byte(I2C_MODULE id, UINT8 slaveAddr)
{
	UINT8  byte;
//	print_i2c_status(id);
	 
	while (!I2CBusIsIdle(id)){ DBPRINTF("Waiting on bus idle \n");}

    DBPRINTF(" !reading start\n");
//    print_i2c_status(id);
    if (I2CStart(id) == I2C_SUCCESS )
    {	// transmiter is ready to send
	    DBPRINTF("Start sent \n");
//	    print_i2c_status(id);
	    
	    // send slave addr int write mode
	 	byte = (slaveAddr << 1)|I2C_WRITE;
	    send(id, byte );
	       	    
	 	byte = get(id);
		
	    DBPRINTF("stop\n");
	    I2CStop(id);
 	}      	
	else
	{ 
		DBPRINTF("i2c%d failed to start", id + 1 );
		return NULL;
	}
		
	return byte;
}	

UINT8  eeprom_write_current_byte(I2C_MODULE id, UINT8 slaveAddr, UINT8 byte)
{
		while (!I2CBusIsIdle(id)){ DBPRINTF("Waiting on bus idle \n");}
		DBPRINTF("eeprom_write_current_byte starting\n");
		
		
}

UINT8  read_I2C(I2C_MODULE id, UINT8 slaveAddr)
{
	UINT8  byte;
	print_i2c_status(id);
	 
	while (!I2CBusIsIdle(id)){ DBPRINTF("Waiting on bus idle \n");}

    DBPRINTF(" !reading start\n");
    print_i2c_status(id);
    if (I2CStart(id) == I2C_SUCCESS )
    {	// transmiter is ready to send
	    DBPRINTF("Start sent \n");
	    print_i2c_status(id);
	    
	    // send slave addr int write mode
	 	byte = (slaveAddr << 1)|I2C_WRITE;
	    send(id, byte );
	    
	    // send high address to write to
	    byte = 0x00;
	    send(id, byte );
	    
	    // send low address to write to
	    byte = 0x00;
	    send(id, byte );
	    
	    // send start again
	    I2CStart(id);
	    // send slave addr int write mode
	 	byte = (slaveAddr << 1)|I2C_READ;
	    send(id, byte );	 
	       	    
	 	byte = get(id);
		
	    DBPRINTF("stop\n");
	    I2CStop(id);
 	}      	
	else
	{ 
		DBPRINTF("i2c%d failed to start", id + 1 );
		return NULL;
	}
		
	return byte;
}	