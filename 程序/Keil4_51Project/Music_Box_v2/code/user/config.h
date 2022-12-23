#ifndef __CONFIG_H__
#define __CONFIG_H__

/*-------------------基础函数头文件-------------------*/
#include <intrins.h>
#include <string.h>

/*------------------类型重定义-------------------*/
typedef unsigned char    uint8_t;
typedef unsigned int     uint16_t;
typedef unsigned long    uint32_t;
typedef char    int8_t;
typedef int     int16_t;
typedef long    int32_t;
typedef void (*void_func_void)(void);

/*--------------选择使用的51单片机，并定义所使用的引脚---------------*/
// #define USE_STC89C52RC
#define USE_STC12C5A60S2

#if defined(USE_STC89C52RC)
    #include <STC89C5xRC.H>
#elif defined(USE_STC12C5A60S2)
    #include <STC12C5A60S2.H>
    sbit MP_RX    = P1^2;       // 单片机RX, 接MP的TX
    sbit MP_TX    = P1^3;       // 单片机TX, 接MP的RX
#endif

sbit LED        = P1^0;

sbit OLED_SCL   = P3^7;
sbit OLED_SDA   = P3^6;
sbit OLED2_SCL  = P3^4;
sbit OLED2_SDA  = P3^5;

sbit BTNL_L     = P2^2;
sbit BTNL_D     = P2^3;
sbit BTNL_U     = P2^4;
sbit BTNL_M     = P2^1;
sbit BTNL_R     = P2^0;
sbit BTNR_L     = P0^0;
sbit BTNR_D     = P0^3;
sbit BTNR_U     = P0^4;
sbit BTNR_M     = P0^1;
sbit BTNR_R     = P0^2;

sbit MP_ADKEY2  = P1^0;
sbit MP_ADKEY1  = P1^1;
sbit MP_IO2     = P0^6;
sbit MP_IO1     = P0^5;

/*-------------------晶振频率、指令周期(频率)------------------*/
// #define MAIN_FOSC    35000000L
// #define MAIN_FOSC    24000000L
// #define MAIN_FOSC    22118400L
 #define MAIN_FOSC    12000000L
// #define MAIN_FOSC    11059200L

/*-------------------用户头文件-------------------*/
#include "delay/delay.h"

#endif
