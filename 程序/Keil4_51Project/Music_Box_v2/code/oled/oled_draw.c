#include "oled/oled_draw.h"
#include "oled/oled_driver.h"
#include "oled/oled_font.h"
#include "oled/oled_bmp.h"
#include "oled/oled_misc.h"
#include "oled/oled_warning.h"

//Oled设备
extern eOledDevice device;
//字库参数结构体
extern sFontPara code fontPara[OLED_FONT_NUM];
//非ASCII字库索引结构体
extern sNonAsciiIndex code nonAsciiIndex[OLED_NON_ASCII_NUM];
//BMP参数结构体
extern sBmpPara code bmpPara[OLED_BMP_NUM];

//IIC通讯速率
uint8_t iic_ok = 1;         //IIC计算速率结束 - 1
uint8_t oledIICRate = 0;    //是否在Oled上显示IIC通信速率
uint32_t iic_rate = 0;      //IIC速率 - bytes/s

//绘画字符是否是字库外特殊字符
static uint8_t oledExtraChar = 0;

//是否在绘画前检查是否超出范围
static uint8_t oledCheckRange = OLED_CHECK_RANGE;

//是否启用Oled调试绘画，在绘画过程中加入延时方便观察
static uint16_t oledDebugDelay = OLED_DEBUG_DELAY;  //每绘制 1byte 延时时间
#if (OLED_DEBUG_DRAW == 1)
    static uint8_t oledIsDebug = 1;
#else
    static uint8_t oledIsDebug = 0;
#endif

//是否在绘制字符串时加入边框
static uint8_t oledUseEdge = 0;

/*********************************************
 *              以下为Oled画图函数
 *              无屏幕缓存版本
 *
 *********************************************/
//-------------------------------------------------------------------------
/**************************************************************************
                    //坐标说明
                     x(0~127)
             ------------------>
            |
            |
            |y(0~63)
            |
            |
            v

//--------------------------------------------------------------------------
**************************************************************************/
/*********************************************/
//选择Oled设备
void Oled_SetDevice(eOledDevice dev)
{
    device = dev;
}
//Oled开启显示
void Oled_DisplayOn()
{
    Oled_I2C_Write_Cmd(0x8D);   // Set DCDC
    Oled_I2C_Write_Cmd(0x14);   // DCDC On
    Oled_I2C_Write_Cmd(0xAF);   // Display On
}
/*********************************************/
//Oled关闭显示
void Oled_DispalyOff()
{
    Oled_I2C_Write_Cmd(0x8D);   // Set DCDC
    Oled_I2C_Write_Cmd(0x10);   // DCDC Off
    Oled_I2C_Write_Cmd(0xAE);   // Display Off
}
/*********************************************/
//Oled正/反色显示
//color: 1 - 正色  0 - 反色
void Oled_DisplayColor(uint8_t color)
{
    if(color){
        Oled_I2C_Write_Cmd(0xA6);   // 正色
    }else{
        Oled_I2C_Write_Cmd(0xA7);   // 反色
    }
}
/*********************************************/
//Oled正/反转显示
//dir: 1 - 正向  0 - 反向
void Oled_DisplayDir(uint8_t dir)
{
    if(dir){
        Oled_I2C_Write_Cmd(0xC8);   // 正向
        Oled_I2C_Write_Cmd(0xA1);
    }else{
        Oled_I2C_Write_Cmd(0xC0);   // 反向
        Oled_I2C_Write_Cmd(0xA0);
    }
}
/*********************************************/
//Oled是否启用调试绘画
//debug: 1 - 启用  0 - 停用
void Oled_SetDebugDraw(uint8_t debug)
{
    oledIsDebug = debug;
}
/*********************************************/
//Oled设置调试绘画的延时时间
void Oled_SetDebugDelay(uint16_t ms)
{
    oledDebugDelay = ms;
}
/*********************************************/
//Oled清屏
void Oled_Clear()
{
    uint8_t page,col;

    for(page=0; page<8; page++)
    {
        Oled_I2C_Write_Cmd(0xB0+page);
        Oled_I2C_Write_Cmd(0x00);
        Oled_I2C_Write_Cmd(0x10);
        for(col=0; col<128; col++)
        {
            Oled_I2C_Write_Dat(0x00);   // Set all RAMs as 0x00
        }
    }
}
/*********************************************/
//Oled部分区域清屏
void Oled_ParticalClear(uint8_t start_x, uint8_t start_y,
                        uint8_t end_x,   uint8_t end_y)
{
    uint8_t page,col;
    uint8_t start_page = start_y/OLED_PAGE_NUM;
    uint8_t end_page = end_y/OLED_PAGE_NUM;

    if(end_page>7){
        end_page = 7;
    }
    for(page=start_page; page<=end_page; page++)
    {
        Oled_I2C_Write_Cmd(0xB0+page);
        Oled_I2C_Write_Cmd(start_x&0x0F);
        Oled_I2C_Write_Cmd(0x10|((start_x&0xF0)>>4));
        for(col=start_x; col<=end_x; col++)
        {
            Oled_I2C_Write_Dat(0x00);   // Set all RAMs as 0x00
            if(oledIICRate && iic_ok){
                return;
            }
        }
    }
}
/*********************************************/
//Oled延时函数
void Oled_DelayMs(uint16_t ms)
{
    do{
        delay_1ms();
    }while(ms--);
}
/*********************************************/
//Oled page和col位置设置
static void Oled_SetPos(uint8_t page, uint8_t col)
{
    Oled_I2C_Write_Cmd(0xB0+page);
    Oled_I2C_Write_Cmd( ((col&0xF0)>>4)|0x10 );
    Oled_I2C_Write_Cmd(  (col&0x0F) );
}
/*********************************************/
//Oled设置是否使用边框显示字
//edge: 1 - 启用  0 - 停用
void Oled_UseStringEdge(uint8_t edge)
{
    oledUseEdge = edge;
}
/*********************************************/
//Oled画一条垂直线
static void Oled_DrawVerLine(uint8_t x, uint8_t start_y, uint8_t end_y)
{
    uint8_t page;
    uint8_t s_page;     //绘制起始页
    uint8_t e_page;     //绘制结束页
    uint8_t s_offset;   //起始坐标偏移
    uint8_t e_offset;   //结束坐标偏移
    uint8_t ey_out=0;   //end_y超出范围置1
    uint8_t dat;

    if(start_y>end_y){
        dat = start_y;
        start_y = end_y;
        end_y = dat;
    }
    //判断是否超出范围
    if(x>=OLED_COLUMN_NUM || start_y>=OLED_ROW_NUM){
        OLED_SET_WARNING(DRAW_OUT_OF_RANGE);  //绘画超出范围警告
        return;
    }
    s_page = start_y / OLED_PAGE_NUM;
    e_page = end_y / OLED_PAGE_NUM;
    if(e_page>7){
        ey_out = 1; e_page = 7;
    }
    s_offset = start_y % OLED_PAGE_NUM;
    e_offset = end_y % OLED_PAGE_NUM;
    if(s_page==e_page){  //没跨页
        dat = ey_out ? (0xFF<<s_offset) : ((0xFF<<s_offset)&(0xFF>>(7-e_offset)));
        Oled_SetPos(s_page, x);
        Oled_I2C_Write_Dat(dat);
    }else{
        for(page=s_page; page<=e_page; page++){
            if(page==s_page){
                dat = 0xFF<<s_offset;
            }else if(page==e_page){
                dat = ey_out ? 0xFF : (0xFF>>(7-e_offset));
            }else{
                dat = 0xFF;
            }
            Oled_SetPos(page, x);
            Oled_I2C_Write_Dat(dat);
        }
    }
}
/*********************************************/
//Oled画一条直线
void Oled_DrawLine(uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y)
{
    uint8_t cur_y;      //绘制点当前y轴坐标
    uint8_t next_y;     //下个绘制点y坐标
    uint8_t cur_x;      //绘制点当前x轴坐标
    float   slope;      //斜率
    
    //判断是否超出范围
    if(start_x>=OLED_COLUMN_NUM || end_x>=OLED_COLUMN_NUM ||
       start_y>=OLED_ROW_NUM    || end_y>=OLED_ROW_NUM){
        OLED_SET_WARNING(DRAW_OUT_OF_RANGE);  //绘画超出范围警告
    }
    
    if(start_x==end_x){     //垂直线直接画
        Oled_DrawVerLine(start_x, start_y, end_y);
    }else{
        //计算绘图参数
        slope = (float)(end_y-start_y)/(end_x-start_x);
        cur_y = start_y;
        next_y = start_y+slope;
        Oled_DrawVerLine(start_x, cur_y, (cur_y+next_y)/2);
        Oled_DrawVerLine(start_x+1, (cur_y+next_y)/2, next_y);
        for(cur_x=start_x+1; cur_x<=end_x; cur_x+=2){
            cur_y = next_y;
            next_y = start_y + (cur_x-start_x)*slope;
            Oled_DrawVerLine(cur_x, cur_y, (cur_y+next_y)/2);
            Oled_DrawVerLine(cur_x+1, (cur_y+next_y)/2, next_y);
        }
    }
}
/*********************************************/
//Oled在指定位置显示一个ASCII字符
// ch - 字符
// font - 字体
void Oled_ShowChar(uint8_t x, uint8_t y, uint8_t ch, eFontName font)
{
    uint8_t ch_h, ch_w, ch_disw;      //字体的高、宽、显示宽度(不让字符连一起)
    uint16_t ch_size;                 //一个字符占用字节数
    uint8_t *ch_loc;                  //字符在字库中位置
    uint16_t fresh_size;              //Oled需要刷新的字节数
    uint8_t page = y/OLED_PAGE_NUM;   //字符起始页
    uint8_t col = x;                  //字符起始列
    uint8_t offset = y%OLED_PAGE_NUM; //字符相对页偏移
    uint8_t b_offset;                 //字符底相对页偏移
    uint8_t prev_byte=0, next_byte=0; //防止页重叠的字节拼接
    uint8_t edge_byteU=0x00, edge_byteD=0x00;  //边框显示
    uint8_t cross_page;          //字符是否跨页
    uint8_t joint_times;         //拼接次数
    uint8_t dat;
    uint8_t page_cover;          //字符横跨的页数
    uint8_t *font_p;             //字库指针
    uint8_t font_i;              //搜索字库索引
    uint8_t ch_i;                //字符的索引值
    uint16_t col_i = 0;          //当前列索引
    uint8_t col_cur = 0;         //当前列(相对起始位置)

    //设置绘图参数
    for(font_i=0; font_i<OLED_FONT_NUM; font_i++){
        if(font == fontPara[font_i].font){
            ch_h = fontPara[font_i].ch_h;
            ch_w = fontPara[font_i].ch_w;
            page_cover = fontPara[font_i].page_cover;
            font_p = fontPara[font_i].font_p;
            if(fontPara[font_i].font>=50){  //汉字字库
                ch_i = ch;
            }else{
                ch_i = ch-' ';   //ASCII码字库
            }
            break;
        }
    }
    if(font_i == OLED_FONT_NUM){
        OLED_SET_WARNING(FONT_NOT_EXIST); //字体不存在警告
        return;
    }
    //判断是否超出范围
    if(oledCheckRange && (x>=OLED_COLUMN_NUM-ch_w || y>=OLED_ROW_NUM-ch_h)) {
        OLED_SET_WARNING(DRAW_OUT_OF_RANGE);  //绘画超出范围警告
        if(x>OLED_COLUMN_NUM || y>OLED_ROW_NUM){     //全部字符移出屏幕外
            return;
        }
    }
    cross_page = ((y+ch_h)/OLED_PAGE_NUM == page) ? 0 : 1;
    if(oledUseEdge){    //画上下边框
        b_offset = (y+ch_h)%OLED_PAGE_NUM;
        edge_byteU |= 0x01<<offset;
        edge_byteD |= 0x01<<b_offset;
    }
    ch_disw = ch_w+OLED_CHAR_SPACE;
    ch_size = (ch_h/8+((ch_h%8)?1:0))*ch_w;
    ch_loc = font_p+(ch_i*ch_size);
    fresh_size = ch_disw*page_cover;
    joint_times = page_cover;
    //绘图
    for(col_i=0; col_i<fresh_size; col_i++){
        if(oledIsDebug){
            Oled_DelayMs(oledDebugDelay);
        }
        if((col_i%ch_disw)==0){
            col_cur = 0;
            if(page<8) Oled_SetPos(page++, col);
            else       return;
            if(col_i!=0) joint_times--;
        }
        if(col_cur<ch_w){
            if(col_cur+x>OLED_COLUMN_NUM){
                continue;
            }
            if(page_cover==2){
                if(joint_times==2){
                    next_byte = *(ch_loc+col_cur);
                    dat = (next_byte<<offset) | edge_byteU;        //Mode2-1
                    if(!cross_page){
                        dat |= edge_byteD;   //Mode1
                    }
                }else if(joint_times==1 && cross_page){
                    prev_byte = *(ch_loc+col_cur);
                    dat = (prev_byte>>(8-offset)) | edge_byteD;            //Mode2-2
                }
            }else{  //page_cover>=3
                if(joint_times==page_cover){
                    next_byte = *(ch_loc+col_cur);
                    dat = (next_byte<<offset) | edge_byteU;
                }else if(joint_times>=2){
                    prev_byte = *(ch_loc+col_cur+ch_w*(page_cover-joint_times-1));
                    next_byte = *(ch_loc+col_cur+ch_w*(page_cover-joint_times));
                    dat = (prev_byte>>(8-offset)) | (next_byte<<offset);        //Mode2-1
                }else if(joint_times==1 && cross_page){
                    prev_byte = *(ch_loc+col_cur+ch_w*(page_cover-joint_times-1));
                    dat = (prev_byte>>(8-offset)) | edge_byteD;            //Mode2-2
                }
            }
        }else{
            if(page_cover==2){
                if(joint_times==2){
                    dat = edge_byteU;
                    if(!cross_page){
                        dat |= edge_byteD;
                    }
                }else if(joint_times==1 && cross_page){
                    dat |= edge_byteD;
                }
            }else{  //page_cover>=3
                if(joint_times==page_cover){
                    dat = edge_byteU;
                }else if(joint_times>=2){
                    dat = 0x00;
                }else if(joint_times==1){
                    dat = edge_byteD;
                }
            }
        }
        col_cur++;
        if(joint_times==1 && cross_page==0){    //最后一页为空，不再写
            continue;
        }
        Oled_I2C_Write_Dat(dat);
    }
}
/*********************************************/
//Oled在指定位置显示一个汉字字符(或其他UTF-8编码字符)  -注意: 只能解析UTF-8编码汉字
// zh - 汉字字符
// font - 字体
void Oled_ShowCharZh(uint8_t x, uint8_t y, uint8_t* ch, eFontName font)
{
    uint8_t ch_zh_i;      //中文字符索引
    uint8_t font_i;       //搜索字库索引
    uint8_t han[4];       //UTF-8编码由1~4字节组成
    uint8_t utf8_i=0;     //UTF-8编码字节索引
    uint8_t ch_len=0;     //字符所占字节数
    uint8_t str_len=0;    //已扫描字节数
    uint8_t same;         //字符匹配为1

    for(font_i=0; font_i<OLED_NON_ASCII_NUM; font_i++){
        if(font == nonAsciiIndex[font_i].font){
            break;
        }
    }
    if(font_i == OLED_NON_ASCII_NUM){
        OLED_SET_WARNING(FONT_NOT_EXIST); //字体不存在警告
        return;
    }
    //绘制汉字
    while(ch[utf8_i+1]!='\0') //UTF-8字符最后一个字节
    {
        han[utf8_i] = ch[utf8_i]; utf8_i++;
    }
    ch_len = utf8_i;
    for(ch_zh_i=0; ch_zh_i<nonAsciiIndex[font_i].ch_num; ch_zh_i++){
        same = 1;
        for(utf8_i=0; utf8_i<=4; utf8_i++){
            if(han[utf8_i]==nonAsciiIndex[font_i].ch_all[str_len+utf8_i]){
                if(utf8_i+1==ch_len && same){
                    Oled_ShowChar(x, y, ch_zh_i, font);
                    return;
                }
            }else{
                same = 0;
            }
            if(nonAsciiIndex[font_i].ch_all[str_len+utf8_i]=='@' ||
               nonAsciiIndex[font_i].ch_all[str_len+utf8_i]=='\0'){
                str_len += utf8_i+1;
                break;
            }
        }
    }
    OLED_SET_WARNING(ZH_CHAR_NOT_EXIST); //汉字不存在警告
}
/*********************************************/
//Oled在指定位置显示一个ASCII码字符串
// str - 字符串
// font - 字体
void Oled_ShowString(uint8_t x, uint8_t y, uint8_t* str, eFontName font)
{
    uint8_t index = 0;
    uint8_t ch_h, ch_w;    //字体的高、宽
    uint8_t ch_disw;       //字体显示宽度
    uint8_t font_i;        //搜索字库索引

    for(font_i=0; font_i<OLED_FONT_NUM; font_i++){
        if(font == fontPara[font_i].font){
            ch_h = fontPara[font_i].ch_h;
            ch_w = fontPara[font_i].ch_w;
        }
    }
    if(font_i == OLED_FONT_NUM+1){
        OLED_SET_WARNING(FONT_NOT_EXIST); //字体不存在警告
        return;
    }
    ch_disw = ch_w+OLED_CHAR_SPACE;
    if (oledUseEdge){   //左边框
        Oled_DrawLine(x-1, y, x-1, y+ch_h);
    }
    while(str[index]!='\0')
    {
        Oled_ShowChar(x, y, str[index++], font);
        x += ch_disw;
    }//LED = 0;Oled_DelayMs(200);
    if (oledUseEdge){   //右边框
        Oled_DrawLine(x, y, x, y+ch_h);
    }
}
/*********************************************/
//Oled在指定位置显示一个汉字字符串  -注意: 输入汉字字符串的格式 例: "你 好 世 界"(UTF-8编码)
// str - 字符串
// font - 字体
void Oled_ShowChinese(uint8_t x, uint8_t y, uint8_t* str, eFontName font)
{
    uint8_t font_i;       //搜索字库索引
    uint8_t han[5];       //UTF-8编码由1~4字节组成
    uint8_t utf8_i=0;     //UTF-8编码字节索引
    uint8_t str_len=0;    //已扫描字节数
    uint8_t ch_h, ch_w;   //字体的高、宽
    uint8_t ch_disw;      //字体显示宽度
    uint8_t str_end=0;    //字符串结束

    for(font_i=0; font_i<OLED_NON_ASCII_NUM; font_i++){
        if(font == nonAsciiIndex[font_i].font){
            ch_h = nonAsciiIndex[font_i].ch_h;
            ch_w = nonAsciiIndex[font_i].ch_w;
            break;
        }
    }
    if(font_i == OLED_NON_ASCII_NUM){
        OLED_SET_WARNING(FONT_NOT_EXIST); //字体不存在警告
        return;
    }
    ch_disw = ch_w+OLED_CHAR_SPACE;
    if (oledUseEdge){   //左边框
        Oled_DrawLine(x-1, y, x-1, y+ch_h);
    }
    //绘制汉字字符串
    while(!str_end)
    {
        while(str[str_len+utf8_i]!=' ')
        {
            if(str[str_len+utf8_i]!='\0'){
                han[utf8_i] = str[str_len+utf8_i]; utf8_i++;
            }else{
                str_end = 1; utf8_i++; break;
            }
        }
        han[utf8_i] = '\0';
        str_len += utf8_i+1;
        Oled_ShowCharZh(x, y, han, font);
        while(utf8_i!=0){han[--utf8_i] = '\0';}    //清零
        x += ch_disw;
    }
    if (oledUseEdge){   //右边框
        Oled_DrawLine(x, y, x, y+ch_h);
    }
}
/*********************************************/
//Oled设置在显示加入边框的字符串
// str - 字符串
// font - 字体
void Oled_EdgeString(uint8_t x, uint8_t y, uint8_t* str, eFontName font)
{
    oledUseEdge = 1;
    Oled_ShowString(x, y, str, font);
    oledUseEdge = 0;
}
/*********************************************/
//Oled设置在显示加入边框的汉字字符串  -注意: 输入汉字字符串的格式 例: "你 好 世 界"(UTF-8编码)
// str - 字符串
// font - 字体
void Oled_EdgeChinese(uint8_t x, uint8_t y, uint8_t* str, eFontName font)
{
    oledUseEdge = 1;
    Oled_ShowChinese(x, y, str, font);
    oledUseEdge = 0;
}
/*********************************************/
//Oled显示bmp位图
void Oled_ShowBmp(uint8_t x, uint8_t y, eBmpName bmp)
{
    uint8_t bmp_h, bmp_w;             //bmp的高, 宽
    uint16_t fresh_size;              //Oled需要刷新的字节数
    uint8_t page = y/OLED_PAGE_NUM;   //bmp起始页
    uint8_t col = x;                  //bmp起始列
    uint8_t offset = y%OLED_PAGE_NUM; //bmp相对起始页偏移
    uint8_t prev_byte=0, next_byte=0; //防止页重叠的字节拼接
    uint8_t cross_page;               //字符是否跨页
    uint8_t joint_times;              //拼接次数
    uint8_t dat;
    uint8_t page_cover;         //bmp最多横跨的页数
    uint8_t *bmp_p;             //bmp指针
    uint16_t col_i = 0;         //当前列索引
    uint8_t col_cur = 0;        //当前列(相对起始位置)
    uint8_t bmp_i;              //搜索bmp索引

    //设置绘图参数
    for(bmp_i=0; bmp_i<OLED_BMP_NUM; bmp_i++){
        if(bmp == bmpPara[bmp_i].bmp){
            bmp_h = bmpPara[bmp_i].bmp_h;
            bmp_w = bmpPara[bmp_i].bmp_w;
            bmp_p = bmpPara[bmp_i].bmp_p;
        }
    }
    if(bmp_i == OLED_BMP_NUM+1){
        OLED_SET_WARNING(BMP_NOT_EXIST); //bmp不存在警告
        return;
    }
    if(oledCheckRange && (x>OLED_COLUMN_NUM-bmp_w || y>OLED_ROW_NUM-bmp_h)){
        OLED_SET_WARNING(DRAW_OUT_OF_RANGE);  //绘画超出范围警告
        if(x>OLED_COLUMN_NUM || y>OLED_ROW_NUM){     //全部图像移出屏幕外
            return;
        }
    }
    cross_page = ((y+bmp_h)/OLED_PAGE_NUM == page) ? 0 : 1;
    page_cover = bmp_h/8+((bmp_h%8)?1:0)+1;
    if(page_cover>8){
        page_cover = 8;
    }
    fresh_size = bmp_w*page_cover;
    joint_times = page_cover;
    //绘图
    for(col_i=0; col_i<fresh_size; col_i++){
        if(oledIsDebug){
            Oled_DelayMs(oledDebugDelay);
        }
        if((col_i%bmp_w)==0){
            col_cur = 0;
            if(page<8){
                Oled_SetPos(page++, col);
            }
            if(col_i!=0){
                joint_times--;
            }
        }
        if(col_cur<bmp_w){
            if(col_cur+x>OLED_COLUMN_NUM){
                continue;
            }
            if(page_cover==2){
                if(joint_times==2){
                    next_byte = *(bmp_p+col_i);
                    dat = (next_byte<<offset);
                }else if(joint_times==1 && cross_page){
                    prev_byte = *(bmp_p+col_i);
                    dat = (prev_byte>>(8-offset));
                }
            }else{  //page_cover>=3
                if(joint_times==page_cover){
                    next_byte = *(bmp_p+col_i);
                    dat = (next_byte<<offset);
                }else if(joint_times>=2){
                    prev_byte = *(bmp_p+col_i-bmp_w);
                    next_byte = *(bmp_p+col_i);
                    dat = (prev_byte>>(8-offset)) | (next_byte<<offset);
                }else if(joint_times==1){
                    prev_byte = *(bmp_p+col_i-bmp_w);
                    dat = (prev_byte>>(8-offset));
                }
            }
            Oled_I2C_Write_Dat(dat);
            col_cur++;
        }
    }
}
/*********************************************/
//Oled显示数字
//num: 传入数字的地址
//type: 数字类型  
void Oled_ShowNum(uint8_t x, uint8_t y, void* num, eNumType type,
                  eFontName font)
{
    uint8_t str[30];
    uint32_t num_i;
    double num_f;

    switch(type){
        case nINT8:
            num_i = *((uint8_t*)num);   // 不显示负数
            int_to_str(str, num_i);
        break;
        case nINT16:
            num_i = *((uint16_t*)num);
            int_to_str(str, num_i);
        break;
        case nINT32:
            num_i = *((uint32_t*)num);
            int_to_str(str, num_i);
        break;
        case nFLOAT:
            num_f = *((double*)num);
            float_to_str(str, num_f);
        break;
        default:
            OLED_SET_WARNING(NUM_TYPE_NOT_EXIST);  //数字类型不存在警告
            return;
        break;
    }
    Oled_ShowString(x, y, str, font);
}
/*********************************************/
//Oled为IIC速率显示数字
//num: 传入数字的地址
void Oled_ShowIICRate(uint8_t x, uint8_t y, void* num)
{
    uint8_t str[10];
    uint32_t num_i;

    num_i = *((uint16_t*)num);
    int_to_str(str, num_i);
    Oled_ShowString(x, y, str, ASCII_H16W8);
}
/*********************************************/
//Oled初始化 SSD1306   
void Oled_Init()
{
    Oled_I2C_Write_Cmd(0xAE);//--display off
    Oled_I2C_Write_Cmd(0x00);//---set low column address
    Oled_I2C_Write_Cmd(0x10);//---set high column address
    Oled_I2C_Write_Cmd(0x40);//--set start line address  
    Oled_I2C_Write_Cmd(0xB0);//--set page address
    Oled_I2C_Write_Cmd(0x81);// contract control
    Oled_I2C_Write_Cmd(0xFF);//--128   
    Oled_I2C_Write_Cmd(0xA1);//set segment remap 
    Oled_I2C_Write_Cmd(0xA6);//--normal / reverse
    Oled_I2C_Write_Cmd(0xA8);//--set multiplex ratio(1 to 64)
    Oled_I2C_Write_Cmd(0x3F);//--1/32 duty
    Oled_I2C_Write_Cmd(0xC8);//Com scan direction
    Oled_I2C_Write_Cmd(0xD3);//-set display offset
    Oled_I2C_Write_Cmd(0x00);//
    Oled_I2C_Write_Cmd(0xD5);//set osc division
    Oled_I2C_Write_Cmd(0x80);//
    Oled_I2C_Write_Cmd(0xD8);//set area color mode off
    Oled_I2C_Write_Cmd(0x05);//
    Oled_I2C_Write_Cmd(0xD9);//Set Pre-Charge Period
    Oled_I2C_Write_Cmd(0xF1);//
    Oled_I2C_Write_Cmd(0xDA);//set com pin configuartion
    Oled_I2C_Write_Cmd(0x12);//
    Oled_I2C_Write_Cmd(0xDB);//set Vcomh
    Oled_I2C_Write_Cmd(0x30);//
    Oled_I2C_Write_Cmd(0x8D);//set charge pump enable
    Oled_I2C_Write_Cmd(0x14);//
    Oled_Clear();            //clear screen
    Oled_I2C_Write_Cmd(0xAF);//--turn on oled panel
}
