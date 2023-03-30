





#include "SchemeFunction.h"

/*
    _Event_NormalStatus,
    _Event_CurveStatus,
    _Event_Roundabout,
    _Event_TridentRoad,
    _Event_Crossroads,
    _Event_CrossWalk,
    _Event_WaitStop,
    _Event_RampWay,
    _Event_Special


    { "平均速度",   &VehicleInformation.SpeedDeparture},

    { "StarP_比例", &StarSpeedP},
    { "StarI_积分", &StarSpeedI},
    { "StarD_微分", &StarSpeedD},

    { "StopP_比例", &StopSpeedP},
    { "StopI_积分", &StopSpeedI},
    { "StopD_微分", &StopSpeedD},

    {"舵机基本Kp", &SteerParam_BaseKp             },
    {"舵机加权Kp", &SteerParam_RateKp             },
    {"舵机基本Kd", &SteerParam_BaseKd             },
    {"舵机加权Kd", &SteerParam_RateKd             },

    {"差数"     , &ErrorSpeedRatio               },
    {"停止距离"  , &Stop_Distance                 },
    {"三叉启动"  , &_Trident_Setup                },
    {"加速数度"  , &SetTargerSpeed                },
    {"停止白点",   &_StopPiont},
*/

#define _ElementLockNumber  30
Uint16  _ElementList[_ElementLockNumber];
Uint16  _ElementLock;


//-----------------------------------------------------------------------------------------------
//方案一


void DefaultScheme_SchemeFunction(void)
{
    EventStatusTask[_Event_NormalStatus]._EventCurrentStatus = _NormalStatus_Standby;
    EventStatusTask[_Event_NormalStatus].fpStatusFunction    = ObserverStatus_TaskFunction;
    EventStatusTask[_Event_NormalStatus].fpEventFunction     = NullFunction_TaskFunction;
}



void DefaultScheme_ParamFunction(void)
{
//    VehicleInformation.SpeedDeparture = 70;
//
//    StarSpeedP = 420;
//    StarSpeedI = 260;
//    StarSpeedD = 3.6;
//
//
//    StopSpeedP = 36;
//    StopSpeedI = 0;
//    StopSpeedD = 60;
//
//    SteerParam_BaseKp = 0.566;
//    SteerParam_RateKp = 0.126;
//
//    SteerParam_BaseKd = 0.120;
//    SteerParam_RateKd = 0.180;
//
//    ErrorSpeedRatio = 0.306;
//    Stop_Distance = 3200;
//    _Trident_Setup = 0;
//    SetTargerSpeed = 2;
//    _StopPiont = 5;
}


//-----------------------------------------------------------------------------------------------
//方案二
void ProgrammeB_SchemeFunction(void)
{
    EventStatusTask[_Event_NormalStatus]._EventCurrentStatus = _NormalStatus_Standby;
    EventStatusTask[_Event_NormalStatus].fpStatusFunction    = PlanBStatus_TaskFunction;
    EventStatusTask[_Event_NormalStatus].fpEventFunction     = PlanBEvent_TaskFunction;
}

//-----------------------------------------------------------------------------------------------

void ProgrammeC_SchemeFunction(void)
{
    EventStatusTask[_Event_NormalStatus]._EventCurrentStatus = _NormalStatus_Standby;
    EventStatusTask[_Event_NormalStatus].fpStatusFunction    = PlanCStatus_TaskFunction;
    EventStatusTask[_Event_NormalStatus].fpEventFunction     = PlanCEvent_TaskFunction;

    _ElementLock = 0;
}
//-----------------------------------------------------------------------------------------------





//方案二  无环岛
void PlanBStatus_TaskFunction(void)
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

    /*******************************************************************/

    if(_StatusMonitor._fpEventControl == _Event_WaitStop)
    {
        //开始斑马线不识别元素
    }
    else if(_Header_Element == 1)
    {
        //------------------------------------------------------------------------------------------------
        //三叉
        if(Ypt0_found == true && Ypt1_found == true && m_i16OutWidthROWNum > 20)
        {
            _Header_Element = 0;
            _StatusMonitor._fpEventControl = _Event_TridentRoad;
            EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_Chick;

        }
        else if((Ypt0_found == true  && LeftLoopYangle < 0 && m_i16LBotcornerFindFlag == 1 && m_i16WOutWidthROWNum > 12) ||
                (Ypt1_found == true  && ReftLoopYangle < 0 && m_i16RBotcornerFindFlag == 1 && m_i16WOutWidthROWNum > 12))
        {
            _Header_Element = 0;
            _StatusMonitor._fpEventControl = _Event_TridentRoad;
            EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_Chick;
        }
        else
          if(
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
                      _Header_Element = 0;
                      _StatusMonitor._fpEventControl = _Event_TridentRoad;
                      EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_Chick;
                      BEEP_ON;
                  }
          }
          else if(
                      m_i16LBotcornerFindFlag == 1 && ipts1_num < 30 &&
                      m_u16LineBAr[LBOT.m_i16x + 5] > m_u16LineBAr[LBOT.m_i16x + 30] &&
                      _anglesignal.L_edge == _anglesignal_max)
          {

               ST_2D_INT_POINT_INFO Temp;
               TridentMiddlePoint = LBOT.m_i16x + 10;
               t_i16LoopX = TridentMiddlePoint;
               t_i16LoopY = IMGH;
               Temp.m_i16y = 0;
               _TridentRoadCosA = 0;

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

                   t_i16LoopY = 5;
                   Temp.m_i16y = 0;
                   MSPOT.m_i16x = 0;

                   if(far_ipts0[far_ipts0_num - 1][1] < 40)
                   {
                       while((++t_i16LoopY) < far_ipts0_num)
                       {
                           if(
                                   far_ipts0[t_i16LoopY][1] >  far_ipts0[t_i16LoopY - 4][1] &&
                                   far_ipts0[t_i16LoopY][0] >  far_ipts0[t_i16LoopY - 4][0] &&
                                   far_ipts0[t_i16LoopY][1] >= far_ipts0[t_i16LoopY + 4][1]
                             )
                           {
                               Temp.m_i16x = far_ipts0[t_i16LoopY][0];
                               Temp.m_i16y = far_ipts0[t_i16LoopY][1];
                               MSPOT = Temp;
                               break;
                           }

                       }
                   }

                   if(Temp.m_i16y)
                   {
                       ST_2D_INT_POINT_INFO TempUp, TempDm;
                       Uint8 templine;

                       templine = t_i16LoopY - 5;
                       templine = LIMIT(templine, 0, far_ipts0_num);

                       TempUp.m_i16x = far_ipts0[templine][0];
                       TempUp.m_i16y = far_ipts0[templine][1];

                       templine = t_i16LoopY + 5;
                       templine = LIMIT(templine, 0, far_ipts0_num);
                       TempDm.m_i16x = far_ipts0[templine][0];
                       TempDm.m_i16y = far_ipts0[templine][1];

                       if(TempUp.m_i16x < Temp.m_i16x && TempUp.m_i16y < Temp.m_i16y &&
                          TempDm.m_i16x > Temp.m_i16x && TempDm.m_i16y <= Temp.m_i16y + 2)
                           _TridentRoadCosA = IncludedAngleCOS(TempUp, Temp, TempDm);

                       else
                           _TridentRoadCosA = 0;
                   }
               }

               if(_TridentRoadCosA < -0.76f && _TridentRoadCosA > -0.98f)
               {
                     _StatusMonitor._fpEventControl = _Event_TridentRoad;
                     _Header_Element = 0;
                     EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_Chick;
                     BEEP_ON;
               }
          }
        //-------------------------------------------------------------------------------------------------
    }

    else if(_Header_Element == 2)
    {
        //-------------------------------------------------------------------------------------------------
         if(
                 m_i16WOutWidthROWNum > 8    &&
                 m_i16RTopcornerFindFlag == 0 &&
                 m_i16RBotcornerFindFlag == 0 &&
                 m_i16LTopcornerFindFlag == 1 &&
                 m_i16LBotcornerFindFlag == 1 &&
                 ReftLoopYangle > 1.2         &&
                 LeftLoopYangle < 0.1         &&
                 _SetupAngle < 0
         )
         {
             _Header_Element = 0;
             _StatusMonitor._fpEventControl = _Event_CrossWalk;
             EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _Task_CrossWalk_LCheck;
         }
         else if(
             m_i16RTopcornerFindFlag == 1 &&
             m_i16RBotcornerFindFlag == 1 &&
             m_i16LTopcornerFindFlag == 0 &&
             m_i16LBotcornerFindFlag == 0 &&
             m_i16WOutWidthROWNum > 8     &&
             LeftLoopYangle > 1.2        &&
             ReftLoopYangle < 0.1        &&
             _SetupAngle > 0

         )
         {
             _Header_Element = 0;
             _StatusMonitor._fpEventControl = _Event_CrossWalk;
             EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _Task_CrossWalk_RCheck;
         }
         //-------------------------------------------------------------------------------------------------

         //-------------------------------------------------------------------------------------------------
           //斑马线判断

           else if(_StatusMonitor._fpEventControl != _Event_CrossWalk)
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
                           _Header_Element = 0;
                           _StatusMonitor._fpEventControl = _Event_CrossWalk;
                           EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _Task_CrossWalk_Chick;
                           break;
                       }
                   }
               }
           }
    }
    else if(_Header_Element == 3)
    {
        if(
                    m_i16WOutWidthROWNum > 6    &&
                    m_i16RTopcornerFindFlag == 0 &&
                    m_i16RBotcornerFindFlag == 0 &&
                    m_i16LTopcornerFindFlag == 2 &&
                    m_i16LBotcornerFindFlag == 1 &&
                    ReftLoopYangle > 1.2
              )
            {
              BEEP_ON;
              _Header_Element = 0;
                  _StatusMonitor._fpEventControl = _Event_Roundabout;
                  EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Ready;
            }
            else if(
                    m_i16WOutWidthROWNum > 6    &&
                    m_i16RTopcornerFindFlag == 0 &&
                    m_i16RBotcornerFindFlag == 0 &&
                    m_i16LTopcornerFindFlag == 1 &&
                    m_i16LBotcornerFindFlag == 1 &&
                    ReftLoopYangle > 1.2
            )
            {
                BEEP_ON;
                _Header_Element = 0;
                _StatusMonitor._fpEventControl = _Event_Roundabout;
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Ready;
            }
            else if(
                        ReftLoopYangle > 1.2      &&
                        m_i16RTopcornerFindFlag == 0 &&
                        m_i16RBotcornerFindFlag == 0 &&
                        m_i16LTopcornerFindFlag == 1 &&
                        m_i16LBotcornerFindFlag == 2 &&
                        m_i16WOutWidthROWNum > 6
                    )
            {
                BEEP_ON;
                _Header_Element = 0;
                _StatusMonitor._fpEventControl = _Event_Roundabout;
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Ready;
            }
    }
    else if(_Header_Element == 4)
    {
        if(
                      m_i16WOutWidthROWNum > 6    &&
                      m_i16RTopcornerFindFlag == 2 &&
                      m_i16RBotcornerFindFlag == 1 &&
                      m_i16LTopcornerFindFlag == 0 &&
                      m_i16LBotcornerFindFlag == 0 &&
                      LeftLoopYangle > 1.2
              )
              {
            _Header_Element = 0;
                  _StatusMonitor._fpEventControl = _Event_Roundabout;
                  EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Ready;
              }
              else if(
                      m_i16RTopcornerFindFlag == 1 &&
                      m_i16RBotcornerFindFlag == 1 &&
                      m_i16LTopcornerFindFlag == 0 &&
                      m_i16LBotcornerFindFlag == 0 &&
                      m_i16WOutWidthROWNum > 6    &&
                      LeftLoopYangle > 1.2

              )
              {
                  _Header_Element = 0;
                  _StatusMonitor._fpEventControl = _Event_Roundabout;
                  EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Ready;
              }
              else if(
                          LeftLoopYangle > 1.2      &&
                          m_i16LTopcornerFindFlag == 0 &&
                          m_i16LBotcornerFindFlag == 0 &&
                          m_i16RTopcornerFindFlag == 1 &&
                          m_i16RBotcornerFindFlag == 2 &&
                          m_i16WOutWidthROWNum > 6
                      )
              {
                  _Header_Element = 0;
                  _StatusMonitor._fpEventControl = _Event_Roundabout;
                  EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Ready; //暂时不用等做完在调用
              }
    }



    /*******************************************************************/
    //十字路口识别的优先级最高
//    else if(Lpt0_found == true && Lpt1_found == true)
//    {
//        _StatusMonitor._fpEventControl = _Event_Crossroads;
//        EventStatusTask[_Event_Crossroads]._EventCurrentStatus = _NormalStatus_Standby;
//    }
//    else if(m_i16CorneerPointNumber == 4 && m_i16OutWidthROWNum > 20 )
//    {
//        _StatusMonitor._fpEventControl = _Event_Crossroads;
//        EventStatusTask[_Event_Crossroads]._EventCurrentStatus = _NormalStatus_Standby;
//    }
//    else if(m_i16WOutWidthROWNum > 12 && m_i16CorneerPointNumber > 2 &&
//            ((Lpt0_found == true && ipts1_num < 20) || (Lpt1_found == true && ipts0_num < 20)))
//    {
//        _StatusMonitor._fpEventControl = _Event_Crossroads;
//        EventStatusTask[_Event_Crossroads]._EventCurrentStatus = _NormalStatus_Standby;
//    }
//    else if(m_i16AbsoluteOutWidth > 8 &&  m_i16WOutWidthROWNum > 8 && m_i16CorneerPointNumber > 1 )
//    {
//        _StatusMonitor._fpEventControl = _Event_Crossroads;
//        EventStatusTask[_Event_Crossroads]._EventCurrentStatus = _NormalStatus_Standby;
//    }
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
}


void PlanBEvent_TaskFunction(void)
{
    if(m_i16WOutWidthROWNum > 8 || m_i16OutWidthROWNum > 30)
    {
        ElementHandle_NormalStatus();
    }
}


/* *
 *  元素锁模块
 * */

/**
 *  1 -> 左环岛
 *  2 -> 右环岛
 *
 **/

typedef enum
{
    _ElementLock_Null = 0,
    _ElementLock_LRoundabout,
    _ElementLock_RRoundabout,
    _ElementLock_CrossRoad,
    _ElementLock_TridentRoad,
    _ElementLock_CrossWalk,

}_ElementLock_Enum;

typedef struct
{
    void (*fpSubElement)(void);
} __ElementSubunitsTypeDef;

#define         _SubElementNumber           7
__ElementSubunitsTypeDef _SubElementSchedule[_SubElementNumber] =
{
        {       NullEvent_ElementLock       },
        {       LRoundabout_ElementLock     },
        {       RRoundabout_ElementLock     },
        {       CrossRoad_ElementLock       },
        {       TridentRoad_ElementLock     },
        {       CrossWalk_ElementLock       },
        {       RampWay_ElementLock         },


};

void PlanCStatus_TaskFunction(void)
{
    if(_StatusMonitor._fpEventControl == _Event_WaitStop)
    {
        //开始斑马线不识别元素
    }
    else
    {
        _SubElementSchedule[_ElementLock].fpSubElement();
    }

}


//-------------------------------------------------------------------------------------------------

void LRoundabout_ElementLock(void)
{
    //----------------------------------------------
    //左环岛元素锁
    if(
          ReftLoopYangle > 1.4f        &&
          m_i16WOutWidthROWNum > 8     &&
          m_i16RTopcornerFindFlag == 0 &&
          m_i16RBotcornerFindFlag == 0 &&
          m_i16LTopcornerFindFlag == 2 &&
          m_i16LBotcornerFindFlag == 1
    )
    {
      _ElementLock++;                 //指向下一个元素
      //------------------------------------------

        _StatusMonitor._fpEventControl = _Event_Roundabout;
        EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Ready;
    }
    else if(
          ReftLoopYangle > 1.4f        &&
          m_i16WOutWidthROWNum > 8     &&
          m_i16RTopcornerFindFlag == 0 &&
          m_i16RBotcornerFindFlag == 0 &&
          m_i16LTopcornerFindFlag == 1 &&
          m_i16LBotcornerFindFlag == 1
    )
    {
      _ElementLock++;                 //指向下一个元素
      //------------------------------------------

      _StatusMonitor._fpEventControl = _Event_Roundabout;
      EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Ready;
    }
    else if(
              ReftLoopYangle > 1.4f        &&
              m_i16RTopcornerFindFlag == 0 &&
              m_i16RBotcornerFindFlag == 0 &&
              m_i16LTopcornerFindFlag == 1 &&
              m_i16LBotcornerFindFlag == 2 &&
              m_i16WOutWidthROWNum > 15
          )
    {
      _ElementLock++;                 //指向下一个元素
      //------------------------------------------

      _StatusMonitor._fpEventControl = _Event_Roundabout;
      EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Ready;
    }
}

//-------------------------------------------------------------------------------------------------

void RRoundabout_ElementLock(void)
{
    if(
            LeftLoopYangle > 1.4        &&
            m_i16WOutWidthROWNum > 15    &&
            m_i16RTopcornerFindFlag == 2 &&
            m_i16RBotcornerFindFlag == 1 &&
            m_i16LTopcornerFindFlag == 0 &&
            m_i16LBotcornerFindFlag == 0
    )
    {

        _ElementLock++;                 //指向下一个元素
        //------------------------------------------


        _StatusMonitor._fpEventControl = _Event_Roundabout;
        EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Ready;
    }
    else if(
            LeftLoopYangle > 1.4        &&
            m_i16RTopcornerFindFlag == 1 &&
            m_i16RBotcornerFindFlag == 1 &&
            m_i16LTopcornerFindFlag == 0 &&
            m_i16LBotcornerFindFlag == 0 &&
            m_i16WOutWidthROWNum > 8

    )
    {
        _ElementLock++;                 //指向下一个元素
        //------------------------------------------

        _StatusMonitor._fpEventControl = _Event_Roundabout;
        EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Ready;
    }
    else if(
                LeftLoopYangle > 1.4        &&
                m_i16LTopcornerFindFlag == 0 &&
                m_i16LBotcornerFindFlag == 0 &&
                m_i16RTopcornerFindFlag == 1 &&
                m_i16RBotcornerFindFlag == 2 &&
                m_i16WOutWidthROWNum > 15
            )
    {
        _ElementLock++;                 //指向下一个元素
        //------------------------------------------

        _StatusMonitor._fpEventControl = _Event_Roundabout;
        EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Ready;
    }
}

//-------------------------------------------------------------------------------------------------

void CrossRoad_ElementLock(void)
{

    //----------------------------------------------
    //十字路口元素锁
    if(Lpt0_found == true && Lpt1_found == true)
    {

        _ElementLock++;                 //指向下一个元素
        //------------------------------------------

        _StatusMonitor._fpEventControl = _Event_Crossroads;
        EventStatusTask[_Event_Crossroads]._EventCurrentStatus = _NormalStatus_Standby;
    }
    else if(m_i16CorneerPointNumber == 4)
    {

        _ElementLock++;                 //指向下一个元素
        //------------------------------------------

        _StatusMonitor._fpEventControl = _Event_Crossroads;
        EventStatusTask[_Event_Crossroads]._EventCurrentStatus = _NormalStatus_Standby;
    }
    else if((Lpt0_found == true || Lpt1_found == true) && m_i16WOutWidthROWNum > 8)
    {

        _ElementLock++;                 //指向下一个元素
        //------------------------------------------

        _StatusMonitor._fpEventControl = _Event_Crossroads;
        EventStatusTask[_Event_Crossroads]._EventCurrentStatus = _NormalStatus_Standby;
    }
    else if(m_i16AbsoluteOutWidth > 8 &&  m_i16WOutWidthROWNum > 15 && m_i16CorneerPointNumber > 1 )
    {

        _ElementLock++;                 //指向下一个元素
        //------------------------------------------

        _StatusMonitor._fpEventControl = _Event_Crossroads;
        EventStatusTask[_Event_Crossroads]._EventCurrentStatus = _NormalStatus_Standby;
    }
}


//-------------------------------------------------------------------------------------------------
void TridentRoad_ElementLock(void)
{
    if(Ypt0_found == true && Ypt1_found == true && m_i16WOutWidthROWNum > 8)
    {
        _ElementLock++;                 //指向下一个元素
        //------------------------------------------

        _StatusMonitor._fpEventControl = _Event_TridentRoad;
        EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_Chick;
    }
    else if(LeftLoopYangle <= 0 && ReftLoopYangle <= 0 && (Ypt0_found == true || Ypt1_found == true))
    {

        _ElementLock++;                 //指向下一个元素
        //------------------------------------------

        _StatusMonitor._fpEventControl = _Event_TridentRoad;
        EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_Chick;
    }
    else if(Lpt0_found == true && m_i16AbsoluteOutWidth < 3 && LeftLoopYangle <= 0 && ReftLoopYangle <= 0)
    {
        _ElementLock++;                 //指向下一个元素
        //------------------------------------------

        _StatusMonitor._fpEventControl = _Event_TridentRoad;
        EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_Chick;
    }
    else if(Lpt1_found == true && m_i16AbsoluteOutWidth < 3 && LeftLoopYangle <= 0 && ReftLoopYangle <= 0)
    {
        _ElementLock++;                 //指向下一个元素
        //------------------------------------------

        _StatusMonitor._fpEventControl = _Event_TridentRoad;
        EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_Chick;
    }
}
//-------------------------------------------------------------------------------------------------

//float _StopPiont;
void CrossWalk_ElementLock(void)
{

//    if(_SetupAngle > 0 && LeftLoopYangle > 1.0 && ipts1_num == true && Lpt1_rpts1s_id < _StopPiont) //右面入库
//    {
//        _ElementLock++;                 //指向下一个元素
//        //------------------------------------------
//        _StatusMonitor._fpEventControl = _Event_CrossWalk;
//        EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _Task_CrossWalk_Enable;
//    }
//    else if(_SetupAngle < 0 && ReftLoopYangle > 1.0 && ipts0_num == true && Lpt0_rpts0s_id < _StopPiont)
//    {
//        _ElementLock++;                 //指向下一个元素
//        //------------------------------------------
//        _StatusMonitor._fpEventControl = _Event_CrossWalk;
//        EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _Task_CrossWalk_Enable;
//    }
//    else
    if(_SetupAngle > 0 && m_i16RTopcornerFindFlag == 1 && m_i16RBotcornerFindFlag == 1)
    {
        Int16 CrossWalk_i16LoopX;
        Int16 CrossWalk_i16LoopY;
        Uint8 CrossWalk_BlackWhite = 0, CrossWalk_WhiteBlack = 0;
        Uint8 (*InImg)[IMGW];
        InImg = mt9v03x_output_image;


        for(CrossWalk_i16LoopY = RTOP.m_i16y; CrossWalk_i16LoopY < RBOT.m_i16y;CrossWalk_i16LoopY++)
        {
            CrossWalk_SetupPoint = 0;
            for (CrossWalk_i16LoopX = 30; CrossWalk_i16LoopX < IMGW - 30; CrossWalk_i16LoopX++)
            {
                if(InImg[CrossWalk_i16LoopY][CrossWalk_i16LoopX] == B_WHITE)
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
                    if(CrossWalk_SetupPoint > 7)
                    {
                        break;
                    }
                }
            }

            if(CrossWalk_SetupPoint > 7)
            {
                break;
            }
        }

        if(CrossWalk_SetupPoint > 7)
        {
            _ElementLock++;                 //指向下一个元素
            //------------------------------------------

            _StatusMonitor._fpEventControl = _Event_CrossWalk;
            EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _Task_CrossWalk_Enable;
        }
    }
    else if(_SetupAngle < 0 && m_i16LTopcornerFindFlag == 1 && m_i16LBotcornerFindFlag == 1)
    {

        Int16 CrossWalk_i16LoopX;
        Int16 CrossWalk_i16LoopY;
        Uint8 CrossWalk_BlackWhite = 0, CrossWalk_WhiteBlack = 0;
        Uint8 (*InImg)[IMGW];
        InImg = mt9v03x_output_image;

        for(CrossWalk_i16LoopY = LTOP.m_i16y; CrossWalk_i16LoopY < LBOT.m_i16y;CrossWalk_i16LoopY++)
        {
            CrossWalk_SetupPoint = 0;
            for (CrossWalk_i16LoopX = 30; CrossWalk_i16LoopX < IMGW - 30; CrossWalk_i16LoopX++)
            {
                if(InImg[CrossWalk_i16LoopY][CrossWalk_i16LoopX] == B_WHITE)
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
                    if(CrossWalk_SetupPoint > 7)
                    {
                        break;
                    }
                }
            }

            if(CrossWalk_SetupPoint > 7)
            {
                break;
            }
        }

        if(CrossWalk_SetupPoint > 7)
        {
            _ElementLock++;                 //指向下一个元素
            //------------------------------------------

            _StatusMonitor._fpEventControl = _Event_CrossWalk;
            EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _Task_CrossWalk_Enable;
        }

    }
    else
    {
        Int16 CrossWalk_i16LoopX;
        Uint8 CrossWalk_BlackWhite = 0, CrossWalk_WhiteBlack = 0;
        Uint8 (*InImg)[IMGW];
        InImg = mt9v03x_output_image;


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
                    _ElementLock++;                 //指向下一个元素
                    //------------------------------------------

                    _StatusMonitor._fpEventControl = _Event_CrossWalk;
                    EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _Task_CrossWalk_Chick;
                    break;
                }
            }
        }
    }
}
//-------------------------------------------------------------------------------------------------

void RampWay_ElementLock(void)
{

//    if(_Measure_Distance > 2000)
//    {
//        _ElementLock++;                 //指向下一个元素
//        //------------------------------------------
//
//        _StatusMonitor._fpEventControl = _Event_RampWay;
//        EventStatusTask[_Event_RampWay]._EventCurrentStatus = _Task_RampWay_Chick;
//    }
//    else if(BasicAttitude.gy > 1.6f)
//    {
//        _ElementLock++;                 //指向下一个元素
//        //------------------------------------------
//
//        _StatusMonitor._fpEventControl = _Event_RampWay;
//        EventStatusTask[_Event_RampWay]._EventCurrentStatus = _Task_RampWay_Chick;
//    }
}

//-------------------------------------------------------------------------------------------------

void NullEvent_ElementLock(void)
{
    //-------------------------------------
    /*不识别元素*/
}

void PlanCEvent_TaskFunction(void)
{
    //-------------------------------------
    /*不出意外 我也是一个空函数 热闹是他们的 哎*/
}

//-------------------------------------------------------------------------------------------------


