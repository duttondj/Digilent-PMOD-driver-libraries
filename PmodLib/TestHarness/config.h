#ifndef _CONFIG_H_
#define _CONFIG_H

/* ------------------------------------------------------------ */
/*				PIC32 Configuration Settings					*/
/* ------------------------------------------------------------ */
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1  //(8 MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
#pragma config FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL
#pragma config FPBDIV = DIV_2


/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */
#define SYSTEM_CLOCK 80000000
#define PB_CLOCK (SYSTEM_CLOCK/2)
#define SPI_BITRATE 625000

#define PMODSF 1
#define PMODJSTK 0

#endif

