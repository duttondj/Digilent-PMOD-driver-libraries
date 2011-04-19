
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

UINT8 writeTo(UINT8 addr)
{
	return 	(addr << 1)|I2C_WRITE;
}

UINT8 readTo(UINT8 addr)
{
	return (addr << 1) | I2C_READ;	
}	



	
UINT8 sendWithAck(I2C_MODULE id, UINT8 byte)
{
		while (!I2CTransmitterIsReady(id)){DBPRINTF("Waiting for transmitter to be ready\n");}
				// send address and write to slave
				I2CSendByte(id, byte);
				//print_i2c_status(id);
				while (!I2CTransmissionHasCompleted(I2C1)){DBPRINTF("Waiting for trans to complete\n");}

			 	if (I2CByteWasAcknowledged(id))
			    {
			//	   print_i2c_status(id);
			       DBPRINTF("ack - slave responds to call\n");
			       return 1;
			    }
			    return 0;
}



int eeprom_write_address( I2C_MODULE id, UINT8 slaveAddr, UINT8 highAddr, UINT8 lowAddr, UINT8 data)
{
	DBPRINTF("eeprom_write_address\n");
	while (!I2CBusIsIdle(id)){ DBPRINTF("Waiting on bus idle \n");}
	if (I2CStart(id) != I2C_SUCCESS )
		return 0;
   	
   	// transmiter is ready to send
    DBPRINTF("Start sent \n");
    // send slave addr int write mode
    if( !sendWithAck(id, (slaveAddr << 1)|I2C_WRITE ))
    	return 0;
	//wait for byte to be sent
	if( !sendWithAck(id, highAddr))
		return 0;
	if( !sendWithAck(id, lowAddr))
		return 0;
	if(! sendWithAck(id, data))
		return 0;
	// Send stopbit
	I2CStop(id);
	
	return 1;
}


UINT getNoAck( I2C_MODULE id)
{
	UINT8 data;
	I2CReceiverEnable(id, TRUE);
	while(!I2CReceivedDataIsAvailable(id)){}
	
//	I2CAcknowledgeByte(id, FALSE);
	
	data = I2CGetByte(id);
	
	I2CReceiverEnable(id, FALSE);
	return data;	
}

UINT8 eeprom_read_address( I2C_MODULE id, UINT8 slaveAddr, UINT8 highAddr, UINT8 lowAddr)
{
	DBPRINTF("eeprom_read_address\n");
	while (!I2CBusIsIdle(id)){ DBPRINTF("Waiting on bus idle \n");}
	if (I2CStart(id) != I2C_SUCCESS )
		return 0;
   	
   	// transmiter is ready to send
    DBPRINTF("Start sent \n");
    // send slave addr in write mode (to set address)
    if( !sendWithAck(id, writeTo(slaveAddr)))
    	return 0;

	if( !sendWithAck(id, highAddr))
		return 0;
	if( !sendWithAck(id, lowAddr))
		return 0;
	// send start again (current addresss is now set on the eeprom)
	if (I2CStart(id) != I2C_SUCCESS )
		return 0;
 	 if( !sendWithAck(id, readTo(slaveAddr)))
    	return 0;		
   	UINT data = getNoAck(id); 
	I2CStop(id);

	return data;
	   	
}

UINT8 eeprom_read_current( I2C_MODULE id, UINT8 slaveAddr)
{
	
	
	while (!I2CBusIsIdle(id)){ DBPRINTF("Waiting on bus idle \n");}
	if (I2CStart(id) != I2C_SUCCESS )
		return 0;
   	
   	// transmiter is ready to send
    DBPRINTF("Start sent \n");
    // send slave addr in write mode (to set address)

 	 if( !sendWithAck(id, readTo(slaveAddr ) ))
    	return 0;		
   	UINT data = getNoAck(id); 


	return data;
}

UINT8 eeprom_set_current(I2C_MODULE id, UINT8 slaveAddr, UINT8 highAddr, UINT8 lowAddr)
{
	while (!I2CBusIsIdle(id)){ DBPRINTF("Waiting on bus idle \n");}
	if (I2CStart(id) != I2C_SUCCESS )
		return 0;
   	
   	// transmiter is ready to send
    DBPRINTF("Start sent \n");
    // send slave addr in write mode (to set address)
    if( !sendWithAck(id, writeTo(slaveAddr)))
    {	
	    
	    DBPRINTF("error sending slave addr");
	    I2CStop(id);
    	return 0;
	}
	
	if( !sendWithAck(id, highAddr))
	{
		DBPRINTF("error sending high addr");
		I2CStop(id);
		return 0;
	}
	if( !sendWithAck(id, lowAddr))
	{	
		DBPRINTF("error sending low addr");
		I2CStop(id);
		return 0;
	}
	I2CStop(id);		
}

UINT8 eeprom_sequential_read_current(I2C_MODULE id, UINT8 slaveAddr, UINT8 count, UINT8 buffer[])
{
	if( count == 0) return 1;
	
	while (!I2CBusIsIdle(id)){ DBPRINTF("Waiting on bus idle \n");}
	if (I2CStart(id) != I2C_SUCCESS )
		return 0;
   	
   	// transmiter is ready to send
    DBPRINTF("Start sent \n");
    // send slave addr in write mode (to set address)
    if( !sendWithAck(id, readTo(slaveAddr)))
    	return 0;
    int index;
	for(index =0; index < count -1; index++)
	{
		buffer[index] = getNoAck(id);	
		I2CAcknowledgeByte(id, TRUE);	
	}	
	buffer[count - 1] = getNoAck(id);	
	
	I2CStop(id);
	return 1;
}

UINT8 eeprom_wait_4_ready( I2C_MODULE id, UINT8 slaveAddr)
{
/*Send
Write Command
Send Stop
Condition to
Initiate Write Cycle
Send Start
Send Control Byte
with R/W = 0
Did Device
Acknowledge (ACK = 0)?
Next Operation	
*/
		while (!I2CBusIsIdle(id)){ DBPRINTF("Waiting on bus idle \n");}
		
		do
		{
			
			I2CStart(id);
			I2CSendByte(id, writeTo(slaveAddr));
			//print_i2c_status(id);
			while (!I2CTransmissionHasCompleted(I2C1)){DBPRINTF("Waiting for trans to complete\n");}

		} while ( !I2CByteWasAcknowledged(id));
	I2CStop(id);
}	





	
	
UINT8 eeprom_sequential_write_page(I2C_MODULE id, UINT8 slaveAddr,UINT8 highAddr, UINT8 lowAddr, UINT8 count, UINT8 buffer[])
{
	
	if( count > 64) return 0;
	
	while (!I2CBusIsIdle(id)){ DBPRINTF("Waiting on bus idle \n");}
	if (I2CStart(id) != I2C_SUCCESS )
		return 0;
   	
   	// transmiter is ready to send
    DBPRINTF("Start sent \n");
    // send slave addr in write mode (to set address)
    if( !sendWithAck(id, writeTo(slaveAddr)))
    {	
	    DBPRINTF("writepage: error sending slave write");
    	return 0;
    }
    if( !sendWithAck(id, highAddr))
    {	
	    DBPRINTF("writepage: error sending highaddr");
    	return 0;
    }
    if( !sendWithAck(id, lowAddr))
    {	
	    DBPRINTF("writepage: error sending lowaddr");
    	return 0;
    }
    int index;
	for(index =0; index < count ; index++)
	{
		sendWithAck(id, buffer[index]);	
	}	
	
	I2CStop(id);
	return 1;
}