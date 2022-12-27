#include "timer/timer.h"
#include "oled/oled_api.h"
#include "key/key.h"

//IIC通讯速率
extern uint32_t iic_rate;   //IIC速率 - bytes/s
extern uint8_t iic_ok;      //IIC计算速率结束 - 1
extern uint8_t oledIICRate; //是否在Oled上显示IIC通信速率

static uint8_t T0RH = 0;    //T0重载值高字节
static uint8_t T0RL = 0;    //T0重载值低字节

/**
 * 定时器0初始化
 */
void Timer0_Init(uint16_t ms)
{
    uint32_t tmp;

    tmp = 11059200 / 12;        //计算所需的计数值（ms换算成s）
    tmp = (tmp * ms) / 1000;    
    tmp = 65536 - tmp;          //计算定时器重载值
    //tmp = tmp + 13;           //补偿中断响应延时造成的误差
    T0RH = (uint8_t)(tmp>>8);   //定时器重载值拆分为高低字节
    T0RL = (uint8_t)tmp;
    TMOD &= 0xF0;       //清零T0控制位
    TMOD |= 0x01;       //配置T0为模式1
    TH0 = T0RH;         //加载T0重载值
    TL0 = T0RL;
    ET0 = 1;            //使能T0中断
    TR0 = 1;            //启动T0
}

/*T0中断服务函数*/    
void InterruptTimer0() interrupt 1
{
    static uint16_t cnt = 0;

    TH0 = T0RH;     //重载初值
    TL0 = T0RL;

    Key_Scan();
    if(oledIICRate){
        if(cnt==100){
            cnt = 0;
            iic_ok = 1;
        }else{
            cnt++;
        }    
    }
}
