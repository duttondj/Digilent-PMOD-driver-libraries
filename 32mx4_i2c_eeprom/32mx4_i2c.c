#include <plib.h>
#include <stdio.h>
#include "32mx4_i2c.h"



//#define DEBUG_FUNK DBPRINTF
#define DEBUG_FUNK //
// these have to be in the predefined macros you should look
// if not make them into macros

UINT8 writeTo(UINT8 addr)
{
	return (addr << 1)|I2C_WRITE;
}

UINT8 readTo(UINT8 addr)
{
	return (addr << 1) | I2C_READ;  
}   

void I2C_Initialize( I2C_MODULE id, UINT32 i2cClock, UINT16 address )
{
        // Configure Various I2C Options
        I2CConfigure(id, I2C_ENABLE_SLAVE_CLOCK_STRETCHING | I2C_ENABLE_HIGH_SPEED );
        
        // Set Desired Operation Frequency
        UINT32 actualClock = I2CSetFrequency(id, GetPeripheralClock(), i2cClock);
        if ( abs(actualClock-i2cClock) > i2cClock/10 )
        {
            DEBUG_FUNK("  Clock frequency (%d) error exceeds 10\%\r\n", actualClock);
        }
        else
        {
            DEBUG_FUNK("  Clock frequency = %ld Hz\n", actualClock);
        }
        
        //  Set the Slave Address
        // This won't work, maybe use a neg number for this?
                // If the slave interupt works the way I think this would be a good place to init that too
        if( address != NULL)
        {
                I2CSetSlaveAddress(id, address, 0, I2C_USE_7BIT_ADDRESS);
                DEBUG_FUNK("  Slave address = 0x%02X\n", address);
        }
        else
        {       
                DEBUG_FUNK(" I2C%d is master only.", id +1);
        }       
        // Enable the module
        DEBUG_FUNK("Enabling operations...\n");
        I2CEnable(id, TRUE);
}




void print_i2c_status( I2C_MODULE id)
{
	   DEBUG_FUNK("* start checking i2c%d * ", id +1);
	 //I2CClearStatus(I2C1, I2C_ARBITRATION_LOSS);
	I2C_STATUS status = I2CGetStatus(id);
	if(I2C_ARBITRATION_LOSS & status)
	{
	     DEBUG_FUNK("I2C_ARBITRATION_LOSS ");
	}
	if( I2C_DATA_AVAILABLE & status)
	{
	     DEBUG_FUNK(" I2C_DATA_AVAILABLE ");
	}
	        if(I2C_SLAVE_READ & status)
	{
	     DEBUG_FUNK("I2C_SLAVE_READ ");
	}
	        if( I2C_START & status)
	{
	     DEBUG_FUNK(" I2C_START ");
	}
	        if(I2C_STOP & status)
	{
	     DEBUG_FUNK("I2C_STOP ");
	}
	        if(I2C_SLAVE_DATA & status)
	{
	     DEBUG_FUNK("I2C_SLAVE_DATA ");
	}
	        if(I2C_RECEIVER_OVERFLOW & status)
	{
	     DEBUG_FUNK("I2C_RECEIVER_OVERFLOW ");
	}
	        if(I2C_TRANSMITTER_OVERFLOW & status)
	{
	     DEBUG_FUNK("I2C_TRANSMITTER_OVERFLOW ");
	}
	                if(I2C_10BIT_ADDRESS & status)
	{
	     DEBUG_FUNK("I2C_10BIT_ADDRESS ");
	}
	                if( I2C_GENERAL_CALL & status)
	{
	     DEBUG_FUNK(" I2C_GENERAL_CALL ");
	}
	                if( I2C_TRANSMITTER_BUSY & status)
	{
	     DEBUG_FUNK(" I2C_TRANSMITTER_BUSY ");
	}
	                if(I2C_BYTE_ACKNOWLEDGED & status)
	{
	     DEBUG_FUNK("I2C_BYTE_ACKNOWLEDGED ");
	}
	  DEBUG_FUNK("* stop checking i2c%d\n", id +1);
        
}

    

BOOL I2C_startBus(I2C_MODULE id)
{
	while (!I2CBusIsIdle(id)){ DEBUG_FUNK("Waiting on bus idle \n");}
	
	if (I2CStart(id) != I2C_SUCCESS )
	{	
		DEBUG_FUNK("Start fail \n");
		return FALSE;
	}
	else
	{
		DEBUG_FUNK("Start success\n");
		return TRUE;
	}
}

VOID I2C_stopBus(I2C_MODULE id)
{
	I2CStop(id);
}

  /*      
UINT8 I2C_send_checkAck(I2C_MODULE id, UINT8 byte)
{
	sendWithNoAck(id, byte);
	return getAck(id);
}*/


BOOL I2C_send(I2C_MODULE id, UINT8 byte)
{
	while (!I2CTransmitterIsReady(id)){DEBUG_FUNK("Waiting for transmitter to be ready\n");}
	// send address and write to slave
	if( I2CSendByte(id, byte) == I2C_SUCCESS )
	{
		// might want a timeout ability
		// TODO check if the master control has been lost
		while (!I2CTransmissionHasCompleted(I2C1)){DEBUG_FUNK("Waiting for trans to complete\n");}
		return TRUE;
	}
	else
	{
		return FALSE;	
	}	


}

BOOL I2C_send_checkAck(I2C_MODULE id, UINT8 byte)
{
	while (!I2CTransmitterIsReady(id)){DEBUG_FUNK("Waiting for transmitter to be ready\n");}
	// send address and write to slave
	if( I2CSendByte(id, byte) == I2C_SUCCESS )
	{
		// might want a timeout ability
		// TODO check if the master control has been lost
		while (!I2CTransmissionHasCompleted(I2C1)){DEBUG_FUNK("Waiting for trans to complete\n");}
		return I2C_checkAck(  id);
	}
	else
	{
		return FALSE;	
	}	


}

UINT I2C_receive( I2C_MODULE id)
{
        UINT8 data;
        if (I2CReceiverEnable(id, TRUE) == I2C_RECEIVE_OVERFLOW)
       	{
        	DEBUG_FUNK("everflow error: TODO handle and clear this!\n");
        	// you should think about this.. you really should.
        	return 0;
        }
        	
        while(!I2CReceivedDataIsAvailable(id)){}
        
        data = I2CGetByte(id);
        
        I2CReceiverEnable(id, FALSE);
        return data;    
}

UINT I2C_receiveAndAck( I2C_MODULE id)
{
        UINT8 data;
        if (I2CReceiverEnable(id, TRUE) == I2C_RECEIVE_OVERFLOW)
       	{
        	DEBUG_FUNK("everflow error: TODO handle and clear this!\n");
        	// you should think about this.. you really should.
        	return 0;
        }
        	
        while(!I2CReceivedDataIsAvailable(id)){}
        
        data = I2CGetByte(id);
        I2CAcknowledgeByte(id, TRUE); // ack
        I2CReceiverEnable(id, FALSE);
        return data;    
}


UINT I2C_receiveAndNack( I2C_MODULE id)
{
        UINT8 data;
        if (I2CReceiverEnable(id, TRUE) == I2C_RECEIVE_OVERFLOW)
       	{
        	DEBUG_FUNK("everflow error: TODO handle and clear this!\n");
        	// you should think about this.. you really should.
        	return 0;
        }
        	
        while(!I2CReceivedDataIsAvailable(id)){}
        
        data = I2CGetByte(id);
        I2CAcknowledgeByte(id, FALSE); // nack
        I2CReceiverEnable(id, FALSE);
        return data;    
}

BOOL I2C_checkAck( I2C_MODULE id)
{
	if (I2CByteWasAcknowledged(id))
	{
	  // print_i2c_status(id);
	  DEBUG_FUNK("ack\n");
	  return TRUE;
	}
	else
	{
	DEBUG_FUNK("nack\n");
	return FALSE;
	}
}


// Developmental
BOOL I2C_send_string( I2C_MODULE id, UINT8 address, char * string)
{
	// start
	while (!I2CBusIsIdle(id)){ DEBUG_FUNK("Waiting on bus idle \n");}
	if (I2CStart(id) != I2C_SUCCESS )
		return 0;
		
	DEBUG_FUNK("Start sent \n");
	// address slave
	if( !I2C_send_checkAck(id, writeTo(address)))
    {	
	    DEBUG_FUNK("send_string: error sending slave write\n");
    	return 0;
    }
	// start sending data
	int index = 0;
	while(string[index] != 0 )
	{
		I2C_send_checkAck(id, string[index]);
		index++;	
	}	
//	sendByte(id, string[index]);
	I2CStop(id);
	return 1;
	
}





/*
UINT8 get( I2C_MODULE id)
{

        UINT8 data;
        I2CReceiverEnable(id, TRUE);
        while(!I2CReceivedDataIsAvailable(id)){}
        
//      I2CAcknowledgeByte(id, FALSE);
        
        data = I2CGetByte(id);
        
        I2CReceiverEnable(id, FALSE);
        
}
*/