#include "oled/oled_driver.h"
#include "oled/oled_warning.h"


extern uint32_t iic_rate;
extern uint8_t oledIICRate;

//Oled设备
eOledDevice device = Oled1;

/*********************************************
 *              以下为Oled SSD1306驱动
 *
 *
 *********************************************/
/*********************************************/
static void Oled_I2C_Start()
{
    switch(device)
    {
        case Oled1:
            OLED_SDA_H;
            OLED_SCL_H;
            delay_1us();
            OLED_SDA_L;
            delay_1us();
            OLED_SCL_L;
        break;
        case Oled2:
            OLED2_SDA_H;
            OLED2_SCL_H;
            delay_1us();
            OLED2_SDA_L;
            delay_1us();
            OLED2_SCL_L;
        break;
    }
    
}
/*********************************************/
static void Oled_I2C_Stop()
{
    switch(device)
    {
        case Oled1:
            OLED_SCL_L;
            OLED_SDA_L;
            delay_1us();
            OLED_SCL_H;
            delay_1us();
            OLED_SDA_H;
        break;
        case Oled2:
            OLED2_SCL_L;
            OLED2_SDA_L;
            delay_1us();
            OLED2_SCL_H;
            delay_1us();
            OLED2_SDA_H;
        break;
    }

}
/*********************************************/
//返回值：1，接收应答失败
//        0，接收应答成功
static uint8_t Oled_I2C_Wait_ACK()
{
    uint8_t t = 0;
    
    switch(device)
    {
        case Oled1:
            OLED_SDA_H;
            delay_1us();
            OLED_SCL_H;
            delay_1us();
            while(OLED_SDA_Read)
            {
                t++;
                if(t > 250)
                {
                    OLED_SET_WARNING(IIC_ERROR); // 检测IIC通讯是否正常
                    Oled_I2C_Stop();
                    return 1;
                }
            }
            OLED_RESET_WARNING(IIC_ERROR);
            OLED_SCL_L;
            return 0;
        break;
        case Oled2:
            OLED2_SDA_H;
            delay_1us();
            OLED2_SCL_H;
            delay_1us();
            while(OLED2_SDA_Read)
            {
                t++;
                if(t > 250)
                {
                    OLED_SET_WARNING(IIC_ERROR); // 检测IIC通讯是否正常
                    Oled_I2C_Stop();
                    return 1;
                }
            }
            OLED_RESET_WARNING(IIC_ERROR);
            OLED2_SCL_L;
            return 0;
        break;
        default:
            return 1;
        break;
    }
    
}
/*********************************************/
//产生ACK应答
static void Oled_I2C_ACK()
{
    switch(device)
    {
        case Oled1:
            OLED_SCL_L;
            OLED_SDA_L;
            delay_1us(); delay_1us();
            OLED_SCL_H;
            delay_1us(); delay_1us();
            OLED_SCL_L;
        break;
        case Oled2:
            OLED2_SCL_L;
            OLED2_SDA_L;
            delay_1us(); delay_1us();
            OLED2_SCL_H;
            delay_1us(); delay_1us();
            OLED2_SCL_L;
        break;
    }
    
}
/*********************************************/
//不产生ACK应答  
static void Oled_I2C_NACK()
{
    switch(device)
    {
        case Oled1:
            OLED_SCL_L;
            OLED_SDA_H;
            delay_1us();
            OLED_SCL_H;
            delay_1us();
            OLED_SCL_L;
        break;
        case Oled2:
            OLED2_SCL_L;
            OLED2_SDA_H;
            delay_1us();
            OLED2_SCL_H;
            delay_1us();
            OLED2_SCL_L;
        break;
    }

}
/*********************************************/
//I2C发送一个字节
static void Oled_I2C_Send_Byte(uint8_t byte)
{
    uint8_t i;
    
    switch(device)
    {
        case Oled1:
            OLED_SCL_L;
            for(i = 0; i < 8; i++)
            {
                if(((byte&0x80)>>7)==1)
                    OLED_SDA_H;
                else
                    OLED_SDA_L;
                byte <<= 1;
                delay_1us();
                OLED_SCL_H;
                delay_1us();
                OLED_SCL_L;
                delay_1us();
            }
            if(oledIICRate){
                iic_rate++;
            }
        break;
        case Oled2:
            OLED2_SCL_L;
            for(i = 0; i < 8; i++)
            {
                if(((byte&0x80)>>7)==1)
                    OLED2_SDA_H;
                else
                    OLED2_SDA_L;
                byte <<= 1;
                delay_1us();
                OLED2_SCL_H;
                delay_1us();
                OLED2_SCL_L;
                delay_1us();
            }
            if(oledIICRate){
                iic_rate++;
            }
        break;
    }
    
}
/*********************************************/
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
static uint8_t Oled_I2C_Read_Byte(uint8_t ack)
{
    uint8_t i,receive = 0;
    
    switch(device)
    {
        case Oled1:
            for(i = 0; i < 8; i++)
            {
                OLED_SCL_L;
                delay_1us();
                OLED_SCL_H;
                receive <<= 1;
                if(OLED_SDA_Read) receive++;
                delay_1us();
            }
            if (!ack) Oled_I2C_NACK();      //发送nACK
            else Oled_I2C_ACK();            //发送ACK 

            return receive;
        break;
        case Oled2:
            for(i = 0; i < 8; i++)
            {
                OLED2_SCL_L;
                delay_1us();
                OLED2_SCL_H;
                receive <<= 1;
                if(OLED2_SDA_Read) receive++;
                delay_1us();
            }
            if (!ack) Oled_I2C_NACK();      //发送nACK
            else Oled_I2C_ACK();            //发送ACK 
            
            return receive;
        break;
        default:
            return 0;
        break;
    }
    
}
/*********************************************/
//Oled写入命令
void Oled_I2C_Write_Cmd(uint8_t cmd)
{
    EA = 0;
    Oled_I2C_Start();
    Oled_I2C_Send_Byte(0x78);   // Slave address
    Oled_I2C_Wait_ACK();
    Oled_I2C_Send_Byte(0x00);   // write command
    Oled_I2C_Wait_ACK();
    Oled_I2C_Send_Byte(cmd);
    Oled_I2C_Wait_ACK();
    Oled_I2C_Stop();
    EA = 1;
}
/*********************************************/
//Oled写入数据
void Oled_I2C_Write_Dat(uint8_t dat)
{
    EA = 0;
    Oled_I2C_Start();
    Oled_I2C_Send_Byte(0x78);   // Slave address
    Oled_I2C_Wait_ACK();
    Oled_I2C_Send_Byte(0x40);   // write data
    Oled_I2C_Wait_ACK();
    Oled_I2C_Send_Byte(dat);
    Oled_I2C_Wait_ACK();
    Oled_I2C_Stop();
    EA = 1;
}

