#include "HMIControl.h"


//车体信息
__VehicleInformation_Struct VehicleInformation;

__StoreParamFlash_Struct    StoreFlashParam;

__HMIControl_Struct _HMIControl;
__HMIDisplay_Struct _HMIDisplay[HMITaskNumber] =
{
        {1, _KeySwitch_Close,  TaskDisplay_MainFunction},
        {0, _KeySwitch_Close, TaskDisplay_ImageFunction},
        {0, _KeySwitch_Close, TaskDisplay_MotorFunction},
        {0, _KeySwitch_Close, TaskDisplay_SteerFunction},
        {0, _KeySwitch_Close, TaskDisplay_OtherFunction},
        {0, _KeySwitch_Close, TaskDisplay_StoreFunction},
};

/******************************************************
 *                      函数重要变量
 ******************************************************/
float **DataSprtCorrect;
uint8_t LoopCount;
uint8_t PageStare;
uint8_t ControlNumber = 0;


/******************************************************
 *                      参数 数据修改
 ******************************************************/
#define         ImageFunction           1
#define         MotorFunction           2
#define         SteerFunction           3
#define         OtherFunction           4
#define         StoreFunction           5


#define         ParamImageNumber            6

#define         ParamMenuNumber             7
__DSPMenu_Struct MenuAlter[ParamMenuNumber] =
{
        {   "菜单"     },
        {   "图像参数"  },
        {   "电机参数"  },
        {   "舵机参数"  },
        {   "其它参数"  },
        {   "存储参数"  },
        {   "鲸落入海"  }
};

#define         ParamStoreNumber            3
__StoreControl_Struct _StoreControl[ParamStoreNumber] =
{
        {"LOOK ALL History"},
        {"LOOK ESP History"},
        {"  *StoreSpecial*"}
};
float _Guide = 0;

#define         ParamOtherNumber            6
__ParamAlter    _OtherAlter[ParamOtherNumber] =
{
        {"参数复位",   0, 0},
        {"元素复位",   0, 0},
        {"电感原始值",  0, 0},
        {"电感滤波值",  0, 0},
        {"PlanA",    &_Guide, 0},
        {"PlanB",    &_Guide, 0},

};


#define         ParamMotorNumber        5
__ParamAlter    _MotorAlter[ParamMotorNumber] =
{
    { "uplimit", &s.speed_uplimit, 1},
    { "downlimit", &s.speed_downlimit ,1 },
    { "second_speed", &s.second_speed ,1 },
    { "differential", &s.steer_angle, 0.001},
    { "R>0,L<0", &_SetupAngle, 200}

};

#define         ParamSteerNumber        7
__ParamAlter    _SteerAlter[ParamSteerNumber] =
{
        { "P",   &pid_Steer.Kp,0.001},
        { "R",   &pid_Steer.R,0.001},
        { "dis_tar", &dis.target_val, 10},
        { "finish", &dis.finish, 10},
        { "tar_s", &dis.target_val_s, 10},
        { "tar_s_end", &dis.tar_s_end, 10},
        { "stop_dis", &stop_dis, 100},
};

/************************************************
 *                  存储数据缓存结构体
 ************************************************/

#define         FloatNumber     (ParamMotorNumber + ParamSteerNumber + 1)
#define         SignalNumber    2

typedef struct _StoreBufferParam
{
    //数据类型
    float   BufferFloat[FloatNumber + 1];
    //信号标志 同一存储
    unsigned char SignalFlag[SignalNumber];

}__StoreBufferParam_Struct;



__StoreBufferParam_Struct _StoreBufferParam;

void InitalParameter_HIMControl(void)
{
    //控制状态初始化
    _HMIControl.KeyStatus_Present = _KeyStatus_Reset;
    _HMIControl.KeyStatus_HMIDisplay  = _HIMDisplay_Open;
    _HMIControl.KeyStatus_StoreData = _KeyStore_NULL;
    _HMIControl.HMIControl_Enable     = _HMIControl_Enable;

    _HMIControl.KeyStatus_CurrentTask = 0;



    //开始的次数
    StoreFlashParam.StoreCountLoop = 0;
}


void BaseConfigure_HIMFunction(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);

    TIM_TimeBaseStructure.TIM_Period= 599;           //20ms
    TIM_TimeBaseStructure.TIM_Prescaler= 479;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);


    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ClearFlag(TIM7, TIM_FLAG_Update);
    TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM7, ENABLE);
 }

void TaskImplement_HIMFunction(void)
{
    switch(_HMIControl.KeyStatus_Present)
    {
        case _KeyStatus_Reset:
            if(KEY_Up == GPIO_LOW || KEY_Down == GPIO_LOW || KEY_Left == GPIO_LOW || KEY_Right == GPIO_LOW || KEY_Enable == GPIO_LOW)
            {
                _HMIControl.KeyStatus_Present = _KeyStatus_Chick;
            }
            break;
        case _KeyStatus_Chick:
            if(KEY_Up == GPIO_LOW || KEY_Down == GPIO_LOW || KEY_Left == GPIO_LOW || KEY_Right == GPIO_LOW || KEY_Enable == GPIO_LOW)
            {
                _HMIControl.KeyStatus_Present = _KeyStatus_Enable;
            }
            else
            {
                _HMIControl.KeyStatus_Present = _KeyStatus_Reset;
            }
            break;
        case _KeyStatus_Enable:

            _HMIControl.HMIControl_ModelCnt = _KEY_CntClear;
            _HMIControl.HMIControl_Refresh  = _HIMDisplay_Enable;
            _HMIControl.KeyStatus_Present   = _KeyStatus_Clear;

            if(KEY_Up == GPIO_LOW)
            {
                _HMIControl.KeyStatus_StoreData = _KeyStore_KEYUp;
            }
            else if(KEY_Down == GPIO_LOW)
            {
                _HMIControl.KeyStatus_StoreData = _KeyStore_KEYDown;
            }
            else if(KEY_Left == GPIO_LOW)
            {
                _HMIControl.KeyStatus_StoreData = _KeyStore_KEYLeft;
            }
            else if(KEY_Right == GPIO_LOW)
            {
                _HMIControl.KeyStatus_StoreData = _KeyStore_KEYRight;
            }
            else if(KEY_Enable == GPIO_LOW)
            {
                _HMIControl.KeyStatus_StoreData = _KeyStore_KEYEnable;
            }
            else
            {
                _HMIControl.KeyStatus_StoreData = _KeyStore_NULL;
                _HMIControl.KeyStatus_Present   = _KeyStatus_Reset;
                _HMIControl.HMIControl_Refresh  = _HIMDisplay_Resert;
            }
            _HMIControl.HMIControl_LongData = _HMIControl.KeyStatus_StoreData;
            break;
        case _KeyStatus_Clear:
            if(KEY_Up == GPIO_HIGH && KEY_Down == GPIO_HIGH && KEY_Left == GPIO_HIGH && KEY_Right == GPIO_HIGH && KEY_Enable == GPIO_HIGH)
            {
                _HMIControl.KeyStatus_Present = _KeyStatus_Reset;

                //长按模式清零
                _HMIControl.HMIControl_LongData = _KeyStore_NULL;
                _HMIControl.HMIControl_ModelCnt = _KEY_CntClear;
            }
            else
            {
                //长按模式实现
                if(_HMIControl.HMIControl_ModelCnt > _KEY_LongMode)
                {
                    _HMIControl.KeyStatus_StoreData = _HMIControl.HMIControl_LongData;
                    _HMIControl.HMIControl_Refresh  = _HIMDisplay_Enable;
                }
                else
                {
                    _HMIControl.HMIControl_ModelCnt++;
                }
            }
            break;
    }
}


#define         HMIMAINLine            80
void TaskDisplay_MainFunction(void)
{
    if(_HMIControl.HMIControl_Refresh == _HIMDisplay_Enable)
    {
          switch(_HMIControl.KeyStatus_StoreData)
          {
              case _KeyStore_KEYUp:
                  if((--_HMIDisplay[0].TaskSerial) < 1)
                  {
                      _HMIDisplay[0].TaskSerial = 1;
                  }
                  break;
              case _KeyStore_KEYDown:
                  if((++_HMIDisplay[0].TaskSerial) > 6)
                  {
                      _HMIDisplay[0].TaskSerial = 6;
                  }
                  break;
              case _KeyStore_KEYRight:
                  //进入下一个函数
                  if(_HMIDisplay[0].TaskSerial < 6)
                  {
                      ips114_clear(DSP_BLACK);
                      _HMIControl.KeyStatus_CurrentTask = _HMIDisplay[0].TaskSerial;
                      _HMIControl.HMIControl_Refresh = _HIMDisplay_Enable;
                  }
                  else
                  {
                      _HMIDisplay[0].SelectData = _KeySwitch_Open;
                  }
                  break;
              case _KeyStore_KEYEnable:
                  if(_HMIDisplay[0].TaskSerial == 6 && _HMIDisplay[0].SelectData == _KeySwitch_Open)
                  {
//                      ips114_clear(DSP_BLACK);
                      //存储数据
//                      _StoreParam.SuccessSignalFlag = 0;
//                      _StoreParam.StoreSignalStatus = _StoreSignal_Stare;
//                      AutoParam_StoreFunction ();

                      TaskRuning_StareupFunction();
                  }
                  break;
          }
          _HMIControl.KeyStatus_StoreData = _KeyStore_NULL;

        if(_HMIControl.KeyStatus_CurrentTask == 0)
        {

            DSP_Clear(DSP_BLACK);

            for(LoopCount = 0; LoopCount < ParamMenuNumber; LoopCount++)
            {
                if(LoopCount == _HMIDisplay[0].TaskSerial)
                {
                    GuideChese_DSPFunction(160,LoopCount * 20, DSP_BLACK, DSP_YELLOW, (DSP_Uchar8Type*)"吾");
                    GuideChese_DSPFunction(20, LoopCount * 20, DSP_RED, DSP_BLACK, (DSP_Uchar8Type*)MenuAlter[LoopCount].DSPName);
                }
                else
                {
                    GuideChese_DSPFunction(20, LoopCount * 20, DSP_PINK, DSP_BLACK,(DSP_Uchar8Type*)MenuAlter[LoopCount].DSPName);
                }
            }
            _HMIControl.HMIControl_Refresh = _HIMDisplay_Resert;
        }
    }
}



void TaskDisplay_ImageFunction(void)
{

    DisPalyImg(&mt9v03x_output_image[0][0], 188, 60);

    switch (_HMIDisplay[ImageFunction].TaskSerial)
    {
        case 0:
            ips114_show_string(20,70,"TH");
            ips114_show_string(20,86,"TL");
            ips114_show_string(20,102,"PNum");

            ips114_show_string(130,70,"LT");
            ips114_show_string(130,86,"LB");
            ips114_show_string(130,102,"RT");
            ips114_show_string(130,118,"RB");

            ips114_show_uint(90, 70, Threshold, 3);
            ips114_show_uint(90, 86, Threshold_Limt, 3);
            ips114_show_uint(90, 102,m_i16CorneerPointNumber,3);
            ips114_show_uint(20, 118,   ranging_counter,    4);
//            ips114_show_uint(90, 118,CrossWalk_SetupPoint,3);

            ips114_show_int(200,70 ,m_i16LTopcornerFindFlag,2);
            ips114_show_int(200,86 ,m_i16LBotcornerFindFlag,2);
            ips114_show_int(200,102,m_i16RTopcornerFindFlag,2);
            ips114_show_int(200,118,m_i16RBotcornerFindFlag,2);
            break;
        case 1:
            ips114_show_string(20,70,"OWi");
            ips114_show_string(20,86,"WOW");
            ips114_show_string(20,102,"AbsOW");

            ips114_show_string(130,70,"s_fb");
            ips114_show_string(130,86,"s_e");
            ips114_show_string(130,102,"inum");
            ips114_show_string(130,118,"inum");

            ips114_show_int(90, 70,m_i16OutWidthROWNum,2);
            ips114_show_int(90, 86,m_i16WOutWidthROWNum,2);
            ips114_show_int(90, 102,m_i16AbsoluteOutWidth,2);
//            ips114_show_int(90, 118,_StoreParam.SuccessSignalFlag,2);

            ips114_show_int(200, 70, _StatusMonitor._fpEventControl,2);
            ips114_show_int(200, 86,_StatusMonitor._EventStatus._EventCurrentStatus,2);
            ips114_show_int(200, 102, ipts0_num,2);
            ips114_show_int(200, 118, ipts1_num,2);
            break;
        case 2:

            ips114_show_string(10,70,"K_LT");
            ips114_show_string(10,86,"K_LB");
            ips114_show_string(10,102,"K_RT");
            ips114_show_string(10,118,"K_RB");

            ips114_show_string(120,70,"err");
            ips114_show_string(120,86,"pros");
            ips114_show_string(120,102,"l_A");
            ips114_show_string(120,118,"r_A");

            ips114_set_color(DSP_RED,DSP_BLACK);
            ips114_show_float (80, 70,  KSlope_LTop, 2,1);
            ips114_show_float (80, 86,  KSlope_LBot, 2,1);
            ips114_show_float (80, 102, KSlope_RTop, 2,1);
            ips114_show_float (80, 118, KSlope_RBot, 2,1);

            ips114_show_float (190, 70,  m_f32LineAllError, 3, 2);
            ips114_show_int   (190, 86,  _Cammer_Prospect,2);
            ips114_show_float (190, 102,  leftAngular, 2, 2);
            ips114_show_float (190, 118, rightAngular, 2, 2);
            break;
        case 3:
            ips114_show_string(10,70,"Yf");
            ips114_show_string(10,86,"Yf");
            ips114_show_string(10,102,"Lf");
            ips114_show_string(10,118,"Lf");

            ips114_show_string(120,70,"L_e");
            ips114_show_string(120,86,"R_e");
            ips114_show_string(120,102,"l_A");
            ips114_show_string(120,118,"r_A");

            ips114_set_color(DSP_RED,DSP_BLACK);
            ips114_show_int   (80, 70,   Ypt0_found, 1);
            ips114_show_int   (80, 86,   Ypt1_found, 1);
            ips114_show_int   (80, 102,   Lpt0_found, 1);
            ips114_show_int   (80, 118,   Lpt1_found, 1);

            ips114_show_int   (190, 70,   _anglesignal.L_edge, 1);
            ips114_show_int   (190, 86,   _anglesignal.R_edge, 1);
            ips114_show_float (190, 102,  leftAngular, 2, 2);
            ips114_show_float (190, 118, rightAngular, 2, 2);
            break;
        case 4:
            if(_HMIDisplay[ImageFunction].SelectData == _KeySwitch_Open)
                GuideChese_DSPFunction(0, 70 + ControlNumber * 16, DSP_BLACK, DSP_YELLOW, (DSP_Uchar8Type*)"吾");

            ips114_show_string(10,70,"CP");
            ips114_show_string(10,86,"SP");
            ips114_show_string(10,102,"TP");
            ips114_show_string(10,118,"RP");

            ips114_show_string(120,70,"LA");
            ips114_show_string(120,86,"RA");
            ips114_show_string(120,102,"KL");
            ips114_show_string(120,118,"KR");

            ips114_show_int   (90, 70,   _Cammer_Prospect,     2);
            ips114_show_int   (90, 86,   _Special_Prospect,    2);
            ips114_show_int   (90, 102,  _Trident_Prospect,    2);
            ips114_show_int   (90, 118,  _Roundabout_Prospect, 2);
//
            ips114_show_float (190, 70,  LeftLoopYangle, 2, 2);
            ips114_show_float (190, 86,  ReftLoopYangle, 2, 2);
            ips114_show_float (190, 102, KSlope_Left, 2, 2);
            ips114_show_float (190, 118, KSlope_Right, 2, 2);
            break;
        case 5:
            ips114_show_string(10,70,"LF");
            ips114_show_string(10,86,"RF");
            ips114_show_string(10,102,"dis");

            ips114_show_string(120,70,"LFID");
            ips114_show_string(120,86,"RFID");
            ips114_show_string(120,102,"3cosA");
            ips114_show_string(120,118,"MD");


            ips114_set_color(DSP_RED,DSP_BLACK);
            ips114_show_int   (90, 70,   Lpt0_found ,   2);
            ips114_show_int   (90, 86,   Lpt1_found,    2);
//            ips114_show_int   (90, 118,  TheEdge, 3);
//
            ips114_show_int (190, 70,  Lpt0_rpts0s_id, 2);
            ips114_show_int (190, 86,  Lpt1_rpts1s_id, 2);
            ips114_show_float (190, 102, _TridentRoadCosA, 2, 2);
            ips114_show_int   (190, 118,  _Measure_Distance, 4);

            break;
    }


    if(_HMIControl.HMIControl_Refresh == _HIMDisplay_Enable)
    {
        _HMIControl.HMIControl_Refresh = _HIMDisplay_Resert;

        switch(_HMIControl.KeyStatus_StoreData)
        {
            case _KeyStore_KEYUp:
                if(_HMIDisplay[ImageFunction].SelectData == _KeySwitch_Open)
                {
                    if(_HMIDisplay[ImageFunction].TaskSerial == 0)
                    {
                        Threshold_Limt += 1;
                    }
                    else if(_HMIDisplay[ImageFunction].TaskSerial == 4 && ControlNumber == 0)
                    {
                        _Cammer_Prospect += 1;
                    }
                    else if(_HMIDisplay[ImageFunction].TaskSerial == 4 && ControlNumber == 1)
                    {
                        _Special_Prospect += 1;
                    }
                    else if(_HMIDisplay[ImageFunction].TaskSerial == 4 && ControlNumber == 2)
                    {
                        _Trident_Prospect += 1;
                    }
                    else if(_HMIDisplay[ImageFunction].TaskSerial == 4 && ControlNumber == 3)
                    {
                        _Roundabout_Prospect += 1;
                    }
                    else if(_HMIDisplay[ImageFunction].TaskSerial == 5)
                    {
                          TheEdge += 1;
                    }
                }
                else
                {
                    ips114_clear(DSP_BLACK);
                    if((--_HMIDisplay[ImageFunction].TaskSerial) < 0)
                    {
                        _HMIDisplay[ImageFunction].TaskSerial = 0;
                    }
                }
                break;
            case _KeyStore_KEYDown:
                if(_HMIDisplay[ImageFunction].SelectData == _KeySwitch_Open)
                {
                    if(_HMIDisplay[ImageFunction].TaskSerial == 0)
                    {
                        Threshold_Limt -= 1;
                    }
                    else if(_HMIDisplay[ImageFunction].TaskSerial == 4 && ControlNumber == 0)
                    {
                        _Cammer_Prospect -= 1;
                    }
                    else if(_HMIDisplay[ImageFunction].TaskSerial == 4 && ControlNumber == 1)
                    {
                        _Special_Prospect -= 1;
                    }
                    else if(_HMIDisplay[ImageFunction].TaskSerial == 4 && ControlNumber == 2)
                    {
                        _Trident_Prospect -= 1;
                    }
                    else if(_HMIDisplay[ImageFunction].TaskSerial == 4 && ControlNumber == 3)
                    {
                        _Roundabout_Prospect -= 1;
                    }
                    else if(_HMIDisplay[ImageFunction].TaskSerial == 5)
                    {
                          TheEdge -= 1;
                    }
                }
                else
                {
                    ips114_clear(DSP_BLACK);
                    if((++_HMIDisplay[ImageFunction].TaskSerial) > ParamImageNumber - 1)
                    {
                        _HMIDisplay[ImageFunction].TaskSerial = ParamImageNumber - 1;
                    }
                }
                break;
            case _KeyStore_KEYEnable:
                ips114_clear(DSP_BLACK);
                if(_HMIDisplay[ImageFunction].TaskSerial == 4)
                {
                    if(++ControlNumber > 3)
                    {
                        ControlNumber = 0;
                    }
                }
                break;
            case _KeyStore_KEYRight:
                _HMIDisplay[ImageFunction].SelectData = _KeySwitch_Open;
                break;

            case _KeyStore_KEYLeft:
                if(_HMIDisplay[ImageFunction].SelectData == _KeySwitch_Open)
                {

                    _HMIDisplay[ImageFunction].SelectData = _KeySwitch_Close;
                }
                else
                {

                    _HMIControl.KeyStatus_CurrentTask = 0;
                    _HMIControl.HMIControl_Refresh = _HIMDisplay_Enable;
                }
                break;
        }
        _HMIControl.KeyStatus_StoreData = _KeyStore_NULL;

    }
}


//电机想要修改的参数

void TaskDisplay_MotorFunction(void)
{
    if(_HMIControl.HMIControl_Refresh == _HIMDisplay_Enable)
    {
          switch(_HMIControl.KeyStatus_StoreData)
          {
              case _KeyStore_KEYUp:
                  if(_HMIDisplay[MotorFunction].SelectData == _KeySwitch_Open)
                  {
                      DataSprtCorrect = &_MotorAlter[_HMIDisplay[MotorFunction].TaskSerial].Paramtersprt;
                      **DataSprtCorrect += _MotorAlter[_HMIDisplay[MotorFunction].TaskSerial].Stepping;
                  }
                  else
                  {
                      if((--_HMIDisplay[MotorFunction].TaskSerial) < 0)
                      {
                          _HMIDisplay[MotorFunction].TaskSerial = 0;
                      }
                  }
                  break;
              case _KeyStore_KEYDown:
                  if(_HMIDisplay[MotorFunction].SelectData == _KeySwitch_Open)
                  {
                      DataSprtCorrect = &_MotorAlter[_HMIDisplay[MotorFunction].TaskSerial].Paramtersprt;
                      **DataSprtCorrect -= _MotorAlter[_HMIDisplay[MotorFunction].TaskSerial].Stepping;
                  }
                  else
                  {
                      if((++_HMIDisplay[MotorFunction].TaskSerial) > ParamMotorNumber - 1)
                      {
                          _HMIDisplay[MotorFunction].TaskSerial = ParamMotorNumber - 1;
                      }
                  }
                  break;
              case _KeyStore_KEYLeft:
                  if(_HMIDisplay[MotorFunction].SelectData == _KeySwitch_Open)
                  {
                      _HMIDisplay[MotorFunction].SelectData = _KeySwitch_Close;
                  }
                  else
                  {
//                      ips114_clear(WHITE);
                      _HMIControl.KeyStatus_CurrentTask = 0;
                      _HMIControl.HMIControl_Refresh = _HIMDisplay_Enable;
                  }
                  break;
              case _KeyStore_KEYRight:
                  //选择该参数 确定修改该参数
                  _HMIDisplay[MotorFunction].SelectData = _KeySwitch_Open;
                  break;
          }
          _HMIControl.KeyStatus_StoreData = _KeyStore_NULL;
        if(_HMIControl.KeyStatus_CurrentTask == MotorFunction)
        {
            _HMIControl.HMIControl_Refresh = _HIMDisplay_Resert;
            DisplayPage_HIMFunction(MotorFunction, ParamMotorNumber, _MotorAlter);
        }
    }
}


void TaskDisplay_SteerFunction(void)
{
    if(_HMIControl.HMIControl_Refresh == _HIMDisplay_Enable)
    {
          switch(_HMIControl.KeyStatus_StoreData)
          {
              case _KeyStore_KEYUp:
                  if(_HMIDisplay[SteerFunction].SelectData == _KeySwitch_Open)
                  {
                      DataSprtCorrect = &_SteerAlter[_HMIDisplay[SteerFunction].TaskSerial].Paramtersprt;
                      **DataSprtCorrect += _SteerAlter[_HMIDisplay[SteerFunction].TaskSerial].Stepping;
                  }
                  else
                  {
                      if((--_HMIDisplay[SteerFunction].TaskSerial) < 0)
                      {
                          _HMIDisplay[SteerFunction].TaskSerial = 0;
                      }
                  }
                  break;
              case _KeyStore_KEYDown:
                  if(_HMIDisplay[SteerFunction].SelectData == _KeySwitch_Open)
                  {
                      DataSprtCorrect = &_SteerAlter[_HMIDisplay[SteerFunction].TaskSerial].Paramtersprt;
                      **DataSprtCorrect -= _SteerAlter[_HMIDisplay[SteerFunction].TaskSerial].Stepping;
                  }
                  else
                  {
                      if((++_HMIDisplay[SteerFunction].TaskSerial) > ParamSteerNumber - 1)
                      {
                          _HMIDisplay[SteerFunction].TaskSerial = ParamSteerNumber - 1;
                      }
                  }
                  break;
              case _KeyStore_KEYLeft:
                  if(_HMIDisplay[SteerFunction].SelectData == _KeySwitch_Open)
                  {
                      _HMIDisplay[SteerFunction].SelectData = _KeySwitch_Close;
                  }
                  else
                  {


                      _HMIControl.KeyStatus_CurrentTask = 0;
                      _HMIControl.HMIControl_Refresh = _HIMDisplay_Enable;
                  }
                  break;
              case _KeyStore_KEYRight:
                  //选择该参数 确定修改该参数
                  _HMIDisplay[SteerFunction].SelectData = _KeySwitch_Open;
                  break;
          }

          _HMIControl.KeyStatus_StoreData = _KeyStore_NULL;

        if(_HMIControl.KeyStatus_CurrentTask == SteerFunction)
        {
            _HMIControl.HMIControl_Refresh = _HIMDisplay_Resert;
            DisplayPage_HIMFunction(SteerFunction, ParamSteerNumber, _SteerAlter);
        }
    }
}

void DisplayPage_HIMFunction(unsigned char TaskFunction, unsigned char TaskParamNumber, __ParamAlter TaskAlter[])
{
    PageStare = _HMIDisplay[TaskFunction].TaskSerial - _HMIDisplay[TaskFunction].TaskSerial % 6;
    LoopCount = PageStare - 1;

    DSP_Clear(DSP_BLACK);
    while((++LoopCount) < PageStare + 6 && LoopCount < TaskParamNumber)
    {
        if(LoopCount == _HMIDisplay[TaskFunction].TaskSerial)
        {
            ips114_set_color(DSP_RED,DSP_BLACK);
            DataSprtCorrect = &(TaskAlter[LoopCount].Paramtersprt);
            ips114_show_float (160, (LoopCount % 6) * 20, **DataSprtCorrect, 4, 3);
            GuideChese_DSPFunction(20,(LoopCount % 6) * 20, DSP_RED, DSP_BLACK, (DSP_Uchar8Type*)TaskAlter[LoopCount].ParamterName);
        }
        else
        {
            GuideChese_DSPFunction(20,(LoopCount % 6) * 20, DSP_PINK, DSP_BLACK, (DSP_Uchar8Type*)TaskAlter[LoopCount].ParamterName);
        }
    }
}



/************************************************************
 *                      储存参数
 *  1.每次发车前都保存本次发车的数据       按下确定发车
 *  2.这个是用来保存较好的数据函数           由发车后按下确定         将本次参数储存特殊的地址
 *
 *  //功能1 可以查询历史参数
 *  //功能2 可以保持历史参数
 *
 *  //256个扇区                        20 到  160   180 到 240
 *  保持的历史数据70次         需要2个扇区          1个动态区域       1个静态历史储存地址
 *
 *  好的数据保存30次
 *
 *  //存储好的数据    指针自动移位
 ************************************************************/

void TaskDisplay_StoreFunction(void)
{
    if(_HMIControl.HMIControl_Refresh == _HIMDisplay_Enable)
    {

        switch(_HMIControl.KeyStatus_StoreData)
        {
            case _KeyStore_KEYUp:
                if(_HMIDisplay[StoreFunction].SelectData == _KeySwitch_Open)
                {

                }
                else
                {
                    if((--_HMIDisplay[StoreFunction].TaskSerial) < 0)
                    {
                        _HMIDisplay[StoreFunction].TaskSerial = 0;
                    }
                }
                break;
            case _KeyStore_KEYDown:
                if(_HMIDisplay[StoreFunction].SelectData == _KeySwitch_Open)
                {

                }
                else
                {
                    if((++_HMIDisplay[StoreFunction].TaskSerial) > ParamStoreNumber - 1)
                    {
                        _HMIDisplay[StoreFunction].TaskSerial = ParamStoreNumber - 1;
                    }
                }
                break;
            case _KeyStore_KEYLeft:
                if(_HMIDisplay[OtherFunction].SelectData == _KeySwitch_Open)
                {
                    _HMIDisplay[OtherFunction].SelectData = _KeySwitch_Close;
                }
                else
                {
                    _HMIControl.KeyStatus_CurrentTask = 0;
                    _HMIControl.HMIControl_Refresh = _HIMDisplay_Enable;
                }
                break;
            case _KeyStore_KEYRight:
                _HMIDisplay[OtherFunction].SelectData = _KeySwitch_Open;
                break;

            case _KeyStore_KEYEnable:

                break;
        }
        _HMIControl.KeyStatus_StoreData = _KeyStore_NULL;
        if(_HMIControl.KeyStatus_CurrentTask == StoreFunction)
        {
            _HMIControl.HMIControl_Refresh = _HIMDisplay_Resert;

            if(_HMIDisplay[OtherFunction].SelectData == _KeySwitch_Open)
            {
                switch(_HMIDisplay[StoreFunction].TaskSerial)
                {
                    case 0:
                        //先读取数据

                        //其次显示数据

                        break;
                    case 1:
                        //先读取数据

                        //其次显示数据

                        break;
                    case 2:
                        //存储数据
                        break;
                }
            }
            else
            {
                for(LoopCount = 0; LoopCount < ParamStoreNumber; LoopCount++)
                {
                    if(LoopCount == _HMIDisplay[StoreFunction].TaskSerial)
                    {
//                        ips114_set_color(BLACK,RED);
                    }
                    else
                    {
//                        ips114_set_color(WHITE,BLACK);
                    }
                    ips114_show_string(40, (LoopCount % 6) * 20, (const char *)_StoreControl[LoopCount].ControlName);
                }
            }
        }
    }
}



/************************************************************
 *        其他需要修改的参数           用户自行修改              不会储存到FLASH
 ************************************************************/
void TaskDisplay_OtherFunction(void)
{
    if(_HMIControl.HMIControl_Refresh == _HIMDisplay_Enable)
    {
        switch(_HMIControl.KeyStatus_StoreData)
        {
            case _KeyStore_KEYUp:
                if((--_HMIDisplay[OtherFunction].TaskSerial) < 0)
                {
                    _HMIDisplay[OtherFunction].TaskSerial = 0;
                }
                break;
            case _KeyStore_KEYDown:
                if((++_HMIDisplay[OtherFunction].TaskSerial) > ParamOtherNumber - 1)
                {
                    _HMIDisplay[OtherFunction].TaskSerial = ParamOtherNumber - 1;
                }
                break;
            case _KeyStore_KEYLeft:
                if(_HMIDisplay[OtherFunction].SelectData == _KeySwitch_Open)
                {
                    _HMIDisplay[OtherFunction].SelectData = _KeySwitch_Close;
                }
                else
                {
                    DSP_Clear(DSP_BLACK);
                    _HMIControl.KeyStatus_CurrentTask = 0;
                    _HMIControl.HMIControl_Refresh = _HIMDisplay_Enable;
                }
                break;
            case _KeyStore_KEYRight:
                //选择该参数 确定修改该参数
                _HMIDisplay[OtherFunction].SelectData = _KeySwitch_Open;
                break;
        }

        _HMIControl.KeyStatus_StoreData = _KeyStore_NULL;
        _HMIControl.HMIControl_Refresh = _HIMDisplay_Resert;

      if(0)
      {

      }
      else if(_HMIControl.KeyStatus_CurrentTask == OtherFunction && _HMIDisplay[OtherFunction].SelectData == _KeySwitch_Close)
      {
          DisplayPage_HIMFunction(OtherFunction, ParamOtherNumber, _OtherAlter);
      }
      else
      {
          DSP_Clear(DSP_BLACK);
          if(_HMIDisplay[OtherFunction].TaskSerial == 0)
          {
              Parameter_StareDispose();
              GuideChese_DSPFunction(60, 60, DSP_PINK, DSP_BLACK,(DSP_Uchar8Type*)"参数复位成功");
              _HMIDisplay[OtherFunction].SelectData = _KeySwitch_Close;
          }

          else if(_HMIDisplay[OtherFunction].TaskSerial == 4)
          {
              DefaultScheme_SchemeFunction();
              GuideChese_DSPFunction(60, 60, DSP_PINK, DSP_BLACK,(DSP_Uchar8Type*)"PlanA成功");
              _HMIDisplay[OtherFunction].SelectData = _KeySwitch_Close;
          }
          else if(_HMIDisplay[OtherFunction].TaskSerial == 5)
          {
              ProgrammeB_SchemeFunction();
              GuideChese_DSPFunction(60, 60, DSP_PINK, DSP_BLACK,(DSP_Uchar8Type*)"PlanB成功");
              _HMIDisplay[OtherFunction].SelectData = _KeySwitch_Close;
          }
      }
    }

    if(_HMIDisplay[OtherFunction].TaskSerial == 2 && _HMIDisplay[OtherFunction].SelectData == _KeySwitch_Open)
    {
        GuideChese_DSPFunction(60, 10, DSP_PINK, DSP_BLACK,(DSP_Uchar8Type*)"左平电感");
        GuideChese_DSPFunction(60, 40, DSP_PINK, DSP_BLACK,(DSP_Uchar8Type*)"左斜电感");
        GuideChese_DSPFunction(60, 70, DSP_PINK, DSP_BLACK,(DSP_Uchar8Type*)"右斜电感");
        GuideChese_DSPFunction(60, 100,DSP_PINK, DSP_BLACK,(DSP_Uchar8Type*)"右平电感");

//        ips114_set_color(DSP_RED,DSP_BLACK);
//        ips114_show_uint(150, 10, Road_Main_ADC[ADC_Left].ad,5);
//        ips114_show_uint(150, 40, Road_Main_ADC[ADC_LMiddle].ad,5);
//        ips114_show_uint(150, 70, Road_Main_ADC[ADC_RMiddle].ad,5);
//        ips114_show_uint(150, 100, Road_Main_ADC[ADC_Right].ad,5);
    }
    else if(_HMIDisplay[OtherFunction].TaskSerial == 3 && _HMIDisplay[OtherFunction].SelectData == _KeySwitch_Open)
    {
        GuideChese_DSPFunction(60, 10, DSP_PINK, DSP_BLACK,(DSP_Uchar8Type*)"左平电感");
        GuideChese_DSPFunction(60, 40, DSP_PINK, DSP_BLACK,(DSP_Uchar8Type*)"左斜电感");
        GuideChese_DSPFunction(60, 70, DSP_PINK, DSP_BLACK,(DSP_Uchar8Type*)"右斜电感");
        GuideChese_DSPFunction(60, 100,DSP_PINK, DSP_BLACK,(DSP_Uchar8Type*)"右平电感");

//        ips114_set_color(DSP_RED,DSP_BLACK);
//        ips114_show_float (150, 10, Road_Main_ADC[ADC_Left].ad_value, 3, 2);
//        ips114_show_float (150, 40, Road_Main_ADC[ADC_LMiddle].ad_value, 3, 2);
//        ips114_show_float (150, 70, Road_Main_ADC[ADC_RMiddle].ad_value, 3, 2);
//        ips114_show_float (150, 100,Road_Main_ADC[ADC_Right].ad_value, 3, 2);
    }
}


/*********************************************************
   *                                                         小车启动函数
 *********************************************************/
void TaskRuning_StareupFunction(void)
{
    BEEP_ON;
    rt_thread_delay(500);
    BEEP_OFF;
    rt_thread_delay(500);
    BEEP_ON;
    rt_thread_delay(500);
    BEEP_OFF;
    _HMIControl.HMIControl_Enable = _HMIControl_Disable;
    VehicleInformation.StartDeparture = _VehicleInformation_Stare;
    rt_event_send(event,GO);
}


void TaskStopend_StopFunction(void)
{
    DropLine_ProtectFunction();

    if( VehicleInformation.StartDeparture == _VehicleInformation_Stop )
    {
        rt_event_send(event,STOP);
        _HMIControl.HMIControl_Enable = _HMIControl_Enable;
    }
}


