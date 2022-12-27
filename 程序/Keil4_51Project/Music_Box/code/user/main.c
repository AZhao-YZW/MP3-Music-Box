#include "config.h"
#include "oled/oled_api.h"
#include "oled/oled_test.h"
#include "timer/timer.h"
#include "menu/menu.h"
#include "key/key.h"
#include "musicplayer/mp_uart_driver.h"

void main()
{
    o_dev(Oled1); Oled_Init();
    o_dev(Oled2); Oled_Init();
    Key_Init();
    Menu_Init();
    Timer0_Init(10);    // 100Hz
    LED = 0;

    MP_UartDriverInit();

    while(1)
    {
        Key_Driver();
        // MP_SendCmd(MPCMD_MUSIC_PAUSE, MP_FEEDBACK_ON, 0);
        MP_RecProcess();
        // Oled_Test();
    }
}