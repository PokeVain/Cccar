





#include "TaskFunction.h"

//状态控制器
__StatusMonitorStruct _StatusMonitor;

//子状态任务
__EventSatsusStruct EventStatusTask[EventNumber] =
{
        /*总控制状态 父状态*/
        {_NormalStatus_Standby,   ObserverStatus_TaskFunction,    NullFunction_TaskFunction},
        //弯道状态识别
        {_NormalStatus_Standby,   CurveStatus_TaskFunction,       NullFunction_TaskFunction},
        //环岛状态识别处理
        {_NormalStatus_Standby,   StatusRoundabout_TaskFunction,  RepairRoundabout_TaskFunction},
        //三叉状态识别处理
        {_NormalStatus_Standby,   StatusTridentRoad_TaskFunction, RepairTrident_TaskFunction},
        //十字状态识别处理
        {_NormalStatus_Standby,   StatusCrossRoad_TaskFunction,   RepairCrossRoad_TaskFunction},
        //斑马线识别处理
        {_NormalStatus_Standby,   StatusCrossWalk_TaskFunction,   RepairCrossWalk_TaskFunction},
        //斑马线等车处理
        {_NormalStatus_Standby  , StatusWaitStop_TaskFunction,    RepairWaitStop_TaskFunction},
        //坡道处理
        {_NormalStatus_Standby ,  NullFunction_TaskFunction,     NullFunction_TaskFunction},
        //特殊处理
        {_NormalStatus_Standby,   NullFunction_TaskFunction,      NullFunction_TaskFunction},
};

//extern __MessageControl_Struct _MessageSchedule[MessageNumber];
Uint16 _Measure_Distance;

float _StopPiont;
float _TridentRoadCosA;


void InitalParam_TaskFunction(void)
{
    _StatusMonitor._fpEventControl = _Event_WaitStop;
    _StatusMonitor._EventStatus = EventStatusTask[_Event_WaitStop];


    MSPOT.m_i16x = 0;
    MSPOT.m_i16y = 0;

    KSlope_LTop = 0;
    KSlope_LBot = 0;
    KSlope_RTop = 0;
    KSlope_RBot = 0;

    BlackPointArea = 0;

    CrossWalk_SetupPoint = 0;

    Element_LoopY = 30;

    LeftLoopYangle = 0;

    ReftLoopYangle = 0;

    _Measure_Distance = 0;

}

void DisplayParam_TaskFunction(void)
{
    printf("%d,%d,%f\r\n",_StatusMonitor._fpEventControl, EventStatusTask[_StatusMonitor._fpEventControl]._EventCurrentStatus,m_f32LineAllError);
}

void ResetElement_TaskFunction(void)
{
    //复位元素锁

    //复位初始姿态
    _StatusMonitor._fpEventControl = _Event_WaitStop;
    _StatusMonitor._EventStatus = EventStatusTask[_Event_WaitStop];
//    _StatusMonitor.fpSpecialEvent = SpecialEvent_TaskFunction;

    //复位元素状态
    EventStatusTask[_Event_NormalStatus]._EventCurrentStatus = _NormalStatus_Standby;
    EventStatusTask[_Event_CurveStatus ]._EventCurrentStatus = _NormalStatus_Standby;
    EventStatusTask[_Event_Roundabout  ]._EventCurrentStatus = _NormalStatus_Standby;
    EventStatusTask[_Event_TridentRoad ]._EventCurrentStatus = _NormalStatus_Standby;
    EventStatusTask[_Event_Crossroads  ]._EventCurrentStatus = _NormalStatus_Standby;
    EventStatusTask[_Event_CrossWalk   ]._EventCurrentStatus = _NormalStatus_Standby;
    EventStatusTask[_Event_WaitStop    ]._EventCurrentStatus = _Task_WaitStop_Start;
    EventStatusTask[_Event_RampWay     ]._EventCurrentStatus = _NormalStatus_Standby;

    //三叉元素复位
//    _TridentDirectionFlag = _TridentRoad_Right;
}


void TaskImplement_TaskFunction(void)
{
    _StatusMonitor._EventStatus = EventStatusTask[_StatusMonitor._fpEventControl];
    _StatusMonitor._EventStatus.fpStatusFunction();
    _StatusMonitor._EventStatus.fpEventFunction();
}

/**
 *  特殊事件处理
 **/
void SpecialEvent_TaskFunction(void)
{


}

/***
 *  父状态观测器 检测子状态发生
 **/
void ObserverStatus_TaskFunction(void)
{

    /*******************************************************************/

    //三叉变量
    Int16 t_i16LoopY;                                       /*用来循环行*/
    Int16 t_i16LoopX;                                       /*用来循环列*/

    Int16 Count_I1 = 0;
    Int16 Temp_LoopX = 0;
    Int16 TridentMinPoint = IMGH - 1;

    Int16 TridentMiddlePoint = 0;

    uint8_t (*InImg)[IMGW];

    InImg = mt9v03x_output_image;

    //斑马线识别
    Int16 CrossWalk_i16LoopX;


    Uint8 CrossWalk_WhiteBlack = 0;
    Uint8 CrossWalk_BlackWhite = 0;




    //-------------------------------------------------------------------------------------------------
    //发车处理

    if(_StatusMonitor._fpEventControl == _Event_WaitStop)
    {
        //开始斑马线不识别元素
    }
    //-------------------------------------------------------------------------------------------------


    //左环岛
  else if(
            m_i16WOutWidthROWNum > 8    &&
            m_i16RTopcornerFindFlag == 0 &&
            m_i16RBotcornerFindFlag == 0 &&
            m_i16LTopcornerFindFlag == 2 &&
            m_i16LBotcornerFindFlag == 1 &&
            ReftLoopYangle > 1.4        &&
            _anglesignal.L_edge == _anglesignal_max
      )
    {
      BEEP_ON;
          _StatusMonitor._fpEventControl = _Event_Roundabout;
          EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Ready;
    }
    else if(
            m_i16WOutWidthROWNum > 8    &&
            m_i16RTopcornerFindFlag == 0 &&
            m_i16RBotcornerFindFlag == 0 &&
            m_i16LTopcornerFindFlag == 1 &&
            m_i16LBotcornerFindFlag == 1 &&
            ReftLoopYangle > 1.4        &&
            LeftLoopYangle < 0.1
    )
    {
        BEEP_ON;
        _StatusMonitor._fpEventControl = _Event_Roundabout;
        EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Chick;
    }
    else if(
                ReftLoopYangle > 1.4      &&
                m_i16RTopcornerFindFlag == 0 &&
                m_i16RBotcornerFindFlag == 0 &&
                m_i16LTopcornerFindFlag == 1 &&
                m_i16LBotcornerFindFlag == 2 &&
                m_i16WOutWidthROWNum > 8    &&
                LeftLoopYangle == 0.0
            )
    {
        BEEP_ON;
        _StatusMonitor._fpEventControl = _Event_Roundabout;
        EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Ready;
    }


      //右环岛
      else if(
              m_i16WOutWidthROWNum > 8    &&
              m_i16RTopcornerFindFlag == 2 &&
              m_i16RBotcornerFindFlag == 1 &&
              m_i16LTopcornerFindFlag == 0 &&
              m_i16LBotcornerFindFlag == 0 &&
              LeftLoopYangle > 1.4        &&
              _anglesignal.R_edge == _anglesignal_max
      )
      {
          _StatusMonitor._fpEventControl = _Event_Roundabout;
          EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Ready;
      }
      else if(
              m_i16RTopcornerFindFlag == 1 &&
              m_i16RBotcornerFindFlag == 1 &&
              m_i16LTopcornerFindFlag == 0 &&
              m_i16LBotcornerFindFlag == 0 &&
              m_i16WOutWidthROWNum > 8    &&
              LeftLoopYangle > 1.4        &&
              ReftLoopYangle < 0.1

      )
      {
          _StatusMonitor._fpEventControl = _Event_Roundabout;
          EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Chick;
      }
      else if(
                  LeftLoopYangle > 1.4      &&
                  m_i16LTopcornerFindFlag == 0 &&
                  m_i16LBotcornerFindFlag == 0 &&
                  m_i16RTopcornerFindFlag == 1 &&
                  m_i16RBotcornerFindFlag == 2 &&
                  m_i16WOutWidthROWNum > 8    &&
                  ReftLoopYangle < 0.1
              )
      {
          _StatusMonitor._fpEventControl = _Event_Roundabout;
          EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Ready; //暂时不用等做完在调用
      }


    //-------------------------------------------------------------------------------------------------


    /*******************************************************************/
    //十字路口识别的优先级最高
      else if(Lpt0_found == true && Lpt1_found == true)
      {
          _StatusMonitor._fpEventControl = _Event_Crossroads;
          EventStatusTask[_Event_Crossroads]._EventCurrentStatus = _NormalStatus_Standby;
      }

//      else if(Ypt0_found == true && Ypt1_found == true && m_i16OutWidthROWNum > 20)
//      {
//          _StatusMonitor._fpEventControl = _Event_TridentRoad;
//          EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_Chick;
//
//      }

      else if(m_i16CorneerPointNumber == 4 && m_i16AbsoluteOutWidth > 3)
      {
          _StatusMonitor._fpEventControl = _Event_Crossroads;
          EventStatusTask[_Event_Crossroads]._EventCurrentStatus = _NormalStatus_Standby;
      }

      else if(m_i16AbsoluteOutWidth > 8 &&  m_i16WOutWidthROWNum > 15 && m_i16CorneerPointNumber > 1 )
      {

          _StatusMonitor._fpEventControl = _Event_Crossroads;
          EventStatusTask[_Event_Crossroads]._EventCurrentStatus = _NormalStatus_Standby;
      }

    //右斜十字
    //-------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------------
    //坡道检测
    // 1.0) 暂时通过加速度判断 陀螺仪还是有点问题

    else if (Gyro.y > 3000)
    {
        _StatusMonitor._fpEventControl = _Event_RampWay;
    }


    //-------------------------------------------------------------------------------------------------


    //------------------------------------------------------------------------------------------------
//三叉
    else if(Ypt0_found == true && Ypt1_found == true && LeftLoopYangle < 0 && ReftLoopYangle < 0 && m_i16WOutWidthROWNum > 8 )
    {
        _StatusMonitor._fpEventControl = _Event_TridentRoad;
        EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_Chick;

    }
    else
      if(
                     m_i16WOutWidthROWNum > 8   &&
                     LeftLoopYangle < 0 && ReftLoopYangle < 0 &&
                     m_i16LBotcornerFindFlag == 1         &&
                     m_i16RBotcornerFindFlag == 1         &&
                     m_u16LineBAr[LBOT.m_i16x + 5] > m_u16LineBAr[(LBOT.m_i16x + RBOT.m_i16x) / 2] &&
                     m_u16LineBAr[RBOT.m_i16x - 5] > m_u16LineBAr[(LBOT.m_i16x + RBOT.m_i16x) / 2]
       )
      {
               //此处进一步的判断是否为三岔
               t_i16LoopX = RBOT.m_i16x + 1;
               t_i16LoopY = 0;

               TridentMinPoint = IMGH - 1;
               Triden_LoopX = 0;               //中线
               Count_I1 = 0;
               for(Temp_LoopX = LBOT.m_i16x + 1; Temp_LoopX < RBOT.m_i16x; Temp_LoopX++)
               {
                   if(m_u16LineBAr[Temp_LoopX] <= TridentMinPoint && m_u16LineBAr[Temp_LoopX] != 0)
                   {
                       Count_I1 = 0;
                       Triden_LoopXEnd = Temp_LoopX;
                       TridentMinPoint = m_u16LineBAr[Temp_LoopX];
                   }
                   else
                   {
                      Count_I1++;
                      if(Count_I1 > 4)
                          break;
                   }
               }
               //往前搜索 这个时候同时也可以把顶点确定
               Triden_LoopXStart = Triden_LoopXEnd;
               while((--Triden_LoopXStart) && Triden_LoopXStart > LBOT.m_i16x)
               {
                   if(m_u16LineBAr[Triden_LoopXStart] > m_u16LineBAr[Triden_LoopXEnd])
                   {
                       break;
                   }
               }
               Triden_LoopXStart++;
               Triden_LoopX = (int)(Triden_LoopXStart + Triden_LoopXEnd) / 2;


               MSPOT.m_i16x = Triden_LoopX;
               MSPOT.m_i16y = 59 - m_u16LineBAr[Triden_LoopX];

               //斜率判断
               t_i16LoopY = MSPOT.m_i16y - 3;
               t_i16LoopX = MSPOT.m_i16x;

               while((++t_i16LoopX) < RBOT.m_i16x + 10)
               {
                   if(InImg[t_i16LoopY][t_i16LoopX] == B_WHITE)
                   {
                       KSlope_RTop = t_i16LoopX - MSPOT.m_i16x;
                       break;
                   }
                   else
                   {
                       KSlope_RTop = 0;
                   }
               }


               t_i16LoopX = MSPOT.m_i16x;
               while((--t_i16LoopX) > LBOT.m_i16x - 10)
               {
                   if(InImg[t_i16LoopY][t_i16LoopX] == B_WHITE)
                   {
                       KSlope_LTop = t_i16LoopX - MSPOT.m_i16x;
                       break;
                   }
                   else
                   {
                       KSlope_LTop = 0;
                   }
               }



               KSlope_RBot = RBOT.m_i16x - m_stRPnt[RBOT.m_i16y + 6].m_i16x;
               KSlope_LBot = LBOT.m_i16x - m_stLPnt[LBOT.m_i16y + 6].m_i16x;

              if(KSlope_RBot > KSlope_LTop && KSlope_LBot < KSlope_RTop && KSlope_RBot < 0 &&  KSlope_LBot> 0)
              {

                  _StatusMonitor._fpEventControl = _Event_TridentRoad;
                  EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_Chick;
                  BEEP_ON;
              }
      }
      else if(
                  m_i16WOutWidthROWNum > 8   &&
                  m_i16LBotcornerFindFlag == 1 && ipts1_num < 20 &&
                  m_u16LineBAr[LBOT.m_i16x + 5] > m_u16LineBAr[LBOT.m_i16x + (TrackWide[LBOT.m_i16y] / 2)] &&
                  _anglesignal.L_edge == _anglesignal_max &&
                  _anglesignal.R_edge == _anglesignal_false)
      {
           ST_2D_INT_POINT_INFO Temp;
           TridentMiddlePoint = LBOT.m_i16x + 5;
           t_i16LoopX = TridentMiddlePoint;
           t_i16LoopY = IMGH;
           Temp.m_i16y = 0;

           while((--t_i16LoopY) > 1)
           {
               if(   InImg[t_i16LoopY - 1][t_i16LoopX] == B_BLACK &&
                     InImg[t_i16LoopY    ][t_i16LoopX] == B_WHITE &&
                     InImg[t_i16LoopY + 1][t_i16LoopX] == B_WHITE)
                  {
                      Temp.m_i16x = t_i16LoopX;
                      Temp.m_i16y = t_i16LoopY;
                      break;
                  }
           }

           if(Temp.m_i16y)
           {
               far_ipts0_num = 80;
               findline_lefthand_adaptive(&img_raw, Threshold, Temp.m_i16x, Temp.m_i16y, far_ipts0, &far_ipts0_num);

               t_i16LoopY = 0;
               Temp.m_i16y = 0;
               while(++(t_i16LoopY) < far_ipts0_num)
               {
                   if(far_ipts0[t_i16LoopY - 1][1] >  far_ipts0[t_i16LoopY][1] &&
                      far_ipts0[t_i16LoopY + 1][1] >= far_ipts0[t_i16LoopY][1])
                   {
                       Temp.m_i16x = far_ipts0[t_i16LoopY][0];
                       Temp.m_i16y = far_ipts0[t_i16LoopY][1];

                       MSPOT = Temp;
                       break;
                   }
               }

               if(Temp.m_i16y)
               {
                   ST_2D_INT_POINT_INFO TempUp, TempDm;
                   Uint8 templine;

                   templine = t_i16LoopY - 10;
                   templine = LIMIT(templine, 0, far_ipts0_num);

                   TempUp.m_i16x = far_ipts0[templine][0];
                   TempUp.m_i16y = far_ipts0[templine][1];

                   templine = t_i16LoopY + 10;
                   templine = LIMIT(templine, 0, far_ipts0_num);
                   TempDm.m_i16x = far_ipts0[templine][0];
                   TempDm.m_i16y = far_ipts0[templine][1];

                   if(TempUp.m_i16x < Temp.m_i16x && TempUp.m_i16y < Temp.m_i16y &&
                      TempDm.m_i16x > Temp.m_i16x && TempDm.m_i16y <= Temp.m_i16y )
                       _TridentRoadCosA = IncludedAngleCOS(TempUp, Temp, TempDm);
                   else
                       _TridentRoadCosA = 0;
               }
           }

           if(_TridentRoadCosA < -0.8f)
           {
               _StatusMonitor._fpEventControl = _Event_TridentRoad;
               EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_Chick;
               BEEP_ON;
           }
      }
    //-------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------------
    //斑马线判断

    else if(
            _StatusMonitor._fpEventControl != _Event_CrossWalk)
    {
        CrossWalk_SetupPoint = 0;
        for (CrossWalk_i16LoopX = 30; CrossWalk_i16LoopX < IMGW - 30; CrossWalk_i16LoopX++)
        {
            if(InImg[_CrossWalkStarLine][CrossWalk_i16LoopX] == B_WHITE)
            {
                CrossWalk_BlackWhite = 1;
            }
            else
            {
                CrossWalk_BlackWhite = 0;
            }

            if(CrossWalk_WhiteBlack != CrossWalk_BlackWhite)
            {
                CrossWalk_WhiteBlack = CrossWalk_BlackWhite;
                CrossWalk_SetupPoint++;
                if(CrossWalk_SetupPoint > 5)
                {
                    _StatusMonitor._fpEventControl = _Event_CrossWalk;
                    EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _Task_CrossWalk_Chick;
                    break;
                }
            }
        }
    }
}



/**
 *  一下是一些元素处理
 **/
/*************************************************************************************/
void CurveStatus_TaskFunction(void)
{
    if (  m_u32LAllArea < 800  && IMGW - 50   < ipts0[ipts0_num - 1][0] && ipts1_num == 0 && ipts0_num > 60) //&&  ipts0[ipts0_num - 1][1] > IMGH / 2)
    {
        EventStatusTask[_Event_CurveStatus]._EventCurrentStatus = _Task_CurveStatus_Right;
    }
    else if(m_u32RAllArea < 800  &&ipts1[ipts1_num - 1][0] <  50 && ipts0_num == 0  && ipts1_num  > 60)
    {
        EventStatusTask[_Event_CurveStatus]._EventCurrentStatus = _Task_CurveStatus_Left;
    }
    else if(m_u32LAllArea < 800 && m_u32RAllArea > 600 && m_stOptimalPoint.m_i16x > IMGW / 2 + 30)
    {
        EventStatusTask[_Event_CurveStatus]._EventCurrentStatus = _Task_CurveStatus_Right;
    }
    else if(m_u32RAllArea < 800 && m_u32LAllArea > 600 && m_stOptimalPoint.m_i16x > IMGW / 2 - 30)
    {
        EventStatusTask[_Event_CurveStatus]._EventCurrentStatus = _Task_CurveStatus_Left;
    }
    else
    {
        EventStatusTask[_Event_CurveStatus]._EventCurrentStatus = _Task_CurveStatus_Straight;
    }

}

void NormalRoad_TaskFunction(void)
{
    if(
                 m_i16LTopcornerFindFlag == 0 &&
                 m_i16LBotcornerFindFlag == 0 &&
                 m_i16RTopcornerFindFlag == 0 &&
                 m_i16RBotcornerFindFlag == 0 &&
                 m_i16WOutWidthROWNum    == 0 &&
                 m_i16OutWidthROWNum     == 0
      )
    {
            EventStatusTask[_StatusMonitor._fpEventControl]._EventCurrentStatus = _NormalStatus_Standby;
            _StatusMonitor._fpEventControl = _Event_NormalStatus;
    }
}

void NullFunction_TaskFunction(void)
{

    /*******************************/
    //该函数没有作用 防止程序跑飞
}













