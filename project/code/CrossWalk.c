













#include "CrossWalk.h"


ST_2D_INT_POINT_INFO _CrossWalkStarePoint;
Uint8 _CrossWalkDirectionFlag = _CrossWalk_Right;
Uint8 _CrossWalkStareRowNumber;
Uint8 _CrossWalkErrorControl; //误差值
Uint8 _CrossWalkCircle = 0;   //圈数
Uint8 _CrossWalkRecord = 0;

float Stop_Distance;
float _StopAngle = 100;

float _CrossWalkAngle_Record;
Uint8 CrossWalk_SetupPoint;
float stop_dis = 5000;
Uint64 _CrossWalk_Record;

void StatusCrossWalk_TaskFunction(void)
{
     Int16 t_i16LoopX;
     Int16 t_i16LoopY;

     Uint16 TargetCount = 0;
     Uint16 TargetRegion = 0;


     uint8_t (*InImg)[IMGW];
     InImg = mt9v03x_output_image;

     switch(EventStatusTask[_Event_CrossWalk]._EventCurrentStatus)
     {
          case _Task_CrossWalk_Chick:
              /*可以先检测一次 然后在通过多检测几次 来加强识别*/
              TargetRegion = 0;
              for(t_i16LoopY = _CrossWalk_UpperLine; t_i16LoopY < _CrossWalk_BottowLine; t_i16LoopY++)
              {
                  TargetCount = 0;
                  for(t_i16LoopX = 30 ; t_i16LoopX < IMGW - 30; t_i16LoopX++)
                  {
                      if(
                              InImg[t_i16LoopY][t_i16LoopX - 1] == B_WHITE &&
                              InImg[t_i16LoopY][t_i16LoopX]     == B_BLACK &&
                              InImg[t_i16LoopY][t_i16LoopX + 1] == B_BLACK
                        )
                      {
                          TargetCount++;
                      }
                      else if(
                              InImg[t_i16LoopY][t_i16LoopX - 1] == B_BLACK &&
                              InImg[t_i16LoopY][t_i16LoopX]     == B_WHITE &&
                              InImg[t_i16LoopY][t_i16LoopX + 1] == B_WHITE
                        )
                      {
                          TargetCount++;
                      }

                  }

                  if(TargetCount > 7)
                  {
                      TargetRegion++;

                      if(TargetRegion > 2)
                          break;
                  }
              }

              if(TargetRegion > 2)
              {
                  EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _Task_CrossWalk_Enable;
              }
              else
              {
                  _StatusMonitor._fpEventControl = _Event_NormalStatus;
                  EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _NormalStatus_Standby;
              }
              break;
          case _Task_CrossWalk_Enable:
              if(++_CrossWalkCircle > _CrossWalk_Circle - 1)
              {

                  _CrossWalk_Record = _Runing_Distance;
                  EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _Task_CrossWalk_Exit;
              }
              else
              {
                  _CrossWalk_Record = _Runing_Distance;
                  EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _Task_CrossWalk_Waking;
              }
              break;
          case _Task_CrossWalk_Waking:
              if(_CrossWalkDirectionFlag == _CrossWalk_Right)
              {
                  if(m_i16RBotcornerFindFlag == 2 || (Lpt1_found == true && Lpt1_rpts1s_id < 2))
                  {
                      _CrossWalk_Record = _Runing_Distance;
                      EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _Task_CrossWalk_Outing;
                  }
              }
              else if(_CrossWalkDirectionFlag == _CrossWalk_Left)
              {
                  if(m_i16LBotcornerFindFlag == 2 || (Lpt0_found == true && Lpt0_rpts0s_id < 2))
                  {
                      _CrossWalk_Record = _Runing_Distance;
                      EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _Task_CrossWalk_Outing;
                  }
              }
              else if(_Runing_Distance - _CrossWalk_Record > 4600)
              {
                  _CrossWalk_Record = _Runing_Distance;
                  EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _Task_CrossWalk_Outing;
              }
              break;
          case _Task_CrossWalk_Outing: //直行
              if(_CrossWalkDirectionFlag == _CrossWalk_Right && m_i16RTopcornerFindFlag != 1 && ipts1_num > 40 && m_i16WOutWidthROWNum < 5)
              {
                  _StatusMonitor._fpEventControl = _Event_NormalStatus;
                  EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _NormalStatus_Standby;
              }
              else if(_CrossWalkDirectionFlag == _CrossWalk_Left && m_i16LTopcornerFindFlag != 1 && ipts0_num > 40 && m_i16WOutWidthROWNum < 5)
              {
                  _StatusMonitor._fpEventControl = _Event_NormalStatus;
                  EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _NormalStatus_Standby;
              }
              else if(_Runing_Distance - _CrossWalk_Record > 6000)
              {
                  _StatusMonitor._fpEventControl = _Event_NormalStatus;
                  EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _NormalStatus_Standby;
              }
              break;
          case _Task_CrossWalk_Finish:
              /*入库*/
              if(AllAreaWhitePoint < 1200)
              {
                  VehicleInformation.StartDeparture = _VehicleInformation_Stop;
                  EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _Task_CrossWalk_Exit;
              }

              else if(_Runing_Distance - _CrossWalk_Record > 3600)
              {
                  VehicleInformation.StartDeparture = _VehicleInformation_Stop;
                  EventStatusTask[_Event_CrossWalk]._EventCurrentStatus = _Task_CrossWalk_Exit;
              }
              break;
          case _Task_CrossWalk_Exit:
              if(_Runing_Distance - _CrossWalk_Record > (int)stop_dis)
              {
                VehicleInformation.StartDeparture = _VehicleInformation_Stop;
              }
//              if(_CrossWalkDirectionFlag == _CrossWalk_Right)
//              {
//                  m_stOptimalPoint = m_stROptimalPoint;
//              }
//              else
//              {
//                  m_stOptimalPoint = m_stLOptimalPoint;
//              }
              /*入库*/


              break;

              //-------------------------------------------------------------------------------------

          case _Task_CrossWalk_LCheck:
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
                     _StatusMonitor._fpEventControl = _Event_NormalStatus;
                 }
              }
              break;


          case _Task_CrossWalk_RCheck:
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
                     _StatusMonitor._fpEventControl = _Event_NormalStatus;
                 }
              }
             break;
        }
}


void RepairCrossWalk_TaskFunction(void)
{

    float Temp_KSlope = 0;
    Int16 Count_I1;
    Int16 t_i16LoopY;
    Int16 t_i16LoopX;

    ST_2D_INT_POINT_INFO Temp;

    uint8_t (*InImg)[IMGW];
    InImg = mt9v03x_output_image;

    switch(EventStatusTask[_Event_CrossWalk]._EventCurrentStatus)
    {
        case _Task_CrossWalk_Waking:

            if(_CrossWalkDirectionFlag == _CrossWalk_Right)
            {

                if(m_i16RBotcornerFindFlag == 1)
                {
                    //重新找点
                    t_i16LoopX = RBOT.m_i16x;
                    t_i16LoopY = RBOT.m_i16y - 5;

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
                        Temp.m_i16x = fat_ipts1[far_ipts1_num - 1][0];
                        Temp.m_i16y = fat_ipts1[far_ipts1_num - 1][1];
                        while((++t_i16LoopY) < far_ipts1_num)
                        {
                            if(
                                    fat_ipts1[t_i16LoopY - 1][0] >  fat_ipts1[t_i16LoopY][0] &&
                                    fat_ipts1[t_i16LoopY - 1][1] >  fat_ipts1[t_i16LoopY][1] &&
                                    fat_ipts1[t_i16LoopY + 1][0] <  fat_ipts1[t_i16LoopY][0] &&
                                    fat_ipts1[t_i16LoopY + 1][1] <  fat_ipts1[t_i16LoopY][1] )
                            {
                                Temp.m_i16x = fat_ipts1[t_i16LoopY][0];
                                Temp.m_i16y = fat_ipts1[t_i16LoopY][1];
                                break;
                            }
                        }

                        Temp_KSlope = Temp.m_i16x;
                        Count_I1    = Temp.m_i16y;
                        KSlope_Right = (float)(Temp.m_i16x - RBOT.m_i16x)/(float)(Temp.m_i16y-RBOT.m_i16y);

                        while((++Count_I1) < RBOT.m_i16y)
                        {
                            Temp_KSlope += KSlope_Right;
                            m_stRPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                            m_stRPnt[Count_I1].m_i16y = Count_I1;
                        }
                    }
                    else
                    {
                        Temp_KSlope = RBOT.m_i16x;
                        KSlope_Right = regression(m_stRPnt, RBOT.m_i16y + 5, RBOT.m_i16y);

                        for(Count_I1 = RBOT.m_i16y - 1; Count_I1 > 0; Count_I1--)
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
            }
            else
            {
                if(m_i16LBotcornerFindFlag == 1)
                {
                    //重新找点
                    t_i16LoopX = LBOT.m_i16x;
                    t_i16LoopY = LBOT.m_i16y - 5;

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
                       far_ipts0_num = 80;
                       findline_lefthand_adaptive(&img_raw, Threshold, Temp.m_i16x, Temp.m_i16y, far_ipts0, &far_ipts0_num);
                       //找到拐点
                       t_i16LoopY = 0;
                       Temp.m_i16x = far_ipts0[far_ipts1_num - 1][0];
                       Temp.m_i16y = far_ipts0[far_ipts1_num - 1][1];
                       while((++t_i16LoopY) < far_ipts0_num)
                       {
                           if(
//                                   fat_ipts1[t_i16LoopY - 1][0] >  fat_ipts1[t_i16LoopY][0] &&
                                   far_ipts0[t_i16LoopY - 1][1] >  far_ipts0[t_i16LoopY][1] &&
//                                   fat_ipts1[t_i16LoopY + 1][0] <  fat_ipts1[t_i16LoopY][0] &&
                                   far_ipts0[t_i16LoopY + 1][1] <  far_ipts0[t_i16LoopY][1] )
                           {
                               Temp.m_i16x = far_ipts0[t_i16LoopY][0];
                               Temp.m_i16y = far_ipts0[t_i16LoopY][1];
                               break;
                           }
                       }

                       Temp_KSlope = Temp.m_i16x;
                       Count_I1    = Temp.m_i16y;
                       KSlope_Left = (float)(Temp.m_i16x - LBOT.m_i16x)/(float)(Temp.m_i16y-LBOT.m_i16y);

                       while((++Count_I1) < LBOT.m_i16y)
                       {
                           Temp_KSlope += KSlope_Left;
                           m_stLPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                           m_stLPnt[Count_I1].m_i16y = Count_I1;
                       }

                   }
                   else
                   {
                       Temp_KSlope = LBOT.m_i16x;
                       KSlope_Right = regression(m_stLPnt, LBOT.m_i16y + 5, LBOT.m_i16y);

                       for(Count_I1 = LBOT.m_i16y - 1; Count_I1 > 0; Count_I1--)
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
            }
            break;
        case _Task_CrossWalk_Outing:

            if(_CrossWalkDirectionFlag == _CrossWalk_Right)
            {
                t_i16LoopX = IMGW - 30;
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
                    Temp.m_i16x = fat_ipts1[far_ipts1_num - 1][0];
                    Temp.m_i16y = fat_ipts1[far_ipts1_num - 1][1];
                    while((++t_i16LoopY) < far_ipts1_num)
                    {
                        if(
                                fat_ipts1[t_i16LoopY - 1][0] >  fat_ipts1[t_i16LoopY][0] &&
                                fat_ipts1[t_i16LoopY - 1][1] >  fat_ipts1[t_i16LoopY][1] &&
                                fat_ipts1[t_i16LoopY + 1][0] <  fat_ipts1[t_i16LoopY][0] &&
                                fat_ipts1[t_i16LoopY + 1][1] <  fat_ipts1[t_i16LoopY][1] )
                        {
                            Temp.m_i16x = fat_ipts1[t_i16LoopY][0];
                            Temp.m_i16y = fat_ipts1[t_i16LoopY][1];
                            break;
                        }
                    }

                    Temp_KSlope = Temp.m_i16x;
                    Count_I1    = Temp.m_i16y;
                    KSlope_Right = (float)(m_stRPnt[Count_I1 - 5].m_i16x - Temp.m_i16x)/(float)(m_stRPnt[Count_I1 - 5].m_i16y- Temp.m_i16y);

                    while((++Count_I1) < IMGH)
                    {
                        Temp_KSlope += KSlope_Right;
                        m_stRPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                        m_stRPnt[Count_I1].m_i16y = Count_I1;
                    }
                }
                else
                {
                    Temp_KSlope = m_stRPnt[RTOP.m_i16y].m_i16x;
                    KSlope_Right = regression(m_stRPnt, RTOP.m_i16y, RTOP.m_i16y - 6);

                    for(Count_I1 = RTOP.m_i16y + 1; Count_I1 < IMGH; Count_I1++)
                    {
                        Temp_KSlope += KSlope_Right;
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
                    }
                }
            }
            else
            {
                //----------------------------------------------------------------------------------

                t_i16LoopX = 30;
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
                 far_ipts0_num = 80;
                 findline_lefthand_adaptive(&img_raw, Threshold, Temp.m_i16x, Temp.m_i16y, far_ipts0, &far_ipts0_num);
                 //找到拐点
                 t_i16LoopY = 0;
                 Temp.m_i16x = far_ipts0[far_ipts1_num - 1][0];
                 Temp.m_i16y = far_ipts0[far_ipts1_num - 1][1];
                 while((++t_i16LoopY) < far_ipts0_num)
                 {
                     if(
//                                   fat_ipts1[t_i16LoopY - 1][0] >  fat_ipts1[t_i16LoopY][0] &&
                             far_ipts0[t_i16LoopY - 1][1] >  far_ipts0[t_i16LoopY][1] &&
//                                   fat_ipts1[t_i16LoopY + 1][0] <  fat_ipts1[t_i16LoopY][0] &&
                             far_ipts0[t_i16LoopY + 1][1] <  far_ipts0[t_i16LoopY][1] )
                     {
                         Temp.m_i16x = far_ipts0[t_i16LoopY][0];
                         Temp.m_i16y = far_ipts0[t_i16LoopY][1];
                         break;
                     }
                 }

                Temp_KSlope = Temp.m_i16x;
                Count_I1    = Temp.m_i16y;
                KSlope_Left = (float)(m_stLPnt[Count_I1 - 5].m_i16x - Temp.m_i16x)/(float)(m_stLPnt[Count_I1 - 5].m_i16y- Temp.m_i16y);


                while((++Count_I1) < IMGH)
                {
                  Temp_KSlope += KSlope_Left;
                  m_stLPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                  m_stLPnt[Count_I1].m_i16y = Count_I1;
                }
             }
             else
             {
                Temp_KSlope = m_stLPnt[LTOP.m_i16y].m_i16x;
                KSlope_Left = regression(m_stLPnt, LTOP.m_i16y, LTOP.m_i16y - 6);

                for(Count_I1 = LTOP.m_i16y + 1; Count_I1 < IMGH; Count_I1++)
                {
                    Temp_KSlope += KSlope_Left;
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
                }
             }
            }
            break;
        case _Task_CrossWalk_Finish:
            if(_CrossWalkDirectionFlag == _CrossWalk_Right)
            {
                _SpecialHandle_FixedDeviation = 0;
                t_i16LoopX = IMGW - 30;
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
                           break;
                       }
                   }

                   if(Temp.m_i16y)
                   {
                       KSlope_Left = (float)(Temp.m_i16x - fat_ipts1[t_i16LoopY - 5][0])/(float)(Temp.m_i16y-fat_ipts1[t_i16LoopY - 5][1]);

                       if(KSlope_Left > 0)
                       {
                           KSlope_Left = (float)(Temp.m_i16x - m_stLPnt[58].m_i16x)/(float)(Temp.m_i16y-58);
                       }

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
                       _SpecialHandle_FixedDeviation = 1;
                       if(_StopAngle < 0) _StopAngle = -1.0 * _StopAngle;
                       _SpecialHandle_FixedError = _StopAngle;
                   }
                }
                else
                {
                    _SpecialHandle_FixedDeviation = 1;
                    if(_StopAngle < 0) _StopAngle = -1.0 * _StopAngle;
                    _SpecialHandle_FixedError = _StopAngle;
                }
            }
            else
            {
                 t_i16LoopX = 30;
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
                  far_ipts0_num = 80;
                  findline_lefthand_adaptive(&img_raw, Threshold, Temp.m_i16x, Temp.m_i16y, far_ipts0, &far_ipts0_num);
                  //找到拐点
                  t_i16LoopY = 0;
                  Temp.m_i16x = far_ipts0[far_ipts1_num - 1][0];
                  Temp.m_i16y = far_ipts0[far_ipts1_num - 1][1];
                  while((++t_i16LoopY) < far_ipts0_num)
                  {
                      if(
 //                                   fat_ipts1[t_i16LoopY - 1][0] >  fat_ipts1[t_i16LoopY][0] &&
                              far_ipts0[t_i16LoopY - 1][1] >  far_ipts0[t_i16LoopY][1] &&
 //                                   fat_ipts1[t_i16LoopY + 1][0] <  fat_ipts1[t_i16LoopY][0] &&
                              far_ipts0[t_i16LoopY + 1][1] <  far_ipts0[t_i16LoopY][1] )
                      {
                          Temp.m_i16x = far_ipts0[t_i16LoopY][0];
                          Temp.m_i16y = far_ipts0[t_i16LoopY][1];
                          break;
                      }
                  }

                  if(Temp.m_i16y)
                  {
                       KSlope_Right = (float)(Temp.m_i16x - far_ipts0[t_i16LoopY - 5][0])/(float)(Temp.m_i16y-far_ipts0[t_i16LoopY - 5][1]);

                       if(KSlope_Right < 0)
                       {
                           KSlope_Right = (float)(Temp.m_i16x - m_stRPnt[58].m_i16x)/(float)(Temp.m_i16y-58);
                       }

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

                          if(m_stRPnt[Count_I1].m_i16x > IMGW - 2)
                              m_stRPnt[Count_I1].m_i16x = IMGW - 2;
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
                        _SpecialHandle_FixedDeviation = 1;
                        if(_StopAngle > 0) _StopAngle = -1.0 * _StopAngle;
                        _SpecialHandle_FixedError = _StopAngle;
                    }
              }
              else
              {
                    _SpecialHandle_FixedDeviation = 1;
                    if(_StopAngle > 0) _StopAngle = -1.0 * _StopAngle;
                    _SpecialHandle_FixedError = _StopAngle;
              }
            }
            break;
    }
}



//发车处理


Uint32 _WaitStop_Record;
float _SetupAngle;
void StatusWaitStop_TaskFunction(void)
{

    switch(EventStatusTask[_Event_WaitStop]._EventCurrentStatus)
    {
        case _NormalStatus_Standby:
            _StopAngle = _SetupAngle;
                       //保持打角
           if(_SetupAngle < 0)
           {
               _CrossWalkDirectionFlag = _CrossWalk_Left;
           }
           else
           {
               _CrossWalkDirectionFlag = _CrossWalk_Right;
           }

            if(_Runing_Distance > 3000)
            {
                _StatusMonitor._fpEventControl = _Event_NormalStatus;
            }

            break;
        case _Task_WaitStop_Ready:
            if(
                    m_u16LineBAr[30] > 10 && m_u16LineBAr[30] < 35 &&
                    m_u16LineBAr[IMGW - 30] > 10 && m_u16LineBAr[IMGW - 30] < 35
              )
            {
                EventStatusTask[_Event_WaitStop]._EventCurrentStatus = _Task_WaitStop_Start;
            }
            break;
        case  _Task_WaitStop_Start:
            _StopAngle = _SetupAngle;
            //保持打角
            if(_SetupAngle < 0)
            {
                _CrossWalkDirectionFlag = _CrossWalk_Left;

                if(ipts1_num > 40 && ReftLoopYangle > 0.25)
                {
                    EventStatusTask[_Event_WaitStop]._EventCurrentStatus = _Task_WaitStop_Stop;
                }
                else if(m_i16AbsoluteOutWidth == 0 && m_i16WOutWidthROWNum == 0)
                {
                    EventStatusTask[_Event_WaitStop]._EventCurrentStatus = _Task_WaitStop_Stop;
                }
            }
            else
            {
                _CrossWalkDirectionFlag = _CrossWalk_Right;

                if(ipts0_num > 40 && LeftLoopYangle > 0.25)
                {
                    EventStatusTask[_Event_WaitStop]._EventCurrentStatus = _Task_WaitStop_Stop;
                }
                else if(m_i16AbsoluteOutWidth == 0 && m_i16WOutWidthROWNum == 0)
                {
                    EventStatusTask[_Event_WaitStop]._EventCurrentStatus = _Task_WaitStop_Stop;
                }

            }

            if(_Runing_Distance - _WaitStop_Record > 4000 && m_i16WOutWidthROWNum == 0)
            {
                EventStatusTask[_Event_WaitStop]._EventCurrentStatus = _Task_WaitStop_Stop;
            }

            break;
        case _Task_WaitStop_Stop:
            _SpecialHandle_FixedDeviation = 0;
            VehicleInformation.SpeedSpecialPolicy = -1;
            _StatusMonitor._fpEventControl = _Event_NormalStatus;
            break;
        case _Task_WaitStop_Sucessful:

            break;
    }
}


void RepairWaitStop_TaskFunction(void)
{
    float Temp_KSlope = 0;
    Int16 t_i16LoopY = 0;

    uint8_t (*InImg)[IMGW];
    InImg = mt9v03x_output_image;

    switch(EventStatusTask[_Event_WaitStop]._EventCurrentStatus)
    {
        case _NormalStatus_Standby:
            if(_CrossWalkDirectionFlag == _CrossWalk_Right)
            {
                for(t_i16LoopY = 0; t_i16LoopY < IMGH; t_i16LoopY++)
                {
                   m_stRPnt[t_i16LoopY].m_i16x =  m_stLPnt[t_i16LoopY].m_i16x + TrackWide[t_i16LoopY];
                   m_stRPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                }
            }

            break;
        case _Task_WaitStop_Ready:

            _SpecialHandle_FixedDeviation = 1;
            _SpecialHandle_FixedError = 0;
            break;
        case _Task_WaitStop_Start:
            if(_CrossWalkDirectionFlag == _CrossWalk_Left)
            {
                ST_2D_INT_POINT_INFO Temp;
                Temp.m_i16y = 0;


                m_stLOptimalPoint.m_i16x = 20;//LIMIT(m_stLOptimalPoint.m_i16x, 5, 30);
                Temp.m_i16x = m_stLOptimalPoint.m_i16x;
                t_i16LoopY = IMGH - 1;

                while(--(t_i16LoopY) > 3)
                {
                    if(
                            InImg[t_i16LoopY - 1][m_stLOptimalPoint.m_i16x] == B_BLACK &&
                            InImg[t_i16LoopY    ][m_stLOptimalPoint.m_i16x] == B_BLACK &&
                            InImg[t_i16LoopY + 1][m_stLOptimalPoint.m_i16x] == B_WHITE
                      )
                    {
                        Temp.m_i16y = t_i16LoopY;
                        break;
                    }
                }


                if(Temp.m_i16y)
                {
                    _SpecialHandle_FixedDeviation = 0;

                    /*左边和右下做斜*/

                    /*左边线处理 全部清空*/
                    t_i16LoopY = IMGH;
                    while((--t_i16LoopY) > 0)
                    {
                        m_stLPnt[t_i16LoopY].m_i16y = 0;
                    }

                    /*右边线 补线*/
                    m_stRPnt[Temp.m_i16y] = Temp;
                    KSlope_Right = (float)(Temp.m_i16x - (IMGW - 40))/(float)(Temp.m_i16y-58);

                    Temp_KSlope = Temp.m_i16x;
                    t_i16LoopY = Temp.m_i16y;

                    while((++t_i16LoopY) < IMGH)
                    {
                        Temp_KSlope += KSlope_Right;
                        m_stRPnt[t_i16LoopY].m_i16x = (int)Temp_KSlope;
                        m_stRPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                        m_i8RMonotonicity[t_i16LoopY] = 0;
                    }

                    Temp_KSlope = Temp.m_i16x;
                    t_i16LoopY = Temp.m_i16y;

                    while((--t_i16LoopY) > -1)
                    {
                        Temp_KSlope -= KSlope_Right;
                        m_stRPnt[t_i16LoopY].m_i16x = (int)Temp_KSlope;
                        m_stRPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                        m_i8RMonotonicity[t_i16LoopY] = 0;
                    }
                }
                else
                {
                    _SpecialHandle_FixedDeviation = 1;
                    _SpecialHandle_FixedError = _SetupAngle;
                }

            }
            else
            {
                ST_2D_INT_POINT_INFO Temp;
                Temp.m_i16y = 0;

                m_stROptimalPoint.m_i16x = IMGW - 20;//LIMIT(m_stROptimalPoint.m_i16x, IMGW - 5, IMGW - 30);
                Temp.m_i16x = m_stROptimalPoint.m_i16x;

                t_i16LoopY = IMGH - 1;

                while(--(t_i16LoopY) > 3)
                {
                    if(
                            InImg[t_i16LoopY - 1][m_stROptimalPoint.m_i16x] == B_BLACK &&
                            InImg[t_i16LoopY    ][m_stROptimalPoint.m_i16x] == B_BLACK &&
                            InImg[t_i16LoopY + 1][m_stROptimalPoint.m_i16x] == B_WHITE
                      )
                    {
                        Temp.m_i16y = t_i16LoopY;
                        break;
                    }
                }

                if(Temp.m_i16y)
                {
                   _SpecialHandle_FixedDeviation = 0;

                   /*左边和右下做斜*/

                   /*左边线处理 全部清空*/
                   t_i16LoopY = IMGH;
                   while((--t_i16LoopY) > 0)
                   {
                       m_stRPnt[t_i16LoopY].m_i16y = 0;
                   }

                   /*右边线 补线*/
                   m_stLPnt[Temp.m_i16y] = Temp;
                   KSlope_Left = (float)(Temp.m_i16x - 40)/(float)(Temp.m_i16y-58);

                   Temp_KSlope = Temp.m_i16x;
                   t_i16LoopY = Temp.m_i16y;

                   while((++t_i16LoopY) < IMGH)
                   {
                       Temp_KSlope += KSlope_Left;
                       m_stLPnt[t_i16LoopY].m_i16x = (int)Temp_KSlope;
                       m_stLPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                       m_i8LMonotonicity[t_i16LoopY] = 0;
                   }

                   Temp_KSlope = Temp.m_i16x;
                   t_i16LoopY = Temp.m_i16y;

                   while((--t_i16LoopY) > -1)
                   {
                       Temp_KSlope -= KSlope_Right;
                       m_stLPnt[t_i16LoopY].m_i16x = (int)Temp_KSlope;
                       m_stLPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                       m_i8LMonotonicity[t_i16LoopY] = 0;
                   }
                }
                else
                {
                   _SpecialHandle_FixedDeviation = 1;
                   _SpecialHandle_FixedError = _SetupAngle;
                }
            }
            break;
    }
}

