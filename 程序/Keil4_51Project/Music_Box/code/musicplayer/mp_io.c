#include "musicplayer/mp_io.h"

// MP IO口初始化
void MPIO_Init()
{
    // 设置 AD_KEY1 AD_KEY2 为高阻输入
    P1M1 = 0x03;//0b00000011;
    P1M0 = 0x00;//0b00000000;
    // 设置 IO1 IO2 为高阻输入
    P0M1 = 0x60;//0b01100000;
    P0M0 = 0x00;//0b00000000;
}

/* 通过 AD_KEY 播放指定曲目 */
void MPIO_PointPlay()
{

}

/* 全部循环播放 - AD_KEY1接地 */
void MPIO_PlayAll()
{
    // 设置 AD_KEY1 为准双向口
    P1M1 = 0x02;//0b00000010;
    P1M0 = 0x00;//0b00000000;
    MP_ADKEY1 = 0;
    delay_100ms();
    MP_ADKEY1 = 1;
    // 设置 AD_KEY1 AD_KEY2 为高阻输入
    P1M1 = 0x03;//0b00000011;
    P1M0 = 0x00;//0b00000000;
}

/* 下一曲 */
void MPIO_MusicNext()
{
    // 设置 IO2 为准双向口
    P0M1 = 0x20;//0b00100000;
    P0M0 = 0x00;//0b00000000;
    MP_IO2 = 0;
    delay_100ms();
    MP_IO2 = 1;
    // 设置 IO1 IO2 为高阻输入
    P0M1 = 0x60;//0b01100000;
    P0M0 = 0x00;//0b00000000;
}

/* 上一曲 */
void MPIO_MusicPrev()
{
    // 设置 IO1 为准双向口
    P0M1 = 0x40;//0b01000000;
    P0M0 = 0x00;//0b00000000;
    MP_IO1 = 0;
    delay_100ms();
    MP_IO1 = 1;
    // 设置 IO1 IO2 为高阻输入
    P0M1 = 0x60;//0b01100000;
    P0M0 = 0x00;//0b00000000;
}

/* 音量+ */
void MPIO_VolUP()
{
    // 设置 IO2 为准双向口
    P0M1 = 0x20;//0b00100000;
    P0M0 = 0x00;//0b00000000;
    MP_IO2 = 0;
    delay_100ms();delay_100ms();delay_100ms();delay_100ms();delay_100ms();
    delay_100ms();delay_100ms();delay_100ms();delay_100ms();delay_100ms();
    delay_100ms();delay_100ms();delay_100ms();delay_100ms();delay_100ms();
    delay_100ms();delay_100ms();delay_100ms();delay_100ms();delay_100ms();
    delay_100ms();delay_100ms();delay_100ms();delay_100ms();delay_100ms();
    delay_100ms();delay_100ms();delay_100ms();delay_100ms();delay_100ms();
    MP_IO2 = 1;
    // 设置 IO1 IO2 为高阻输入
    P0M1 = 0x60;//0b01100000;
    P0M0 = 0x00;//0b00000000;
}

/* 音量- */
void MPIO_VolDown()
{
    // 设置 IO1 为准双向口
    P0M1 = 0x40;//0b01000000;
    P0M0 = 0x00;//0b00000000;
    MP_IO1 = 0;
    delay_100ms();delay_100ms();delay_100ms();delay_100ms();delay_100ms();
    delay_100ms();delay_100ms();delay_100ms();delay_100ms();delay_100ms();
    delay_100ms();delay_100ms();delay_100ms();delay_100ms();delay_100ms();
    delay_100ms();delay_100ms();delay_100ms();delay_100ms();delay_100ms();
    delay_100ms();delay_100ms();delay_100ms();delay_100ms();delay_100ms();
    delay_100ms();delay_100ms();delay_100ms();delay_100ms();delay_100ms();
    MP_IO1 = 1;
    // 设置 IO1 IO2 为高阻输入
    P0M1 = 0x60;//0b01100000;
    P0M0 = 0x00;//0b00000000;
}





