/*
 * PID控制
 */
#include "JJJ_pid.h"

_pid pid_Speed_r      = {0,0,0        ,0,0,0      ,90,2.7,50    ,1500};
_pid pid_Speed_l      = {0,0,0        ,0,0,0      ,90,2.7,50    ,1500};

_pid_place pid_Steer           = {0,0,0,0      ,0.578,0,0.488};

_speed s = {85,70,0   ,0,0,   0.411, 67};

int32_t _Runing_Distance;

void Ackermann_turn(_speed *s)
{
    s->Ackermann_speed = s->present_speed * (77.5/(77.5+200.0/tanf(A2R(  s->steer_angle * fabsf(limit(pid_Steer.actual_val,72)) ))));
    if (pid_Steer.actual_val < 0)           /*差速配合舵机*/
        s->Ackermann_speed = -s->Ackermann_speed;

    s->Ackermann_speed_ = 0.4 * s->Ackermann_speed + 0.6 * s->Ackermann_speed_;

    Change_target_val(&pid_Speed_r, s->present_speed + s->Ackermann_speed_);
    Change_target_val(&pid_Speed_l, s->present_speed - s->Ackermann_speed_);
}

void PID_Place(_pid *p,float NowPlace)
{
    p->err = p->target_val - NowPlace;                                                  //求出误差

    p->integral += p->err;                                                              //求误差累加和

    p->integral = limit(p->integral,p->limit);                                          //积分限幅

    p->actual_val = p->Kp * p->err
                  + p->Ki * p->integral
                  + p->Kd * (p->err-p->err_last);                                       //主体计算

    p->err_last = p->err;                                                               //误差传递
}

void PID_place_steer(_pid_place *p,float NowPlace)
{
    p->err = p->target_val - NowPlace;                                                  //求出误差

    p->Kd = 0.7 + myabs(p->err)*0.015;

    p->actual_val = (p->Kp + ((p->err*p->err)/5000) * p->R) * p->err + p->Kd * (p->err - p->err_last);                                       //主体计算

    p->err_last = p->err;                                                               //误差传递
}



void PID_Increase(_pid *p,float NowPlace)
{
    p->err = p->target_val - NowPlace;                                                  //求出误差

    p->actual_val = p->Kp * (p->err - p->err_last)                                      //主体计算
                  + p->Ki *  p->err
                  + p->Kd * (p->err - 2*p->err_last + p->err_previous);

    p->err_previous = p->err_last;                                                      //误差传递
    p->err_last = p->err;
}


void Change_target_val(_pid *p,float val)
{
    p->target_val = val;
}

void Change_D_val(_pid *p,float val)
{
    p->Kd = val;
}
void Change_P_val(_pid *p,float val)
{
    p->Kp = val;
}
void Change_I_val(_pid *p,float val)
{
    p->Ki = val;
}

void Clear_integral(_pid *p)
{
    p->integral = 0;
}

