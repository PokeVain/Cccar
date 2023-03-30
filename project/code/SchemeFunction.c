





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


    { "ƽ���ٶ�",   &VehicleInformation.SpeedDeparture},

    { "StarP_����", &StarSpeedP},
    { "StarI_����", &StarSpeedI},
    { "StarD_΢��", &StarSpeedD},

    { "StopP_����", &StopSpeedP},
    { "StopI_����", &StopSpeedI},
    { "StopD_΢��", &StopSpeedD},

    {"�������Kp", &SteerParam_BaseKp             },
    {"�����ȨKp", &SteerParam_RateKp             },
    {"�������Kd", &SteerParam_BaseKd             },
    {"�����ȨKd", &SteerParam_RateKd             },

    {"����"     , &ErrorSpeedRatio               },
    {"ֹͣ����"  , &Stop_Distance                 },
    {"��������"  , &_Trident_Setup                },
    {"��������"  , &SetTargerSpeed                },
    {"ֹͣ�׵�",   &_StopPiont},
*/

#define _ElementLockNumber  30
Uint16  _ElementList[_ElementLockNumber];
Uint16  _ElementLock;


//-----------------------------------------------------------------------------------------------
//����һ


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
//������
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





//������  �޻���
void PlanBStatus_TaskFunction(void)
{

    /*******************************************************************/

    //�������
    Int16 t_i16LoopY;                                       /*����ѭ����*/
    Int16 t_i16LoopX;                                       /*����ѭ����*/

    Int16 Count_I1 = 0;
    Int16 Temp_LoopX = 0;
    Int16 TridentMinPoint = IMGH - 1;

    Int16 TridentMiddlePoint = 0;

    uint8_t (*InImg)[IMGW];

    InImg = mt9v03x_output_image;

    //������ʶ��
    Int16 CrossWalk_i16LoopX;


    Uint8 CrossWalk_WhiteBlack = 0;
    Uint8 CrossWalk_BlackWhite = 0;

    /*******************************************************************/

    if(_StatusMonitor._fpEventControl == _Event_WaitStop)
    {
        //��ʼ�����߲�ʶ��Ԫ��
    }
    else if(_Header_Element == 1)
    {
        //------------------------------------------------------------------------------------------------
        //����
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
                   //�˴���һ�����ж��Ƿ�Ϊ����
                   t_i16LoopX = RBOT.m_i16x + 1;
                   t_i16LoopY = 0;

                   TridentMinPoint = IMGH - 1;
                   Triden_LoopX = 0;               //����
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
                   //��ǰ���� ���ʱ��ͬʱҲ���԰Ѷ���ȷ��
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

                   //б���ж�
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
           //�������ж�

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
                  EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Ready; //��ʱ���õ������ڵ���
              }
    }



    /*******************************************************************/
    //ʮ��·��ʶ������ȼ����
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
    //��бʮ��
    //-------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------------
    //�µ����
    // 1.0) ��ʱͨ�����ٶ��ж� �����ǻ����е�����
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
 *  Ԫ����ģ��
 * */

/**
 *  1 -> �󻷵�
 *  2 -> �һ���
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
        //��ʼ�����߲�ʶ��Ԫ��
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
    //�󻷵�Ԫ����
    if(
          ReftLoopYangle > 1.4f        &&
          m_i16WOutWidthROWNum > 8     &&
          m_i16RTopcornerFindFlag == 0 &&
          m_i16RBotcornerFindFlag == 0 &&
          m_i16LTopcornerFindFlag == 2 &&
          m_i16LBotcornerFindFlag == 1
    )
    {
      _ElementLock++;                 //ָ����һ��Ԫ��
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
      _ElementLock++;                 //ָ����һ��Ԫ��
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
      _ElementLock++;                 //ָ����һ��Ԫ��
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

        _ElementLock++;                 //ָ����һ��Ԫ��
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
        _ElementLock++;                 //ָ����һ��Ԫ��
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
        _ElementLock++;                 //ָ����һ��Ԫ��
        //------------------------------------------

        _StatusMonitor._fpEventControl = _Event_Roundabout;
        EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Ready;
    }
}

//-------------------------------------------------------------------------------------------------

void CrossRoad_ElementLock(void)
{

    //----------------------------------------------
    //ʮ��·��Ԫ����
    if(Lpt0_found == true && Lpt1_found == true)
    {

        _ElementLock++;                 //ָ����һ��Ԫ��
        //------------------------------------------

        _StatusMonitor._fpEventControl = _Event_Crossroads;
        EventStatusTask[_Event_Crossroads]._EventCurrentStatus = _NormalStatus_Standby;
    }
    else if(m_i16CorneerPointNumber == 4)
    {

        _ElementLock++;                 //ָ����һ��Ԫ��
        //------------------------------------------

        _StatusMonitor._fpEventControl = _Event_Crossroads;
        EventStatusTask[_Event_Crossroads]._EventCurrentStatus = _NormalStatus_Standby;
    }
    else if((Lpt0_found == true || Lpt1_found == true) && m_i16WOutWidthROWNum > 8)
    {

        _ElementLock++;                 //ָ����һ��Ԫ��
        //------------------------------------------

        _StatusMonitor._fpEventControl = _Event_Crossroads;
        EventStatusTask[_Event_Crossroads]._EventCurrentStatus = _NormalStatus_Standby;
    }
    else if(m_i16AbsoluteOutWidth > 8 &&  m_i16WOutWidthROWNum > 15 && m_i16CorneerPointNumber > 1 )
    {

        _ElementLock++;                 //ָ����һ��Ԫ��
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
        _ElementLock++;                 //ָ����һ��Ԫ��
        //------------------------------------------

        _StatusMonitor._fpEventControl = _Event_TridentRoad;
        EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_Chick;
    }
    else if(LeftLoopYangle <= 0 && ReftLoopYangle <= 0 && (Ypt0_found == true || Ypt1_found == true))
    {

        _ElementLock++;                 //ָ����һ��Ԫ��
        //------------------------------------------

        _StatusMonitor._fpEventControl = _Event_TridentRoad;
        EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_Chick;
    }
    else if(Lpt0_found == true && m_i16AbsoluteOutWidth < 3 && LeftLoopYangle <= 0 && ReftLoopYangle <= 0)
    {
        _ElementLock++;                 //ָ����һ��Ԫ��
        //------------------------------------------

        _StatusMonitor._fpEventControl = _Event_TridentRoad;
        EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_Chick;
    }
    else if(Lpt1_found == true && m_i16AbsoluteOutWidth < 3 && LeftLoopYangle <= 0 && ReftLoopYangle <= 0)
    {
        _ElementLock++;                 //ָ����һ��Ԫ��
        //------------------------------------------

        _StatusMonitor._fpEventControl = _Event_TridentRoad;
        EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_Chick;
    }
}
//-------------------------------------------------------------------------------------------------

//float _StopPiont;
void CrossWalk_ElementLock(void)
{

//    if(_SetupAngle > 0 && LeftLoopYangle > 1.0 && ipts1_num == true && Lpt1_rpts1s_id < _StopPiont) //�������
//    {
//        _ElementLock++;                 //ָ����һ��Ԫ��
//        //------------------------------------------
//        _StatusMonitor._fpEventControl = _Event_CrossWalk;
//        EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _Task_CrossWalk_Enable;
//    }
//    else if(_SetupAngle < 0 && ReftLoopYangle > 1.0 && ipts0_num == true && Lpt0_rpts0s_id < _StopPiont)
//    {
//        _ElementLock++;                 //ָ����һ��Ԫ��
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
            _ElementLock++;                 //ָ����һ��Ԫ��
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
            _ElementLock++;                 //ָ����һ��Ԫ��
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
                    _ElementLock++;                 //ָ����һ��Ԫ��
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
//        _ElementLock++;                 //ָ����һ��Ԫ��
//        //------------------------------------------
//
//        _StatusMonitor._fpEventControl = _Event_RampWay;
//        EventStatusTask[_Event_RampWay]._EventCurrentStatus = _Task_RampWay_Chick;
//    }
//    else if(BasicAttitude.gy > 1.6f)
//    {
//        _ElementLock++;                 //ָ����һ��Ԫ��
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
    /*��ʶ��Ԫ��*/
}

void PlanCEvent_TaskFunction(void)
{
    //-------------------------------------
    /*�������� ��Ҳ��һ���պ��� ���������ǵ� ��*/
}

//-------------------------------------------------------------------------------------------------


