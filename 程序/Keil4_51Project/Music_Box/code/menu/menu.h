#ifndef __MENU_H__
#define __MENU_H__

#include "menu/menu_settings.h"


// 菜单最大Id+1(菜单元素个数)
#define EndId           29
// 菜单最大层数
#define MENU_MAX_LEVEL	4

/* 菜单基本函数 */
void Menu_Init();
void Menu_Display();
void Menu_KeyRegister();
void Menu_Switch(uint8_t scode);

#endif
