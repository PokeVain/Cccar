#ifndef __SPLIT_ULTRASONIC_H__
#define __SPLIT_ULTRASONIC_H__

#include "zf_driver_uart.h"
#include "zf_common_function.h"
#include "JJJ_pid.h"
#include "ImageProcess.h"
#include "Communication_doublecar.h"
#include "rtthread.h"
#include "TaskFunction.h"

extern rt_event_t event;

#define SPLIT_ULTRASONIC_UART       UART_6
#define SPLIT_ULTRASONIC_BAUD       115200
#define SPLIT_ULTRASONIC_TX         UART6_TX_E10        // ������ȥ�� UART-RX ���� Ҫ���ڵ�Ƭ�� TX ��
#define SPLIT_ULTRASONIC_RX         UART6_RX_E11        // ������ȥ�� UART-TX ���� Ҫ���ڵ�Ƭ�� RX ��

extern uint16 ranging_counter;                             // �������

extern uint8 receipt_data;
extern uint8 dat[3];
extern uint8 num;

extern uint8 count;

typedef struct
{
    float Dis_encoder;             //����
    float target_val;
    float target_val_s;
    float tar_s_end;
    float pz;
    float pj;
    float err;

    float finish;
} _Distance;

extern _Distance dis;
extern uint8 sta;
void uart_ultrasonic_handler (void);

#endif  /*__SPLIT_ULTRASONIC_H__*/
