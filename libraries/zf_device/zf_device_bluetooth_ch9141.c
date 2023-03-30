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

#include "zf_device_bluetooth_ch9141.h"
#include "zf_device_type.h"

static  fifo_struct         bluetooth_ch9141_fifo;
static  uint8             bluetooth_ch9141_buffer[BLUETOOTH_CH9141_BUFFER_SIZE];                                // 对应第一个 无线串口 数据存放数组
static  uint8             bluetooth_ch9141_data;


//-------------------------------------------------------------------------------------------------------------------
//  @brief      蓝牙转串口模块 串口中断回调函数
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note       该函数在ISR文件 串口8中断程序被调用
//-------------------------------------------------------------------------------------------------------------------
void bluetooth_ch9141_uart_callback_ch1 (void)
{
    uart_query_byte(BLUETOOTH_CH9141_INDEX, &bluetooth_ch9141_data);                // 读取串口数据
    fifo_write_buffer(&bluetooth_ch9141_fifo, &bluetooth_ch9141_data, 1);           // 存入 FIFO
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      蓝牙转串口模块初始化
//  @param      mode    蓝牙模式 MASTER_MODE(主机)或者SLAVE_MODE(从机)
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note
//-------------------------------------------------------------------------------------------------------------------
uint8 bluetooth_ch9141_init_ch1 (void)
{
    set_wireless_type(BLUETOOTH_CH9141);

    // 本函数使用的波特率为115200，为蓝牙转串口模块的默认波特率，如需其他波特率请使用上位机修改模块参数
    // 初始化流控引脚

    fifo_init(&bluetooth_ch9141_fifo, bluetooth_ch9141_buffer, BLUETOOTH_CH9141_BUFFER_SIZE);
    uart_init(BLUETOOTH_CH9141_INDEX, BLUETOOTH_CH9141_BUAD_RATE, BLUETOOTH_CH9141_RX_PIN, BLUETOOTH_CH9141_TX_PIN);
    gpio_init(BLUETOOTH_CH9141_RTS_PIN, GPI, 1, GPI_PULL_UP);
    uart_rx_interrupt(BLUETOOTH_CH9141_INDEX, 1);

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      蓝牙转串口模块 发送函数
//  @param      buff        需要发送的数据地址
//  @param      len         发送长度
//  @return     uint32      剩余未发送的字节数
//  @since      v1.0
//  Sample usage:
//  @note
//-------------------------------------------------------------------------------------------------------------------
uint32 bluetooth_ch9141_send_buff_ch1 (uint8 *buff, uint32 len)
{
    uint16 time_count = 0;
    while(len > 30)
    {
        time_count = 0;
        while(gpio_get_level(BLUETOOTH_CH9141_RTS_PIN) && time_count++ < BLUETOOTH_CH9141_TIMEOUT_COUNT)                  // 如果RTS为低电平，则继续发送数据
            system_delay_ms(1);
        if(time_count >= BLUETOOTH_CH9141_TIMEOUT_COUNT)
            return len;                                                                         // 模块忙,如果允许当前程序使用while等待 则可以使用后面注释的while等待语句替换本if语句
        uart_write_buffer(BLUETOOTH_CH9141_INDEX, buff, 30);

        buff += 30;                                                                             // 地址偏移
        len -= 30;                                                                              // 数量
    }

    time_count = 0;
    while(gpio_get_level(BLUETOOTH_CH9141_RTS_PIN) && time_count++ < BLUETOOTH_CH9141_TIMEOUT_COUNT)                      // 如果RTS为低电平，则继续发送数据
        system_delay_ms(1);
    if(time_count >= BLUETOOTH_CH9141_TIMEOUT_COUNT)
        return len;                                                                             // 模块忙,如果允许当前程序使用while等待 则可以使用后面注释的while等待语句替换本if语句
    uart_write_buffer(BLUETOOTH_CH9141_INDEX, buff, len);                                              // 发送最后的数据

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      蓝牙转串口模块 读取函数
//  @param      buff        需要读取的数据地址
//  @param      len         发送长度
//  @return     uint32      读取的数据长度 如果没有这么多数据这里会被修改
//  @since      v1.0
//  Sample usage:
//  @note
//-------------------------------------------------------------------------------------------------------------------
uint32 bluetooth_ch9141_read_buff_ch1(uint8 *buff, uint32 len)
{
    uint32 data_len = len;
    fifo_read_buffer(&bluetooth_ch9141_fifo, buff, &data_len, FIFO_READ_AND_CLEAN);
    return data_len;
}





