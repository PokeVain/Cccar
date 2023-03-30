#ifndef __JJJ_PID_H__
#define __JJJ_PID_H__

#include "zf_common_function.h"
#include "math.h"

#define CAR_LENGTH  (200.0f)                                                                                                                                                                                                 //车身长，单位mm
#define CAR_WIDTH   (155.0f)
#define PI_JJJ (3.14159265358979)
#define A2R(x) (PI_JJJ * (x) / 180.0f)


typedef struct
{
    float target_val;               //目标值
    float actual_val;               //实际值
    float integral;                 //定义积分值

    float err;                      //定义偏差值
    float err_last;                 //  e(k-1)
    float err_previous;             //  e(k-2)

    float Kp;               //定义比例、积分、微分系数
    float Ki;               //定义比例、积分、微分系数
    float Kd;               //定义比例、积分、微分系数
    float limit;            //积分限幅
} _pid;

typedef struct
{
    float target_val;               //目标值
    float actual_val;               //输出值

    float err;                      //定义偏差值
    float err_last;                 //  e(k-1)

    float Kp;               //定义比例、积分、微分系数
    float Kd;               //定义比例、积分、微分系数

    float R;
} _pid_place;



typedef struct
{
    float speed_uplimit;        //速度上限
    float speed_downlimit;      //速度下限
    float present_speed;        //当前速度

    float Ackermann_speed;      //Ackermann转向速度
    float Ackermann_speed_;

    float steer_angle;          //舵机角度换算比   （差速大小调节）

    float second_speed;
} _speed;


extern _pid  pid_Speed_r,pid_Speed_l;
extern _pid_place pid_Steer;
extern _speed s;
extern int32_t _Runing_Distance;

void Ackermann_turn(_speed *s);

void PID_place_steer(_pid_place *p,float NowPlace);
void PID_Place(_pid *p,float NowPlace);
void PID_Increase(_pid *p,float NowPlace);



void Clear_integral(_pid *p);
void Change_D_val(_pid *p,float val);
void Change_I_val(_pid *p,float val);
void Change_P_val(_pid *p,float val);
void Change_target_val(_pid *p,float val);

#endif /*__JJJ_PID_H__*/
