/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_common_headfile
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
********************************************************************************************************************/

#ifndef __HEADFILE_H
#define __HEADFILE_H

#include "stdio.h"
#include "stdint.h"
#include "string.h"

//===================================================芯片 SDK 底层===================================================
#include "ch32v30x_adc.h"
#include "ch32v30x_bkp.h"
#include "ch32v30x_can.h"
#include "ch32v30x_crc.h"
#include "ch32v30x_dac.h"
#include "ch32v30x_dbgmcu.h"
#include "ch32v30x_dma.h"
#include "ch32v30x_exti.h"
#include "ch32v30x_flash.h"
#include "ch32v30x_fsmc.h"
#include "ch32v30x_gpio.h"
#include "ch32v30x_i2c.h"
#include "ch32v30x_iwdg.h"
#include "ch32v30x_pwr.h"
#include "ch32v30x_rcc.h"
#include "ch32v30x_rtc.h"
#include "ch32v30x_sdio.h"
#include "ch32v30x_spi.h"
#include "ch32v30x_tim.h"
#include "ch32v30x_usart.h"
#include "ch32v30x_wwdg.h"
#include "ch32v30x_misc.h"
#include "ch32v30x.h"
//===================================================芯片 SDK 底层===================================================

//====================================================开源库公共层====================================================
#include "zf_common_clock.h"
#include "zf_common_debug.h"
#include "zf_common_font.h"
#include "zf_common_function.h"
#include "zf_common_interrupt.h"
#include "zf_common_fifo.h"
#include "zf_common_typedef.h"
//====================================================开源库公共层====================================================

//===================================================芯片外设驱动层===================================================
#include "zf_driver_adc.h"
#include "zf_driver_delay.h"
#include "zf_driver_dvp.h"
#include "zf_driver_encoder.h"
#include "zf_driver_exti.h"
#include "zf_driver_flash.h"
#include "zf_driver_gpio.h"
//#include "zf_driver_iic.h"
#include "zf_driver_pit.h"
#include "zf_driver_pwm.h"
//#include "zf_driver_soft_iic.h"
//#include "zf_driver_soft_spi.h"
#include "zf_driver_spi.h"
#include "zf_driver_timer.h"
#include "zf_driver_uart.h"
#include "zf_driver_usb_cdc.h"

//===================================================芯片外设驱动层===================================================

//===================================================外接设备驱动层===================================================
#include "zf_device_camera.h"
#include "zf_device_icm20602.h"
#include "zf_device_ips114.h"
#include "zf_device_tft180.h"
#include "zf_device_ips200_parallel8.h"
#include "zf_device_mt9v03x_dvp.h"
#include "zf_device_mpu6050.h"
#include "zf_device_type.h"
#include "zf_device_wireless_uart.h"
#include "zf_device_oled.h"
#include "zf_device_scc8660_dvp.h"
#include "zf_device_bluetooth_ch9141.h"
#include "zf_device_bluetooth_ch9141_ch2.h"
#include "zf_device_wireless_ch573.h"
#include "zf_device_wireless_uart.h"
//===================================================外接设备驱动层===================================================

//===================================================RT-THREAD驱动层===================================================

#include <rtthread.h>
#include <rthw.h>
//===================================================RT-THREAD驱动层===================================================
#include "JJJ_pid.h"
#include "Split_ultrasonic.h"
#include "Communication_doublecar.h"
#include "IMU.h"

#endif

