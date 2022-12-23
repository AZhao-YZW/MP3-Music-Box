#ifndef __MUSIC_APP_H__
#define __MUSIC_APP_H__

#include "musicplayer/mp_config.h"
#include "menu/menu_settings.h"


// 音乐播放器菜单Id总数
#define MUSIC_APP_ENDID         21
// 音乐播放器菜单最大层数
#define MUSIC_APP_MAX_LEVEL     4

/* 菜单基本函数 */
void MusicApp_Init();
void MusicApp_Display();
void MusicApp_KeyRegister();
void MusicApp_Switch(uint8_t scode);
/* 控制MP */
void MusicApp_Operate(eMusicAppOptCode optcode);


#endif
