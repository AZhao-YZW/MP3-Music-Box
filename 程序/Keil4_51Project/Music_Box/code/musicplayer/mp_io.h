#ifndef __MP_IO_H__
#define __MP_IO_H__

#include "musicplayer/mp_config.h"
/*********************************************
 *       Mini MP3 Player IO控制方式.h文件
 *
 *          by AZhao, B站 @阿釗同学
 *********************************************/

void MPIO_Init();
void MPIO_PointPlay();
void MPIO_PlayAll();
void MPIO_MusicNext();
void MPIO_MusicPrev();
void MPIO_VolUP();
void MPIO_VolDown();

#endif
