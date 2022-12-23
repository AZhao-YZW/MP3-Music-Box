#ifndef __OLED_BMP_H__
#define __OLED_BMP_H__

#include "oled/oled_config.h"

//bmp数目
#define OLED_BMP_NUM       1

//bmp参数结构体
typedef struct
{
	eBmpName bmp;
    uint8_t *bmp_p;
    uint8_t bmp_h;
    uint8_t bmp_w;
}sBmpPara;

extern uint8_t code BMP1[];

#endif
