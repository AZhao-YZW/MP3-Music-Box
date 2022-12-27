#ifndef __OLED_MISC_H__
#define __OLED_MISC_H__

#include "oled/oled_config.h"

//float_to_str小数部分位数  C51中float与double都是4byte 精度小数点6位
//转换后精度到5位
#define DECIMAL_PLACES      5

void int_to_str(uint8_t *str, int32_t num);
void float_to_str(uint8_t *str, double num);


#endif
