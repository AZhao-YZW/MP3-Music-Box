#ifndef __OLED_CONFIG_H__
#define __OLED_CONFIG_H__

#include "config.h"
/*********************************************
 *              以下为Oled配置.h文件
 *              包含一些Oled特有的配置              
 *
 *          by AZhao, B站 @阿釗同学
 *********************************************/

/*------------------Oled设置-------------------*/
// 是否启用Oled错误警告
#define OLED_ERROR_ALARM    1   /* 1 - 启用, 0 - 停用 */
// 是否在绘画前检查是否超出范围
#define OLED_CHECK_RANGE    1   /* 1 - 启用, 0 - 停用 */
// 是否启用Oled调试绘画，在绘画过程中加入延时方便观察
#define OLED_DEBUG_DRAW     0   /* 1 - 启用, 0 - 停用 */
// 是否使用Oled绘图api的别名
#define OLED_USE_ALIAS      1   /* 1 - 启用, 0 - 停用 */
// Oled显示字符时字符间间距 - 像素
#define OLED_CHAR_SPACE     0
// Oled调试时每绘制 1byte 延时时间
#define OLED_DEBUG_DELAY    20
/*------------------Oled枚举类型定义-------------------*/
// Oled设备名称
typedef enum OledDevice {
    Oled1, Oled2
}eOledDevice;
// 数字类型
typedef enum NumType {
    nINT8, nINT16, nINT32, nFLOAT
}eNumType;
// 字体名称
typedef enum FontName {
    //ASCII码字库
    ASCII_H7W5=0,    ASCII_H8W6,    ASCII_H16W8,
    //汉字字库
    ZH_H16W16=50,    ZH_H32W32,
}eFontName;
// bmp位图名称
typedef enum BmpName {
    Menu1
}eBmpName;
//Oled警告码
typedef enum OledWarningCodes {
    IIC_ERROR,

    DRAW_OUT_OF_RANGE,

    FONT_NOT_EXIST, BMP_NOT_EXIST, NUM_TYPE_NOT_EXIST, ZH_CHAR_NOT_EXIST
}eOledWarningCodes;



#endif
