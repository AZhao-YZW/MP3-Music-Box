#include "menu/menu.h"
#include "oled/oled_api.h"
#include "oled/oled_test.h"
#include "key/key.h"
#include "musicplayer/music_app.h"
#include "menu/menu_builder.h"
#include "musicplayer/mp_uart.h"

// Oled Menu
static sMenu Menu[EndId] = {0};
// Menu position memory structure
static sMenuLoc MenuLoc[MENU_MAX_LEVEL] = {0};
// Menu operation structure
//      - Keil C51 compiler only supports C90 syntax and some extensions, struct initialization must be sequential
static sMenuOpt MenuOpt = {
    Menu,               // sMenu* menu;
    EndId,              // uint8_t endid;
    0,                  // uint8_t curId;
    1,                  // uint8_t pointId;
    M_BACKMT,           // uint8_t backMaintain;
    MenuLoc,            // sMenuLoc* menuLoc;
    0,                  // uint8_t disFont;
};
// Empty function
static void x(){}
// Menu switch single function - for easy key registration
static void Menu_Pre()      {Menu_Switch(M_PRE);}
static void Menu_Next()     {Menu_Switch(M_NEXT);}
static void Menu_Ok()       {Menu_Switch(M_OK);}
static void Menu_Back()     {Menu_Switch(M_BACK);}
static void Menu_BackRoot() {Menu_Switch(M_BACKROOT);}
// Register menu function keys
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

// Menu initialization
void Menu_Init()
{
    uint8_t i;

    // Initialize directory elements   Format: (language, type, level, self_node, son_node/func, name)
    i=0-1;  // Make the first element start from i=0
    M_SETARGS(Menu, M_ASCII,  M_FOLDER,   0,  0,  3,                        "Main Menu");

    /* Music Player */
    M_SETARGS(Menu, M_ASCII,  M_FILE,     1,  1,  MusicApp_Init,                "Music App");

    /* Oled Test */
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

    /* Menu File Test */
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

    // Key function mapping
    Menu_KeyRegister();
    // Menu display
    Menu_Display();
    // MP3 player init, pause music first
    MPUart_Init();
}


// Menu display
void Menu_Display()
{
    MenuBuilder_Display(&MenuOpt,Oled1);
}

// Menu switch
// scode: switch operation
void Menu_Switch(uint8_t scode)
{
    switch(scode)
    {
        /* Previous */
        case M_PRE:
            MenuBuilder_Switch(&MenuOpt,Oled1,scode);
        break;
        /* Next */
        case M_NEXT:
            MenuBuilder_Switch(&MenuOpt,Oled1,scode);
        break;
        /* Confirm */
        case M_OK:
            MenuBuilder_Switch(&MenuOpt,Oled1,scode);
        break;
        /* Back */
        case M_BACK:
            MenuBuilder_Switch(&MenuOpt,Oled1,scode);
        break;
        /* Back to root */
        case M_BACKROOT:
            MenuBuilder_Switch(&MenuOpt,Oled1,scode);
        break;
    }
}

