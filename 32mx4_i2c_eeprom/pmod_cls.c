

#include <plib.h>
#include "pmod_cls.h"




unsigned char enable_display[] = {27, '[', '3', 'e', '\0'};
unsigned char set_cursor[] = {27, '[', '1', 'c', '\0'};
unsigned char home_cursor[] = {27, '[', 'j', '\0'};
unsigned char wrap_line[] = {27, '[', '0', 'h', '\0'};



int initialize_CLS ( I2C_MODULE id, UINT8 address)  //Initial PmodCLS setup.
{
	DBPRINTF("enabling display\n");
	if( !send_string( id, address, enable_display))
		return FALSE;
	
	DBPRINTF("Setting cursor cls\n");
	if(!send_string( id, address, set_cursor))
		return FALSE;
	DBPRINTF("Homing cursor cls\n");
	if(!send_string( id, address, home_cursor))
		return FALSE;
	DBPRINTF("line wrap cls\n");
	if(!send_string( id, address, wrap_line))
		return FALSE;
	return TRUE;
}
