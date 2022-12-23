#ifndef __DELAY_H__
#define __DELAY_H__

#include "config.h"

#if defined(USE_STC89C52RC)     /* 12T 51单片机 */
    #if MAIN_FOSC == 22118400L
        #define Delay1us()      _nop_()
    #elif MAIN_FOSC == 12000000L
        #define Delay1us()      _nop_()
    #elif MAIN_FOSC == 11059200L
        #define Delay1us()      _nop_()
    #endif
#elif defined(USE_STC12C5A60S2) /* 1T 51单片机 */
    #if MAIN_FOSC == 22118400L
        #define Delay1us()      _nop_();_nop_()
    #elif MAIN_FOSC == 12000000L
        #define Delay1us()      _nop_();_nop_()
    #elif MAIN_FOSC == 11059200L
        #define Delay1us()      _nop_();
    #endif
#endif

void Delay1ms();
void Delay10ms();
void Delay100ms();

#define delay_1us()       Delay1us()
#define delay_1ms()       Delay1ms()
#define delay_10ms()      Delay10ms()
#define delay_100ms()     Delay100ms()

#endif
