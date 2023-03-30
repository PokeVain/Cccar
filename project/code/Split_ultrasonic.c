#include "Split_ultrasonic.h"

uint16 ranging_counter = 0;                             // �������

uint8 receipt_data;
uint8 dat[3];
uint8 num;
uint8 count = 0;

_Distance dis = {0,600,600,900,0.01, 0.03, 0 ,500};

uint8 sta = 0;

void uart_ultrasonic_handler (void)     /*split ultrasonic*/
{
    uart_query_byte(SPLIT_ULTRASONIC_UART, &receipt_data);
    dat[num] = receipt_data;
    if(dat[0] != 0xa5)  num = 0;                                                    //����һ�������Ƿ�Ϊ0xa5
    else                num++;

    if(num == 3)                                                                    //������� ��ʼ��������
    {
        num = 0;
        ranging_counter = dat[1]<<8 | dat[2];                                       //�õ�������ģ�����ľ���
    }
}

