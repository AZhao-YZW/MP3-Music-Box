#include "musicplayer/mp_control.h"
#if (MP_CTRL_MODE == 0)
    #include "musicplayer/mp_io.h"
#else
    #include "musicplayer/mp_uart.h"
#endif

/*------------------------------------------- IO控制方式 ---------------------------------------*/
#if (MP_CTRL_MODE == 0)
// 音乐播放器初始化
void MP_Init()
{
    MPIO_Init();
}

// 音乐播放器操作
void MPCtrl_Operate(eMusicAppOptCode optcode)
{
    switch(optcode)
    {
        /* 播放 */
        case MPAPP_MUSIC_PLAY:
            MPUart_MusicPlay();
        break;
        /* 暂停 */
        case MPAPP_MUSIC_PAUSE:
            MPUart_MusicPause();
        break;
        /* 下一曲 */
        case MPAPP_NEXT_FILE:
            MPIO_MusicNext();
        break;
        /* 上一曲 */
        case MPAPP_PREV_FILE:
            MPIO_MusicPrev();
        break;
        /* 音量+ */
        case MPAPP_VOL_UP:
            MPIO_VolUP();
        break;
        /* 音量- */
        case MPAPP_VOL_DOWN:
            MPIO_VolDown();
        break;
        /* 全部循环播放 */
        case MPAPP_PLAYALL:
            MPIO_PlayAll();
        break;
    }
}
/*------------------------------------------- 串口控制方式 ---------------------------------------*/
#else

// 音乐播放器初始化
void MP_Init()
{
    MPUart_Init();
}

// 音乐播放器操作
void MPCtrl_Operate(eMusicAppOptCode optcode)
{
    switch(optcode)
    {
        /* 播放 */
        case MPAPP_MUSIC_PLAY:
            MPUart_MusicPlay();
        break;
        /* 暂停 */
        case MPAPP_MUSIC_PAUSE:
            MPUart_MusicPause();
        break;
        /* 下一曲 */
        case MPAPP_NEXT_FILE:
            MPUart_MusicNext();
        break;
        /* 上一曲 */
        case MPAPP_PREV_FILE:
            MPUart_MusicPrev();
        break;
        /* 音量+ */
        case MPAPP_VOL_UP:
            MPUart_VolUP();
        break;
        /* 音量- */
        case MPAPP_VOL_DOWN:
            MPUart_VolDown();
        break;
        /* 指定EQ(0-5:Normal/Pop/Rock/Jazz/Classic/Bass) */
        case MPAPP_EQ_POINT:

        break;
        /* 单曲循环指定曲目播放(1-2999) */
        case MPAPP_SINGLE_LOOP:

        break;
        /* 全部循环播放 */
        case MPAPP_PLAYALL:
            MPUart_PlayAll();
        break;
        /* 随机播放 */
        case MPAPP_RANDOM_PLAY:

        break;
        /* 进入睡眠(低功耗) */
        case MPAPP_SLEEP_MODE:

        break;
        /* 查询当前状态 */
        case MPAPP_STATUS:
            MPUart_QueryStatus();
        break;
        /* 查询当前音量 */
        case MPAPP_VOL:
            MPUart_QueryVol();
        break;
        /* 查询当前 EQ */
        case MPAPP_EQ:
            MPUart_QueryEQ();
        break;
        /* 查询当前播放模式 */
        case MPAPP_PLAYMODE:
            MPUart_QueryPlayMode();
        break;
    }
}


#endif




