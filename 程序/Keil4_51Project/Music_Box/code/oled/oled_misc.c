#include "oled/oled_misc.h"


/*********************************************
 *              以下为Oled杂项函数
 *              提供一些辅助函数
 *
 *********************************************/
/*********************************************/
//整型数转字符串
//str - 存储转换后的字符串，注意申请空间足够
void int_to_str(uint8_t *str, int32_t num)
{
    uint8_t index = 0;
    uint8_t conv_i = 0;
    uint8_t tmp;

    //得到翻转的转换后字符串
    while(num>0)
    {
        str[index++] = num%10 + '0';
        num /= 10;
    }  
    str[index--] = '\0';
    //翻转回来
    while(conv_i < index)
    {
        tmp = str[conv_i];
        str[conv_i] = str[index];
        str[index] = tmp;
        conv_i++; index--;
    }
}
/*********************************************/
//浮点数转字符串
//str - 存储转换后的字符串，注意申请空间足够
void float_to_str(uint8_t *str, double num)
{
    uint32_t high;      //浮点数整数部分
    double low;         //浮点数小数部分
    uint8_t index = 0;
    uint8_t int_i;
    uint8_t conv_i = 0;
    uint8_t tmp;
    uint8_t bits = DECIMAL_PLACES;  //小数位数
    uint8_t df;         //小数部分

    high = (uint32_t)num;
    low = num - high;
    //得到整数部分字符
    while(high>0)
    {
        str[index++] = (uint8_t)high%10 + '0';
        high /= 10;
    }
    int_i = index - 1;
    //整数部分翻转回来
    while(conv_i < int_i)
    {
        tmp = str[conv_i];
        str[conv_i] = str[int_i];
        str[int_i] = tmp;
        conv_i++; int_i--;
    }
    //小数点
    str[index++] = '.';
    //小数部分
    while(bits--)
    {
        df = low*10;    //无需强转，自动转换
        str[index++] = df + '0';
        low = low*10.0 - df;
    }
    str[index] = '\0';
}

