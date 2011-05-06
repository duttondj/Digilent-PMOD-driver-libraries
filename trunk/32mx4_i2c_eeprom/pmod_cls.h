#ifndef _PMOD_CLS_H
	#define _PMOD_CLS_H
	
extern unsigned char enable_display[];
extern unsigned char set_cursor[];
extern unsigned char home_cursor[];
extern unsigned char wrap_line[];

#define PMOD_CLS_DEFAULT_TWI_ADDRESS 0x48
	

int initialize_CLS ( I2C_MODULE id, UINT8 address);
	
#endif
