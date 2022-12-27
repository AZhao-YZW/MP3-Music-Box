#include "oled/oled_test.h"
#include "oled/oled_api.h"
#include "menu/menu.h"

extern uint32_t iic_rate;
extern uint8_t iic_ok;
extern uint8_t oledIICRate;

/*********************************************
 *              以下为Oled绘图API测试
 *              无屏幕缓存版本
 *  测试单片机:
 *      STC89C52RC
 *      STC12C5A60S2
 *  测试内容:
 *      1. Oled亮/熄屏
 *      2. Oled正/反色显示
 *      3. Oled正/反转显示
 *      4. Oled调试绘画测试
 *      5. Oled全部清屏和部分清屏对比
 *      6. Oled显示IIC速率(10s检测一次)
 *      7. Oled多个设备显示
 *      8. Oled综合测试
 *          Ⅰ. 所有字符显示、数字显示、汉字显示测试
 *          Ⅱ. "DVD"字符串移动显示
 *          Ⅲ. 时钟功能演示
 *          Ⅳ. 画正弦波波形 正常&调试绘画
 *          Ⅴ. 菜单显示
 *          Ⅵ. 显示动图
 *
 *          by AZhao, B站 @阿釗同学
 *********************************************/

/**
 * Oled绘图API测试
 */
void Oled_Test()
{
    //-----1. Oled亮/熄屏
    // Oled_Dis_Test();
    //-----2. Oled正/反色显示
    // Oled_Color_Test();
    //-----3. Oled正/反转显示
    // Oled_Dir_Test();
    //-----4. Oled调试绘画测试
    // Oled_DebugDraw_Test();
    //-----5. Oled全部清屏和部分清屏对比
    // Oled_Clear_Test();
    //-----6. Oled显示IIC速率
    // Oled_ShowIICRate_Test();
    //-----7. Oled多个设备显示
    Oled_ShowMuti_Test();
    //-----8. Oled综合测试
    //----------Ⅰ. 所有字符显示、数字显示、汉字显示测试
    // Oled_AllChars_Test();
    //----------Ⅱ. "DVD"字符串移动显示
    // Oled_DVDMove_Test();
    //----------Ⅲ. 时钟功能演示
    // Oled_Timer_Test();
    //----------Ⅳ. 画正弦波波形 正常&调试绘画
    // Oled_DrawSin_Test();
    //----------Ⅴ. 菜单显示
    // Oled_Menu_Test();
    //----------Ⅵ. 显示动图
    // Oled_Gif_Test();
}

/**
 * 1. Oled亮/熄屏
 */
void Oled_Dis_Test()
{
    uint8_t i;

    o_dev(Oled2);
    o_clr();
    o_chinese(0,0,"阿 釗 同 学",ZH_H16W16);
    for(i=0; i<3; i++){
        o_off();
        o_delayms(500);
        o_on();
        o_delayms(500);
    }
}

/**
 * 2. Oled正/反色显示
 */
void Oled_Color_Test()
{
    uint8_t i;

    o_dev(Oled2);
    o_clr();
    o_chinese(0,0,"阿 釗 同 学",ZH_H16W16);
    for(i=0; i<3; i++){
        o_color(1);
        o_delayms(500);
        o_color(0);
        o_delayms(500);
    }
    o_color(1);
}

/**
 * 3. Oled正/反转显示
 */
void Oled_Dir_Test()
{
    uint8_t i;

    o_dev(Oled2);
    o_clr();
    o_chinese(0,0,"阿 釗 同 学",ZH_H16W16);
    for(i=0; i<3; i++){
        o_dir(1);
        o_delayms(500);
        o_dir(0);
        o_delayms(500);
    }
    o_dir(1);
}

/**
 * 4. Oled调试绘画测试
 */
void Oled_DebugDraw_Test()
{
    uint8_t i;
    uint8_t delay_time = 10;

    o_dev(Oled2);
    o_dedraw(1);
    for(i=0; i<3; i++){
        o_clr();
        o_chinese(0,0,"阿 釗 同 学",ZH_H16W16);
        delay_time += 20;
        o_dedelay(delay_time);
        o_delayms(500);
    }
    o_dedelay(10);
    o_dedraw(0);
}

/**
 * 5. Oled全部清屏和部分清屏对比  -??
 */
void Oled_Clear_Test()
{
    static uint8_t i=0;    // 定义成静态变量，防止被其他程序共享此变量空间

    o_dev(Oled2);
    for(i=0; i<10; i++){
        o_clr();
        o_chinese(0,0,"阿 釗 同 学",ZH_H16W16);
        o_num(0,48,&i,nINT8,ASCII_H16W8);
    }
    for(i=0; i<50; i++){
        o_partclr(0,0,64,16);
        o_partclr(0,48,16,64);
        o_chinese(0,0,"阿 釗 同 学",ZH_H16W16);
        o_num(0,48,&i,nINT8,ASCII_H16W8);
    }
}

/**
 * 6. Oled显示IIC速率(10s检测一次)
 */
void Oled_ShowIICRate_Test()
{
    uint16_t i;

    o_dev(Oled2);
    o_clr();
    o_str(0,0,"IIC rate: ",ASCII_H16W8);
    o_str(32,32,"FPS: ",ASCII_H16W8);
    oledIICRate = 1;
    for(i=0; i<100; i++){
        o_partclr(0,48,128,64);
        if(iic_ok){
            ET0 = 0;
            o_partclr(80,0,128,15);
            o_partclr(80,32,128,47);
            o_iicrate(80,0,&iic_rate);
            iic_rate /= 1024;
            o_iicrate(80,32,&iic_rate);
            iic_ok = 0;
            iic_rate = 0;
            ET0 = 1;
        }
    }
    oledIICRate = 0;
}

/**
 * 7. Oled多个设备显示
 */
void Oled_ShowMuti_Test()
{
    o_dev(Oled1);
    o_clr();
    o_chinese(0,0,"阿 釗 同 学",ZH_H16W16);
    o_dev(Oled2);
    o_clr();
    o_chinese(0,0,"阿 釗 同 学",ZH_H16W16);
    o_delayms(500);
    o_off();
    o_delayms(500);
    o_on();
    o_delayms(500);
    o_off();
    o_delayms(500);
    o_on();
}

/**
 * 8. Oled综合测试
 *     Ⅰ. 所有字符显示、数字显示、汉字显示测试
 */
void Oled_AllChars_Test()
{

}

/**
 * 8. Oled综合测试
 *     Ⅱ. "DVD"字符串移动显示
 */
void Oled_DVDMove_Test()
{

}

/**
 * 8. Oled综合测试
 *     Ⅲ. 时钟功能演示
 */
void Oled_Timer_Test()
{

}

/**
 * 8. Oled综合测试
 *     Ⅳ. 画正弦波波形 正常&调试绘画
 */
void Oled_DrawSin_Test()
{

}

/**
 * 8. Oled综合测试
 *     Ⅴ. 菜单显示
 */
void Oled_Menu_Test()
{
    o_delayms(500);Menu_Switch(M_NEXT);
    o_delayms(500);Menu_Switch(M_PRE);
    o_delayms(500);Menu_Switch(M_OK);  
    o_delayms(500);Menu_Switch(M_OK);  
    o_delayms(500);Menu_Switch(M_NEXT);
    o_delayms(500);Menu_Switch(M_NEXT);
    o_delayms(500);Menu_Switch(M_OK);  
    o_delayms(500);Menu_Switch(M_BACK);
    o_delayms(500);Menu_Switch(M_BACK);
    o_delayms(500);Menu_Switch(M_BACK);
    o_delayms(500);Menu_Switch(M_BACK);
}

/**
 * 7. Oled综合测试
 *     Ⅵ. 显示动图
 */
void Oled_Gif_Test()
{

}
