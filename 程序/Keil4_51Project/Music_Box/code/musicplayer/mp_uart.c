#include "musicplayer/mp_uart.h"
#include "musicplayer/mp_uart_driver.h"

/*---------------------------------------------- 控制操作 -----------------------------------------------*/
// MP 串口初始化
void MPUart_Init()
{
    MP_UartDriverInit();
    MPUart_MusicPause();
    MPUart_SetVol(20);
}

/* 播放 */
void MPUart_MusicPlay()
{
    MP_SendCmd(MPCMD_MUSIC_PLAY, MP_FEEDBACK_ON, 0);
}

/* 暂停 */
void MPUart_MusicPause()
{
    MP_SendCmd(MPCMD_MUSIC_PAUSE, MP_FEEDBACK_ON, 0);
}

/* 全部循环播放 开始/停止 */
void MPUart_PlayAll()
{
    static uint8_t flag = 0; // 1-开始  0-停止
    if(flag){
        MP_SendCmd(MPCMD_PLAYALL, MP_FEEDBACK_ON, 0x01); flag = 0;
    }else{
        MP_SendCmd(MPCMD_PLAYALL, MP_FEEDBACK_ON, 0); flag = 1;
    }
}

/* 下一曲 */
void MPUart_MusicNext()
{
    MP_SendCmd(MPCMD_NEXT_FILE, MP_FEEDBACK_ON, 0);
}

/* 上一曲 */
void MPUart_MusicPrev()
{
    MP_SendCmd(MPCMD_PREV_FILE, MP_FEEDBACK_ON, 0);
}

/* 播放指定歌曲 */
void MPUart_PointMusic(uint8_t music)
{
    MP_SendCmd(MPCMD_NUM_POINT, MP_FEEDBACK_ON, music);
}

/* 音量+ */
void MPUart_VolUP()
{
    MP_SendCmd(MPCMD_VOL_UP, MP_FEEDBACK_ON, 0);
}

/* 音量- */
void MPUart_VolDown()
{
    MP_SendCmd(MPCMD_VOL_DOWN, MP_FEEDBACK_ON, 0);
}

/* 设置音量 */
void MPUart_SetVol(uint8_t vol)
{
    MP_SendCmd(MPCMD_VOL_POINT, MP_FEEDBACK_ON, vol);
}


/*---------------------------------------------- 查询操作 -----------------------------------------------*/

/* 查询当前状态 */
void MPUart_QueryStatus()
{
    MP_SendCmd(MPCMD_STATUS, MP_FEEDBACK_ON, 0);
}

/* 查询当前音量 */
void MPUart_QueryVol()
{
    MP_SendCmd(MPCMD_VOL, MP_FEEDBACK_ON, 0);
}

/* 查询当前 EQ */
void MPUart_QueryEQ()
{
    MP_SendCmd(MPCMD_EQ, MP_FEEDBACK_ON, 0);
}

/* 查询当前播放模式 */
void MPUart_QueryPlayMode()
{
    MP_SendCmd(MPCMD_PLAYMODE, MP_FEEDBACK_ON, 0);
}