#ifndef __OLED_DRIVER_H__
#define __OLED_DRIVER_H__

#include "oled/oled_config.h"

/*-------------------SSD1306 Oled命令---------------------*/
// 见 Oled Command Table & Oled Command Descriptions
// Oled1
#define OLED_SCL_H      OLED_SCL   =  1
#define OLED_SCL_L      OLED_SCL   =  0
#define OLED_SDA_H      OLED_SDA   =  1
#define OLED_SDA_L      OLED_SDA   =  0
#define OLED_SDA_Read   OLED_SDA
// Oled2
#define OLED2_SCL_H     OLED2_SCL  =  1
#define OLED2_SCL_L     OLED2_SCL  =  0
#define OLED2_SDA_H     OLED2_SDA  =  1
#define OLED2_SDA_L     OLED2_SDA  =  0
#define OLED2_SDA_Read  OLED2_SDA


void Oled_I2C_Write_Cmd(uint8_t cmd);
void Oled_I2C_Write_Dat(uint8_t dat);

#endif
