/*
 * @brief 双车通信
 * @note 还需修改哦
 */
#include "Communication_doublecar.h"

uint8_t _Header_Element = 0;

void Message_Analysis(rt_uint8_t dat)
{
    switch (dat)
    {
        case 0x02:
            rt_event_send(event, GO);
            uart_write_byte(UART_2,0x01);
            break;
        case 0x04:
            rt_event_send(event, STOP);
            uart_write_byte(UART_2,0x01);
            break;
        case 0x08:
            rt_event_send(event, second_start);
            uart_write_byte(UART_2,0X07);
            break;
        case 0x55:
            rt_event_send(event, STOP);       /*完赛*/
            uart_write_byte(UART_2,0x58);
            break;
        case 0x36:                          /*三岔*/
            _Header_Element = 1;
            break;
        case 0x37:                          /*三岔*/
            _Header_Element = 2;
            break;
        case 0x38:
            //左环岛
            _Header_Element = 3;
            break;
        case 0x39:
            _Header_Element = 4;
            break;
        case 0x65:
            _Header_Element = 5;
            break;
        case 0x66:
            _CrossWalkCircle = 2;
            _Header_Element = 2;
            break;
//        case 0x11:
//            breast_tape_ = 0;
//            break;
//        case 0x12:
//            breast_tape_ = 2;
//            break;
    }
}
