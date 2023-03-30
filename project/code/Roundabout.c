



#include "Roundabout.h"


//Uint8 DroplineStatus_Roundabout;


#define         SBUFFSIZE           100

float Average_Error[SBUFFSIZE + 1];
Uint8 Error_Count = 0;

Uint32 _Roundabout_Record;//_Trident_Distance

float _Angle_Record;


/**
 *  环岛控制状态
 **/

/***************************************************************************************************/

void DiscernRoundabout_TaskFunction(void)
{
    Int16 tempCnt = 50;
    Uint16 LosePoint_LC = 0, LosePoint_RC = 0;

    if(m_i16OutWidthROWNum > 20)    //一定有超宽的赛道
      {
          //也可以通过拐点来判断
          while ((tempCnt--) > 10)
          {
              if (m_stLPnt[tempCnt].m_i16x < 5)
              {
                  LosePoint_LC++;
              }
              if (m_stRPnt[tempCnt].m_i16x > IMGW - 6)
              {
                  LosePoint_RC++;
              }
          }
      }

    /*左环岛*/
    if(LosePoint_LC > 2 && LosePoint_RC == 0)
    {
        if(
           m_i16RTopcornerFindFlag == 0 &&
           m_i16RBotcornerFindFlag == 0 &&
           m_i16LTopcornerFindFlag == 1 &&
           m_i16LBotcornerFindFlag == 1 )
        {
            _StatusMonitor._fpEventControl = _Event_Roundabout;
            EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Chick;
        }
        else if(
                /*弯道有相同的条件 可以根据出现的点大于某个范围解决*/
                m_i16LTopcornerFindFlag == 1 &&
                m_i16LBotcornerFindFlag == 2 &&
                m_i16RTopcornerFindFlag == 0 &&
                m_i16RBotcornerFindFlag == 0 &&
                LTOP.m_i16x       > 20
                )
        {
            _StatusMonitor._fpEventControl = _Event_Roundabout;
            EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Chick;
        }

    }
    /*右环岛*/
    else if(LosePoint_LC == 0 && LosePoint_RC > 2)
    {
        //待补右环岛

        if(
           m_i16RTopcornerFindFlag == 1 &&
           m_i16RBotcornerFindFlag == 1 &&
           m_i16LTopcornerFindFlag == 0 &&
           m_i16LBotcornerFindFlag == 0 )
        {
            _StatusMonitor._fpEventControl = _Event_Roundabout;
            EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Chick;
        }
        else if(
                /*弯道有相同的条件 可以根据出现的点大于某个范围解决*/
                m_i16LTopcornerFindFlag == 0 &&
                m_i16LBotcornerFindFlag == 0 &&
                m_i16RTopcornerFindFlag == 1 &&
                m_i16RBotcornerFindFlag == 2 &&
                RTOP.m_i16x     <  IMGW - 22 )
        {
            _StatusMonitor._fpEventControl = _Event_Roundabout;
            EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Chick;
        }
    }
}

/***************************************************************************************************/
//环岛状态转移



void StatusRoundabout_TaskFunction(void)
{

    Uint16 LWhitePnt1 = m_u16LineBAr[10];
    Uint16 LWhitePnt2 = m_u16LineBAr[25];
    Uint16 LWhitePnt3 = m_u16LineBAr[40];
    Uint16 RWhitePnt1 = m_u16LineBAr[IMGW - 11];
    Uint16 RWhitePnt2 = m_u16LineBAr[IMGW - 26];
    Uint16 RWhitePnt3 = m_u16LineBAr[IMGW - 41];

    switch(EventStatusTask[_Event_Roundabout]._EventCurrentStatus)
    {
        case _Task_LRoundabout_Chick:
            if(_SetupAngle < 0)
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
                    _StatusMonitor._fpEventControl = _Event_CrossWalk;
                    EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _Task_CrossWalk_Enable;
                }
                else
                {
                    EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Ready;
                }
            }
            else
            {
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Ready;
            }
            break;
        case _Task_LRoundabout_Ready:   //通过外面的调节判断 有点就补线
            if(LWhitePnt1 > 5 && LWhitePnt2 > 10 && LWhitePnt3 > 15)
            {
                _Roundabout_Record = _Runing_Distance;
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Stare;
            }
            break;
        case _Task_LRoundabout_Stare:   //采用半宽补线
            if(
                    LWhitePnt1 < 5 && (LWhitePnt2 < 10 && LWhitePnt3 < 15)
                  && (m_i16LTopcornerFindFlag == 1)
              )
            {
                //_Angle_Record = _OutAngle.yaw;
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Enable;
            }
            else if(LWhitePnt1 > 15 && (LWhitePnt2 > 10 && LWhitePnt3 > 5) &&
                    LWhitePnt1 > LWhitePnt3 && LTOP.m_i16y > 35)
            {
                //_Angle_Record = _OutAngle.yaw;
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Enable;
            }
            break;
        case _Task_LRoundabout_Enable:
            if(
                    (LWhitePnt1 > 20 && LWhitePnt2 > 15 && LWhitePnt3 > 10)
              )
              {
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Entry;
                BEEP_ON;
              }
            break;
        case _Task_LRoundabout_Entry:
            if (
                    ((ipts0_num == 0 && ipts1_num == 80) || m_i16LTopcornerFindFlag == 2) &&
                     m_u32RAllArea < 800 && m_i16WOutWidthROWNum  < 5 && ReftLoopYangle < 0.6f
               )

            {
                BEEP_OFF;
                Error_Count = 0;
                _Roundabout_Record = _Runing_Distance;
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Ring;
            }
            break;
        case _Task_LRoundabout_Ring:
              Average_Error[Error_Count] = m_f32LineAllError;
              if(++Error_Count > SBUFFSIZE)
              {
                  Error_Count = SBUFFSIZE;
              }
              if(
                      m_i16LTopcornerFindFlag == 2 &&
                      m_i16LBotcornerFindFlag == 2 &&
                      m_i16RTopcornerFindFlag == 2 &&
                      _anglesignal.R_edge == _anglesignal_max          &&
                      ABS(_Runing_Distance - _Roundabout_Record) > 2000
                      )
             {
                  float error_sum = 0;
                  for(int i = 0; i < Error_Count; i++)
                  {
                      error_sum += Average_Error[i];
                  }
                  _SpecialHandle_FixedError = error_sum / Error_Count;
                  if(_SpecialHandle_FixedError > -75) _SpecialHandle_FixedError = -75;
                  EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Exit;
             }
            break;
        case _Task_LRoundabout_Exit:
            if(
                    m_i16LTopcornerFindFlag == 2 &&
                    m_i16LBotcornerFindFlag == 2 &&
                    m_i16RTopcornerFindFlag == 2 &&
                    m_i16RBotcornerFindFlag == 2 &&
                    m_i16AbsoluteOutWidth > 5
              )
              {
                /*这里可能要补线*/
                BEEP_ON;
                _Roundabout_Record = _Runing_Distance;
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Out;
              }
            else if(
                    (ipts0_num < 10 && ipts1_num < 10) &&
                    m_i16AbsoluteOutWidth    > 5
                    )
            {
                BEEP_ON;
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Out;
            }
            break;

        case _Task_LRoundabout_Out:
            if(ReftLoopYangle > 0.7f && m_i16LTopcornerFindFlag == 1  && m_i16AbsoluteOutWidth < 5)
            {
                _Roundabout_Record = _Runing_Distance;
                _SpecialHandle_FixedDeviation = 0;
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Finish;
            }
            else if(ReftLoopYangle > 0.6f && m_i16WOutWidthROWNum < 5)
            {
                _Roundabout_Record = _Runing_Distance;
                _SpecialHandle_FixedDeviation = 0;
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_LRoundabout_Finish;
            }
            break;
        case _Task_LRoundabout_Finish:
            if(
                     (ipts0_num > 40) &&
                     (_Runing_Distance - _Roundabout_Record > 3000)
              )
            {
                    BEEP_OFF;
                    _StatusMonitor._fpEventControl = _Event_NormalStatus;
                    EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _NormalStatus_Standby;
            }
            else  if( LeftLoopYangle > 0.4f  && m_i16AbsoluteOutWidth < 8
                    )
            {
                if(_Runing_Distance - _Roundabout_Record > 3000)
                {
                    _StatusMonitor._fpEventControl = _Event_NormalStatus;
                    EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _NormalStatus_Standby;
                }
            }
            else if(LTOP.m_i16y > 40 && m_i16LTopcornerFindFlag == 1 && _Runing_Distance - _Roundabout_Record > 3000)
            {
                BEEP_OFF;
                _StatusMonitor._fpEventControl = _Event_NormalStatus;
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _NormalStatus_Standby;
            }
            break;


/*=======================================================================================================*/
        case _Task_RRoundabout_Chick:
            if(_SetupAngle > 0)
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
                    _StatusMonitor._fpEventControl = _Event_CrossWalk;
                    EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _Task_CrossWalk_Enable;
                }
                else
                {
                    EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Ready;
                }
            }
            else
            {
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Ready;
            }
            break;
        case _Task_RRoundabout_Ready:
            if(RWhitePnt1 > 5 && RWhitePnt2 > 10 && RWhitePnt3 > 15)
            {
                _Roundabout_Record = _Runing_Distance;
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Stare;
            }
            break;
        case _Task_RRoundabout_Stare:
            if(
                    RWhitePnt1 < 5 && (RWhitePnt2 < 10 && RWhitePnt3 < 15) &&
                    m_i16RTopcornerFindFlag == 1
              )
            {
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Enable;
            }
            else if(RWhitePnt1 > 15 && RWhitePnt2 > 10 && RWhitePnt3 > 5
                   && RWhitePnt1 > RWhitePnt3 && RTOP.m_i16y > 35)
            {
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Enable;
            }
            break;
        case _Task_RRoundabout_Enable:
            if(
                    RWhitePnt1 > 20 && RWhitePnt2 > 15 && RWhitePnt3 > 10
              )
              {
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Entry;
                BEEP_ON;
              }
            break;
        case _Task_RRoundabout_Entry:
            if (
                    ((ipts0_num == 80 && ipts1_num == 0) || m_i16RTopcornerFindFlag == 2) &&
                    m_u32LAllArea < 800 && m_i16WOutWidthROWNum  < 5 && LeftLoopYangle < 0.6f
               )

            {
                BEEP_OFF;
                Error_Count = 0;
                _Roundabout_Record = _Runing_Distance;
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Ring;
            }
            break;
        case _Task_RRoundabout_Ring:
            Average_Error[Error_Count] = m_f32LineAllError;
            if(++Error_Count > SBUFFSIZE)
            {
                Error_Count = SBUFFSIZE;
            }
            if(
                    m_i16LTopcornerFindFlag == 2 &&
                    m_i16RTopcornerFindFlag == 2 &&
                    m_i16RBotcornerFindFlag == 2 &&
                    _anglesignal.L_edge == _anglesignal_max          &&
                    ABS(_Runing_Distance - _Roundabout_Record) > 2000
                    )
           {
                float error_sum = 0;
                for(int i = 0; i < Error_Count; i++)
                {
                    error_sum += Average_Error[i];
                }
                _SpecialHandle_FixedError = error_sum / Error_Count;
                if(_SpecialHandle_FixedError < 75) _SpecialHandle_FixedError = 75;
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Exit;
           }
            break;
        case _Task_RRoundabout_Exit:
            if(
                    m_i16LTopcornerFindFlag == 2 &&
                    m_i16LBotcornerFindFlag == 2 &&
                    m_i16RTopcornerFindFlag == 2 &&
                    m_i16RBotcornerFindFlag == 2 &&
                    m_i16AbsoluteOutWidth    > 5
              )
              {
                /*这里可能要补线*/
                BEEP_ON;
                _Roundabout_Record = _Runing_Distance;
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Out;
              }
            else if(
                    (ipts0_num < 10 && ipts1_num < 10) &&
                    m_i16AbsoluteOutWidth    > 8
                    )
            {
                BEEP_ON;
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Out;
            }
            break;
        case _Task_RRoundabout_Out:
            if(LeftLoopYangle > 0.7f && m_i16RTopcornerFindFlag == 1 && m_i16AbsoluteOutWidth < 8)
            {
                _Roundabout_Record = _Runing_Distance;
                _SpecialHandle_FixedDeviation = 0;
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Finish;
            }
            else if(LeftLoopYangle > 0.5f && m_i16WOutWidthROWNum < 8)
            {
                _Roundabout_Record = _Runing_Distance;
                _SpecialHandle_FixedDeviation = 0;
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _Task_RRoundabout_Finish;
            }
            break;
        case _Task_RRoundabout_Finish:
            if(
                     (ipts1_num > 40) && (ipts0_num > 40) &&
                     (_Runing_Distance - _Roundabout_Record > 3000)
              )
            {
                    BEEP_OFF;
                    _StatusMonitor._fpEventControl = _Event_NormalStatus;
                    EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _NormalStatus_Standby;
            }
            else  if( ReftLoopYangle > 0.2  && m_i16AbsoluteOutWidth < 8
                    )
            {
                if(_Runing_Distance - _Roundabout_Record > 3000)
                {
                    _StatusMonitor._fpEventControl = _Event_NormalStatus;
                    EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _NormalStatus_Standby;
                }
            }
            else if(RTOP.m_i16y > 40 && m_i16RTopcornerFindFlag == 1 && _Runing_Distance - _Roundabout_Record > 3000)
            {
                BEEP_OFF;
                _StatusMonitor._fpEventControl = _Event_NormalStatus;
                EventStatusTask[_Event_Roundabout]._EventCurrentStatus = _NormalStatus_Standby;
            }
            break;
    }
}


void RepairRoundabout_TaskFunction(void)
{



    Int16 t_i16LoopY;                                       /*用来循环行*/
    Int16 t_i16LoopX;                                       /*用来循环列*/

    float Temp_KSlope = 0;
    Int16 Count_I1 = 0;
    Int16 Temp_LoopX = 0;

    uint8_t (*InImg)[IMGW];
    InImg = mt9v03x_output_image;

    ST_2D_INT_POINT_INFO Temp;

    switch(EventStatusTask[_Event_Roundabout]._EventCurrentStatus)
    {

        case _Task_LRoundabout_Ready:
            ElementHandle_SpecialStatus();
        break;
        case _Task_LRoundabout_Enable:
        case _Task_LRoundabout_Stare:
                for(Count_I1 = 0; Count_I1 < IMGH; Count_I1++)
                {
                   m_stLPnt[Count_I1].m_i16x =  m_stRPnt[Count_I1].m_i16x - TrackWide[Count_I1];
                   m_stLPnt[Count_I1].m_i16y = Count_I1;
                }
            break;
//        case _Task_LRoundabout_Enable:
//                  /*从左上点开始寻找 重新寻线*/
//                  if(m_i16LTopcornerFindFlag == 1)
//                  {
//                      /*顶点已存在*/
//                  }
//                  else
//                  {
//                        t_i16LoopX = 20;
//                        t_i16LoopY = IMGH;
//
//                        Temp.m_i16y = 0;
//
//                        while((--t_i16LoopY) > 0)
//                        {
//                            if(     InImg[t_i16LoopY - 1][t_i16LoopX] == B_BLACK &&
//                                    InImg[t_i16LoopY    ][t_i16LoopX] == B_WHITE &&
//                                    InImg[t_i16LoopY + 1][t_i16LoopX] == B_WHITE)
//                            {
//                                Temp.m_i16x = t_i16LoopX;
//                                Temp.m_i16y = t_i16LoopY;
//                                break;
//                            }
//                        }
//
//                        if(Temp.m_i16y)
//                        {
//                            far_ipts0_num = 80;
//                            findline_lefthand_adaptive(&img_raw, Threshold, Temp.m_i16x, Temp.m_i16y, far_ipts0, &far_ipts0_num);
//                            //找到拐点
//                            t_i16LoopY = 0;
//                            Temp.m_i16y = 0;
//
//                            while((++t_i16LoopY) < far_ipts0_num)
//                            {
//                                if(     far_ipts0[t_i16LoopY - 1][1] >  far_ipts0[t_i16LoopY][1] &&
//                                        far_ipts0[t_i16LoopY + 1][1] <  far_ipts0[t_i16LoopY][1]  )
//                                {
//                                    Temp.m_i16x = far_ipts0[t_i16LoopY][0];
//                                    Temp.m_i16y = far_ipts0[t_i16LoopY][1];
//                                    m_i16LTopcornerFindFlag = 1;
//                                    LTOP = Temp;
//                                    break;
//                                }
//                            }
//                        }
//                        else
//                        {
//                            LTOP.m_i16x = 2;
//                            LTOP.m_i16y = 10;
//                        }
//
//
//                  }
//                  /*根据左下角点 */
//                  t_i16LoopX = LTOP.m_i16x;
//                  t_i16LoopY = LTOP.m_i16y;
//                  Temp_KSlope = t_i16LoopX;
//
//                  m_stRPnt[LTOP.m_i16y] = LTOP;
//
//                  if(ipts1_num && LTOP.m_i16y < 40 && m_i16LTopcornerFindFlag == 1)
//                  {
//                      KSlope_Right = (float)(t_i16LoopX - m_stRPnt[LBOT.m_i16y].m_i16x)/(float)(t_i16LoopY - LBOT.m_i16y);
//                      while((++t_i16LoopY) < LBOT.m_i16y)
//                      {
//                          Temp_KSlope += KSlope_Right;
//                          m_stRPnt[t_i16LoopY].m_i16x = (int)Temp_KSlope;
//                          m_stRPnt[t_i16LoopY].m_i16y = t_i16LoopY;
////                          m_i8RMonotonicity[t_i16LoopY] = 0;
//                      }
//                  }
//                  else
//                  {
//                      KSlope_Right = (float)(t_i16LoopX - (IMGW - 40))/(float)(LTOP.m_i16y-58);
//                      while((++t_i16LoopY) < 60)
//                      {
//                          Temp_KSlope += KSlope_Right;
//                          m_stRPnt[t_i16LoopY].m_i16x = (int)Temp_KSlope;
//                          m_stRPnt[t_i16LoopY].m_i16y = t_i16LoopY;
////                          m_i8RMonotonicity[t_i16LoopY] = 0;
//                      }
//                  }
//
//                  t_i16LoopX = LTOP.m_i16x;
//                  t_i16LoopY = LTOP.m_i16y;
//                  Temp_KSlope = t_i16LoopX;
////                  m_i8RMonotonicity[t_i16LoopY] = 0;
//
//                  while((--t_i16LoopY) > -1)
//                  {
//                      Temp_KSlope -= KSlope_Right;
//                      m_stRPnt[t_i16LoopY].m_i16x = (int)Temp_KSlope;
//                      m_stRPnt[t_i16LoopY].m_i16y = t_i16LoopY;
//
//                      if(m_stRPnt[t_i16LoopY].m_i16x < 1)
//                          m_stRPnt[t_i16LoopY].m_i16x = 1;
//
////                      m_i8RMonotonicity[t_i16LoopY] = 0;
//                  }
//
//                  t_i16LoopY = IMGH;
//                  while((--t_i16LoopY) > -1)
//                  {
//                      m_i8RMonotonicity[t_i16LoopY] = 0;
//                  }
//
//                  //处理左边线
//                  if(ipts0_num && LTOP.m_i16y < 40 && m_i16LTopcornerFindFlag == 1)
//                  {
//                      t_i16LoopY = ipts0[ipts0_num - 1][1];
//                  }
//                  else
//                  {
//                      t_i16LoopY = IMGH - 1;
//                  }
//
//                  while((--t_i16LoopY) > -1)
//                  {
//                      m_stLPnt[t_i16LoopY].m_i16y = 0;
//                  }
//
//                  t_i16LoopY = IMGH - 1;
//
//                  while((--t_i16LoopY) > -1 && m_stLPnt[t_i16LoopY].m_i16y)
//                  {
//                      if(m_stLPnt[t_i16LoopY].m_i16x > m_stRPnt[t_i16LoopY].m_i16x)
//                      {
//                          m_stLPnt[t_i16LoopY].m_i16y = 0;
//                      }
//                  }
//
//            break;
        case _Task_LRoundabout_Entry:

            t_i16LoopX = 20;
            t_i16LoopY = IMGH;

            Temp.m_i16y = 0;

            while((--t_i16LoopY) > 0)
            {
                if(     InImg[t_i16LoopY - 1][t_i16LoopX] == B_BLACK &&
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
                far_ipts0_num = 120;
                findline_lefthand_adaptive(&img_raw, Threshold, Temp.m_i16x, Temp.m_i16y, far_ipts0, &far_ipts0_num);
                //找到拐点
                t_i16LoopY = 0;
                Temp.m_i16y = 0;

                while((++t_i16LoopY) < far_ipts0_num)
                {
                    if(     far_ipts0[t_i16LoopY - 1][1] >  far_ipts0[t_i16LoopY][1] &&
                            far_ipts0[t_i16LoopY + 1][1] <  far_ipts0[t_i16LoopY][1]  )
                    {
                        Temp.m_i16x = far_ipts0[t_i16LoopY][0];
                        Temp.m_i16y = far_ipts0[t_i16LoopY][1];
                        MSPOT = Temp;
                        break;
                    }
                }

                if(Temp.m_i16y)
                {

                }
                else
                {
                    Temp.m_i16x = 2;
                    Temp.m_i16y = 10;
                }

                KSlope_Right = (float)(Temp.m_i16x - (IMGW - 50))/(float)(Temp.m_i16y-58);

                Temp_KSlope = Temp.m_i16x;
                Count_I1    = Temp.m_i16y;

                m_stRPnt[Temp.m_i16y] = Temp;

                while((++Count_I1) < IMGH)
                {
                    Temp_KSlope += KSlope_Right;
                    m_stRPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                    m_stRPnt[Count_I1].m_i16y = Count_I1;
                }

                while((--Count_I1) > 0)
                {
                    Temp_KSlope -= KSlope_Right;
                    m_stRPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                    m_stRPnt[Count_I1].m_i16y = Count_I1;

                    if(m_stRPnt[Count_I1].m_i16x < 2)
                        m_stRPnt[Count_I1].m_i16x = 2;
                }

                Count_I1 = 0;
                while((++Count_I1) < IMGH)
                {
                    m_stLPnt[Count_I1].m_i16y = 0;
                    m_i8RMonotonicity[Count_I1] = 0;
                }

            }
            else
            {
                Temp.m_i16x = 2;
                Temp.m_i16y = 10;

                KSlope_Right = (float)(Temp.m_i16x - (IMGW - 60))/(float)(Temp.m_i16y-58);

                Temp_KSlope = Temp.m_i16x;
                Count_I1    = Temp.m_i16y;
                m_stRPnt[Count_I1] = Temp;
                while((++Count_I1) < IMGH)
                {
                     Temp_KSlope += KSlope_Right;
                     m_stRPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                     m_stRPnt[Count_I1].m_i16y = Count_I1;
                }

                while((--Count_I1) > 0)
                {
                    Temp_KSlope -= KSlope_Right;
                    m_stRPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                    m_stRPnt[Count_I1].m_i16y = Count_I1;

                    if(m_stRPnt[Count_I1].m_i16x < 2)
                        m_stRPnt[Count_I1].m_i16x = 2;
                }

                Count_I1 = 0;
                while((++Count_I1) < IMGH)
                {
                    m_stLPnt[Count_I1].m_i16y = 0;
                    m_i8RMonotonicity[Count_I1] = 0;
                }
            }
            break;
        case _Task_LRoundabout_Exit:
              if(m_i16RBotcornerFindFlag == 1)
              {
                  Temp_KSlope = RBOT.m_i16x;
                  KSlope_Right = regression(m_stRPnt, RBOT.m_i16y + 5, RBOT.m_i16y);
                  for(Count_I1 = RBOT.m_i16y - 1; Count_I1 > 0; Count_I1--)
                  {
                      Temp_KSlope -= KSlope_Right;
                      m_stRPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                      m_stRPnt[Count_I1].m_i16y = Count_I1;
                      m_i8LMonotonicity[Count_I1] = 0;
                      if(m_stRPnt[Count_I1].m_i16x < 0)
                      {
                          m_stRPnt[Count_I1].m_i16x = 1;
                          m_stRPnt[Count_I1].m_i16y = Count_I1;
                          m_i8LMonotonicity[Count_I1] = -1;
                      }
                  }
                  Count_I1 = -1;
                  Temp_LoopX = 0;
                  while((++Count_I1) < IMGH)
                  {
                      if(m_stLPnt[Count_I1].m_i16x == 1)
                      {
                          Temp_LoopX++;
                          if(Temp_LoopX > 5)
                              break;
                      }
                      else
                      {
                         m_stLPnt[Count_I1].m_i16x = 1;
                         m_i8LMonotonicity[Count_I1] = -1;
                      }

                  }
              }
              else
              {
                  LTOP.m_i16x = 2;
                  LTOP.m_i16y = 1;

                  RBOT.m_i16x = IMGW - 40;
                  RBOT.m_i16y = IMGH - 1;

                  Temp_KSlope = RBOT.m_i16x;
                  KSlope_Right = (float)(LTOP.m_i16x - RBOT.m_i16x)/(float)(LTOP.m_i16y-RBOT.m_i16y);

                  for(Count_I1 = RBOT.m_i16y - 1; Count_I1 > LTOP.m_i16y; Count_I1--)
                  {
                      Temp_KSlope -= KSlope_Right;
                      m_stRPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                      m_stRPnt[Count_I1].m_i16y = Count_I1;
                      if(m_stRPnt[Count_I1].m_i16x > IMGW - 2)
                      {
                          m_stRPnt[Count_I1].m_i16x = IMGW - 2;
                      }
                      else if(m_stRPnt[Count_I1].m_i16x < 1)
                      {
                          m_stRPnt[Count_I1].m_i16x = 1;
                      }
                      m_stLPnt[Count_I1].m_i16x = 0;
                      m_stLPnt[Count_I1].m_i16y = 0;
                  }

              }
            break;
        case _Task_LRoundabout_Out:
                _SpecialHandle_FixedDeviation = 1;
                m_stOptimalPoint.m_i16x = IMGW / 2;

            break;
        case _Task_LRoundabout_Finish:
            if(m_i16LTopcornerFindFlag == 1)
            {
                Temp_KSlope = LTOP.m_i16x;
                KSlope_Left = regression(m_stLPnt, LTOP.m_i16y, LTOP.m_i16y - 5);
                for(Count_I1 = LTOP.m_i16y + 1; Count_I1 < IMGH; Count_I1++)
                {
                    Temp_KSlope += KSlope_Left;
                    m_stLPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                    m_stLPnt[Count_I1].m_i16y = Count_I1;
                    if(m_stLPnt[Count_I1].m_i16x < 0)
                    {
                        m_stLPnt[Count_I1].m_i16x = 1;
                    }
                }
            }
            else
            {
                for(Count_I1 = 0; Count_I1 < IMGH; Count_I1++)
                {
                   m_stLPnt[Count_I1].m_i16x =  m_stRPnt[Count_I1].m_i16x - TrackWide[Count_I1];
                   m_stLPnt[Count_I1].m_i16y = Count_I1;
                }
            }
            break;

            /************************************************************************/
            //右面补线
        case _Task_RRoundabout_Ready:
            ElementHandle_SpecialStatus();
            break;
        case _Task_RRoundabout_Enable:
        case _Task_RRoundabout_Stare:
            for(Count_I1 = 0; Count_I1 < IMGH; Count_I1++)
            {
               m_stRPnt[Count_I1].m_i16x =  m_stLPnt[Count_I1].m_i16x + TrackWide[Count_I1];
               m_stRPnt[Count_I1].m_i16y = Count_I1;
            }
            break;

//        case _Task_RRoundabout_Enable:
//            if(m_i16RTopcornerFindFlag == 1)
//            {
//
//            }
//            else
//            {
//                RTOP.m_i16x = IMGW -2;
//                RTOP.m_i16y = 10;
//
//                t_i16LoopX = IMGW - 20;
//                t_i16LoopY = IMGH;
//
//                Temp.m_i16y = 0;
//
//                while((--t_i16LoopY) > 0)
//                {
//                    if(     InImg[t_i16LoopY - 1][t_i16LoopX] == B_BLACK &&
//                            InImg[t_i16LoopY    ][t_i16LoopX] == B_WHITE &&
//                            InImg[t_i16LoopY + 1][t_i16LoopX] == B_WHITE)
//                    {
//                        Temp.m_i16x = t_i16LoopX;
//                        Temp.m_i16y = t_i16LoopY;
//                        break;
//                    }
//                }
//
//                if(Temp.m_i16y)
//                {
//                   far_ipts1_num = 120;
//                   findline_righthand_adaptive(&img_raw, Threshold, Temp.m_i16x, Temp.m_i16y, fat_ipts1, &far_ipts1_num);
//                   //找到拐点
//                   t_i16LoopY = 0;
//                   Temp.m_i16y = 0;
//                   while((++t_i16LoopY) < far_ipts1_num)
//                   {
//                       if(     fat_ipts1[t_i16LoopY - 1][1] >  fat_ipts1[t_i16LoopY][1] &&
//                               fat_ipts1[t_i16LoopY + 1][1] <  fat_ipts1[t_i16LoopY][1]  )
//                       {
//                           Temp.m_i16x = fat_ipts1[t_i16LoopY][0];
//                           Temp.m_i16y = fat_ipts1[t_i16LoopY][1];
//                           RTOP = Temp;
//                           m_i16RTopcornerFindFlag = 1;
//                           break;
//                       }
//                   }
//                }
//
//            }
//
//            m_stLPnt[RTOP.m_i16y] = RTOP;
//
//            t_i16LoopX = RTOP.m_i16x;
//            t_i16LoopY = RTOP.m_i16y;
//            Temp_KSlope = t_i16LoopX;
//
//            if(ipts0_num && RTOP.m_i16y < 40 && m_i16RTopcornerFindFlag == 1)
//            {
//                KSlope_Left = (float)(t_i16LoopX - m_stLPnt[RBOT.m_i16y].m_i16x)/(float)(t_i16LoopY - RBOT.m_i16y);
//                while((++t_i16LoopY) < RBOT.m_i16y)
//                {
//                    Temp_KSlope += KSlope_Left;
//                    m_stLPnt[t_i16LoopY].m_i16x = (int)Temp_KSlope;
//                    m_stLPnt[t_i16LoopY].m_i16y = t_i16LoopY;
//                }
//            }
//            else
//            {
//                KSlope_Left = (float)(t_i16LoopX - 40)/(float)(RTOP.m_i16y-58);
//                while((++t_i16LoopY) < 60)
//                {
//                    Temp_KSlope += KSlope_Left;
//                    m_stLPnt[t_i16LoopY].m_i16x = (int)Temp_KSlope;
//                    m_stLPnt[t_i16LoopY].m_i16y = t_i16LoopY;
//                }
//            }
//
//            t_i16LoopX = RTOP.m_i16x;
//            t_i16LoopY = RTOP.m_i16y;
//            Temp_KSlope = t_i16LoopX;
//            while((--t_i16LoopY) > -1)
//            {
//                Temp_KSlope -= KSlope_Left;
//                m_stLPnt[t_i16LoopY].m_i16x = (int)Temp_KSlope;
//                m_stLPnt[t_i16LoopY].m_i16y = t_i16LoopY;
//
//                if(m_stLPnt[t_i16LoopY].m_i16x > IMGW - 2)
//                {
//                    m_stLPnt[t_i16LoopY].m_i16x = IMGW - 2;
//                }
//            }
//
//            t_i16LoopY = IMGH;
//            while((--t_i16LoopY) > -1)
//            {
//                m_i8LMonotonicity[t_i16LoopY] = 0;
//            }
//
//            if(ipts1_num && RTOP.m_i16y < 40 && m_i16RTopcornerFindFlag == 1)
//            {
//                t_i16LoopY = ipts1[ipts1_num - 1][1];
//            }
//            else
//            {
//                t_i16LoopY = IMGH - 1;
//            }
//
//            while((--t_i16LoopY) > -1)
//            {
//                m_stRPnt[t_i16LoopY].m_i16y = 0;
//            }
//
//            t_i16LoopY = IMGH - 1;
//
//            while((--t_i16LoopY) > -1 && m_stRPnt[t_i16LoopY].m_i16y)
//            {
//                if(m_stLPnt[t_i16LoopY].m_i16x > m_stRPnt[t_i16LoopY].m_i16x)
//                {
//                    m_stRPnt[t_i16LoopY].m_i16y = 0;
//                }
//            }
//            break;
//

        case _Task_RRoundabout_Entry:

            t_i16LoopX = IMGW - 20;
            t_i16LoopY = IMGH;

            Temp.m_i16y = 0;

            while((--t_i16LoopY) > 0)
            {
                if(     InImg[t_i16LoopY - 1][t_i16LoopX] == B_BLACK &&
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
               far_ipts1_num = 80;
               findline_righthand_adaptive(&img_raw, Threshold, Temp.m_i16x, Temp.m_i16y, fat_ipts1, &far_ipts1_num);
               //找到拐点
               t_i16LoopY = 0;
               Temp.m_i16y = 0;
               while((++t_i16LoopY) < far_ipts1_num)
               {
                   if(     fat_ipts1[t_i16LoopY - 1][1] >  fat_ipts1[t_i16LoopY][1] &&
                           fat_ipts1[t_i16LoopY + 1][1] <  fat_ipts1[t_i16LoopY][1]  )
                   {
                       Temp.m_i16x = fat_ipts1[t_i16LoopY][0];
                       Temp.m_i16y = fat_ipts1[t_i16LoopY][1];
                       MSPOT = Temp;
                       break;
                   }
               }

               if(Temp.m_i16y)
               {

               }
               else
               {
                   Temp.m_i16x = IMGW - 2;
                   Temp.m_i16y = 10;
               }

               t_i16LoopY = IMGH;
               while((--t_i16LoopY) > 0)
               {
                   m_stRPnt[t_i16LoopY].m_i16y = 0;
               }

               /*右边线 补线*/
               m_stLPnt[Temp.m_i16y] = Temp;
               KSlope_Left = (float)(Temp.m_i16x - 50)/(float)(Temp.m_i16y-58);

               Temp_KSlope = Temp.m_i16x;
               Count_I1    = Temp.m_i16y;
               m_stLPnt[Count_I1] = Temp;
               while((++Count_I1) < IMGH)
               {
                   Temp_KSlope += KSlope_Left;
                   m_stLPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                   m_stLPnt[Count_I1].m_i16y = Count_I1;
               }

               while((--Count_I1) > 0)
               {
                   Temp_KSlope -= KSlope_Left;
                   m_stLPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                   m_stLPnt[Count_I1].m_i16y = Count_I1;

                   if(m_stLPnt[Count_I1].m_i16x > IMGW - 2)
                       m_stLPnt[Count_I1].m_i16x = IMGW - 2;
               }

               Count_I1 = 0;
               while((++Count_I1) < IMGH)
               {
                   m_stRPnt[Count_I1].m_i16y = 0;
                   m_i8LMonotonicity[Count_I1] = 0;
               }

            }
            else
            {
                /*固定补线*/
                Temp.m_i16x = IMGW - 2;
                Temp.m_i16y = 10;

                KSlope_Left = (float)(Temp.m_i16x - 60)/(float)(Temp.m_i16y-58);

                Temp_KSlope = Temp.m_i16x;
                Count_I1    = Temp.m_i16y;
                m_stLPnt[Count_I1] = Temp;
                while((++Count_I1) < IMGH)
                {
                     Temp_KSlope += KSlope_Left;
                     m_stLPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                     m_stLPnt[Count_I1].m_i16y = Count_I1;
                }

                while((--Count_I1) > 0)
                {
                    Temp_KSlope -= KSlope_Left;
                    m_stLPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                    m_stLPnt[Count_I1].m_i16y = Count_I1;

                    if(m_stLPnt[Count_I1].m_i16x > IMGW - 2)
                        m_stLPnt[Count_I1].m_i16x = IMGW - 2;
                }

                Count_I1 = 0;
                while((++Count_I1) < IMGH)
                {
                    m_stRPnt[Count_I1].m_i16y = 0;
                    m_i8LMonotonicity[Count_I1] = 0;
                }

            }
            break;
        case _Task_RRoundabout_Exit:
            if(m_i16LBotcornerFindFlag == 1)
            {
                Temp_KSlope = LBOT.m_i16x;
                KSlope_Left = regression(m_stLPnt, LBOT.m_i16y + 5, LBOT.m_i16y);
                for(Count_I1 = LBOT.m_i16y - 1; Count_I1 > 0; Count_I1--)
                {
                    Temp_KSlope -= KSlope_Left;
                    m_stLPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                    m_stLPnt[Count_I1].m_i16y = Count_I1;
                    m_i8LMonotonicity[Count_I1] = 0;
                    if(m_stLPnt[Count_I1].m_i16x > IMGW - 2)
                    {
                        m_stLPnt[Count_I1].m_i16x = IMGW - 2;
                        m_stLPnt[Count_I1].m_i16y = Count_I1;
                        m_i8LMonotonicity[Count_I1] = -1;
                    }
                }

                Count_I1 = -1;
                Temp_LoopX = 0;
                while((++Count_I1) < IMGH)
                {
                    if(m_stRPnt[Count_I1].m_i16x == IMGW - 2)
                    {
                        Temp_LoopX++;
                        if(Temp_LoopX > 5)
                            break;
                    }
                    else
                    {
                       m_stRPnt[Count_I1].m_i16x = IMGW - 2;
                       m_i8RMonotonicity[Count_I1] = -1;
                    }

                }
            }
            else
            {
                RTOP.m_i16x = IMGW-2;
                RTOP.m_i16y = 1;

                LBOT.m_i16x = 40;
                LBOT.m_i16y = IMGH - 1;

                Temp_KSlope = LBOT.m_i16x;
                KSlope_Left = (float)(RTOP.m_i16x - LBOT.m_i16x)/(float)(RTOP.m_i16y-LBOT.m_i16y);
                for(Count_I1 = LBOT.m_i16y - 1; Count_I1 > RTOP.m_i16y; Count_I1--)
                {
                    Temp_KSlope -= KSlope_Left;
                    m_stLPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                    m_stLPnt[Count_I1].m_i16y = Count_I1;
                    if(m_stLPnt[Count_I1].m_i16x > IMGW - 2)
                    {
                        m_stLPnt[Count_I1].m_i16x = IMGW - 2;
                    }
                    else if(m_stLPnt[Count_I1].m_i16x < 1)
                    {
                        m_stLPnt[Count_I1].m_i16x = 1;
                    }
                    m_stRPnt[Count_I1].m_i16x = 0;
                    m_stRPnt[Count_I1].m_i16y = 0;
                }

            }
               break;
           case _Task_RRoundabout_Out:
               _SpecialHandle_FixedDeviation = 1;
               m_stOptimalPoint.m_i16x = IMGW / 2;
               break;
           case _Task_RRoundabout_Finish:
               if(m_i16RTopcornerFindFlag == 1)
               {
                   Temp_KSlope = RTOP.m_i16x;
                   KSlope_Right = regression(m_stRPnt, RTOP.m_i16y, RTOP.m_i16y - 5);
                   for(Count_I1 = RTOP.m_i16y + 1; Count_I1 < IMGH; Count_I1++)
                   {
                       Temp_KSlope += KSlope_Right;
                       m_stRPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                       m_stRPnt[Count_I1].m_i16y = Count_I1;
                       if(m_stLPnt[Count_I1].m_i16x > IMGW - 2)
                       {
                           m_stLPnt[Count_I1].m_i16x = IMGW - 2;
                       }
                   }
               }
               else
               {
                   for(Count_I1 = 0; Count_I1 < IMGH; Count_I1++)
                   {
                      m_stRPnt[Count_I1].m_i16x =  m_stLPnt[Count_I1].m_i16x + TrackWide[Count_I1];
                      m_stRPnt[Count_I1].m_i16y = Count_I1;
                   }
               }
               break;

    }
}


