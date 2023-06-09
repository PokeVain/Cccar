/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_device_mt9v03x
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note             version:
*                   V1.1 2021.12.23 摄像头采集完成标志位增加volatile修饰
*                   V1.2 2022.03.16 修改部分不重要的注释
*                   接线定义：
*                   ------------------------------------
*                   模块管脚            单片机管脚
*                   TXD                 查看 zf_device_mt9v03x.h 中 MT9V03X_COF_UART_TX_DVP        宏定义
*                   RXD                 查看 zf_device_mt9v03x.h 中 MT9V03X_COF_UART_RX_DVP        宏定义
*                   D0                  查看 zf_device_mt9v03x.h 中 MT9V03X_D0_PIN_DVP             宏定义
*                   D1                  查看 zf_device_mt9v03x.h 中 MT9V03X_D1_PIN_DVP             宏定义
*                   D2                  查看 zf_device_mt9v03x.h 中 MT9V03X_D2_PIN_DVP             宏定义
*                   D3                  查看 zf_device_mt9v03x.h 中 MT9V03X_D3_PIN_DVP             宏定义
*                   D4                  查看 zf_device_mt9v03x.h 中 MT9V03X_D4_PIN_DVP             宏定义
*                   D5                  查看 zf_device_mt9v03x.h 中 MT9V03X_D5_PIN_DVP             宏定义
*                   D6                  查看 zf_device_mt9v03x.h 中 MT9V03X_D6_PIN_DVP             宏定义
*                   D7                  查看 zf_device_mt9v03x.h 中 MT9V03X_D7_PIN_DVP             宏定义
*                   PCLK                查看 zf_device_mt9v03x.h 中 MT9V03X_PCLK_PIN_DVP           宏定义
*                   VSYNC               查看 zf_device_mt9v03x.h 中 MT9V03X_VSY_PIN_DVP            宏定义
*                   HSYNC               查看 zf_device_mt9v03x.h 中 MT9V03X_HERF_PIN_DVP           宏定义
*                   ------------------------------------
********************************************************************************************************************/

#include "zf_device_mt9v03x_dvp.h"
#include "zf_device_camera.h"
#include "zf_device_type.h"
#include "rtthread.h"


uint8 *camera_buffer_addr;                                                    // 摄像头缓冲区地址指针

uint8 volatile mt9v03x_finish_flag_dvp = 0;                                                // 一场图像采集完成标志位
uint8 mt9v03x_image_dvp[MT9V03X_DVP_H][MT9V03X_DVP_W];

static          uint8     receive_dvp[3];
static          uint8     receive_num_dvp = 0;
static volatile uint8     uart_receive_flag_dvp;

//需要配置到摄像头的数据
int16 mt9v03x_set_confing_buffer_dvp[CONFIG_FINISH][2]=
{
    {AUTO_EXP,          0},                                                     // 自动曝光设置       范围1-63 0为关闭 如果自动曝光开启  EXP_TIME命令设置的数据将会变为最大曝光时间，也就是自动曝光时间的上限
                                                                                // 一般情况是不需要开启这个功能，因为比赛场地光线一般都比较均匀，如果遇到光线非常不均匀的情况可以尝试设置该值，增加图像稳定性
    {EXP_TIME,          100},                                                   // 曝光时间         摄像头收到后会自动计算出最大曝光时间，如果设置过大则设置为计算出来的最大曝光值
    {FPS,               50},                                                    // 图像帧率         摄像头收到后会自动计算出最大FPS，如果过大则设置为计算出来的最大FPS
    {SET_COL,           MT9V03X_DVP_W},                                         // 图像列数量        范围1-752     K60采集不允许超过188
    {SET_ROW,           MT9V03X_DVP_H},                                         // 图像行数量        范围1-480
    {LR_OFFSET,         0},                                                    // 图像左右偏移量  正值 右偏移   负值 左偏移  列为188 376 752时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
    {UD_OFFSET,         0},                                                     // 图像上下偏移量  正值 上偏移   负值 下偏移  行为120 240 480时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
    {GAIN,              32},                                                    // 图像增益         范围16-64     增益可以在曝光时间固定的情况下改变图像亮暗程度
    {PCLK_MODE,         1},                                                     // 仅总钻风MT9V034 V2.0以及以上版本支持该命令，
                                                                                // 像素时钟模式命令 PCLK模式     默认：0     可选参数为：0 1。        0：不输出消隐信号，1：输出消隐信号。(通常都设置为0，如果使用CH32V307的DVP接口或STM32的DCMI接口采集需要设置为1)


    {INIT,              0}                                                      // 摄像头开始初始化
};

//从摄像头内部获取到的配置数据
int16 mt9v03x_get_confing_buffer_dvp[CONFIG_FINISH-1][2]=
{
    {AUTO_EXP,          0},                                                     // 自动曝光设置
    {EXP_TIME,          0},                                                     // 曝光时间
    {FPS,               0},                                                     // 图像帧率
    {SET_COL,           0},                                                     // 图像列数量
    {SET_ROW,           0},                                                     // 图像行数量
    {LR_OFFSET,         0},                                                     // 图像左右偏移量
    {UD_OFFSET,         0},                                                     // 图像上下偏移量
    {GAIN,              0},                                                     // 图像增益
    {PCLK_MODE,         0},                                                     //像素时钟模式(仅总钻风MT9V034 V1.5以及以上版本支持该命令)
};

//-------------------------------------------------------------------------------------------------------------------
// @brief       配置摄像头内部配置信息 内部调用
// @param       uartn           选择使用的串口
// @param       buff            发送配置信息的地址
// @return      uint8         1-失败 0-成功
// Sample usage:                调用该函数前请先初始化串口
//-------------------------------------------------------------------------------------------------------------------
static uint8 mt9v03x_set_config_dvp (int16 buff[CONFIG_FINISH-1][2])
{
    uint16 temp, i;
    uint8  send_buffer[4];
    volatile int16 timeout = MT9V03X_INIT_TIMEOUT;

    uart_receive_flag_dvp = 0;

    // 设置参数  具体请参看问题锦集手册
    // 开始配置摄像头并重新初始化
    for(i=0; i<CONFIG_FINISH; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = buff[i][0];
        temp = buff[i][1];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8)temp;
        uart_write_buffer(MT9V03X_COF_UART_DVP,send_buffer,4);

        system_delay_ms(2);
    }

    while(!uart_receive_flag_dvp && timeout-- > 0)                                   // 等待接受回传数据
    {
        system_delay_ms(1);
    }
    timeout = MT9V03X_INIT_TIMEOUT;

    uart_receive_flag_dvp = 0;
    while(((0xff != receive_dvp[1]) || (0xff != receive_dvp[2])) && timeout-- > 0)   // 判断数据是否截取到对应内容
    {
        system_delay_ms(1);
    }

    // 以上部分对摄像头配置的数据全部都会保存在摄像头上51单片机的eeprom中
    // 利用set_exposure_time函数单独配置的曝光数据不存储在eeprom中
    if(timeout <= 0)                                                         // 超时
        return 1;
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       获取摄像头内部配置信息 内部调用
// @param       uartn           选择使用的串口
// @param       buff            接收配置信息的地址
// @return      uint8         1-失败 0-成功
// Sample usage:                调用该函数前请先初始化串口
//-------------------------------------------------------------------------------------------------------------------
static uint8 mt9v03x_get_config_dvp (int16 buff[CONFIG_FINISH-1][2])
{
    uint16 temp, i;
    uint8  send_buffer[4];
    volatile int16 timeout = MT9V03X_INIT_TIMEOUT;

    for(i=0; i<(CONFIG_FINISH-1); i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = GET_STATUS;
        temp = buff[i][0];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8)temp;
        uart_write_buffer(MT9V03X_COF_UART_DVP,send_buffer,4);

        timeout = MT9V03X_INIT_TIMEOUT;
        while(!uart_receive_flag_dvp && timeout-- > 0)                              // 等待接受回传数据
        {
            system_delay_ms(1);
        }
        if(timeout <= 0 && i==0)                                                     // 超时
            return 1;

        uart_receive_flag_dvp = 0;
        buff[i][1] = receive_dvp[1]<<8 | receive_dvp[2];
    }
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       MT9V03X摄像头串口中断回调函数
// @param       void
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void mt9v03x_uart_callback_dvp (void)
{
//    receive_dvp[receive_num_dvp] = ((USART_TypeDef*)uart_index[MT9V03X_COF_UART_DVP])->DATAR & 0xFF;
//    receive_num_dvp++;

    if(uart_query_byte(MT9V03X_COF_UART_DVP, &receive_dvp[receive_num_dvp]))
        receive_num_dvp++;

    if(1 == receive_num_dvp && 0XA5!=receive_dvp[0])
        receive_num_dvp = 0;
    if(3 == receive_num_dvp)
    {
        receive_num_dvp = 0;
        uart_receive_flag_dvp = 1;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       获取摄像头固件版本
// @param       void            选择使用的串口
// @return      uint16        0-获取错误 N-版本号
// Sample usage:                调用该函数前请先初始化串口
//-------------------------------------------------------------------------------------------------------------------
uint16 mt9v03x_get_version_dvp (void)
{
    uint16 temp;
    uint8  send_buffer[4];
    volatile int16 timeout = MT9V03X_INIT_TIMEOUT;

    send_buffer[0] = 0xA5;
    send_buffer[1] = GET_STATUS;
    temp = GET_VERSION;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    uart_write_buffer(MT9V03X_COF_UART_DVP,send_buffer,4);

    while(!uart_receive_flag_dvp && timeout-- > 0)                                  // 等待接受回传数据
    {
        system_delay_ms(1);
    }
    uart_receive_flag_dvp = 0;

    if(timeout <= 0)                                                            // 超时
        return 0;
    return ((uint16)(receive_dvp[1]<<8) | receive_dvp[2]);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       单独设置摄像头曝光时间
// @param       uartn           选择使用的串口
// @param       light           设置曝光时间越大图像越亮，摄像头收到后会根据分辨率及FPS计算最大曝光时间如果设置的数据过大，那么摄像头将会设置这个最大值
// @return      uint8         1-失败 0-成功
// Sample usage:                调用该函数前请先初始化串口
//-------------------------------------------------------------------------------------------------------------------
uint8 mt9v03x_set_exposure_time_dvp (uint16 light)
{
    uint16 temp;
    uint8  send_buffer[4];
    volatile int16 timeout = MT9V03X_INIT_TIMEOUT;

    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_EXP_TIME;
    temp = light;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    uart_write_buffer(MT9V03X_COF_UART_DVP,send_buffer,4);

    while(!uart_receive_flag_dvp && timeout-- > 0)                                  // 等待接受回传数据
    {
        system_delay_ms(1);
    }
    uart_receive_flag_dvp = 0;

    temp = receive_dvp[1]<<8 | receive_dvp[2];

    if(timeout <= 0 || temp != light)
        return 0;
    return 1;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       对摄像头内部寄存器进行写操作
// @param       uartn           选择使用的串口
// @param       addr            摄像头内部寄存器地址
// @param       data            需要写入的数据
// @return      uint8         1-失败 0-成功
// Sample usage:                调用该函数前请先初始化串口
//-------------------------------------------------------------------------------------------------------------------
uint8 mt9v03x_set_reg_dvp (uint8 addr, uint16 data)
{
    uint16 temp;
    uint8  send_buffer[4];
    volatile int16 timeout = MT9V03X_INIT_TIMEOUT;

    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_ADDR;
    temp = addr;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    uart_write_buffer(MT9V03X_COF_UART_DVP,send_buffer,4);

    system_delay_ms(10);
    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_DATA;
    temp = data;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    uart_write_buffer(MT9V03X_COF_UART_DVP,send_buffer,4);

    while(!uart_receive_flag_dvp && timeout-- > 0)                                  // 等待接受回传数据
    {
        system_delay_ms(1);
        timeout--;
    }
    uart_receive_flag_dvp = 0;

    temp = receive_dvp[1]<<8 | receive_dvp[2];
    if(timeout <= 0 || temp != data)
        return 0;
    return 1;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V03X摄像头场中断
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:               在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
//-------------------------------------------------------------------------------------------------------------------
//extern rt_sem_t camera_sem;
void mt9v03x_handler_dvp(void)
{

//    rt_sem_release(camera_sem);
    //已经修改为循环采集模式。不需要设置地址和开启DVP
    //DVP->DMA_BUF0 = (uint32)camera_buffer_addr;       // 恢复DMA地址
    mt9v03x_finish_flag_dvp = 1;                        // 摄像头采集完成标志位置1
    //已经修改为循环采集模式。不需要设置地址和开启DVP
    //DVP->CR0 |= RB_DVP_ENABLE;                          // 重新打开DVP，采集下一副图像
}




//-------------------------------------------------------------------------------------------------------------------
// @brief       MT9V03X摄像头初始化
// @param       void
// @return      uint8         1-失败 0-成功
// Sample usage:                使用FLEXIO接口采集摄像头
//-------------------------------------------------------------------------------------------------------------------
uint8 mt9v03x_init_dvp (void)
{
    set_camera_type(CAMERA_GRAYSCALE);             //设置连接摄像头类型为总钻风

    uart_init (MT9V03X_COF_UART_DVP, MT9V03X_COF_BAUR_DVP, MT9V03X_COF_UART_RX_DVP, MT9V03X_COF_UART_TX_DVP);    //初始换串口 配置摄像头
    uart_rx_interrupt(MT9V03X_COF_UART_DVP, ENABLE);

//    interrupt_global_enable();

    //等待摄像头上电初始化成功 方式有两种：延时或者通过获取配置的方式 二选一
    //system_delay_ms(1000);                                                    // 延时方式
    uart_receive_flag_dvp = 0;
    if(mt9v03x_get_config_dvp(mt9v03x_get_confing_buffer_dvp))                  // 获取配置的方式
    {
        zf_assert(0);
        return 1;
    }

    uart_receive_flag_dvp = 0;
    if(mt9v03x_set_config_dvp(mt9v03x_set_confing_buffer_dvp))
    {
        zf_assert(0);
        return 1;
    }

    uart_receive_flag_dvp = 0;
    //获取配置便于查看配置是否正确
    if(mt9v03x_get_config_dvp(mt9v03x_get_confing_buffer_dvp))
    {
        zf_assert(0);
        return 1;
    }

    //interrupt_global_disable();

    // DVP引脚初始化
    dvp_gpio_init(
            MT9V03X_D0_PIN_DVP, MT9V03X_D1_PIN_DVP, MT9V03X_D2_PIN_DVP, MT9V03X_D3_PIN_DVP,
            MT9V03X_D4_PIN_DVP, MT9V03X_D5_PIN_DVP, MT9V03X_D6_PIN_DVP, MT9V03X_D7_PIN_DVP,
            MT9V03X_PCLK_PIN_DVP, MT9V03X_VSY_PIN_DVP, MT9V03X_HERF_PIN_DVP);
    // 设置地址
    camera_buffer_addr = mt9v03x_image_dvp[0];

    // DVP接口初始化
    dvp_camera_init((uint32 *)camera_buffer_addr, NULL, MT9V03X_DVP_W*MT9V03X_DVP_H, 1);
    return 0;
}
