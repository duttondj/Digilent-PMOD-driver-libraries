#ifndef _TIMERHELPER_H_
#define _TIMERHELPER_H_

#define GetSystemClock() (80000000ul)
#define GetPeripheralClock() (GetSystemClock()/(1 << OSCCONbits.PBDIV))

#endif
