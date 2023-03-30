/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_device_bluetooth_ch9141
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note             version
*                   V1.1 2022.01.10 对换CH1和CH2的引脚号
*                   V1.2 2022.03.16 删除配对操作，可以使用配对上位机进行配对
********************************************************************************************************************/
#ifndef _zf_device_bluetooth_ch9141_h_
#define _zf_device_bluetooth_ch9141_h_


#include "zf_common_fifo.h"
#include "zf_driver_gpio.h"
#include "zf_driver_uart.h"
#include "zf_driver_delay.h"

#define BLUETOOTH_CH9141_INDEX              UART_7                                      // 蓝牙模块 1 对应使用的串口号
#define BLUETOOTH_CH9141_BUAD_RATE          115200                                      // 蓝牙模块 1 对应使用的串口波特率
#define BLUETOOTH_CH9141_TX_PIN             UART7_RX_E13                                 // 蓝牙模块 1 对应模块的 TX 要接到单片机的 RX
#define BLUETOOTH_CH9141_RX_PIN             UART7_TX_E12                                 // 蓝牙模块 1 对应模块的 RX 要接到单片机的 TX
#define BLUETOOTH_CH9141_RTS_PIN            E8                                         // 蓝牙模块 1 对应模块的 RTS 引脚

#define BLUETOOTH_CH9141_BUFFER_SIZE        64
#define BLUETOOTH_CH9141_TIMEOUT_COUNT      500


void        bluetooth_ch9141_uart_callback_ch1      (void);
uint8     bluetooth_ch9141_init_ch1               (void);
uint32    bluetooth_ch9141_send_buff_ch1          (uint8 *buff, uint32 len);
uint32    bluetooth_ch9141_read_buff_ch1          (uint8 *buff, uint32 len);

#endif

