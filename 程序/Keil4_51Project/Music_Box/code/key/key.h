#ifndef __KEY_H__
#define __KEY_H__

#include "config.h"

//按键数目
#define KEY_NUMS	10

//按键连续检测的次数，越低灵敏度越高，也越不稳定
#define KEY_DECTECT_TIMES	4

//按键状态
#define KEY_DOWN  0
#define KEY_UP    1

//按键名称
typedef enum BtnN {
	BtnLL=0, BtnLR, BtnLU, BtnLD, BtnLM,
	BtnRL, BtnRR, BtnRU, BtnRD, BtnRM
}eBtnN;

void Key_Init(void);
void Key_Scan(void);
void Key_Driver();
void Key_Register(void (*func)(), uint8_t num);
void Key_Delete(uint8_t num);
void Key_AllDelete();

#endif
