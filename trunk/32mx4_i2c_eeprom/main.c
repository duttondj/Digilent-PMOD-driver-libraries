
//disable legacy code
#define _PLIB_DISABLE_LEGACY
// load in support for all plib functionality
#include <plib.h>
        
#include "eeprom.h"
#include <stdio.h>
#include "32mx4_i2c.h"


int main(void)
{
        UINT8 data = 0;
        DBPRINTF("******I2C1 EEPROM Tests Starting Now!!!\n");
        DBPRINTF("****Initializing!!!\n");        
        I2C_Initialize( I2C1, 400000, NULL );

        UINT8 buffer1[64] = {0,1,2,3,4,5,6,7,8,9,10}; 
        UINT8 buffer2[64];
        int count = 11;
        int index = 0;
        DBPRINTF("****Writing 45 to eeprom 0\n");
        eeprom_write_address( I2C1, 0x50, 0, 0, 45 );
     

        DBPRINTF("******I2c1 eeprom_sequential_write_page\n");
      	DBPRINTF("writing the values 0 - 1 to addresses 1 - 10\n");
        eeprom_sequential_write_page(I2C1, I2C_EEPROM_ADDRESS,0,1, count, buffer1);
        DBPRINTF("******I2c1 eeprom_wait_4_ready\n");
        // this must be done as when the eeprom writes to a page it will NOT answer i2c consider when designing state
        // based code.
        eeprom_wait_4_ready(I2C1, I2C_EEPROM_ADDRESS);
        DBPRINTF("******I2c1 eeprom_set_current setting back to address 1\n");
        if( ! eeprom_set_current(I2C1,I2C_EEPROM_ADDRESS,  0, 1))
        {
	      DBPRINTF("!! eeprom_set_current FAILED!\n");   
	    }
        DBPRINTF("******I2c1 eeprom_sequential_read_current reading in 11 values and printing (should be 0 - 10)\n");
        eeprom_sequential_read_current( I2C1, I2C_EEPROM_ADDRESS, count, buffer2);
        while(index < count)
        {
                DBPRINTF("*I2c1 read count %d: %d\n",index, buffer2[index]);
                index++;
        }
   		data = eeprom_read_address( I2C1, I2C_EEPROM_ADDRESS, 0, 0);
    	DBPRINTF("******I2c1 read address 0 result (should be 45): %d\n", data);
        DBPRINTF("Program Done\n");
        while(1)
        {       
                
        }
}





