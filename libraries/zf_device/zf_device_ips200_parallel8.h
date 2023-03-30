/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_device_ips200_parallel8
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note             接线定义：
*                   ------------------------------------
*                   模块管脚                                        单片机管脚
*                   BL                  查看 zf_device_ips200_parallel8.h 中 IPS200_BL_PIN 宏定义
*                   CS                  查看 zf_device_ips200_parallel8.h 中 IPS200_CS_PIN 宏定义
*                   RST                 查看 zf_device_ips200_parallel8.h 中 IPS200_RST_PIN 宏定义
*                   RS                  查看 zf_device_ips200_parallel8.h 中 IPS200_RS_PIN 宏定义
*                   WR                  查看 zf_device_ips200_parallel8.h 中 IPS200_WR_PIN 宏定义
*                   RD                  查看 zf_device_ips200_parallel8.h 中 IPS200_RD_PIN 宏定义
*                   D0-D7               查看 zf_device_ips200_parallel8.h 中 ICM20602_CS_PIN 宏定义
*                   电源引脚
*                   VCC                 3.3V电源
*                   GND                 电源地
*                   ------------------------------------
********************************************************************************************************************/
#ifndef _SEEKFREE_IPS20_PARALLEL8_H
#define _SEEKFREE_IPS20_PARALLEL8_H

#include "zf_common_clock.h"
#include "zf_common_debug.h"
#include "zf_common_font.h"
#include "zf_common_function.h"

#include "zf_driver_delay.h"
#include "zf_driver_gpio.h"

// 引脚定义
#define IPS200_RD_PIN           B13
#define IPS200_WR_PIN           B15
#define IPS200_RS_PIN           B7
#define IPS200_RST_PIN          D7
#define IPS200_CS_PIN           D4
#define IPS200_BL_PIN           D0

// 控制语句
#define IPS200_RD(x)        	(x? (gpio_high(IPS200_RD_PIN)): (gpio_low(IPS200_RD_PIN)))
#define IPS200_WR(x)        	(x? (gpio_high(IPS200_WR_PIN)): (gpio_low(IPS200_WR_PIN)))
#define IPS200_RS(x)        	(x? (gpio_high(IPS200_RS_PIN)): (gpio_low(IPS200_RS_PIN)))
#define IPS200_RST(x)  			(x? (gpio_high(IPS200_RST_PIN)): (gpio_low(IPS200_RST_PIN)))
#define IPS200_CS(x)			(x? (gpio_high(IPS200_CS_PIN)): (gpio_low(IPS200_CS_PIN)))
#define IPS200_BL(x) 			(x? (gpio_high(IPS200_BL_PIN)): (gpio_low(IPS200_BL_PIN)))



//8个数据引脚必须连续 例如B0-B7,B6-B13等等。
//--------------数据端口寄存器--------------
#define IPS200_DATAPORT     	GPIOE

//--------------数据端口起始地址偏移--------------
#define DATA_START_NUM			0

//例：D1-D8		IPS200_DATAPORT设置为GPIOD   DATA_START_NUM设置为1
//例：C5-C12	    IPS200_DATAPORT设置为GPIOC   DATA_START_NUM设置为5


typedef enum
{
    IPS200_PORTAIT                      = 0,                                    // 竖屏模式
    IPS200_PORTAIT_180                  = 1,                                    // 竖屏模式  旋转180
    IPS200_CROSSWISE                    = 2,                                    // 横屏模式
    IPS200_CROSSWISE_180                = 3,                                    // 横屏模式  旋转180
}ips200_dir_enum;

#define IPS200_DEFAULT_DISPLAY_DIR      IPS200_PORTAIT                          // 默认的显示方向
#define IPS200_DEFAULT_PENCOLOR         RGB565_RED                                     // 默认的画笔颜色
#define IPS200_DEFAULT_BGCOLOR          RGB565_WHITE                                   // 默认的背景颜色

extern  uint16 ips200_pencolor;
extern  uint16 ips200_bgcolor;

extern  ips200_dir_enum ips200_display_dir;
extern  uint16 ips200_x_max;
extern  uint16 ips200_y_max;

void    ips200_clear                    (const uint16 color);
void    ips200_set_dir                  (ips200_dir_enum dir);
void    ips200_set_color                (const uint16 pen, const uint16 bgcolor);
void    ips200_draw_point               (uint16 x, uint16 y, const uint16 color);
void    ips200_draw_line                (uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end, const uint16 color);

void    ips200_show_char                (uint16 x, uint16 y, const char dat);
void    ips200_show_string              (uint16 x, uint16 y, const char dat[]);
void    ips200_show_int                 (uint16 x, uint16 y, const int32 dat, uint8 num);
void    ips200_show_uint                (uint16 x, uint16 y, const uint32 dat, uint8 num);
void    ips200_show_float               (uint16 x, uint16 y, const float dat, uint8 num, uint8 pointnum);

void    ips200_show_binary_image        (uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height);
void    ips200_show_gray_image          (uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 threshold);
void    ips200_show_rgb565_image        (uint16 x, uint16 y, const uint16 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 color_mode);

void    ips200_show_wave                (uint16 x, uint16 y, const uint16 *wave, uint16 width, uint16 value_max, uint16 dis_width, uint16 dis_value_max);
void    ips200_show_chinese             (uint16 x, uint16 y, uint8 size, const uint8 *chinese_buffer, uint8 number, const uint16 color);

#define ips200_displayimage7725(p,width,height)                                 ips200_show_binary_image(0,0,p,width,height,width,height)

#define ips200_displayimage032(p,width,height)                                  ips200_show_gray_image(0,0,p,width,height,width,height,0)
#define ips200_displayimage032_zoom(p,width,height,dis_width,dis_height)        ips200_show_gray_image(0,0,p,width,height,dis_width,dis_height,0)
#define ips200_displayimage032_zoom1(p,width,height,x,y,dis_width,dis_height)   ips200_show_gray_image(x,y,p,width,height,dis_width,dis_height,0)

#define ips200_displayimage8660(p,width,height)                                 ips200_show_rgb565_image(0,0,p,width,height,width,height,1)
#define ips200_displayimage8660_zoom(p,width,height,dis_width,dis_height)       ips200_show_rgb565_image(0,0,p,width,height,dis_width,dis_height,1)
#define ips200_displayimage8660_zoom1(p,width,height,x,y,dis_width,dis_height)  ips200_show_rgb565_image(x,y,p,width,height,dis_width,dis_height,1)

void    ips200_init                     ();
#endif

