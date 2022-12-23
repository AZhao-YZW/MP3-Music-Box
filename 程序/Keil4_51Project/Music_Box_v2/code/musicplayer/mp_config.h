#ifndef __MP_CONFIG_H__
#define __MP_CONFIG_H__

#include "config.h"
/*********************************************
 *      以下为 Mini MP3 Player 配置.h文件
 *      包含一些 Mini MP3 Player 特有的配置
 *
 *          by AZhao, B站 @阿釗同学
 *********************************************/
 
/*------------------Mini MP3 Player设置-------------------*/
// MP控制方式选择
#define MP_CTRL_MODE        1       /* 1 - 串口, 0 - IO口 */
// 发送MP的数据是否要Feedback
#define MP_FEEDBACK_ON      1
#define MP_FEEDBACK_OFF     0
// 是否打印MP命令数据
#define MP_PRINT_DEBUG      0       /* 1 - 打印, 0 - 不打印 */

/*------------------Mini MP3 Player按键行为枚举类型定义-------------------*/
// 音乐播放器操作码
typedef enum MusicAppOptCode {
    /* 控制操作 */
    MPAPP_MUSIC_PLAY,       // 播放
    MPAPP_MUSIC_PAUSE,      // 暂停
    MPAPP_NEXT_FILE,        // 下一曲
    MPAPP_PREV_FILE,        // 上一曲
    MPAPP_VOL_UP,           // 音量+
    MPAPP_VOL_DOWN,         // 音量-
    MPAPP_EQ_POINT,         // 指定EQ(0-5:Normal/Pop/Rock/Jazz/Classic/Bass)
    MPAPP_SINGLE_LOOP,      // 单曲循环指定曲目播放(1-2999)
    MPAPP_PLAYALL,          // 全部循环播放([1:循环播放][0:停止循环播放])
    MPAPP_RANDOM_PLAY,      // 随机播放
    MPAPP_SLEEP_MODE,       // 进入睡眠(低功耗)

    /* 查询操作 */
    MPAPP_STATUS,           // 查询当前状态
    MPAPP_VOL,              // 查询当前音量
    MPAPP_EQ,               // 查询当前 EQ
    MPAPP_PLAYMODE,         // 查询当前播放模式

}eMusicAppOptCode;
/*------------------Mini MP3 Player通讯协议枚举类型定义-------------------*/
// MP接收数据帧
typedef enum MPRecFrame {
    MPREC_START = 0,        // 起始: "$S"-0x7E
    MPREC_VER,              // 版本: 0xFF
    MPREC_LEN,              // MP_LEN后字节数(数据长度不计起始、结束、校验)
    MPREC_CMD,              // 命令: 具体操作
    MPREC_FEEDBACK,         // 命令反馈: 1-反馈  0-不反馈
    MPREC_DATAH,            // 查询数据高字节
    MPREC_DATAL,            // 查询数据低字节
    MPREC_CHECKSUMH,        // 校验和(2bytes): 不计起始
    MPREC_CHECKSUML,
    MPREC_END               // 结束: "$0"-0xEF
}eMPRecFrame;

// MP发送数据帧的指令
typedef enum MPSendFrame {
    /* 直接发送的指令，不需要返回参数 */
    MPCMD_NEXT_FILE = 0x01, // 下一曲
    MPCMD_PREV_FILE,        // 上一曲
    MPCMD_NUM_POINT,        // 指定曲目(1-2999)
    MPCMD_VOL_UP,           // 音量+
    MPCMD_VOL_DOWN,         // 音量-
    MPCMD_VOL_POINT,        // 指定音量(0-30)
    MPCMD_EQ_POINT,         // 指定EQ(0-5:Normal/Pop/Rock/Jazz/Classic/Bass)
    MPCMD_SINGLE_LOOP,      // 单曲循环指定曲目播放(1-2999)
    MPCMD_DEVICE,           // 指定播放设备(U/SD/AUX/SLEEP/FLASH)
    MPCMD_SLEEP_MODE,       // 进入睡眠(低功耗)
    MPCMD_RESERVE,          // 保留(无命令)
    MPCMD_RESET,            // 模块复位
    MPCMD_MUSIC_PLAY,       // 播放
    MPCMD_MUSIC_PAUSE,      // 暂停
    MPCMD_MUSIC_FOLDER,     // 指定文件夹播放(01-99:每个文件夹最多255首歌)
    MPCMD_KALA,             // 扩音设置([DH=1:开扩音] [DL:设置增益0-31])
    MPCMD_PLAYALL,          // 全部循环播放([1:循环播放][0:停止循环播放])
    MPCMD_MP3_POINT,        // 指定"MP3"文件夹曲目(0-9999)
    MPCMD_INSERT_AD,        // 插播广告(0-9999)
    MPCMD_15_FOLDERS,       // 支持15个文件夹
    MPCMD_QUIT_AD,          // 停止插播，播放背景
    MPCMD_STOP_ALL,         // 停止播放
    MPCMD_FOLDER_LOOP,      // 指定文件夹循环播放
    MPCMD_RANDOM_PLAY,      // 随机播放
    MPCMD_LOOP_PLAY,        // 循环播放
    MPCMD_DAC_CONVERT,      // 开启和关闭 DAC

    /* 直接发送的指令，不需要返回参数 */
    MPCMD_STAY1 = 0x3C,
    MPCMD_STAY2,
    MPCMD_STAY3,
    MPCMD_INIT_PARA,        // 发送初始化参数(0 - 0x0F(低四位每位代表一种设备))
    MPCMD_ERR = 0x40,       // 返回错误，请求重发
    MPCMD_ACK,              // 应答
    MPCMD_STATUS,           // 查询当前状态
    MPCMD_VOL,              // 查询当前音量
    MPCMD_EQ,               // 查询当前 EQ
    MPCMD_PLAYMODE,         // 查询当前播放模式
    MPCMD_VER,              // 查询当前软件版本
    MPCMD_UDISK_TOTAL,      // 查询 UDISK 的总文件数
    MPCMD_TF_TOTAL,         // 查询 TF 卡的总文件数
    MPCMD_FLASH_TOTAL,      // 查询 FLASH 的总文件数
    MPCMD_STAY6,            // 保留
    MPCMD_UDISK_FILE,       // 查询 UDISK 的当前曲目
    MPCMD_TF_FILE,          // 查询 TF 卡的当前曲目
    MPCMD_FLASH_FILE,       // 查询 FLASH 的当前曲目
    MPCMD_FILE_NUMS,        // 查询文件夹里的文件总数
    MPCMD_FOLDER_NUMS       // 查询总文件数
}eMPFrameByte;

/*------------------Mini MP3 Player当前状态结构体-------------------*/
typedef struct
{
    uint8_t status;
    uint8_t vol;
    uint8_t eq;
    uint8_t playmode;
}sMPSta;


#endif
