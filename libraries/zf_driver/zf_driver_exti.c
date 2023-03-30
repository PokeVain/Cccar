/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_driver_exti
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ3184284598)
* @version          查看doc内version文件 版本说明
* @Software         IAR 8.32.4 or MDK 5.28
* @Target core      MM32F3277
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-11
********************************************************************************************************************/

#include "zf_driver_exti.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief       EXTI 中断初始化 <同一下标的引脚不能同时初始化为外部中断输入 例如 A0 和 B0 不能同时初始化为外部中断输入>
// @param       pin             选择 EXTI 引脚 (可选择范围由 zf_driver_gpio.h 内 gpio_pin_enum 枚举值确定)
// @param       trigger         选择触发的信号方式 [EXTI_TRIGGER_RISING/EXTI_TRIGGER_FALLING/EXTI_TRIGGER_BOTH]
// @return      void
// Sample usage:                exti_init(A0, EXTI_TRIGGER_RISING);
//-------------------------------------------------------------------------------------------------------------------
void exti_init (gpio_pin_enum pin, exti_trigger_enum trigger)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);                         // 复用时钟使能
    gpio_init(pin, GPI, GPIO_HIGH, GPI_PULL_UP);                                // 初始化选中的引脚
    GPIO_EXTILineConfig(pin >> 5, pin & 0x1F);                                  // 选择哪一组GPIO做中断

    EXTI_InitTypeDef EXTI_InitStructure = {0};
    EXTI_InitStructure.EXTI_Line = 1 << (pin & 0x1F);                           // 引脚号配置
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                         // 触发模式
    EXTI_InitStructure.EXTI_Trigger = trigger;                                  // 触发方式
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                                   // 使能或者失能
    EXTI_Init(&EXTI_InitStructure);

    if((pin&0x1F) < 1)
       interrupt_enable(EXTI0_IRQn);                                             // 使能 Line0 的中断响应
    else if((pin&0x1F) < 2)
       interrupt_enable(EXTI1_IRQn);                                             // 使能 Line1 的中断响应
    else if((pin&0x1F) < 3)
       interrupt_enable(EXTI2_IRQn);                                             // 使能 Line2 的中断响应
    else if((pin&0x1F) < 4)
       interrupt_enable(EXTI3_IRQn);                                             // 使能 Line3 的中断响应
    else if((pin&0x1F) < 5)
       interrupt_enable(EXTI4_IRQn);                                             // 使能 Line4 的中断响应
    else if((pin&0x1F) < 10)
       interrupt_enable(EXTI9_5_IRQn);                                           // 使能 Line5-9 的中断响应
    else
       interrupt_enable(EXTI15_10_IRQn);                                         // 使能 Line10-15 的中断响应
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       EXTI 中断使能
// @param       pin              选择 EXTI 引脚 (可选择范围由 zf_driver_gpio.h 内 gpio_pin_enum 枚举值确定)
// @return      void
// Sample usage:                exti_enable(A0);
//-------------------------------------------------------------------------------------------------------------------
void exti_enable (gpio_pin_enum pin)
{
    EXTI->INTENR |= (0x00000001 << (pin&0x1F));
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       EXTI 中断失能
// @param       pin             选择 EXTI 引脚 (可选择范围由 zf_driver_gpio.h 内 gpio_pin_enum 枚举值确定)
// @return      void
// Sample usage:                exti_disable(A0);
//-------------------------------------------------------------------------------------------------------------------
void exti_disable (gpio_pin_enum pin)
{
    EXTI->INTENR &= ~(0x00000001 << (pin&0x1F));
}
