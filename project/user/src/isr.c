/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             isr
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
*                   V1.1 2022.01.11  对调ch1和ch2的蓝牙回调函数位置
********************************************************************************************************************/

#include "zf_common_headfile.h"

extern rt_sem_t mt9v03x_dvp_sem;

void NMI_Handler(void)       __attribute__((interrupt()));
void HardFault_Handler(void) __attribute__((interrupt()));

void USART1_IRQHandler(void) __attribute__((interrupt()));
void USART2_IRQHandler(void) __attribute__((interrupt()));
//void USART3_IRQHandler(void) __attribute__((interrupt()));
void UART4_IRQHandler (void) __attribute__((interrupt()));
void UART5_IRQHandler (void) __attribute__((interrupt()));
void UART6_IRQHandler (void) __attribute__((interrupt()));
void UART7_IRQHandler (void) __attribute__((interrupt()));

void DVP_IRQHandler (void) __attribute__((interrupt()));
//void TIM1_BRK_IRQHandler        (void)  __attribute__((interrupt()));
void TIM1_UP_IRQHandler         (void)  __attribute__((interrupt()));
//void TIM1_TRG_COM_IRQHandler    (void)  __attribute__((interrupt()));
//void TIM1_CC_IRQHandler         (void)  __attribute__((interrupt()));
void TIM2_IRQHandler            (void)  __attribute__((interrupt()));
void TIM3_IRQHandler            (void)  __attribute__((interrupt()));
void TIM4_IRQHandler            (void)  __attribute__((interrupt()));
void TIM5_IRQHandler            (void)  __attribute__((interrupt()));
void TIM6_IRQHandler            (void)  __attribute__((interrupt()));
void TIM7_IRQHandler            (void)  __attribute__((interrupt()));
//void TIM8_BRK_IRQHandler        (void)  __attribute__((interrupt()));
void TIM8_UP_IRQHandler         (void)  __attribute__((interrupt()));
//void TIM8_TRG_COM_IRQHandler    (void)  __attribute__((interrupt()));
//void TIM8_CC_IRQHandler         (void)  __attribute__((interrupt()));
//void TIM9_BRK_IRQHandler        (void)  __attribute__((interrupt()));
void TIM9_UP_IRQHandler         (void)  __attribute__((interrupt()));
//void TIM9_TRG_COM_IRQHandler    (void)  __attribute__((interrupt()));
//void TIM9_CC_IRQHandler         (void)  __attribute__((interrupt()));
//void TIM10_BRK_IRQHandler       (void)  __attribute__((interrupt()));
void TIM10_UP_IRQHandler        (void)  __attribute__((interrupt()));
//void TIM10_TRG_COM_IRQHandler   (void)  __attribute__((interrupt()));
//void TIM10_CC_IRQHandler        (void)  __attribute__((interrupt()));

void EXTI0_IRQHandler(void) __attribute__((interrupt()));
void EXTI1_IRQHandler(void) __attribute__((interrupt()));
void EXTI2_IRQHandler(void) __attribute__((interrupt()));
void EXTI3_IRQHandler(void) __attribute__((interrupt()));
void EXTI4_IRQHandler(void) __attribute__((interrupt()));
void EXTI9_5_IRQHandler(void) __attribute__((interrupt()));
void EXTI15_10_IRQHandler(void) __attribute__((interrupt()));


void UART6_IRQHandler (void)        /*split ultrasonic*/
{
    /* enter interrupt */
    rt_interrupt_enter();

    if(USART_GetITStatus(UART6, USART_IT_RXNE) != RESET)
    {
        uart_ultrasonic_handler();
        USART_ClearITPendingBit(UART6, USART_IT_RXNE);
    }
    /* leave interrupt */
    rt_interrupt_leave();
}


void USART1_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {

        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
    /* leave interrupt */
    rt_interrupt_leave();
}

void USART2_IRQHandler(void)
{

    /* enter interrupt */
    rt_interrupt_enter();

    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        rt_uint8_t dat;
        uart_query_byte(UART_2, &dat);
        Message_Analysis(dat);
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
    /* leave interrupt */
    rt_interrupt_leave();

}








void UART4_IRQHandler (void)
{

    /* enter interrupt */
    rt_interrupt_enter();
    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(UART4, USART_IT_RXNE);
    }
    /* leave interrupt */
    rt_interrupt_leave();
}
void UART5_IRQHandler (void)
{
    /* enter interrupt */
    rt_interrupt_enter();
    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
    {
        mt9v03x_uart_callback_dvp();
        USART_ClearITPendingBit(UART5, USART_IT_RXNE);
    }
    /* leave interrupt */
    rt_interrupt_leave();
}

void UART7_IRQHandler (void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    if(USART_GetITStatus(UART7, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(UART7, USART_IT_RXNE);
    }
    /* leave interrupt */
    rt_interrupt_leave();
}
//void UART8_IRQHandler (void)
//{
//
//    /* enter interrupt */
//    rt_interrupt_enter();
//    if(USART_GetITStatus(UART8, USART_IT_RXNE) != RESET)
//    {
//
//        USART_ClearITPendingBit(UART8, USART_IT_RXNE);
//    }
//    /* leave interrupt */
//    rt_interrupt_leave();
//
//}



void DVP_IRQHandler(void)
{

    /* enter interrupt */
    rt_interrupt_enter();
    if (DVP->IFR & RB_DVP_IF_FRM_DONE)
    {
//        camera_dvp_handler();
        rt_sem_release(mt9v03x_dvp_sem);
        DVP->IFR &= ~RB_DVP_IF_FRM_DONE;
    }
    /* leave interrupt */
    rt_interrupt_leave();

}


void EXTI2_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();
    if(SET == EXTI_GetITStatus(EXTI_Line2))
    {
        EXTI_ClearITPendingBit(EXTI_Line2);
    }
    /* leave interrupt */
    rt_interrupt_leave();
}

void EXTI3_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();
    if(SET == EXTI_GetITStatus(EXTI_Line3))
    {
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
    /* leave interrupt */
    rt_interrupt_leave();

}
void EXTI9_5_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();
    if(SET == EXTI_GetITStatus(EXTI_Line5))
    {
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
    if(SET == EXTI_GetITStatus(EXTI_Line6))
    {
        EXTI_ClearITPendingBit(EXTI_Line6);
    }
    if(SET == EXTI_GetITStatus(EXTI_Line7))
    {
        EXTI_ClearITPendingBit(EXTI_Line7);
    }
    if(SET == EXTI_GetITStatus(EXTI_Line8))
    {
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
    if(SET == EXTI_GetITStatus(EXTI_Line9))
    {
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
    /* leave interrupt */
    rt_interrupt_leave();
}


void EXTI0_IRQHandler(void)
{

    /* enter interrupt */
    rt_interrupt_enter();
    if(SET == EXTI_GetITStatus(EXTI_Line0))
    {
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
    /* leave interrupt */
    rt_interrupt_leave();

}

void EXTI1_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();
    if(SET == EXTI_GetITStatus(EXTI_Line1))
    {
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
    /* leave interrupt */
    rt_interrupt_leave();
}



void EXTI4_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();
    if(SET == EXTI_GetITStatus(EXTI_Line4))
    {
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
    /* leave interrupt */
    rt_interrupt_leave();
}



void EXTI15_10_IRQHandler(void)
{

    /* enter interrupt */
    rt_interrupt_enter();
    if(SET == EXTI_GetITStatus(EXTI_Line10))
    {
        EXTI_ClearITPendingBit(EXTI_Line10);

    }
    if(SET == EXTI_GetITStatus(EXTI_Line11))
    {
        EXTI_ClearITPendingBit(EXTI_Line11);

    }
    if(SET == EXTI_GetITStatus(EXTI_Line12))
    {
        EXTI_ClearITPendingBit(EXTI_Line12);

    }
    if(SET == EXTI_GetITStatus(EXTI_Line13))
    {
        EXTI_ClearITPendingBit(EXTI_Line13);

    }
    if(SET == EXTI_GetITStatus(EXTI_Line14))
    {
        EXTI_ClearITPendingBit(EXTI_Line14);

    }
    if(SET == EXTI_GetITStatus(EXTI_Line15))
    {
        EXTI_ClearITPendingBit(EXTI_Line15);

    }
    /* leave interrupt */
    rt_interrupt_leave();

}

void TIM1_UP_IRQHandler(void)
{

    /* enter interrupt */
    rt_interrupt_enter();
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
    /* leave interrupt */
    rt_interrupt_leave();

}


void TIM2_IRQHandler(void)
{

    /* enter interrupt */
    rt_interrupt_enter();

    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
       TIM_ClearITPendingBit(TIM2, TIM_IT_Update );

    }
    /* leave interrupt */
    rt_interrupt_leave();

}

void TIM3_IRQHandler(void)
{

    /* enter interrupt */
    rt_interrupt_enter();
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
       TIM_ClearITPendingBit(TIM3, TIM_IT_Update );
    }
    /* leave interrupt */
    rt_interrupt_leave();

}


void TIM4_IRQHandler(void)
{

    /* enter interrupt */
    rt_interrupt_enter();
    if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
       TIM_ClearITPendingBit(TIM4, TIM_IT_Update );


    }
    /* leave interrupt */
    rt_interrupt_leave();

}

void TIM5_IRQHandler(void)
{

    /* enter interrupt */
    rt_interrupt_enter();
    if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
       TIM_ClearITPendingBit(TIM5, TIM_IT_Update );


    }
    /* leave interrupt */
    rt_interrupt_leave();

}

void TIM6_IRQHandler(void)
{

    /* enter interrupt */
    rt_interrupt_enter();
    if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    {
       TIM_ClearITPendingBit(TIM6, TIM_IT_Update );


    }
    /* leave interrupt */
    rt_interrupt_leave();

}

void TIM7_IRQHandler(void)
{

    /* enter interrupt */
    rt_interrupt_enter();
    if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
    {
       TIM_ClearITPendingBit(TIM7, TIM_IT_Update );


    }
    /* leave interrupt */
    rt_interrupt_leave();

}


void TIM8_UP_IRQHandler(void)
{

    /* enter interrupt */
    rt_interrupt_enter();
    if(TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM8, TIM_IT_Update);

    }
    /* leave interrupt */
    rt_interrupt_leave();

}


void TIM9_UP_IRQHandler(void)
{

    /* enter interrupt */
    rt_interrupt_enter();
    if(TIM_GetITStatus(TIM9, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM9, TIM_IT_Update);

    }
    /* leave interrupt */
    rt_interrupt_leave();

}


void TIM10_UP_IRQHandler(void)
{

    /* enter interrupt */
    rt_interrupt_enter();
    if(TIM_GetITStatus(TIM10, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM10, TIM_IT_Update);

    }
    /* leave interrupt */
    rt_interrupt_leave();

}


/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
void NMI_Handler(void)
{

    rt_interrupt_enter();
    rt_kprintf(" NMI Handler\r\n");
    rt_interrupt_leave();

}

/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
void HardFault_Handler(void)
{

    rt_interrupt_enter();
    rt_kprintf(" hardfult\r\n");
    rt_interrupt_leave();


}

