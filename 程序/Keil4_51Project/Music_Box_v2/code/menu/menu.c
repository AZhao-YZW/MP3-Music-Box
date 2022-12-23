#include "menu/menu.h"
#include "oled/oled_api.h"
#include "oled/oled_test.h"
#include "key/key.h"
#include "musicplayer/music_app.h"
#include "menu/menu_builder.h"
#include "musicplayer/mp_uart.h"

// Oled菜单
static sMenu Menu[EndId] = {0};
// 菜单位置记忆结构体
static sMenuLoc MenuLoc[MENU_MAX_LEVEL] = {0};
// 菜单操作结构体
//      - Keil C51编译器 仅支持C90语法和自身定义的一些拓展，结构体初始化只能按顺序
static sMenuOpt MenuOpt = {
    Menu,               // sMenu* menu;
    EndId,              // uint8_t endid;
    0,                  // uint8_t curId;
    1,                  // uint8_t pointId;
    M_BACKMT,           // uint8_t backMaintain;
    MenuLoc,            // sMenuLoc* menuLoc;
    0,                  // uint8_t disFont;
};
// 空函数
static void x(){}
// 菜单切换单个函数 - 便于按键注册
static void Menu_Pre()      {Menu_Switch(M_PRE);}
static void Menu_Next()     {Menu_Switch(M_NEXT);}
static void Menu_Ok()       {Menu_Switch(M_OK);}
static void Menu_Back()     {Menu_Switch(M_BACK);}
static void Menu_BackRoot() {Menu_Switch(M_BACKROOT);}
// 本菜单功能按键注册
void Menu_KeyRegister()
{
    Key_Register(Menu_Back,BtnLL);
    Key_Register(Menu_Ok,BtnLR);
    Key_Register(Menu_Pre,BtnLU);
    Key_Register(Menu_Next,BtnLD);
    Key_Register(Menu_Ok,BtnLM);

    Key_Register(Menu_Back,BtnRL);
    Key_Register(Menu_Ok,BtnRR);
    Key_Register(Menu_Pre,BtnRU);
    Key_Register(Menu_Next,BtnRD);
    Key_Register(Menu_Ok,BtnRM);
}

// 菜单初始化
void Menu_Init()
{
    uint8_t i;

    //初始化目录元素   格式: (文件名语言,类型,层级,自身节点数,子节点数/调用函数/...,文件名)
    i=0-1;  //使设置第一个元素时从i=0开始
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   0,  0,  3,                        "Main Menu");

    /* 音乐播放器 - Music App */
    M_SETARGS(Menu, M_ASCII,  M_FILE,     1,  1,  MusicApp_Init,                "Music App");

    /* Oled测试 */
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   1,  2,  8,                            "Oled Test");
    M_SETARGS(Menu, M_ASCII,  M_FILE,     2,  1,  Oled_Dis_Test,                    "Display On/Off");
    M_SETARGS(Menu, M_ASCII,  M_FILE,     2,  2,  Oled_Color_Test,                  "Color");
    M_SETARGS(Menu, M_ASCII,  M_FILE,     2,  3,  Oled_Dir_Test,                    "Direct");
    M_SETARGS(Menu, M_ASCII,  M_FILE,     2,  4,  Oled_DebugDraw_Test,              "Debug Draw");
    M_SETARGS(Menu, M_ASCII,  M_FILE,     2,  5,  Oled_Clear_Test,                  "Clear Screen");
    M_SETARGS(Menu, M_ASCII,  M_FILE,     2,  6,  Oled_ShowIICRate_Test,            "IIC Rate");
    M_SETARGS(Menu, M_ASCII,  M_FILE,     2,  7,  Oled_ShowMuti_Test,               "Muti Screen");
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   2,  8,  1,                                "System Test");
    M_SETARGS(Menu, M_ASCII,  M_FILE,     3,  1,  Oled_Menu_Test,                       "Oled Menu");

    /* 菜单文件测试 */
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   1,  3,  3,                            "Folder Test");
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   2,  1,  2,                                "Folder A");
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   3,  1,  0,                                    "Folder a1");
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   3,  2,  0,                                    "Folder a2");
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   2,  2,  3,                                "Folder B");
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   3,  1,  0,                                    "Folder b1");
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   3,  2,  0,                                    "Folder b2");
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   3,  3,  0,                                    "Folder b3");
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   2,  3,  8,                                "Folder C");
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   3,  1,  0,                                    "Folder c1");
    M_SETARGS(Menu, M_ZH,     M_FOLDER,   3,  2,  0,                                    "阿 釗 同 学");
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   3,  3,  0,                                    "Folder c3");
    M_SETARGS(Menu, M_ZH,     M_FOLDER,   3,  4,  0,                                    "同 学");
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   3,  5,  0,                                    "Folder c5");
    M_SETARGS(Menu, M_ZH,     M_FOLDER,   3,  6,  0,                                    "阿 釗");
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   3,  7,  0,                                    "Folder c7");
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   3,  8,  0,                                    "Folder c8");

    //按键功能映射
    Menu_KeyRegister();
    //菜单显示
    Menu_Display();
    //音乐播放器初始化，先暂停音乐
    MPUart_Init();
}


// 菜单显示
void Menu_Display()
{
    MenuBuilder_Display(&MenuOpt,Oled1);
}

// 菜单切换
// scode: 切换操作
void Menu_Switch(uint8_t scode)
{
    switch(scode)
    {
        /* 上一个 */
        case M_PRE:
            MenuBuilder_Switch(&MenuOpt,Oled1,scode);
        break;
        /* 下一个 */
        case M_NEXT:
            MenuBuilder_Switch(&MenuOpt,Oled1,scode);
        break;
        /* 确认 */
        case M_OK:
            MenuBuilder_Switch(&MenuOpt,Oled1,scode);
        break;
        /* 上一级 */
        case M_BACK:
            MenuBuilder_Switch(&MenuOpt,Oled1,scode);
        break;
        /* 返回主菜单 */
        case M_BACKROOT:
            MenuBuilder_Switch(&MenuOpt,Oled1,scode);
        break;
    }
}

