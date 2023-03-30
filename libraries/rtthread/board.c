/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-07-24     Tanek        the first version
 * 2018-11-12     Ernest Chen  modify copyright
 */
 
#include "board.h"
#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>

#include "zf_driver_uart.h"
#include "zf_common_debug.h"


//finsh组件接收串口数据，是通过在串口中断内发送邮件，finsh线程接收邮件进行获取的
rt_mailbox_t uart_mb;


// core clock.
extern uint32_t SystemCoreClock;

static uint32_t _SysTick_Config(rt_uint32_t ticks)
{
    NVIC_SetPriority(SysTicK_IRQn,0xf0);
    NVIC_SetPriority(Software_IRQn,0xf0);
    NVIC_EnableIRQ(SysTicK_IRQn);
    NVIC_EnableIRQ(Software_IRQn);
    SysTick->CTLR=0;
    SysTick->SR=0;
    SysTick->CNT=0;
    SysTick->CMP=ticks-1;
    SysTick->CTLR=0xF;
    return 0;
}

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#define RT_HEAP_SIZE (2048)
static uint32_t rt_heap[RT_HEAP_SIZE];     // heap default size: 8K(2048 * 4)
RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif

/**
 * This function will initial your board.
 */
void rt_hw_board_init()
{
    /* System Tick Configuration */
    _SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);
    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif
#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif

#ifdef RT_USING_CONSOLE
    debug_init();
    uart_rx_interrupt(DEBUG_UART_INDEX, 1);

    uart_mb = rt_mb_create("uart_mb", 10, RT_IPC_FLAG_FIFO);

    //rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif
}


void SysTick_Handler(void) __attribute__((interrupt()));
void SysTick_Handler(void)
{
//    GET_INT_SP();
    /* enter interrupt */
    rt_interrupt_enter();
    SysTick->SR=0;
    rt_tick_increase();
    /* leave interrupt */
    rt_interrupt_leave();
//    FREE_INT_SP();

}



void rt_hw_console_output(const char *str)
{
    while(RT_NULL != *str)
    {
        if('\n' == *str)
        {
            uart_write_byte(DEBUG_UART_INDEX, '\r');
        }
        uart_write_byte(DEBUG_UART_INDEX, *str++);
    }
}

char rt_hw_console_getchar(void)
{
    rt_uint32_t dat;
    rt_mb_recv(uart_mb, &dat, RT_WAITING_FOREVER);
    //uart_getchar(DEBUG_UART, &dat);
    return (char)dat;
}

#if SW_UART3
void USART3_IRQHandler(void) __attribute__((interrupt()));
void USART3_IRQHandler(void)
{
//    GET_INT_SP();
    /* enter interrupt */
    rt_interrupt_enter();

    rt_uint8_t dat;

    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        uart_read_byte(DEBUG_UART_INDEX, &dat);
        rt_mb_send(uart_mb, dat);           // 发送邮件
    }

    /* leave interrupt */
    rt_interrupt_leave();
//    FREE_INT_SP();
}
#else

void UART8_IRQHandler(void) __attribute__((interrupt()));
void UART8_IRQHandler(void)
{
//    GET_INT_SP();
    /* enter interrupt */
    rt_interrupt_enter();

    rt_uint8_t dat;

    if(USART_GetITStatus(UART8, USART_IT_RXNE) != RESET)
    {
        uart_read_byte(DEBUG_UART_INDEX, &dat);
        rt_mb_send(uart_mb, dat);           // 发送邮件
    }

    /* leave interrupt */
    rt_interrupt_leave();
//    FREE_INT_SP();
}

#endif

