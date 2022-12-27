#include "key/key.h"

//按键状态
static uint8_t keyState[KEY_NUMS];
static uint8_t keyLastSta[KEY_NUMS];
//空函数
static void Blank_Func(){}
//按下对应按键调用的函数
static void (*BTN_N_Down[KEY_NUMS])();
static uint8_t btnFlag[KEY_NUMS];

//按键初始化
void Key_Init()
{
    uint8_t i;

    for(i=0; i<KEY_NUMS; i++){
        keyState[i] = KEY_UP;
        keyLastSta[i] = KEY_UP;
        BTN_N_Down[i] = Blank_Func;
        btnFlag[i] = KEY_UP;
    }
}

//按键注册
//func-注册函数
//num-按键编号
void Key_Register(void (*func)(void), eBtnN num)
{
    switch(num)
    {
        case BtnLL: BTN_N_Down[BtnLL] = func; break;
        case BtnLR: BTN_N_Down[BtnLR] = func; break;
        case BtnLU: BTN_N_Down[BtnLU] = func; break;
        case BtnLD: BTN_N_Down[BtnLD] = func; break;
        case BtnLM: BTN_N_Down[BtnLM] = func; break;
        case BtnRL: BTN_N_Down[BtnRL] = func; break;
        case BtnRR: BTN_N_Down[BtnRR] = func; break;
        case BtnRU: BTN_N_Down[BtnRU] = func; break;
        case BtnRD: BTN_N_Down[BtnRD] = func; break;
        case BtnRM: BTN_N_Down[BtnRM] = func; break;
    }
}

//按键注销
//num-按键编号
void Key_Delete(eBtnN num)
{
    switch(num)
    {
        case BtnLL: BTN_N_Down[BtnLL] = Blank_Func; break;
        case BtnLR: BTN_N_Down[BtnLR] = Blank_Func; break;
        case BtnLU: BTN_N_Down[BtnLU] = Blank_Func; break;
        case BtnLD: BTN_N_Down[BtnLD] = Blank_Func; break;
        case BtnLM: BTN_N_Down[BtnLM] = Blank_Func; break;
        case BtnRL: BTN_N_Down[BtnRL] = Blank_Func; break;
        case BtnRR: BTN_N_Down[BtnRR] = Blank_Func; break;
        case BtnRU: BTN_N_Down[BtnRU] = Blank_Func; break;
        case BtnRD: BTN_N_Down[BtnRD] = Blank_Func; break;
        case BtnRM: BTN_N_Down[BtnRM] = Blank_Func; break;
    }
}

//所有按键注销
void Key_AllDelete()
{
    uint8_t i;

    for(i=0; i<KEY_NUMS; i++){
        BTN_N_Down[i] = Blank_Func;
    }
}

//按键扫描
void Key_Scan()
{
    static uint8_t keyFlag[KEY_NUMS] = {0};     //防止按键重复检测  0-未检测  1-已检测
    static uint8_t keyFlag2[KEY_NUMS] = {0};    //防止功能重复执行  0-未执行  1-已执行
    static uint8_t keyCnt[KEY_NUMS] = {0};      //按键消抖，连续检测10次1或0改变按键状态
    uint8_t tmp[KEY_NUMS];
    uint8_t i;

    /* 读取按键状态 */
    i=0; tmp[i] = BTNL_L;
    i++; tmp[i] = BTNL_R;
    i++; tmp[i] = BTNL_U;
    i++; tmp[i] = BTNL_D;
    i++; tmp[i] = BTNL_M;
    i++; tmp[i] = BTNR_L;
    i++; tmp[i] = BTNR_R;
    i++; tmp[i] = BTNR_U;
    i++; tmp[i] = BTNR_D;
    i++; tmp[i] = BTNR_M;

    for(i=0; i<KEY_NUMS; i++){
        if(tmp[i]==keyLastSta[i]){
            keyCnt[i]++;
        }else{
            keyCnt[i] = 0;
        }
        if(keyCnt[i]==KEY_DECTECT_TIMES){
            keyState[i] = keyLastSta[i]; keyCnt[i] = 0;
        }
        keyLastSta[i] = tmp[i];
    }
    
    /* 检测按键 */
    for(i=0; i<KEY_NUMS; i++){
        if(keyState[i]==KEY_DOWN && keyFlag[i]==0){
            keyFlag[i] = 1;
        }
    }
    
    /* 实现按键功能 */
    for(i=0; i<KEY_NUMS; i++){
        if(keyFlag[i]==1 && keyFlag2[i]==0){
            btnFlag[i] = KEY_DOWN; keyFlag2[i] = 1;
        }
    }
    
    /* 复位keyFlag、keyFlag2 */
    for(i=0; i<KEY_NUMS; i++){
        if(keyState[i]==KEY_UP && keyFlag[i]==1){
            keyFlag[i] = 0;
        }
        if(keyFlag[i]==0 && keyFlag2[i]==1){
            keyFlag2[i] = 0;
        }
    }
}

// 按键驱动
void Key_Driver()
{
    uint8_t i;
    /* 实现按键功能 */
    for(i=0; i<KEY_NUMS; i++){
        if(btnFlag[i]==KEY_DOWN){
            btnFlag[i] = KEY_UP;
            BTN_N_Down[i]();
        }
    }
}
