#include "oled/oled_warning.h"
//#include "led/led.h"

/*********************************************
 *              以下为Oled警告设置函数
 *              
 *
 *********************************************/
/*********************************************/
//Oled警告设置
#if (OLED_ERROR_ALARM == 1)
void OLED_SET_WARNING(uint8_t wcode)
{
    switch(wcode){
        case IIC_ERROR:
            LED = 1;
        break;
        case DRAW_OUT_OF_RANGE:
            
        break;
        case FONT_NOT_EXIST:
            
        break;
        case BMP_NOT_EXIST:
            
        break;
        case NUM_TYPE_NOT_EXIST:
            
        break;
        case ZH_CHAR_NOT_EXIST:
            
        break;
    }
}
void OLED_RESET_WARNING(uint8_t wcode)
{
    switch(wcode){
        case IIC_ERROR:
            LED = 0;
        break;
        case DRAW_OUT_OF_RANGE:
            
        break;
        case FONT_NOT_EXIST:
            
        break;
        case BMP_NOT_EXIST:
            
        break;
        case NUM_TYPE_NOT_EXIST:
            
        break;
        case ZH_CHAR_NOT_EXIST:
            
        break;
    }
}
#else
void OLED_SET_WARNING(uint8_t wcode){wcode=wcode;}
void OLED_RESET_WARNING(uint8_t wcode){wcode=wcode;}
#endif
/*********************************************/
