#include "musicplayer/mp_uart_driver.h"
#include "oled/oled_api.h"

#define S2RI  0x01          //S2CON.0
#define S2TI  0x02          //S2CON.1
#define S2RB8 0x04          //S2CON.2
#define S2TB8 0x08          //S2CON.3

//Mini MP3 Player当前状态结构体
extern sMPSta MPSta;

// 接收完成标志  1-接收完成  0-接收未完成
uint8_t recOkFlag = 0;

static uint8_t busy = 0;        // 串口是否在发送数据
static uint8_t mpRecFrame[MP_REC_FRAME_LEN];    // MP接收数据帧
static uint8_t mpSendFrame[MP_SEND_FRAME_LEN];  // MP发送数据帧
static uint8_t recbyte = MPREC_START; // 接收到数据帧的第几位
static uint8_t recOverFlag = 0;     // 单片机接收一帧数据  1-接收完成
static uint8_t recErrorFlag = 0;    // MP通信数据错误(40H)  错误码见MP3-TF-16P使用说明书


/*********************************************
 *         以下为 Mini MP3 Player 串口驱动
 *
 *
 *********************************************/
/*------------------------------------------驱动部分------------------------------------------*/
/*********************************************/
// Mini MP3 Player串口初始化  baud-9600
void MP_UartDriverInit()
{
    S2CON = 0x50;                   //8-bit variable UART
    BRT = -(MAIN_FOSC/32/9600);     //Set auto-reload vaule of baudrate generator
    AUXR = 0x14;                    //Baudrate generator work in 1T mode
    IE2 = 0x01;                     //Enable UART2 interrupt
    EA = 1;                         //Open master interrupt switch
}
/*********************************************/
// MP串口发送数据
static void MP_SendByte(uint8_t dat)
{
    while (busy);           //Wait for the completion of the previous data is sent
    S2BUF = dat;            //Send data to UART2 buffer
    busy = 1;
}
/*********************************************/
// MP串口发送字符串
void MP_SendString(uint8_t* str)
{
    while (*str)              //Check the end of the string
    {
        MP_SendByte(*str++);  //Send current char and increment string ptr
    }
}
/*------------------------------------------接收处理部分------------------------------------------*/
/*********************************************/
// MP串口中断处理函数
// 接收存储收到的MP数据帧
void MP_UartInterrupt() interrupt 8
{
    uint8_t recDat;             // 接收的1位数据

    if (S2CON & S2RI)
    {
        S2CON &= ~S2RI;     //Clear receive interrupt flag
        /*********************************************/
        recDat = S2BUF;
        switch(recbyte)
        {
            case MPREC_START:       if(0x7E==recDat)     {recbyte=MPREC_VER;}      break;
            case MPREC_VER:         mpRecFrame[0]=recDat; recbyte=MPREC_LEN;       break;
            case MPREC_LEN:         mpRecFrame[1]=recDat; recbyte=MPREC_CMD;       break;
            case MPREC_CMD:         mpRecFrame[2]=recDat; recbyte=MPREC_FEEDBACK;  break;
            case MPREC_FEEDBACK:    mpRecFrame[3]=recDat; recbyte=MPREC_DATAH;     break;
            case MPREC_DATAH:       mpRecFrame[4]=recDat; recbyte=MPREC_DATAL;     break;
            case MPREC_DATAL:       mpRecFrame[5]=recDat; recbyte=MPREC_CHECKSUMH; break;
            case MPREC_CHECKSUMH:   mpRecFrame[6]=recDat; recbyte=MPREC_CHECKSUML; break;
            case MPREC_CHECKSUML:   mpRecFrame[7]=recDat; recbyte=MPREC_END;       break;
            case MPREC_END:         if(0xEF==recDat)     {recbyte=MPREC_START; recOverFlag=1;} break;
            default: break;
        }
        /*********************************************/
    }
    if (S2CON & S2TI)
    {
        S2CON &= ~S2TI;     //Clear transmit interrupt flag
        busy = 0;           //Clear transmit busy flag
    }
}
/*********************************************/
// 对从MP接收的数据帧进行处理
static void MP_MsgProcess()
{
    switch(mpRecFrame[MPREC_CMD-1])
    {
        case MPCMD_STATUS:  MPSta.status = mpRecFrame[MPREC_DATAL-1];   break;
        case MPCMD_VOL:     MPSta.vol = mpRecFrame[MPREC_DATAL-1]; break;
        case MPCMD_EQ:      MPSta.eq = mpRecFrame[MPREC_DATAL-1];       break;
        case MPCMD_PLAYMODE:MPSta.playmode = mpRecFrame[MPREC_DATAL-1]; break;
        /* 错误 */
        case MPCMD_ERR: recErrorFlag = 1; recbyte = MPREC_START; break;
    }
}
// 打印MP返回的命令和数据
static void MP_RecPrint()
{
    o_dev(Oled2);
    o_str(0,0,"REC SUCCESS",ASCII_H16W8);
    o_str(0,16,"CMD: ",ASCII_H16W8);
    o_char(40,16,mpRecFrame[MPREC_CMD-1]/10+'0',ASCII_H16W8);
    o_char(48,16,mpRecFrame[MPREC_CMD-1]%10+'0',ASCII_H16W8);
    o_str(0,32,"DATAH:",ASCII_H16W8);
    o_char(48,32,mpRecFrame[MPREC_DATAH-1]/10+'0',ASCII_H16W8);
    o_char(56,32,mpRecFrame[MPREC_DATAH-1]%10+'0',ASCII_H16W8);
    o_str(0,48,"DATAL:",ASCII_H16W8);
    o_char(48,48,mpRecFrame[MPREC_DATAL-1]/10+'0',ASCII_H16W8);
    o_char(56,48,mpRecFrame[MPREC_DATAL-1]%10+'0',ASCII_H16W8);
    if(recErrorFlag==1){
        recErrorFlag = 0;
        // // 出错 打印错误码
        // o_dev(Oled2); o_partclr(0,0,48,16);
        // o_str(0,0,"ERROR",ASCII_H16W8);
        // o_str(0,16,"CODE: ",ASCII_H16W8);
        // o_char(48,16,mpRecFrame[MPREC_DATAL-1]%10+'0',ASCII_H16W8);
    }
}

// 对从MP接收的数据帧进行校验并处理
void MP_RecProcess()
{
    uint8_t i;
    uint16_t xorsum=0, xorsum1=0;

    if(recOverFlag){ //一帧数据接收完毕
        recOverFlag = 0 ;
        for(i=0; i<mpRecFrame[1]; i++){
            xorsum  = xorsum + mpRecFrame[i];
        }
        xorsum1 = (uint16_t)(mpRecFrame[i]<<8) | mpRecFrame[i+1];
        xorsum  = xorsum + xorsum1;
        if(xorsum){
            recErrorFlag = 1;  // 校验错误
        }
        // 处理接收命令
        MP_MsgProcess();
        #if (MP_PRINT_DEBUG==1)
            // 打印命令和数据
            MP_RecPrint();
        #endif
    }
}
/*********************************************/
static void DoSum(uint8_t* str, uint8_t len)
{
    uint16_t xorsum = 0;
    uint8_t i;

    for(i=0; i<len; i++){
        xorsum = xorsum + str[i];
    }
    xorsum     = 0 - xorsum;
    *(str+i)   = (uint8_t)(xorsum>>8);
    *(str+i+1) = (uint8_t)(xorsum&0x00ff);
}
/*********************************************/
// MP串口发送含命令的数据帧
void MP_SendCmd(uint8_t cmd, uint8_t feedback, uint16_t dat)
{
    uint8_t i;

    MP_SendByte(0x7E);                  // 开始
    mpSendFrame[0] = 0xFF;              // 版本号
    mpSendFrame[1] = 0x06;              // 长度
    mpSendFrame[2] = cmd;               // 控制指令
    mpSendFrame[3] = feedback;          // 是否需要反馈
    mpSendFrame[4] = (uint8_t)(dat>>8); // datah
    mpSendFrame[5] = (uint8_t)(dat);    // datal
    DoSum(&mpSendFrame[0],6);           // 校验
    for(i=0; i<8; i++){                 // 发送此帧数据
        MP_SendByte(mpSendFrame[i]);
    }
    MP_SendByte(0xEF);                  // 结束
}
/*********************************************/

/*********************************************/


