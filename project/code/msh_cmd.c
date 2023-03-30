#include "msh_cmd.h"

int G(void)
{
    rt_event_send(event, GO);   /**/
    return 0;
}


int S(void)
{
    rt_event_send(event, STOP);
    return 0;
}


int steer(int argc, char *argv[])
{
    uint16 steer;
    steer = str_to_uint(argv[1]);
    pwm_set_duty(TIM1_PWM_CH1_A8, steer);

    return 0;
}

int set(int argc, char *argv[])
{
    s.present_speed = str_to_uint(argv[1]);   /*设定速度*/
    return 0;
}

int pid_l(int argc, char *argv[])
{
    Change_P_val(&pid_Speed_l, (str_to_float(argv[1])));
    Change_I_val(&pid_Speed_l, (str_to_float(argv[2])));
    Change_D_val(&pid_Speed_l, (str_to_float(argv[3])));
    return 0;
}

int pid_r(int argc, char *argv[])
{
    Change_P_val(&pid_Speed_r, (str_to_float(argv[1])));
    Change_I_val(&pid_Speed_r, (str_to_float(argv[2])));
    Change_D_val(&pid_Speed_r, (str_to_float(argv[3])));
    return 0;
}

MSH_CMD_EXPORT(steer,  steer)
MSH_CMD_EXPORT(pid_l,  PID_L)
MSH_CMD_EXPORT(pid_r,  PID_R)
MSH_CMD_EXPORT(set, set parameter);
MSH_CMD_EXPORT(G,  GO__);
MSH_CMD_EXPORT(S,  STOP__);
