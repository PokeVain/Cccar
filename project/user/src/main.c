    /*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             main
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
* @Software         MounRiver Studio V1.7
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
********************************************************************************************************************/

#include "SystemOS.h"

/*-----------------------------Static thread--------------------------*/

struct rt_thread distance_thread;
struct rt_thread mt9v03x_dvp_thread;
struct rt_thread communication_thread;
struct rt_thread show_thread;

ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t distance_stack[2048];         /*线程栈*/
static rt_uint8_t mt9v03x_dvp_stack[2048];         /*线程栈*/
static rt_uint8_t communication_stack[2048];         /*线程栈*/
static rt_uint8_t show_stack[2048];         /*线程栈*/
/*-----------------------------Static thread--------------------------*/


/* ---------------------------Private variables ----------------------*/
rt_sem_t    mt9v03x_dvp_sem = RT_NULL;                          /*摄像头信号量*/

rt_event_t  event = RT_NULL;                                    /*ALL_EVENT*/

rt_int16_t  encoder_data_r, encoder_data_l;
float  encoder_data_r_, encoder_data_l_;

/* ---------------------------Private variables ----------------------*/

static void mt9v03x_dvp_thread_entry(void* parameter)
{
    while(1)
    {
        rt_sem_take(mt9v03x_dvp_sem, RT_WAITING_FOREVER);   /*获取信号量*/
        gpio_set_level(C14, 1);

        TaskImplement_HIMFunction();
        RealTimeOS_OperationSystem();

        PID_place_steer(&pid_Steer, m_f32LineAllError);         /*steer*/
        pwm_set_duty(TIM1_PWM_CH1_A8, 825 + limit((int)pid_Steer.actual_val,72));

        if(_HMIControl.HMIControl_Enable == _HMIControl_Disable)
        {
            TaskStopend_StopFunction();
        }

        if(EventStatusTask[_Event_TridentRoad]._EventCurrentStatus == _Task_TridentRoad_LExit ||
           EventStatusTask[_Event_TridentRoad]._EventCurrentStatus == _Task_TridentRoad_RExit ||
           EventStatusTask[_Event_TridentRoad]._EventCurrentStatus == _Task_TridentRoad_REntry)
        {
            rt_event_send(event,second_start);
        }
        gpio_set_level(C14, 0);
    }
}

static void Speed_Feedback_entry(void* parameter)          /*5ms中断*/
{
    gpio_set_level(C15, 1);

    encoder_data_r = -encoder_get_count(TIM1_ENCOEDER);
    encoder_data_l = encoder_get_count(TIM8_ENCOEDER);
    encoder_clear_count(TIM1_ENCOEDER);
    encoder_clear_count(TIM8_ENCOEDER);

    encoder_data_l_ = 0.08*(float)encoder_data_l + 0.92*encoder_data_l_;
    encoder_data_r_ = 0.08*(float)encoder_data_r + 0.92*encoder_data_r_;

    _Runing_Distance += (encoder_data_l + encoder_data_r) / 2;

    Ackermann_turn(&s);       /*阿克曼转向*/

    PID_Place(&pid_Speed_r,encoder_data_r_);
    PID_Place(&pid_Speed_l,encoder_data_l_);

       /*  R   */
    pwm_set_duty(TIM4_PWM_CH2_D13, (5000 + limit((int)pid_Speed_r.actual_val,4500)));
       /*  L   */
    pwm_set_duty(TIM4_PWM_CH4_D15, (5000 + limit((int)pid_Speed_l.actual_val,4500)));

//    pwm_set_duty(TIM4_PWM_CH1_D12, 5000);       /*  R   */
//     pwm_set_duty(TIM4_PWM_CH2_D13, 5000);
//     pwm_set_duty(TIM4_PWM_CH3_D14, 5000);       /*  L   */
//     pwm_set_duty(TIM4_PWM_CH4_D15, 5000);

    gpio_set_level(C15, 0);
}


static void imu_entry(void* parameter)                                    /*10ms中断,pit5*/
{
    gpio_set_level(C15, 1);
    Mpu6050_getdata();
    gpio_set_level(C15,0);
}

/*
 * 还带修改
 */
static void distance_thread_entry(void* parameter)        /*distance_thread*/
{
    rt_uint32_t e;
    while(1)
    {
        if(rt_event_recv(event,SpeedControl,(RT_EVENT_FLAG_AND),RT_WAITING_FOREVER,&e) == RT_EOK)
        {
            if (_StatusMonitor._fpEventControl == _Event_RampWay)
            {
                s.present_speed = 50;
                rt_thread_delay(600);
                rt_event_send(event,RECOVER);
                rt_thread_delay(400);
                _StatusMonitor._fpEventControl = _Event_NormalStatus;
            }
            else if (EventStatusTask[_Event_Roundabout]._EventCurrentStatus == _Task_LRoundabout_Ready
               || EventStatusTask[_Event_Roundabout]._EventCurrentStatus == _Task_RRoundabout_Ready)
            {
                s.present_speed = 60;
                rt_event_recv(event,second_start,(RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR),RT_WAITING_NO,&e);
                rt_thread_delay(500);
                rt_event_send(event,RECOVER);
                rt_thread_delay(300);
            }

            else if (_Header_Element == 1)
            {
                s.present_speed = 55;
                rt_event_recv(event,second_start,(RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR),RT_WAITING_FOREVER,&e);
                if (EventStatusTask[_Event_TridentRoad]._EventCurrentStatus == _Task_TridentRoad_REntry)
                {
//                    s.speed_downlimit = s.second_speed + 5;
                    s.speed_downlimit = s.second_speed;
                    sta = 1;
                }
                else
                {
                    dis.target_val =dis.target_val_s;
                    s.speed_downlimit = s.second_speed;
                }
                rt_event_send(event,RECOVER);
                rt_thread_delay(100);
            }
            else if (_Header_Element == 5)
            {
                if(ranging_counter < (int)dis.finish)
                {
                    uart_write_byte(UART_2,0x64);
                }
            }

            else
            {
                dis.err = (float)ranging_counter - dis.target_val;
                if (dis.err > 0)
                {
                    s.present_speed = s.speed_downlimit + limit_ab(dis.pz * dis.err, -10, 10);
                }
                else
                {
                    s.present_speed = s.speed_downlimit + limit_ab(dis.pj * dis.err, -10, 10);
                }

            }

            if (sta == 1)
            {
                if(ranging_counter < (int)dis.tar_s_end)
                {
                    count++;
                    if (count > 2)
                    {
                        uart_write_byte(UART_2,0x63);
                        sta = 0;
                    }

                }
                else
                {
                    count = 0;
                }
            }

            if ((encoder_data_r_ < 10 && pid_Speed_r.actual_val > 4000) || (encoder_data_l_ < 10 && pid_Speed_l.actual_val > 4000))
            {
                VehicleInformation.StartDeparture = _VehicleInformation_Stop;
            }
            if (VehicleInformation.StartDeparture == _VehicleInformation_Stop)
            {
                rt_event_send(event,STOP);
            }
//            s.present_speed = 80 + limit_ab ((dis.p * ((float)ranging_counter - dis.target_val)), -50, 5);

//            if (ranging_counter < (uint16)dis.distance_keep)
//            {
//                rt_event_send(event,Decelerate);
//            }
//            else if (ranging_counter > (uint16)(dis.distance_keep+200) && ranging_counter < 3000)
//            {
//                rt_event_send(event,Accelerate);
//            }
            rt_thread_delay(5);
        }


    }
}

static void show_thread_entry(void* parameter)        /*distance_thread*/
{
    while(1)
    {
        if(_HMIControl.HMIControl_Enable == _HMIControl_Enable)
        {
            _HMIDisplay[_HMIControl.KeyStatus_CurrentTask].TaskFunction();
        }
        printf("%d,%d,%d,%d\n",_StatusMonitor._EventStatus._EventCurrentStatus,_StatusMonitor._fpEventControl,ranging_counter,_Header_Element);
//        printf("%d,%d\n",Gyro.y, _StatusMonitor._fpEventControl );
//        printf("%d,%d,%d,%f\n",Gyro.y,_Runing_Distance,ranging_counter,s.present_speed);
//        printf("%d,%d,%d,%d\n",Gyro.y,_StatusMonitor._EventStatus._EventCurrentStatus,_StatusMonitor._fpEventControl,ranging_counter);
//        printf("%f,%f,%f,%f,%f,%f\n",pid_Speed_r.target_val,pid_Speed_l.target_val,pid_Speed_r.actual_val,pid_Speed_l.actual_val,encoder_data_r_,encoder_data_l_);
//        printf("%f,%f,%f\n",s.present_speed,encoder_data_r_,encoder_data_l_);
//        printf("%d,%d,%f,%f,%f,%f\n",encoder_data_r,encoder_data_l,pid_Speed_r.actual_val,pid_Speed_l.actual_val,encoder_data_r_,encoder_data_l_);
//        printf("%d,%d,%f,%f,%f,%f\n",encoder_data_r,encoder_data_l,pid_Speed_r.target_val,pid_Speed_l.target_val,encoder_data_r_,encoder_data_l_);
        rt_thread_delay(20);
    }

}

static void communication_thread_entry(void* parameter)        /*distance_thread*/
{

}


/*----------------------------------------------------------------------------------------------------------*/
/*
 * uart还待修改
 * */
static void STOP_thread_entry(void* parameter)          /*STOP*/
{
    rt_uint32_t e;
    while(1)
    {
        if(rt_event_recv(event,STOP,(RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR),RT_WAITING_FOREVER,&e) == RT_EOK)
        {
            rt_event_recv(event,SpeedControl,(RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR),RT_WAITING_NO,&e);
            uart_write_byte(UART_2,0x04);
            s.present_speed = 0;
            rt_thread_mdelay(180);
            pid_Speed_r.integral = 0;
            pid_Speed_l.integral = 0;
            VehicleInformation.StartDeparture = _VehicleInformation_Stop;
        }
    }
}


static void GO_thread_entry(void* parameter)            /*GO*/
{
    rt_uint32_t e;
    while(1)
    {
        if(rt_event_recv(event,GO,(RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR),RT_WAITING_FOREVER,&e) == RT_EOK)
        {
            for(; s.present_speed < s.speed_downlimit;s.present_speed = (int)s.present_speed + 2)
            {
                rt_thread_mdelay(10);
            }
            rt_event_send(event,SpeedControl);
        }
    }
}

static void recover_thread_entry(void* parameter)            /*recover*/
{
    rt_uint32_t e;
    while(1)
    {
        if(rt_event_recv(event,RECOVER,(RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR),RT_WAITING_FOREVER,&e) == RT_EOK)
        {
            for(; s.present_speed < s.speed_downlimit;s.present_speed = (int)s.present_speed + 2)
            {
                rt_thread_mdelay(10);
            }
        }
    }
}


/*-----------------------------------------------init-----------------------------------------------------------*/

int main(void)          /*参数初始化*/
{
    //人机界面初始化
    InitalParameter_HIMControl();
    ElementInital_UltraWideParam();
    InitalParam_TaskFunction();
    Threshold_Limt = 130;                    /*大津法限幅*/
    img_raw.data = mt9v03x_output_image[0];
}

int app_init(void)
{
    rt_thread_t tid = RT_NULL;
    rt_timer_t  timer = RT_NULL;

    mt9v03x_dvp_sem = rt_sem_create("mt9v03x_dvp_sem", 0 ,RT_IPC_FLAG_PRIO);    /*mt9v03x_dvp_sem    init   0  */

    event = rt_event_create("event", RT_IPC_FLAG_PRIO);                         /*all event*/

    tid = rt_thread_create("GO",                                /*GO_thread_entry*/
                            GO_thread_entry,
                            RT_NULL,
                            512,
                            1,
                            20);
    if(tid != RT_NULL)   rt_thread_startup(tid);

    tid = rt_thread_create("STOP",                              /*STOP_thread_entry*/
                            STOP_thread_entry,
                            RT_NULL,
                            512,
                            1,
                            20);
    if(tid != RT_NULL)   rt_thread_startup(tid);

    tid = rt_thread_create("recover",                              /*recover*/
                            recover_thread_entry,
                            RT_NULL,
                            512,
                            2,
                            20);
    if(tid != RT_NULL)   rt_thread_startup(tid);

/*静态*/
    rt_thread_init(&communication_thread,                            /*communication_stack*/
                    "communication",
                    communication_thread_entry,
                    RT_NULL,
                    communication_stack,
                    sizeof(communication_stack),
                    1,
                    5);

    rt_thread_startup(&communication_thread);

    rt_thread_init(&distance_thread,                            /*distance_thread_entry*/
                    "distance",
                    distance_thread_entry,
                    RT_NULL,
                    distance_stack,
                    sizeof(distance_stack),
                    1,
                    20);

    rt_thread_startup(&distance_thread);

    rt_thread_init(&mt9v03x_dvp_thread,                         /*mt9v03x_dvp_thread*/
                    "mt9v03x_dvp",
                    mt9v03x_dvp_thread_entry,
                    RT_NULL,
                    mt9v03x_dvp_stack,
                    sizeof(mt9v03x_dvp_stack),
                    5,
                    20);

    rt_thread_startup(&mt9v03x_dvp_thread);

    rt_thread_init(&show_thread,                            /*show*/
                    "show",
                    show_thread_entry,
                    RT_NULL,
                    show_stack,
                    sizeof(show_stack),
                    8,
                    20);

    rt_thread_startup(&show_thread);

/*定时器*/
    /*5ms*/
    timer = rt_timer_create("speed_timer",                       /*speed_timer*/
                            Speed_Feedback_entry,
                            RT_NULL,
                            5,
                            RT_TIMER_FLAG_PERIODIC);

    if(timer != RT_NULL) rt_timer_start(timer);

    timer = rt_timer_create("imu",                       /*imu*/
                            imu_entry,
                            RT_NULL,
                            10,
                            RT_TIMER_FLAG_PERIODIC);

    if(timer != RT_NULL) rt_timer_start(timer);

    return 0;
}

INIT_APP_EXPORT(app_init);


int device_init(void)
{
    ips114_init();                                                  /*lcd*/
    ShowLoge_DSPFunction(LEGO, 0, 0, 111, 135);
    gpio_init(E1, GPO, 0, GPIO_PIN_CONFIG);             /*beep*/

    gpio_init(C14, GPO, 0, GPIO_PIN_CONFIG);             /*led*/
    gpio_init(C15, GPO, 0, GPIO_PIN_CONFIG);

    gpio_init(E2, GPI, 0, GPI_PULL_UP);                 /*key*/
    gpio_init(E3, GPI, 0, GPI_PULL_UP);
    gpio_init(E4, GPI, 0, GPI_PULL_UP);
    gpio_init(E5, GPI, 0, GPI_PULL_UP);
    gpio_init(E6, GPI, 0, GPI_PULL_UP);
    gpio_init(C13, GPI, 0, GPI_PULL_UP);
    ips114_show_string(120,0,"key/led/beep_ok");

    BEEP_OFF;

    adc_init(ADC_IN10_C0);
    adc_init(ADC_IN11_C1);
    adc_init(ADC_IN12_C2);
    adc_init(ADC_IN13_C3);

//    pwm_init(TIM1_PWM_CH1_A8, 50, 827 );               /*steer*/      /* 760   830 900*/


      gpio_init(D12,GPO,1,GPIO_PIN_CONFIG);
      gpio_init(D14,GPO,1,GPIO_PIN_CONFIG);              /*l*/       /*motor*/
    pwm_init(TIM4_PWM_CH2_D13, 17000, 5000);             /*r*/
    pwm_init(TIM4_PWM_CH4_D15, 17000, 5000);

    ips114_show_string(120,15,"steer/motor_ok");

    encoder_init_dir(TIM1_ENCOEDER, TIM1_CH1_ENCOEDER_E9, TIM1_CH2_ENCOEDER_E11);        /*encoder*/
    encoder_init_dir(TIM8_ENCOEDER, TIM8_CH1_ENCOEDER_C6, TIM8_CH2_ENCOEDER_C7);
    ips114_show_string(120,30,"encoder_ok");

//    W25Q64_init();                                                  /*W25Q64*/
////    StoreFunction_StareReadLast();
//    rt_kprintf("W25Q64_ok\n");
    if(mt9v03x_init_dvp() == 0)                                             /*mt9v03x*/
        ips114_show_string(120,75,"mt9v03x_ok");
    else
        ips114_show_string(120,75,"mt9v03x_fail");
    if(mpu6050_init() == 0)                                                /*Mpu6050*/
        ips114_show_string(120,60,"mpu6050_ok");
    else
        ips114_show_string(120,60,"mpu6050_fail");

    uart_init(UART_7, 115200, UART7_TX_E12,UART7_RX_E13);             /*蓝牙/无线串口*/

    uart_rx_interrupt(UART_7, 1);

    ips114_show_string(120,45,"uart_ok");

    BEEP_OFF;

    return 0;
}
INIT_DEVICE_EXPORT(device_init);
