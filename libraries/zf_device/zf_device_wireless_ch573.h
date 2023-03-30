/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_device_wireless_ch573
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2022-03-02
* @note
********************************************************************************************************************/
#ifndef _zf_device_wireless_ch573_h_
#define _zf_device_wireless_ch573_


#include "zf_driver_gpio.h"
#include "zf_driver_uart.h"
#include "zf_driver_delay.h"



#define WIRELESS_CH573_NUMBER            1                                           // 定义接入的CH573无线模块的数量 默认最大就三个
#define WIRELESS_CH573_INDEX             UART_2                                      // CH573无线模块1 对应使用的串口号
#define WIRELESS_CH573_BUAD_RATE         115200                                      // CH573无线模块1 对应使用的串口波特率
#define WIRELESS_CH573_TX_PIN            UART2_RX_D6                                 // CH573无线模块1 对应模块的 TX 要接到单片机的 RX
#define WIRELESS_CH573_RX_PIN            UART2_TX_D5                                 // CH573无线模块1 对应模块的 RX 要接到单片机的 TX
#define WIRELESS_CH573_RTS_PIN           E10                                         // CH573无线模块1 对应模块的 RTS 引脚


#define WIRELESS_CH573_BUFFER_SIZE       64
#define WIRELESS_CH573_TIMEOUT_COUNT     0x64



void        wireless_ch573_callback      (void);

uint32      wireless_ch573_send_byte         (const uint8 data);

uint32      wireless_ch573_send_buff         (const uint8 *buff, uint32 len);
uint32      wireless_ch573_send_string       (const uint8 *str);
void        wireless_ch573_send_image        (const uint8 *image_addr, uint32 image_size);

uint32      wireless_ch573_read_buff     (uint8 *buff, uint32 len);

uint8       wireless_ch573_init          (void);

#endif
