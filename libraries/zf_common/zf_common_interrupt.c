/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_common_interrupt
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note             version:
*                   V1.1 2022.03.16 修改开关总中断函数名称，增加中断嵌套计数
********************************************************************************************************************/


#include "zf_common_interrupt.h"

static uint32 interrupt_nest_count = 0;


//-------------------------------------------------------------------------------------------------------------------
// @brief       指定中断使能
// @param       irqn            指定中断号 可查看 isr.c 对应中断服务函数的标注
// @return      void
// Sample usage:                interrupt_enable(UART1_IRQn);
//-------------------------------------------------------------------------------------------------------------------
void interrupt_enable (IRQn_Type irqn)
{
    NVIC_EnableIRQ(irqn);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       指定中断屏蔽
// @param       irqn            指定中断号 可查看 isr.c 对应中断服务函数的标注
// @return      void
// Sample usage:                interrupt_disable(UART1_IRQn);
//-------------------------------------------------------------------------------------------------------------------
void interrupt_disable (IRQn_Type irqn)
{
    NVIC_DisableIRQ(irqn);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       指定中断设置优先级
// @param       irqn            指定中断号 可查看 isr.c 对应中断服务函数的标注
// @param       priority        bit7-bit5为抢占优先级，bit4-bit0为次优先级，值越低优先级越高
// @return      void
// Sample usage:                interrupt_enable(UART1_IRQn, (1<<5) || 2);
//                              抢占优先级设置为1,次优先级设置为2
//-------------------------------------------------------------------------------------------------------------------
void interrupt_set_priority (IRQn_Type irqn, uint8 priority)
{
    NVIC_SetPriority(irqn, priority);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       中断组初始化 clock_init 内部调用
// @param       void
// @return      void
// Sample usage:                interrupt_init();
//-------------------------------------------------------------------------------------------------------------------
void interrupt_init (void)
{
    //NVIC_SetPriorityGrouping(4);

}

//-------------------------------------------------------------------------------------------------------------------
// @brief       全局中断使能
// @param       void            还原的嵌套层 如果传入 0 则直接清空嵌套层数开启总中断
// @return      void
// Sample usage:                interrupt_global_enable();
//-------------------------------------------------------------------------------------------------------------------
void interrupt_global_enable (void)
{
    if(interrupt_nest_count)
        interrupt_nest_count--;

    if(interrupt_nest_count == 0)
    {
        extern void en_interrupt();
        en_interrupt();
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       全局中断屏蔽
// @param       void
// @return      void
// Sample usage:                interrupt_disable_all();
//-------------------------------------------------------------------------------------------------------------------
void interrupt_global_disable (void)
{
    if(!interrupt_nest_count)
    {
        extern void dis_interrupt();
        dis_interrupt();
    }
    interrupt_nest_count++;
    //返回当前mstatus寄存器的值
//    uint32 temp;
//    temp = __get_MSTATUS();
//    return temp;
}
