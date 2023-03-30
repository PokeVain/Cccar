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

#include "zf_device_wireless_ch573.h"
#include "zf_common_fifo.h"
#include "zf_device_type.h"

static  fifo_struct         wireless_ch573_fifo;
static  uint8             wireless_ch573_buffer[WIRELESS_CH573_BUFFER_SIZE];                                // 对应第一个 无线串口 数据存放数组

static  uint8             wireless_ch573_data;

//-------------------------------------------------------------------------------------------------------------------
// @brief       CH573无线模块中断回调函数
// @param       void
// @return      void
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
void wireless_ch573_callback (void)
{
    uart_query_byte(WIRELESS_CH573_INDEX, &wireless_ch573_data);
    fifo_write_buffer(&wireless_ch573_fifo, &wireless_ch573_data, 1);

}


//-------------------------------------------------------------------------------------------------------------------
// @brief       无线转串口模块 发送数据
// @param       data            8bit 数据
// @return      uint32          剩余发送长度
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_ch573_send_byte (const uint8 data)
{
    uint16 time_count = 0;
    while(gpio_get_level(WIRELESS_CH573_RTS_PIN))                                      // 如果RTS为低电平，则继续发送数据
    {
        if(time_count++ > WIRELESS_CH573_TIMEOUT_COUNT)
            return 1;                                                           // 模块忙,如果允许当前程序使用while等待 则可以使用后面注释的while等待语句替换本if语句
        system_delay_ms(1);
    }
    uart_write_byte(WIRELESS_CH573_INDEX, data);                                 // 发送最后的数据

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       CH573无线模块 串口发送数据
// @param       buff            数据地址
// @param       len             数据长度
// @return      uint32        提示信息
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_ch573_send_buff (const uint8 *buff, uint32 len)
{
    uint16 time_count = 0;
    uint8 pack_len = 62;
    while(len > pack_len)
    {
        time_count = 0;
        while(gpio_get_level(WIRELESS_CH573_RTS_PIN) && time_count++ < WIRELESS_CH573_TIMEOUT_COUNT)  // 如果RTS为低电平，则继续发送数据
            system_delay_ms(1);
        if(time_count >= WIRELESS_CH573_TIMEOUT_COUNT)
            return len;                                                                             // 模块忙,如果允许当前程序使用while等待 则可以使用后面注释的while等待语句替换本if语句
        uart_write_buffer(WIRELESS_CH573_INDEX, buff, 30);

        buff += pack_len;                                                                           // 地址偏移
        len -= pack_len;                                                                            // 数量
    }

    time_count = 0;
    while(gpio_get_level(WIRELESS_CH573_RTS_PIN) && time_count++ < WIRELESS_CH573_TIMEOUT_COUNT)  // 如果RTS为低电平，则继续发送数据
        system_delay_ms(1);
    if(time_count >= WIRELESS_CH573_TIMEOUT_COUNT)
        return len;                                                                                 // 模块忙,如果允许当前程序使用while等待 则可以使用后面注释的while等待语句替换本if语句
    uart_write_buffer(WIRELESS_CH573_INDEX, buff, len);                                            // 发送最后的数据

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       CH573无线模块 串口接收数据
// @param       buff            数据地址
// @param       len             数据长度
// @return      void
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_ch573_read_buff (uint8 *buff, uint32 len)
{
    uint32 data_len = len;
    fifo_read_buffer(&wireless_ch573_fifo, buff, &data_len, FIFO_READ_AND_CLEAN);
    return data_len;
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       无线转串口模块 发送摄像头图像至上位机查看图像
// @param       *image_addr     需要发送的图像地址
// @param       image_size      图像的大小
// @return      void
// Sample usage:                wireless_uart_send_image(&mt9v03x_image[0][0], MT9V03X_IMAGE_SIZE);
//-------------------------------------------------------------------------------------------------------------------
void wireless_ch573_send_image (const uint8 *image_addr, uint32 image_size)
{
    extern uint8 camera_send_image_frame_header[4];
    wireless_ch573_send_buff(camera_send_image_frame_header, 4);
    wireless_ch573_send_buff((uint8 *)image_addr, image_size);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       CH573无线模块初始化
// @param       void
// @return      void
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_ch573_init (void)
{

    set_wireless_type(WIRELESS_CH573);

    fifo_init(&wireless_ch573_fifo, wireless_ch573_buffer, WIRELESS_CH573_BUFFER_SIZE);
    gpio_init(WIRELESS_CH573_RTS_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);

    uart_init (WIRELESS_CH573_INDEX, WIRELESS_CH573_BUAD_RATE, WIRELESS_CH573_RX_PIN, WIRELESS_CH573_TX_PIN);
    uart_rx_interrupt(WIRELESS_CH573_INDEX, 1);

    return 0;
}



