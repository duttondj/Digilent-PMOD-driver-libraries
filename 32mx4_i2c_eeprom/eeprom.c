 


#include <plib.h>
#include <stdio.h>


//#define DEBUG_FUNK DBPRINTF
#define DEBUG_FUNK //


UINT8 eeprom_set_current(I2C_MODULE id, UINT8 slaveAddr, UINT8 highAddr, UINT8 lowAddr)
{
	
        
    // transmiter is ready to send
   
    // send slave addr in write mode (to set address)
    
    /*if( !I2C_send_checkAck(id, writeTo(slaveAddr)))
    {   
            
            DEBUG_FUNK("error sending slave addr");
            I2CStop(id);
        return 0;
        }
        
        if( !I2C_send_checkAck(id, highAddr))
        {
                DEBUG_FUNK("error sending high addr");
                I2CStop(id);
                return 0;
        }
        if( !I2C_send_checkAck(id, lowAddr))
        {       
                DEBUG_FUNK("error sending low addr");
                I2CStop(id);
                return 0;
        }
        I2CStop(id);      
        */
    //TODO why not covert this into a 'string' and use that function?
    //Need to add ecsape ability to string function....
  /*  DEBUG_FUNK("eeprom_set_current: sending slaveAddr %d", slaveAddr);    
	if(! I2C_send_checkAck(id, slaveAddr))
	{
		DEBUG_FUNK("eeprom_set_current: error sending slave addr\n");
		I2C_stopBus(id);
		return FALSE;
	}
	
	if(! I2C_send_checkAck(id, highAddr))
	{
		DEBUG_FUNK("eeprom_set_current: error sending slave addr\n");
		I2C_stopBus(id);
		return FALSE;
	}
		
	if(! I2C_send_checkAck(id, lowAddr))
	{
		DEBUG_FUNK("eeprom_set_current: error sending slave addr\n");
		I2C_stopBus(id);
		return FALSE;
	}
	
	I2C_stopBus(id);
	return TRUE;
	 */
	
	//I2C_startBus(id);
	DEBUG_FUNK("eeprom_read_address\n");
    while (!I2CBusIsIdle(id)){ DEBUG_FUNK("Waiting on bus idle \n");}
    if (I2CStart(id) != I2C_SUCCESS )
    	return 0;
        
        // transmiter is ready to send
    DEBUG_FUNK("Start sent \n");
    // send slave addr in write mode (to set address)
    if( !I2C_send_checkAck(id, writeTo(slaveAddr)))
        return 0;

DEBUG_FUNK("send high \n");
        if( !I2C_send_checkAck(id, highAddr))
                return 0;
 DEBUG_FUNK("send low \n");       
        if( !I2C_send_checkAck(id, lowAddr))
                return 0;
        // send start again (current addresss is now set on the eeprom)
DEBUG_FUNK("send start \n"); 
        if (I2CStart(id) != I2C_SUCCESS )
                return 0;
 
 //        if( !I2C_send_checkAck(id, readTo(slaveAddr)))
 //       return 0;               
        //UINT data = I2C_receive(id); 
        I2CStop(id);

        //return data;
        return TRUE;
                
}


/***************************************************************************/
UINT8 eeprom_read_address( I2C_MODULE id, UINT8 slaveAddr, UINT8 highAddr, UINT8 lowAddr)
{
        DEBUG_FUNK("eeprom_read_address\n");
        while (!I2CBusIsIdle(id)){ DEBUG_FUNK("Waiting on bus idle \n");}
        if (I2CStart(id) != I2C_SUCCESS )
                return 0;
        
        // transmiter is ready to send
    DEBUG_FUNK("Start sent \n");
    // send slave addr in write mode (to set address)
    if( !I2C_send_checkAck(id, writeTo(slaveAddr)))
        return 0;

        if( !I2C_send_checkAck(id, highAddr))
                return 0;
        if( !I2C_send_checkAck(id, lowAddr))
                return 0;
        // send start again (current addresss is now set on the eeprom)
        if (I2CStart(id) != I2C_SUCCESS )
                return 0;
         if( !I2C_send_checkAck(id, readTo(slaveAddr)))
        return 0;               
        UINT data = I2C_receive(id); 
        I2CStop(id);

        return data;
                
}

UINT8 eeprom_read_current( I2C_MODULE id, UINT8 slaveAddr)
{
        
        
        while (!I2CBusIsIdle(id)){ DEBUG_FUNK("Waiting on bus idle \n");}
        if (I2CStart(id) != I2C_SUCCESS )
                return 0;
        
        // transmiter is ready to send
    DEBUG_FUNK("Start sent \n");
    // send slave addr in write mode (to set address)

         if( !I2C_send_checkAck(id, readTo(slaveAddr ) ))
        return 0;               
        UINT data = I2C_receive(id); 


        return data;
}

UINT8 eeprom_sequential_read_current(I2C_MODULE id, UINT8 slaveAddr, UINT8 count, UINT8 buffer[])
{
        if( count == 0) return 1;
        
        while (!I2CBusIsIdle(id)){ DEBUG_FUNK("Waiting on bus idle \n");}
        if (I2CStart(id) != I2C_SUCCESS )
                return 0;
        
        // transmiter is ready to send
    DEBUG_FUNK("Start sent \n");
    // send slave addr in write mode (to set address)
    if( !I2C_send_checkAck(id, readTo(slaveAddr)))
        return 0;
    int index;
        for(index =0; index < count -1; index++)
        {
                buffer[index] = I2C_receive(id);   
                I2CAcknowledgeByte(id, TRUE);   
        }       
        buffer[count - 1] = I2C_receive(id);       
        
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
                while (!I2CBusIsIdle(id)){ DEBUG_FUNK("Waiting on bus idle \n");}
                
                do
                {
                        
                        I2CStart(id);
                        I2CSendByte(id, writeTo(slaveAddr));
                        //print_i2c_status(id);
                        while (!I2CTransmissionHasCompleted(I2C1)){DEBUG_FUNK("Waiting for trans to complete\n");}

                } while ( !I2CByteWasAcknowledged(id));
        I2CStop(id);
}       





        
        
UINT8 eeprom_sequential_write_page(I2C_MODULE id, UINT8 slaveAddr,UINT8 highAddr, UINT8 lowAddr, UINT8 count, UINT8 buffer[])
{
        
        if( count > 64) return 0;
        
        while (!I2CBusIsIdle(id)){ DEBUG_FUNK("Waiting on bus idle \n");}
        if (I2CStart(id) != I2C_SUCCESS )
                return 0;
        
        // transmiter is ready to send
    DEBUG_FUNK("Start sent \n");
    // send slave addr in write mode (to set address)
    if( !I2C_send_checkAck(id, writeTo(slaveAddr)))
    {   
            DEBUG_FUNK("writepage: error sending slave write");
        return 0;
    }
    if( !I2C_send_checkAck(id, highAddr))
    {   
            DEBUG_FUNK("writepage: error sending highaddr");
        return 0;
    }
    if( !I2C_send_checkAck(id, lowAddr))
    {   
            DEBUG_FUNK("writepage: error sending lowaddr");
        return 0;
    }
    int index;
        for(index =0; index < count ; index++)
        {
                I2C_send_checkAck(id, buffer[index]); 
        }       
        
        I2CStop(id);
        return 1;
}



int eeprom_write_address( I2C_MODULE id, UINT8 slaveAddr, UINT8 highAddr, UINT8 lowAddr, UINT8 data)
{
        DEBUG_FUNK("eeprom_write_address\n");
        while (!I2CBusIsIdle(id)){ DEBUG_FUNK("Waiting on bus idle \n");}
        if (I2CStart(id) != I2C_SUCCESS )
                return 0;
        
        // transmiter is ready to send
    DEBUG_FUNK("Start sent \n");
    // send slave addr int write mode
    if( !I2C_send_checkAck(id, (slaveAddr << 1)|I2C_WRITE ))
        return 0;
        //wait for byte to be sent
        if( !I2C_send_checkAck(id, highAddr))
                return 0;
        if( !I2C_send_checkAck(id, lowAddr))
                return 0;
        if(! I2C_send_checkAck(id, data))
                return 0;
        // Send stopbit
        I2CStop(id);
        
        return 1;
}




