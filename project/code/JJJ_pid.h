#ifndef __JJJ_PID_H__
#define __JJJ_PID_H__

#include "zf_common_function.h"
#include "math.h"

#define CAR_LENGTH  (200.0f)                                                                                                                                                                                                 //��������λmm
#define CAR_WIDTH   (155.0f)
#define PI_JJJ (3.14159265358979)
#define A2R(x) (PI_JJJ * (x) / 180.0f)


typedef struct
{
    float target_val;               //Ŀ��ֵ
    float actual_val;               //ʵ��ֵ
    float integral;                 //�������ֵ

    float err;                      //����ƫ��ֵ
    float err_last;                 //  e(k-1)
    float err_previous;             //  e(k-2)

    float Kp;               //������������֡�΢��ϵ��
    float Ki;               //������������֡�΢��ϵ��
    float Kd;               //������������֡�΢��ϵ��
    float limit;            //�����޷�
} _pid;

typedef struct
{
    float target_val;               //Ŀ��ֵ
    float actual_val;               //���ֵ

    float err;                      //����ƫ��ֵ
    float err_last;                 //  e(k-1)

    float Kp;               //������������֡�΢��ϵ��
    float Kd;               //������������֡�΢��ϵ��

    float R;
} _pid_place;



typedef struct
{
    float speed_uplimit;        //�ٶ�����
    float speed_downlimit;      //�ٶ�����
    float present_speed;        //��ǰ�ٶ�

    float Ackermann_speed;      //Ackermannת���ٶ�
    float Ackermann_speed_;

    float steer_angle;          //����ǶȻ����   �����ٴ�С���ڣ�

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
