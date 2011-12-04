
#include "./PmodMic/PmodMic.c"

void configure_interrupts (void);
void PmodMicInit(uint32_t pbClock,uint32_t bitRate);
void fnInitPmodMic(UART_MODULE uartID);
void fnInitPmodMic(UART_MODULE uartID);
void PmodMicStartRecording();
void PmodMicStopRecording();
void PmodMicTakeSample();
unsigned char fnTimer1Setup ();
void __ISR(_TIMER_1_VECTOR, ipl7) fnTimer1Int(void);
