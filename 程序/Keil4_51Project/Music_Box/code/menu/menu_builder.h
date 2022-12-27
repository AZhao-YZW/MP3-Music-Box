#ifndef __MENU_BUILDER_H__
#define __MENU_BUILDER_H__

#include "menu/menu_settings.h"
#include "oled/oled_config.h"


void MenuBuilder_Display(sMenuOpt* menuOpt, eOledDevice oledDev);
void MenuBuilder_Switch(sMenuOpt* menuOpt, eOledDevice oledDev, uint8_t scode);

#endif
