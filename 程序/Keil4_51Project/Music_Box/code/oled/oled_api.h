#ifndef __OLED_API_H__
#define __OLED_API_H__

/*********************************************
 *              以下为Oled绘图API
 *              无屏幕缓存版本
 *
 *          by AZhao, B站 @阿釗同学
 *********************************************/
/*------------------Oled头文件-------------------*/
#include "oled/oled_config.h"

// Oled page与y坐标转换
#define PAGE(n)             (n*8)

/*---------------------Oled警告----------------------*/
//可自己在oled_config.h中添加Oled警告  在oled_warning.c中添加Oled警告处理
extern void OLED_SET_WARNING(uint8_t wcode);
extern void OLED_RESET_WARNING(uint8_t wcode);
/*---------------------Oled基础设置----------------------*/
extern void Oled_Init();
extern void Oled_SetDevice(eOledDevice dev);
extern void Oled_DisplayOn();
extern void Oled_DispalyOff();
extern void Oled_DisplayColor(uint8_t color);
extern void Oled_DisplayDir(uint8_t dir);
extern void Oled_SetDebugDraw(uint8_t debug);
extern void Oled_SetDebugDelay(uint16_t ms);
/*---------------------Oled绘图----------------------*/
extern void Oled_Clear();
extern void Oled_ParticalClear(uint8_t start_x, uint8_t start_y, uint8_t end_x,   uint8_t end_y);
extern void Oled_DelayMs(uint16_t ms);
extern void Oled_UseStringEdge(uint8_t edge);
extern void Oled_DrawLine(uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y);
extern void Oled_ShowChar(uint8_t x, uint8_t y, uint8_t ch, eFontName font);
extern void Oled_ShowCharZh(uint8_t x, uint8_t y, uint8_t* ch, eFontName font);
extern void Oled_ShowString(uint8_t x, uint8_t y, uint8_t* str, eFontName font);
extern void Oled_ShowChinese(uint8_t x, uint8_t y, uint8_t* str, eFontName font);
extern void Oled_EdgeString(uint8_t x, uint8_t y, uint8_t* str, eFontName font);
extern void Oled_EdgeChinese(uint8_t x, uint8_t y, uint8_t* str, eFontName font);
extern void Oled_ShowBmp(uint8_t x, uint8_t y, eBmpName bmp);
extern void Oled_ShowNum(uint8_t x, uint8_t y, void* num, eNumType type, eFontName font);
extern void Oled_ShowIICRate(uint8_t x, uint8_t y, void* num);
/*---------------------Oled绘图api别名----------------------*/
#if (OLED_USE_ALIAS == 1)

#define o_init        Oled_Init
#define o_dev         Oled_SetDevice
#define o_on          Oled_DisplayOn
#define o_off         Oled_DispalyOff
#define o_color       Oled_DisplayColor
#define o_dir         Oled_DisplayDir
#define o_dedraw      Oled_SetDebugDraw
#define o_dedelay     Oled_SetDebugDelay
#define o_clr         Oled_Clear
#define o_partclr     Oled_ParticalClear
#define o_delayms     Oled_DelayMs
#define o_useedge     Oled_UseStringEdge
#define o_line        Oled_DrawLine
#define o_char        Oled_ShowChar
#define o_charzh      Oled_ShowCharZh
#define o_str         Oled_ShowString
#define o_chinese     Oled_ShowChinese
#define o_edgestr     Oled_EdgeString
#define o_edgecn      Oled_EdgeChinese
#define o_bmp         Oled_ShowBmp
#define o_num         Oled_ShowNum
#define o_iicrate     Oled_ShowIICRate

#endif

#endif
