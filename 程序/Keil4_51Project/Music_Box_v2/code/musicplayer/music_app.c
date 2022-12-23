#include "musicplayer/music_app.h"
#include "musicplayer/mp_uart.h"
#include "oled/oled_api.h"
#include "key/key.h"
#include "menu/menu_builder.h"
#include "musicplayer/mp_control.h"

// 跳转到目标菜单
extern void Menu_Display();
extern void Menu_KeyRegister();
extern void Menu_Switch(uint8_t scode);

//Mini MP3 Player当前状态结构体
sMPSta MPSta = {0};

// 当前音乐播放状态  0-暂停  1-播放(初始为暂停)
static uint8_t playSta = 0;     
// 音乐播放器Oled菜单
static sMenu Menu[MUSIC_APP_ENDID] = {0};
// 音乐播放器菜单位置记忆结构体
static sMenuLoc MenuLoc[MUSIC_APP_MAX_LEVEL] = {0};
// 音乐播放器菜单操作结构体
//      - Keil C51编译器 仅支持C90语法和自身定义的一些拓展，结构体初始化只能按顺序
static sMenuOpt MenuOpt = {
    Menu,               // sMenu* menu;
    MUSIC_APP_ENDID,    // uint8_t endid;
    0,                  // uint8_t curId;
    1,                  // uint8_t pointId;
    M_BACKMT,           // uint8_t backMaintain;
    MenuLoc,            // sMenuLoc* menuLoc;
    0,                  // uint8_t disFont;
};

// 空函数
static void x(){}
// 菜单切换单个函数 - 便于按键注册
static void Menu_Pre()      {MusicApp_Switch(M_PRE);}
static void Menu_Next()     {MusicApp_Switch(M_NEXT);}
static void Menu_Ok()       {MusicApp_Switch(M_OK);}
static void Menu_Back()     {MusicApp_Switch(M_BACK);}
static void Menu_BackRoot() {MusicApp_Switch(M_BACKROOT);}
// 音乐播放器单个操作 - 便于按键注册&菜单调用
/* 播放/暂停 */
static void MusicApp_MusicPlay()
{
    if(playSta) {MusicApp_Operate(MPAPP_MUSIC_PAUSE); playSta=0;}
    else        {MusicApp_Operate(MPAPP_MUSIC_PLAY);  playSta=1;}
}
static void MusicApp_Next()     {MusicApp_Operate(MPAPP_NEXT_FILE);}    /* 下一曲 */
static void MusicApp_Prev()     {MusicApp_Operate(MPAPP_PREV_FILE);}    /* 上一曲 */
static void MusicApp_VolUp()    {MusicApp_Operate(MPAPP_VOL_UP);}       /* 音量+ */
static void MusicApp_VolDown()  {MusicApp_Operate(MPAPP_VOL_DOWN);}     /* 音量- */
// 音乐播放器单个操作 - 便于菜单调用
static void MusicApp_Status()   {MusicApp_Operate(MPAPP_STATUS);}       /* 查询当前状态 */
static void MusicApp_Vol()      {MusicApp_Operate(MPAPP_VOL);}          /* 查询当前音量 */
static void MusicApp_EQ()       {MusicApp_Operate(MPAPP_EQ);}           /* 查询当前 EQ */
static void MusicApp_PlayMode() {MusicApp_Operate(MPAPP_PLAYMODE);}     /* 查询当前播放模式 */
//------------播放指定曲目
static void Song1()     {MPUart_PointMusic(1); playSta=1;}
static void Song2()     {MPUart_PointMusic(2); playSta=1;}
static void Song3()     {MPUart_PointMusic(3); playSta=1;}

// 本菜单功能按键注册
void MusicApp_KeyRegister()
{
    // 控制菜单
    Key_Register(Menu_Back,BtnLL);
    Key_Register(Menu_Ok,BtnLR);
    Key_Register(Menu_Pre,BtnLU);
    Key_Register(Menu_Next,BtnLD);
    Key_Register(Menu_Ok,BtnLM);
    // 控制音乐播放
    Key_Register(MusicApp_Prev,BtnRL);
    Key_Register(MusicApp_Next,BtnRR);
    Key_Register(MusicApp_VolUp,BtnRU);
    Key_Register(MusicApp_VolDown,BtnRD);
    Key_Register(MusicApp_MusicPlay,BtnRM);
}

// 音乐播放器初始化
void MusicApp_Init()
{
    uint8_t i;

    //初始化目录元素   格式: (文件名语言,类型,层级,自身节点数,子节点数/调用函数/...,文件名)
    i=0-1;  //使设置第一个元素时从i=0开始
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   0,  0,  4,                       "MusicApp");
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   1,  1,  3,                           "Default");
    M_SETARGS(Menu, M_ASCII,  M_FILE,     2,  1,  Song1,                           "Song 1");
    M_SETARGS(Menu, M_ASCII,  M_FILE,     2,  2,  Song2,                           "Song 2");
    M_SETARGS(Menu, M_ASCII,  M_FILE,     2,  3,  Song3,                           "Song 3");
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   1,  2,  0,                           "Like");
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   1,  3,  0,                           "Collect");
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   1,  4,  2,                           "MusicApp Test");
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   2,  1,  7,                               "Control");
    M_SETARGS(Menu, M_ASCII,  M_FILE,     3,  1,  MusicApp_MusicPlay,                  "*Start/Pause Song");
    M_SETARGS(Menu, M_ASCII,  M_FILE,     3,  2,  x,                                   "Stop Song");
    M_SETARGS(Menu, M_ASCII,  M_FILE,     3,  3,  MusicApp_Prev,                       "*Prev Song");
    M_SETARGS(Menu, M_ASCII,  M_FILE,     3,  4,  MusicApp_Next,                       "*Next Song");
    M_SETARGS(Menu, M_ASCII,  M_FILE,     3,  5,  MusicApp_VolUp,                      "*Vol Up");
    M_SETARGS(Menu, M_ASCII,  M_FILE,     3,  6,  MusicApp_VolDown,                    "*Vol Down");
    M_SETARGS(Menu, M_ASCII,  M_FILE,     3,  7,  x,                                   "Quit MusicApp");
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   2,  2,  4,                               "Query");
    M_SETARGS(Menu, M_ASCII,  M_FILE,     3,  1,  MusicApp_Status,                     "*Query Status");
    M_SETARGS(Menu, M_ASCII,  M_FILE,     3,  2,  MusicApp_Vol,                        "*Query Vol");
    M_SETARGS(Menu, M_ASCII,  M_FILE,     3,  3,  MusicApp_EQ,                         "*Query EQ");
    M_SETARGS(Menu, M_ASCII,  M_FILE,     3,  4,  MusicApp_PlayMode,                   "*Query PlayMode");
    
    //初始化菜单操作结构体
    MenuOpt.curId = 0; MenuOpt.pointId = 1;
    MenuOpt.backMaintain = M_BACKNOMT;
    MenuOpt.disFont = 0;
    //按键功能映射
    Key_AllDelete();
    MusicApp_KeyRegister();
    //音乐播放器界面显示
    MusicApp_Display();
    //音乐播放器初始化
    MP_Init();
}

// 音乐播放器界面显示
void MusicApp_Display()
{
    MenuBuilder_Display(&MenuOpt,Oled2);
}

// 菜单切换
// scode: 切换操作
void MusicApp_Switch(uint8_t scode)
{
    switch(scode)
    {
        /* 上一个 */
        case M_PRE:
            MenuBuilder_Switch(&MenuOpt,Oled2,scode);
        break;
        /* 下一个 */
        case M_NEXT:
            MenuBuilder_Switch(&MenuOpt,Oled2,scode);
        break;
        /* 确认 */
        case M_OK:
            // M_SETBACKMT(MenuOpt,0,4,M_BACKMT);      // 进入MusicApp Test设置记忆位置模式
            MenuBuilder_Switch(&MenuOpt,Oled2,scode);
        break;
        /* 上一级 */
        case M_BACK:
            if(MenuOpt.curId==0){    // 退出Music App, 暂停音乐, 返回菜单界面
                MusicApp_Operate(MPAPP_MUSIC_PAUSE); playSta=0;
                o_dev(Oled2);
                o_clr();
                Key_AllDelete();
                Menu_KeyRegister();
                Menu_Switch(M_BACK); //改变curId回到上一级，否则又会跳转到当前File的函数
                return;
            }else{
                MenuBuilder_Switch(&MenuOpt,Oled2,scode);
                // M_SETBACKMT(MenuOpt,0,4,M_BACKNOMT);      // 退出MusicApp Test退出记忆位置模式
            }
        break;
        /* 返回主菜单 */
        case M_BACKROOT:
            MenuBuilder_Switch(&MenuOpt,Oled2,scode);
        break;
    }
}

// 音乐播放器操作
void MusicApp_Operate(eMusicAppOptCode optcode)
{
    // Oled2显示音乐界面
    o_dev(Oled2);

    switch(optcode)
    {
        /* 播放 */
        case MPAPP_MUSIC_PLAY:
            MPCtrl_Operate(MPAPP_MUSIC_PLAY);
        break;
        /* 暂停 */
        case MPAPP_MUSIC_PAUSE:
            MPCtrl_Operate(MPAPP_MUSIC_PAUSE);
        break;
        /* 下一曲 */
        case MPAPP_NEXT_FILE:
            MPCtrl_Operate(MPAPP_NEXT_FILE);
        break;
        /* 上一曲 */
        case MPAPP_PREV_FILE:
            MPCtrl_Operate(MPAPP_PREV_FILE);
        break;
        /* 音量+ */
        case MPAPP_VOL_UP:
            MPCtrl_Operate(MPAPP_VOL_UP);
        break;
        /* 音量- */
        case MPAPP_VOL_DOWN:
            MPCtrl_Operate(MPAPP_VOL_DOWN);
        break;
        /* 指定EQ(0-5:Normal/Pop/Rock/Jazz/Classic/Bass) */
        case MPAPP_EQ_POINT:

        break;
        /* 单曲循环指定曲目播放(1-2999) */
        case MPAPP_SINGLE_LOOP:

        break;
        /* 全部循环播放([1:循环播放][0:停止循环播放]) */
        case MPAPP_PLAYALL:
            MPCtrl_Operate(MPAPP_PLAYALL);
        break;
        /* 随机播放 */
        case MPAPP_RANDOM_PLAY:

        break;
        /* 进入睡眠(低功耗) */
        case MPAPP_SLEEP_MODE:

        break;
        /* 查询当前状态 */
        case MPAPP_STATUS:
            MPCtrl_Operate(MPAPP_STATUS);
            o_partclr(0,48,24,64);
            o_char(0,48,MPSta.status%10+'0',ASCII_H16W8);
        break;
        /* 查询当前音量 */
        case MPAPP_VOL:
            MPCtrl_Operate(MPAPP_VOL);
            // o_partclr(0,48,24,64);
            // o_char(0,48,MPSta.vol/10+'0',ASCII_H16W8);
            // o_char(8,48,MPSta.vol%10+'0',ASCII_H16W8);
        break;
        /* 查询当前 EQ */
        case MPAPP_EQ:
            MPCtrl_Operate(MPAPP_EQ);
            o_partclr(0,48,24,64);
            o_char(0,48,MPSta.eq%10+'0',ASCII_H16W8);
        break;
        /* 查询当前播放模式 */
        case MPAPP_PLAYMODE:
            MPCtrl_Operate(MPAPP_PLAYMODE);
            o_partclr(0,48,24,64);
            o_char(0,48,MPSta.playmode%10+'0',ASCII_H16W8);
        break;
    }
}



