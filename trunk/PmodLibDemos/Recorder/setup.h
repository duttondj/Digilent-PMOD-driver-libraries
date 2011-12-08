#ifndef SETUP_H
#define SETUP_H

#include <stdint.h>
#include <plib.h>
#include <pmodlib.h>

#define SYSTEM_CLOCK 80000000L  // system clock speed (8 MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
#define PB_CLOCK (SYSTEM_CLOCK/2) // peripheral bus clock
#define PMOD_DA2_BITRATE 20000000 // 20Mhz bit rate for PmodDA2
#define PMOD_MIC_BITRATE 12500000 // 12.5Mhz bit rate for PmodMIC

volatile SpiChannel PmodDA2chn;
volatile SpiChannel PmodMICchn;
volatile uint16_t InputSound;
volatile uint16_t OutputSound;

uint8_t RecorderInit(uint8_t BufferCount, uint16_t BufferSize);
uint8_t HandleBTN1(uint8_t action);
uint8_t HandleBTN2(uint8_t action);

#endif