#ifndef __UART_H__
#define __UART_H__

#include "musicplayer/mp_config.h"
/*********************************************
 *       Mini MP3 Player 串口控制方式.h文件
 *
 *          by AZhao, B站 @阿釗同学
 *********************************************/

/*-------------------- 控制操作 ----------------------*/
void MPUart_Init();
void MPUart_MusicPlay();
void MPUart_MusicPause();
void MPUart_PlayAll();
void MPUart_MusicNext();
void MPUart_MusicPrev();
void MPUart_VolUP();
void MPUart_VolDown();
void MPUart_SetVol(uint8_t vol);
void MPUart_PointMusic(uint8_t music);
/*-------------------- 查询操作 ----------------------*/
void MPUart_QueryStatus();
void MPUart_QueryVol();
void MPUart_QueryEQ();
void MPUart_QueryPlayMode();


#endif
