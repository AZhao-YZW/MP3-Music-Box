#ifndef __OLED_DRAW_H__
#define __OLED_DRAW_H__

#include "oled/oled_config.h"

#define OLED_PAGE_NUM       8       // 总页数
#define OLED_ROW_NUM        64      // 总行数
#define OLED_COLUMN_NUM     128     // 总列数

void Oled_Init();
void Oled_SetDevice(eOledDevice dev);
void Oled_DisplayOn();
void Oled_DispalyOff();
void Oled_DisplayColor(uint8_t color);
void Oled_DisplayDir(uint8_t dir);
void Oled_SetDebugDraw(uint8_t debug);
void Oled_SetDebugDelay(uint16_t ms);
void Oled_Clear();
void Oled_ParticalClear(uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y);
void Oled_DelayMs(uint16_t ms);
void Oled_UseStringEdge(uint8_t edge);
void Oled_DrawLine(uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y);
void Oled_ShowChar(uint8_t x, uint8_t y, uint8_t ch, eFontName font);
void Oled_ShowCharZh(uint8_t x, uint8_t y, uint8_t* ch, eFontName font);
void Oled_ShowString(uint8_t x, uint8_t y, uint8_t* str, eFontName font);
void Oled_ShowChinese(uint8_t x, uint8_t y, uint8_t* str, eFontName font);
void Oled_EdgeString(uint8_t x, uint8_t y, uint8_t* str, eFontName font);
void Oled_EdgeChinese(uint8_t x, uint8_t y, uint8_t* str, eFontName font);
void Oled_ShowBmp(uint8_t x, uint8_t y, eBmpName bmp);
void Oled_ShowNum(uint8_t x, uint8_t y, void* num, eNumType type, eFontName font);
void Oled_ShowIICRate(uint8_t x, uint8_t y, void* num);

#endif
