#ifndef __COMMUNICATION_THREECAR_H__
#define __COMMUNICATION_THREECAR_H__

#include "rtthread.h"
#include "zf_driver_pwm.h"
#include "zf_driver_uart.h"
#include "TaskFunction.h"
extern rt_event_t  event;


#define wait_second             (1<<0)
#define GO                      (1<<1)
#define STOP                    (1<<2)
#define second_start            (1<<3)
#define Decelerate              (1<<4)
#define Accelerate              (1<<5)
#define Image_speed             (1<<6)
#define Distance_speed          (1<<7)
#define Finish                  (1<<8)
#define UP                      (1<<9)
#define RECOVER                 (1<<10)
#define ICM                     (1<<11)
#define SpeedControl            (1<<12)
#define breast_tape             (1<<13)
#define Roundabout              (1<<14)
#define _Roundabout             (1<<15)
#define _breast_tape            (1<<16)
#define second_end              (1<<17)

extern uint8_t _Header_Element;

void Message_Analysis(rt_uint8_t dat);

#endif /* __COMMUNICATION_THREECAR_H__ */
