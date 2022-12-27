#ifndef __UART_DRIVER_H__
#define __UART_DRIVER_H__

#include "musicplayer/mp_config.h"
/*********************************************
 *       Mini MP3 Player 串口控制驱动.h文件
 *
 *          by AZhao, B站 @阿釗同学
 *********************************************/

// MP接收数据帧长度
#define MP_REC_FRAME_LEN        10
#define MP_SEND_FRAME_LEN       10


void MP_UartDriverInit();
void MP_RecProcess();
void MP_SendString(uint8_t* str);
void MP_SendCmd(uint8_t cmd, uint8_t feedback, uint16_t dat);

#endif
