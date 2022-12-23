Dog_oled_v11: 控制器：STC89C52/STC12C5A60S2，可以正常使用，完成基本功能。
注：编译优化等级任意

###############################################################################

更新：1. v7: 增加菜单功能，支持中英文菜单，按照格式建立目录（Folder）和执行文件（File）。
      2. v8: 增加多Oled设备功能
	  3. v9: 完善Oled菜单功能，使用menu_builder中提供函数，更方便实现菜单显示和切换功能；
			 菜单基本功能只需实现4个函数: Init, KeyRegister, Display, Switch；
			 修复开始时程序跑飞的BUG
	  4. v10:修复菜单滚动显示的BUG
      5. v11:编译优化等级任意, 增强菜单切换功能可以选择记录上次切换位置

###############################################################################

|----Oled API
|--------Oled警告
void OLED_SET_WARNING(uint8_t wcode);
void OLED_RESET_WARNING(uint8_t wcode);
|--------Oled基础设置
void Oled_Init();
void Oled_SetDevice(eOledDevice dev);
void Oled_DisplayOn();
void Oled_DispalyOff();
void Oled_DisplayColor(uint8_t color);
void Oled_DisplayDir(uint8_t dir);
void Oled_SetDebugDraw(uint8_t debug);
void Oled_SetDebugDelay(uint16_t ms);
|--------Oled绘图
void Oled_Clear();
void Oled_ParticalClear(uint8_t start_x, uint8_t start_y, uint8_t end_x,   uint8_t end_y);
void Oled_DelayMs(uint16_t ms);
void Oled_UseStringEdge(uint8_t edge);
void Oled_DrawLine(uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y);
void Oled_ShowChar(uint8_t x, uint8_t y, uint8_t ch, eFontName font);
void Oled_ShowCharZh(uint8_t x, uint8_t y, uint8_t* ch, eFontName font);
void Oled_ShowString(uint8_t x, uint8_t y, uint8_t* str, eFontName font);
void Oled_ShowChinese(uint8_t x, uint8_t y, uint8_t* str, eFontName font);
void Oled_EdgeString(uint8_t x, uint8_t y, uint8_t* str, eFontName font);
void Oled_EdgeChinese(uint8_t x, uint8_t y, uint8_t* str, eFontName font);
void Oled_ShowBmp(uint8_t x, uint8_t y, eBmpName bmp);
void Oled_ShowNum(uint8_t x, uint8_t y, void* num, eNumType type, eFontName font);
void Oled_ShowIICRate(uint8_t x, uint8_t y, void* num);
|--------Oled API别名
o_init        Oled_Init
o_dev         Oled_SetDevice
o_on          Oled_DisplayOn
o_off         Oled_DispalyOff
o_color       Oled_DisplayColor
o_dir         Oled_DisplayDir
o_dedraw      Oled_SetDebugDraw
o_dedelay     Oled_SetDebugDelay
o_clr         Oled_Clear
o_partclr     Oled_ParticalClear
o_delayms     Oled_DelayMs
o_useedge     Oled_UseStringEdge
o_line        Oled_DrawLine
o_char        Oled_ShowChar
o_charzh      Oled_ShowCharZh
o_str         Oled_ShowString
o_chinese     Oled_ShowChinese
o_edgestr     Oled_EdgeString
o_edgecn      Oled_EdgeChinese
o_bmp         Oled_ShowBmp
o_num         Oled_ShowNum
o_iicrate     Oled_ShowIICRate

###############################################################################

|----Oled Test
/*********************************************
 *              以下为Oled绘图API测试
 *              无屏幕缓存版本
 *  测试单片机:
 *      STC89C52RC
 *      STC12C5A60S2
 *  测试内容:
 *      1. Oled亮/熄屏
 *      2. Oled正/反色显示
 *      3. Oled正/反转显示
 *      4. Oled调试绘画测试
 *      5. Oled全部清屏和部分清屏对比
 *      6. Oled显示IIC速率(10s检测一次)
 *      7. Oled多个设备显示
 *      8. Oled综合测试
 *          Ⅰ. 所有字符显示、数字显示、汉字显示测试
 *          Ⅱ. "DVD"字符串移动显示
 *          Ⅲ. 时钟功能演示
 *          Ⅳ. 画正弦波波形 正常&调试绘画
 *          Ⅴ. 菜单显示
 *          Ⅵ. 显示动图
 *
 *          by AZhao, B站 @阿釗同学
 *********************************************/

###############################################################################

