#ifndef __OLED_FONT_H__
#define __OLED_FONT_H__

#include "oled/oled_config.h"

//字库总数目
#define OLED_FONT_NUM          5
//非ASCII字库数目
#define OLED_NON_ASCII_NUM     2

//字体参数结构体
typedef struct
{
    eFontName font;
    uint8_t *font_p;
    uint8_t ch_h;
    uint8_t ch_w;
    uint8_t page_cover;
}sFontPara;

//非ASCII字库索引结构体
typedef struct
{
    eFontName font;
    uint8_t ch_h;
    uint8_t ch_w;
    uint8_t ch_num;     //字符数量
    uint8_t *ch_all;    //按照顺序的所有字符组成字符串
}sNonAsciiIndex;

extern uint8_t code ascii_h7w5[][5];
extern uint8_t code ascii_h8w6[][6];
extern uint8_t code ascii_h16w8[][8];
extern uint8_t code zh_h16w16[][32];
extern uint8_t code zh_h32w32[][128];

#endif
