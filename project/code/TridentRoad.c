



#include "TridentRoad.h"

#define         MAXDISTANCE             32000

//三叉路元素识别
Int16 Triden_LoopX;
Int16 Triden_LoopXStart;
Int16 Triden_LoopXEnd;

ST_2D_INT_POINT_INFO MSPOT;

float KSlope_LTop;
float KSlope_LBot;
float KSlope_RTop;
float KSlope_RBot;

Int32 BlackPointArea;

//开始三岔的方向
Uint8 _TridentDirectionFlag = _TridentRoad_Left;
Uint8 _TridentDirectionStatus = _Trident_Left;

//三岔退出条件
Uint32 _Trident_Record;
//Uint32 _Trident_Distance;


//积分距离
float _Trident_Setup;

/****************************************************************************************************/

void StatusTridentRoad_TaskFunction(void)
{

    TridentMiddle_TaskFunction();

    switch(EventStatusTask[_Event_TridentRoad]._EventCurrentStatus)
     {
         case  _Task_TridentRoad_Chick:
             if(     m_i16LBotcornerFindFlag != 1         &&
                     m_i16RBotcornerFindFlag != 1         &&
                     m_i16WOutWidthROWNum   > 20
             )
             {
                 BEEP_ON;
                 if(_TridentDirectionFlag == _TridentRoad_Right)
                 {
                     _TridentDirectionStatus = _Trident_Right;
                 }
                 else
                 {
                     _TridentDirectionStatus = _Trident_Left;
                 }
                 _Trident_Record = _Runing_Distance;
                 EventStatusTask[_Event_TridentRoad]._EventCurrentStatus =  _Task_TridentRoad_Stare;
             }
             break;

         case _Task_TridentRoad_Stare:


             if(
                     m_i16AbsoluteOutWidth < 5        &&
                     m_i16OutWidthROWNum   < 40        &&
                     m_i16WOutWidthROWNum  < 10        &&
                     Triden_LoopX         < 0         &&
                     ABS(_Runing_Distance - _Trident_Record) > 2200
               )
             {
                 BEEP_OFF;
                 _Trident_Record = _Runing_Distance;
                 EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_Runing;
//                 m_stOptimalPoint.m_i16x = IMGW / 2;
             }
             break;
         case _Task_TridentRoad_LEntry:
//             if(
//                     (   m_i16WOutWidthROWNum > 15 &&  m_i16OutWidthROWNum > 30 && m_i16AbsoluteOutWidth > 5) ||
//                     ABS( _Trident_Distance - _Trident_Record) > MAXDISTANCE
//               )
//               {
//                     if(ABS(_Trident_Distance - _Trident_Record) > 4000)
//                     {
//                         BEEP_ON;
//                         _Trident_Record = _Trident_Distance;
////                         _TridentDirectionStatus = _Trident_Left;
//                         EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_LExit;
//                     }
//               }
//             else
             if(
                        (m_i16OutWidthROWNum > 30        &&
                         LeftLoopYangle <= 0    &&
                         ReftLoopYangle <= 0    )   ||
                         ABS( _Runing_Distance - _Trident_Record) > MAXDISTANCE
                    )
             {

                 if(ABS(_Runing_Distance - _Trident_Record) > 4000)
                 {
                     BEEP_ON;
                     _Trident_Record = _Runing_Distance;
//                     _TridentDirectionStatus = _Trident_Left;
                     EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_LExit;
                 }

             }
             else if(       (Ypt0_found == true && Ypt1_found == true)     ||
                            ABS( _Runing_Distance - _Trident_Record) > MAXDISTANCE
                      )
                      {
                            if(ABS(_Runing_Distance - _Trident_Record) > 4000)
                            {
                                BEEP_ON;
                                _Trident_Record = _Runing_Distance;
                                EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_LExit;
                            }
                      }
             else if(
                         m_i16WOutWidthROWNum > 20        &&
                         m_i16AbsoluteOutWidth> 20         &&
                        (m_i16LBotcornerFindFlag == 1     ||
                         m_i16RBotcornerFindFlag == 1     )
                     )
             {

                     BEEP_ON;
                     _Trident_Record = _Runing_Distance;
                     _TridentDirectionStatus = _Trident_Left;
                     EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_LExit;

             }
             break;

         case _Task_TridentRoad_REntry:
//             if(
//                   (   m_i16WOutWidthROWNum > 20 &&  m_i16OutWidthROWNum > 40 &&  Triden_LoopX > 0 && m_i16AbsoluteOutWidth > 10)  ||
//                    ABS( _Trident_Distance - _Trident_Record) > MAXDISTANCE
//               )
//               {
//                     if(ABS(_Trident_Distance - _Trident_Record) > 2000)
//                     {
//                         BEEP_ON;
//                         _Trident_Record = _Trident_Distance;
////                         _TridentDirectionStatus = _Trident_Right;
//                         EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_RExit;
//                     }
//               }
//             else
             if(
                        (m_i16OutWidthROWNum > 30        &&
                         LeftLoopYangle <= 0    &&
                         ReftLoopYangle <= 0    )   ||
                         ABS( _Runing_Distance - _Trident_Record) > MAXDISTANCE
                    )
             {
                 if(ABS(_Runing_Distance - _Trident_Record) > 2000)
                 {
                     BEEP_ON;
                     _Trident_Record = _Runing_Distance;
                     EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_RExit;
                 }
             }
             else if(
                         (Ypt0_found == true && Ypt1_found == true )     ||
                          ABS( _Runing_Distance - _Trident_Record) > MAXDISTANCE
                    )
             {
                 if(ABS(_Runing_Distance - _Trident_Record) > 2000)
                 {
                     BEEP_ON;
                     _Trident_Record = _Runing_Distance;
                     EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_RExit;
                 }
             }
             else if(
                               m_i16WOutWidthROWNum > 20        &&
                               m_i16AbsoluteOutWidth> 10         &&
                              (m_i16LBotcornerFindFlag == 1     ||
                               m_i16RBotcornerFindFlag == 1     )
                           )
                   {

                             BEEP_ON;
                             _Trident_Record = _Runing_Distance;
                             EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_RExit;

                   }
             break;

         case _Task_TridentRoad_LExit:
             if(
                  (
                     AllAreaWhitePoint  > 1000       &&
                     m_i16WOutWidthROWNum <  8       &&
                     m_i16AbsoluteOutWidth < 5       &&
                     Triden_LoopX         < 0    )   ||
                     ABS( _Runing_Distance - _Trident_Record) > 10000)
             {
                 if(ABS( _Runing_Distance - _Trident_Record) > 4000)
                 {
                     BEEP_OFF;
                     _Trident_Record = 0;
//                     _Element_Primelock = 3;
                     _TridentDirectionFlag = _TridentRoad_Right;
//                     _TridentDirectionStatus = _Trident_Close;
//                     m_stOptimalPoint.m_i16x = IMGW / 2;
                     _StatusMonitor._fpEventControl = _Event_NormalStatus;
                     EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _NormalStatus_Standby;
                 }
             }
             else if(
                     ipts0_num > 40 && ipts1_num > 40 && ABS( _Runing_Distance - _Trident_Record) > 4000 && Triden_LoopX < 0
                     )
             {
                     BEEP_OFF;
                     _Trident_Record = 0;
//                     _Element_Primelock = 3;
                     _TridentDirectionFlag = _TridentRoad_Right;
//                     _TridentDirectionStatus = _Trident_Close;
//                     m_stOptimalPoint.m_i16x = IMGW / 2;
                     _StatusMonitor._fpEventControl = _Event_NormalStatus;
                     EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _NormalStatus_Standby;
             }
             break;

         case _Task_TridentRoad_RExit:

                 if(
                       (
                         AllAreaWhitePoint  > 1000       &&
                         m_i16WOutWidthROWNum  < 8        &&
                         m_i16AbsoluteOutWidth < 5        &&
                         Triden_LoopX          < 0    )    ||
                         ABS( _Runing_Distance - _Trident_Record) > 10000)
                 {
                     if(ABS( _Runing_Distance - _Trident_Record) > 4000)
                     {
                         BEEP_OFF;
                         _Trident_Record = 0;
//                         _Element_Primelock = 2;
    //                     _TridentDirectionStatus = _Trident_Close;
                         _TridentDirectionFlag = _TridentRoad_Left;
    //                     m_stOptimalPoint.m_i16x = IMGW / 2;
                         _StatusMonitor._fpEventControl = _Event_NormalStatus;
                         EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _NormalStatus_Standby;

                     }
                 }
                 else if(ipts0_num > 40 && ipts1_num > 40 && ABS( _Runing_Distance - _Trident_Record) > 4000 && Triden_LoopX < 0)
                 {

                         BEEP_OFF;
                         _Trident_Record = 0;
//                         _Element_Primelock = 2;
    //                     _TridentDirectionStatus = _Trident_Close;
                         _TridentDirectionFlag = _TridentRoad_Left;
    //                     m_stOptimalPoint.m_i16x = IMGW / 2;
                         _StatusMonitor._fpEventControl = _Event_NormalStatus;
                         EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _NormalStatus_Standby;
                 }

             break;
         case _Task_TridentRoad_Runing:

             if(ABS( _Runing_Distance - _Trident_Record) > 2000)
             {
                     if(_TridentDirectionFlag == _TridentRoad_Right)
                     {

                         EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_REntry;
                     }
                     else
                     {

                         EventStatusTask[_Event_TridentRoad]._EventCurrentStatus = _Task_TridentRoad_LEntry;
                     }
                     BEEP_OFF;
//                     _TridentDirectionStatus = _Trident_Close;
                     _Trident_Record = _Runing_Distance;
             }
             break;
     }
}

#if 0
void RepairTrident_TaskFunction(void)
{

    Int16 t_i16LoopY;                                       /*用来循环行*/
    Int16 t_i16LoopX;                                       /*用来循环列*/

    float Temp_KSlope = 0;
    Int16 Count_I1 = 0;
    Int16 SeekTrack_TargerFlag = 0;

    Uint8 (*InImg)[IMGW];
    InImg = mt9v03x_output_image;

    switch(EventStatusTask[_Event_TridentRoad]._EventCurrentStatus)
    {
        case  _Task_TridentRoad_Chick:
        case  _Task_TridentRoad_Stare:
        case  _Task_TridentRoad_LExit:
        case  _Task_TridentRoad_RExit:

            if(MSPOT.m_i16y)
            {
                if(_TridentDirectionFlag == _TridentRoad_Right)
                {
                    //向底部补线
                    if(m_i16LBotcornerFindFlag == 2)
                    {
                        if(m_stRPnt[57].m_i16y)
                        {
                            LBOT.m_i16x = m_stRPnt[57].m_i16x - TrackWide[59];
                            LBOT.m_i16y = 57;
                        }
                        else
                        {
                            LBOT.m_i16x = 20;
                            LBOT.m_i16y = 57;
                        }
                    }

                    Temp_KSlope = MSPOT.m_i16x;
                    KSlope_Left  = (float)(MSPOT.m_i16x - LBOT.m_i16x)/(float)(MSPOT.m_i16y - LBOT.m_i16y);
                    for(Count_I1 = MSPOT.m_i16y; Count_I1 < LBOT.m_i16y; Count_I1++)
                    {
                        Temp_KSlope += KSlope_Left;
                        m_stLPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                        m_stLPnt[Count_I1].m_i16y = Count_I1;
                        m_i8LMonotonicity[Count_I1] = 0;
                        if(m_stLPnt[Count_I1].m_i16x <  2)
                        {
                            m_stLPnt[Count_I1].m_i16x = 2;
                        }
                    }
                    t_i16LoopX = MSPOT.m_i16x - 1;
                    t_i16LoopY = MSPOT.m_i16y + 1;

                    while((--t_i16LoopY) > 1)
                    {
                          SeekTrack_TargerFlag = 0;
                          while((++t_i16LoopX) < IMGW - 1)
                          {
                                if(InImg[t_i16LoopY][t_i16LoopX]     == B_BLACK   &&
                                   InImg[t_i16LoopY][t_i16LoopX - 1] == B_BLACK   &&
                                   InImg[t_i16LoopY][t_i16LoopX + 1] == B_WHITE    )
                                {
                                    m_stLPnt[t_i16LoopY].m_i16x = t_i16LoopX;
                                    m_stLPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                                    m_i8LMonotonicity[t_i16LoopY] = 0;
                                    SeekTrack_TargerFlag = 1;
                                    break;
                                }
                                else
                                {
                                    m_stLPnt[t_i16LoopY].m_i16x = IMGW - 2;
                                    m_stLPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                                    m_i8LMonotonicity[t_i16LoopY] = 0;
                                }
                          }
                            //以下算法加快点的寻找
                          if(SeekTrack_TargerFlag == 1)
                          {
                              t_i16LoopX = m_stLPnt[t_i16LoopY].m_i16x - 3;
                          }
                          else
                          {
                              t_i16LoopX = MSPOT.m_i16x - 1;
                          }

                          if(m_i8RMonotonicity[t_i16LoopY] != 0 || m_stLPnt[t_i16LoopY].m_i16x > m_stRPnt[t_i16LoopY].m_i16x)
                          {
                              m_stRPnt[t_i16LoopY].m_i16x = IMGW - 2;
                              m_stRPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                              m_i8RMonotonicity[t_i16LoopY] = 0;
                          }
                     }
                }
                else
                {
                    //向底部补线
                    if(m_i16RBotcornerFindFlag == 2)
                    {
                        if(m_stLPnt[57].m_i16y)
                        {
                            RBOT.m_i16x = m_stLPnt[57].m_i16x + TrackWide[59];
                            RBOT.m_i16y = 57;
                        }
                        else
                        {
                            RBOT.m_i16x = IMGW - 20;
                            RBOT.m_i16y = 57;
                        }
                    }

                    Temp_KSlope = MSPOT.m_i16x;
                    KSlope_Right  = (float)(MSPOT.m_i16x - RBOT.m_i16x)/(float)(MSPOT.m_i16y - RBOT.m_i16y);
                    for(Count_I1 = MSPOT.m_i16y; Count_I1 < RBOT.m_i16y; Count_I1++)
                    {
                        Temp_KSlope += KSlope_Right;
                        m_stRPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                        m_stRPnt[Count_I1].m_i16y = Count_I1;
                        m_i8RMonotonicity[Count_I1] = 0;
                        if(m_stRPnt[Count_I1].m_i16x > IMGW - 2)
                        {
                            m_stRPnt[Count_I1].m_i16x = IMGW - 2;
                        }
                    }

                    t_i16LoopX = MSPOT.m_i16x + 1;
                    t_i16LoopY = MSPOT.m_i16y + 1;

                    while((--t_i16LoopY) > 1)
                    {
                          SeekTrack_TargerFlag = 0;
                          while((--t_i16LoopX) > 1)
                          {
                                if(InImg[t_i16LoopY][t_i16LoopX]     == B_BLACK   &&
                                InImg[t_i16LoopY][t_i16LoopX - 1] == B_WHITE   &&
                                InImg[t_i16LoopY][t_i16LoopX + 1] == B_BLACK    )
                                {
                                    m_stRPnt[t_i16LoopY].m_i16x = t_i16LoopX;
                                    m_stRPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                                    m_i8RMonotonicity[t_i16LoopY] = 0;
                                    SeekTrack_TargerFlag = 1;
                                    break;
                                }
                                else
                                {
                                    m_stRPnt[t_i16LoopY].m_i16x = 2;
                                    m_stRPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                                    m_i8RMonotonicity[t_i16LoopY] = 0;
                                }
                          }
                            //以下算法加快点的寻找
                          if(SeekTrack_TargerFlag == 1)
                          {
                              t_i16LoopX = m_stRPnt[t_i16LoopY].m_i16x + 3;
                          }
                          else
                          {
                              t_i16LoopX = MSPOT.m_i16x - 1;
                          }

                          if(m_i8LMonotonicity[t_i16LoopY] != 0 || m_stLPnt[t_i16LoopY].m_i16x > m_stRPnt[t_i16LoopY].m_i16x)
                          {
                              m_stLPnt[t_i16LoopY].m_i16x = 2;
                              m_stLPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                              m_i8LMonotonicity[t_i16LoopY] = 0;
                          }
                     }
                }
            }
            else
            {

                if(_TridentDirectionFlag == _TridentRoad_Right)
                {
                    if(ipts1_num)
                    {
                        t_i16LoopY = IMGH;
                        while((--t_i16LoopY) > -1)
                        {
                            m_stLPnt[t_i16LoopY].m_i16x = m_stRPnt[t_i16LoopY].m_i16x - TrackWide[t_i16LoopY] - 10;
                            m_stLPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                        }
                    }
                    else
                    {
                        //直接右上和左下
                        RTOP.m_i16x = IMGW - 2;
                        RTOP.m_i16y = 25;

                        LBOT.m_i16x = 2;
                        LBOT.m_i16y = 58;

                        Temp_KSlope = LBOT.m_i16x;
                        KSlope_Left  = (float)(RTOP.m_i16x - LBOT.m_i16x)/(float)(RTOP.m_i16y-LBOT.m_i16y);
                        for(Count_I1 = LBOT.m_i16y - 1; Count_I1 > RTOP.m_i16y; Count_I1--)
                        {
                            Temp_KSlope -= KSlope_Left;
                            m_stLPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                            m_stLPnt[Count_I1].m_i16y = Count_I1;

                            m_stRPnt[Count_I1].m_i16y = 0;
                        }

                    }
                }
                else
                {
                    if(ipts0_num)
                    {
                        t_i16LoopY = IMGH;
                        while((--t_i16LoopY) > -1)
                        {
                            m_stRPnt[t_i16LoopY].m_i16x = m_stLPnt[t_i16LoopY].m_i16x + TrackWide[t_i16LoopY] + 10;
                            m_stRPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                        }
                    }
                    else
                    {
                        //直接左上和右下

                        LTOP.m_i16x = 2;
                        LTOP.m_i16y = 25;

                        RBOT.m_i16x = IMGW - 2;
                        RBOT.m_i16y = 58;


                        Temp_KSlope = LBOT.m_i16x;
                        KSlope_Right  = (float)(LTOP.m_i16x - RBOT.m_i16x)/(float)(LTOP.m_i16y-RBOT.m_i16y);
                        for(Count_I1 = RBOT.m_i16y - 1; Count_I1 > LTOP.m_i16y; Count_I1--)
                        {
                            Temp_KSlope -= KSlope_Right;
                            m_stRPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                            m_stRPnt[Count_I1].m_i16y = Count_I1;

                            m_stLPnt[Count_I1].m_i16y = 0;
                        }
                    }
                }

            }
            break;
        default:
            break;

    }
}

#else
void RepairTrident_TaskFunction(void)
{

    Int16 t_i16LoopY;                                       /*用来循环行*/
    Int16 t_i16LoopX;                                       /*用来循环列*/
    Int16 t_i16CTemp;
    Int16 t_i16MFlag = 0;


    float Temp_KSlope = 0;
    Int16 Count_I1 = 0;
    Int16 SeekTrack_TargerFlag = 0;

    Uint8 (*InImg)[IMGW];
    InImg = mt9v03x_output_image;

    switch(EventStatusTask[_Event_TridentRoad]._EventCurrentStatus)
    {
        case  _Task_TridentRoad_Chick:
        case  _Task_TridentRoad_Stare:
        case  _Task_TridentRoad_LExit:
        case  _Task_TridentRoad_RExit:

            if(_TridentDirectionFlag == _TridentRoad_Right)
            {

                if(ipts1_num > 50)
                {
                    t_i16MFlag = 2;
                }
                else if(MSPOT.m_i16y && m_i16LBotcornerFindFlag == 1)
                {
                    t_i16MFlag = 1;
                }
                else if(!MSPOT.m_i16y && m_i16LBotcornerFindFlag == 1)
                {
                    SpecialTopCorner_TaskFunction();
                    if(ChamferCorner.m_i16y)
                    {
                        MSPOT = ChamferCorner;
                        t_i16MFlag = 1;
                    }
                    else if(ipts1_num > 40)
                    {
                        t_i16MFlag = 2;
                    }
                    else
                    {
                        RTOP.m_i16x = IMGW - 2;
                        RTOP.m_i16y = 10;

                        t_i16MFlag = 3;
                    }

                }

                else if(MSPOT.m_i16y && m_i16LBotcornerFindFlag != 1)
                {
                    if(ipts1_num > 40)
                    {

                        LBOT.m_i16x = 10;
                        LBOT.m_i16y = IMGH - 2;

                        t_i16CTemp = IMGH - 2;
                        while((--t_i16CTemp) > 50)
                        {
                            if(m_stRPnt[t_i16CTemp].m_i16y)
                                break;
                        }

                        LBOT.m_i16x = m_stRPnt[t_i16CTemp].m_i16x - 100;
                        LBOT.m_i16y = t_i16CTemp;
                    }
                    else
                    {
                        LBOT.m_i16x = 2;
                        LBOT.m_i16y = 58;
                    }

                    t_i16MFlag = 1;
                }
                else
                {
                    RTOP.m_i16x = IMGW - 2;
                    RTOP.m_i16y = 10;

                    LBOT.m_i16x = 2;
                    LBOT.m_i16y = 58;

                    t_i16MFlag = 3;
                }

                if(t_i16MFlag == 1)
                {
                    Temp_KSlope = MSPOT.m_i16x;
                    KSlope_Left  = (float)(MSPOT.m_i16x - LBOT.m_i16x)/(float)(MSPOT.m_i16y - LBOT.m_i16y);
                    for(Count_I1 = MSPOT.m_i16y; Count_I1 < LBOT.m_i16y; Count_I1++)
                    {
                        Temp_KSlope += KSlope_Left;
                        m_stLPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                        m_stLPnt[Count_I1].m_i16y = Count_I1;
                        m_i8LMonotonicity[Count_I1] = 0;
                        if(m_stLPnt[Count_I1].m_i16x <  2)
                        {
                            m_stLPnt[Count_I1].m_i16x = 2;
                        }
                    }

                    t_i16LoopX = MSPOT.m_i16x - 1;
                    t_i16LoopY = MSPOT.m_i16y + 1;

                    while((--t_i16LoopY) > 1)
                    {
                          SeekTrack_TargerFlag = 0;
                          while((++t_i16LoopX) < IMGW - 1)
                          {
                                if(InImg[t_i16LoopY][t_i16LoopX]     == B_BLACK   &&
                                   InImg[t_i16LoopY][t_i16LoopX - 1] == B_BLACK   &&
                                   InImg[t_i16LoopY][t_i16LoopX + 1] == B_WHITE    )
                                {
                                    m_stLPnt[t_i16LoopY].m_i16x = t_i16LoopX;
                                    m_stLPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                                    m_i8LMonotonicity[t_i16LoopY] = 0;
                                    SeekTrack_TargerFlag = 1;
                                    break;
                                }
                                else
                                {
                                    m_stLPnt[t_i16LoopY].m_i16x = IMGW - 2;
                                    m_stLPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                                    m_i8LMonotonicity[t_i16LoopY] = 0;
                                }
                          }
                            //以下算法加快点的寻找
                          if(SeekTrack_TargerFlag == 1)
                          {
                              t_i16LoopX = m_stLPnt[t_i16LoopY].m_i16x - 3;
                          }
                          else
                          {
                              t_i16LoopX = MSPOT.m_i16x - 1;
                          }

                          if(m_i8RMonotonicity[t_i16LoopY] != 0 || m_stLPnt[t_i16LoopY].m_i16x > m_stRPnt[t_i16LoopY].m_i16x)
                          {
                              m_stRPnt[t_i16LoopY].m_i16x = IMGW - 2;
                              m_stRPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                              m_i8RMonotonicity[t_i16LoopY] = 0;
                          }
                     }
                }
                else if(t_i16MFlag == 2)
                {
                    t_i16LoopY = IMGH - 1;
                    while((--t_i16LoopY) > -1)
                    {
                        m_stLPnt[t_i16LoopY].m_i16x = m_stRPnt[t_i16LoopY].m_i16x - TrackWide[t_i16LoopY] - 12;
                        m_stLPnt[t_i16LoopY].m_i16y = t_i16LoopY;

                        if(m_stLPnt[Count_I1].m_i16x < 2)
                            m_stLPnt[Count_I1].m_i16x = 2;
                    }
                }
                else if(t_i16MFlag == 3)
                {
                    Temp_KSlope = LBOT.m_i16x;
                    KSlope_Left  = (float)(RTOP.m_i16x - LBOT.m_i16x)/(float)(RTOP.m_i16y-LBOT.m_i16y);
                    for(Count_I1 = LBOT.m_i16y - 1; Count_I1 > RTOP.m_i16y; Count_I1--)
                    {
                        Temp_KSlope -= KSlope_Left;
                        m_stLPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                        m_stLPnt[Count_I1].m_i16y = Count_I1;

                        if(m_stLPnt[Count_I1].m_i16x < 2)
                            m_stLPnt[Count_I1].m_i16x = 2;
                        m_stRPnt[Count_I1].m_i16y = 0;
                    }
                }
            }
            else
            {

                if(ipts0_num > 50)
                {
                    t_i16MFlag = 2;
                }
                else if(MSPOT.m_i16y && m_i16RBotcornerFindFlag == 1)
                {
                    t_i16MFlag = 1;
                }
                else if(!MSPOT.m_i16y && m_i16RBotcornerFindFlag == 1)
                {
                    SpecialTopCorner_TaskFunction();
                    if(ChamferCorner.m_i16y)
                    {
                        MSPOT = ChamferCorner;
                        t_i16MFlag = 1;
                    }
                    else if(ipts1_num > 40)
                    {
                        t_i16MFlag = 2;
                    }
                    else
                    {
                        LTOP.m_i16x = 2;
                        LTOP.m_i16y = 10;
                        t_i16MFlag = 3;
                    }

                }


                else if(MSPOT.m_i16y && m_i16RBotcornerFindFlag != 1)
                {
                    if(ipts0_num > 40)
                    {
                        t_i16CTemp = IMGH - 2;
                        while((--t_i16CTemp) > 50)
                        {
                            if(m_stLPnt[t_i16CTemp].m_i16y)
                                break;
                        }
                        RBOT.m_i16x = m_stLPnt[t_i16CTemp].m_i16x + 100;
                        RBOT.m_i16y = t_i16CTemp;
                    }
                    else
                    {
                        RBOT.m_i16x = IMGW - 2;
                        RBOT.m_i16y = 58;
                    }

                    t_i16MFlag = 1;
                }
                else
                {
                    LTOP.m_i16x = 2;
                    LTOP.m_i16y = 10;

                    RBOT.m_i16x = IMGW - 2;
                    RBOT.m_i16y = 58;

                    t_i16MFlag = 3;
                }


                if(t_i16MFlag == 1)
                {
                    Temp_KSlope = MSPOT.m_i16x;
                    KSlope_Right  = (float)(MSPOT.m_i16x - RBOT.m_i16x)/(float)(MSPOT.m_i16y - RBOT.m_i16y);
                    for(Count_I1 = MSPOT.m_i16y; Count_I1 < RBOT.m_i16y; Count_I1++)
                    {
                        Temp_KSlope += KSlope_Right;
                        m_stRPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                        m_stRPnt[Count_I1].m_i16y = Count_I1;
                        m_i8RMonotonicity[Count_I1] = 0;
                        if(m_stRPnt[Count_I1].m_i16x > IMGW - 2)
                        {
                            m_stRPnt[Count_I1].m_i16x = IMGW - 2;
                        }
                    }

                    t_i16LoopX = MSPOT.m_i16x + 1;
                    t_i16LoopY = MSPOT.m_i16y + 1;

                    while((--t_i16LoopY) > 1)
                    {
                          SeekTrack_TargerFlag = 0;
                          while((--t_i16LoopX) > 1)
                          {
                                if(InImg[t_i16LoopY][t_i16LoopX]     == B_BLACK   &&
                                InImg[t_i16LoopY][t_i16LoopX - 1] == B_WHITE   &&
                                InImg[t_i16LoopY][t_i16LoopX + 1] == B_BLACK    )
                                {
                                    m_stRPnt[t_i16LoopY].m_i16x = t_i16LoopX;
                                    m_stRPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                                    m_i8RMonotonicity[t_i16LoopY] = 0;
                                    SeekTrack_TargerFlag = 1;
                                    break;
                                }
                                else
                                {
                                    m_stRPnt[t_i16LoopY].m_i16x = 2;
                                    m_stRPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                                    m_i8RMonotonicity[t_i16LoopY] = 0;
                                }
                          }
                            //以下算法加快点的寻找
                          if(SeekTrack_TargerFlag == 1)
                          {
                              t_i16LoopX = m_stRPnt[t_i16LoopY].m_i16x + 3;
                          }
                          else
                          {
                              t_i16LoopX = MSPOT.m_i16x - 1;
                          }

                          if(m_i8LMonotonicity[t_i16LoopY] != 0 || m_stLPnt[t_i16LoopY].m_i16x > m_stRPnt[t_i16LoopY].m_i16x)
                          {
                              m_stLPnt[t_i16LoopY].m_i16x = 2;
                              m_stLPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                              m_i8LMonotonicity[t_i16LoopY] = 0;
                          }
                     }
                }
                else if(t_i16MFlag == 2)
                {
                    t_i16LoopY = IMGH - 1;
                    while((--t_i16LoopY) > -1)
                    {
                        m_stRPnt[t_i16LoopY].m_i16x = m_stLPnt[t_i16LoopY].m_i16x + TrackWide[t_i16LoopY] + 12;
                        m_stRPnt[t_i16LoopY].m_i16y = t_i16LoopY;

                        if(m_stRPnt[t_i16LoopY].m_i16x > IMGW - 2)
                            m_stRPnt[t_i16LoopY].m_i16x = IMGW - 2;
                    }
                }
                else if(t_i16MFlag == 3)
                {
                    Temp_KSlope = RBOT.m_i16x;
                    KSlope_Right  = (float)(LTOP.m_i16x - RBOT.m_i16x)/(float)(LTOP.m_i16y-RBOT.m_i16y);
                    for(Count_I1 = RBOT.m_i16y; Count_I1 > LTOP.m_i16y; Count_I1--)
                    {
                      Temp_KSlope -= KSlope_Right;
                      m_stRPnt[Count_I1].m_i16x = (int)Temp_KSlope;
                      m_stRPnt[Count_I1].m_i16y = Count_I1;

                      if(m_stRPnt[Count_I1].m_i16x > IMGW - 2)
                          m_stRPnt[Count_I1].m_i16x = IMGW - 2;

                      m_stLPnt[Count_I1].m_i16y = 0;
                    }
                }


            }
            break;

    }
}


#endif



void TridentMiddle_TaskFunction(void)
{

    Int16 t_i16LoopY;                                       /*用来循环行*/
    Int16 t_i16LoopX;                                       /*用来循环列*/


    Int16 Temp_LoopX = 0;

    Int16 TridentMinPoint = IMGH - 1;

    Uint8 (*InImg)[IMGW];

    InImg = mt9v03x_output_image;

    TridentMinPoint = IMGH - 1;

    Triden_LoopXStart = 0;
    Triden_LoopXEnd = 0;


    MSPOT.m_i16x = 0;
    MSPOT.m_i16y = 0;

    /*从左往右寻找点 寻找底部白点数大于10的列进行判断*/

      if(LBOT.m_i16x < 20)  LBOT.m_i16x = 20;
      if(RBOT.m_i16x > IMGW - 21) RBOT.m_i16x = IMGW - 20;


      for(Temp_LoopX = LBOT.m_i16x + 1; Temp_LoopX < RBOT.m_i16x; Temp_LoopX++)
      {
          if(m_u16LineBAr[Temp_LoopX] <= TridentMinPoint && m_u16LineBAr[Temp_LoopX] > 6 && m_u16LineBAr[Temp_LoopX] < 58)
          {
              Triden_LoopXEnd = Temp_LoopX;
              TridentMinPoint = m_u16LineBAr[Temp_LoopX];

          }
      }

      if(TridentMinPoint < 58 && Triden_LoopXEnd < RBOT.m_i16x - 1)
      {
          Triden_LoopXStart = Triden_LoopXEnd;
          while((--Triden_LoopXStart) && Triden_LoopXStart > LBOT.m_i16x)
          {
              if(m_u16LineBAr[Triden_LoopXStart] > m_u16LineBAr[Triden_LoopXEnd])
              {
                  break;
              }
          }

          if(Triden_LoopXStart > LBOT.m_i16x + 1)
          {
              Triden_LoopXStart++;
              Triden_LoopX = (int)(Triden_LoopXStart + Triden_LoopXEnd) / 2;

              t_i16LoopX = Triden_LoopX;
              t_i16LoopY = 56 - m_u16LineBAr[Triden_LoopX];

              while((++t_i16LoopX) < RBOT.m_i16x + 10)
              {
                  if(InImg[t_i16LoopY][t_i16LoopX] == B_WHITE && InImg[t_i16LoopY][t_i16LoopX - 1] == B_WHITE)
                  {
                      KSlope_RTop = t_i16LoopX - Triden_LoopX;
                      break;
                  }
                  else
                  {
                      KSlope_RTop = 0;
                  }
              }

              t_i16LoopX = Triden_LoopX;

              while((--t_i16LoopX) > LBOT.m_i16x - 10)
              {
                  if(InImg[t_i16LoopY][t_i16LoopX] == B_WHITE && InImg[t_i16LoopY][t_i16LoopX + 1] == B_WHITE)
                  {
                      KSlope_LTop = Triden_LoopX - t_i16LoopX;
                      break;
                  }
                  else
                  {
                      KSlope_LTop = 0;
                  }
              }

              if(KSlope_RTop > 0 && KSlope_LTop > 0)
              {
                  MSPOT.m_i16x = Triden_LoopX;
                  MSPOT.m_i16y = 59 - m_u16LineBAr[Triden_LoopX];
              }
              else
              {
                  Triden_LoopX = -3;   /*没有满足该点*/
              }
          }
          else
          {
              Triden_LoopX = -2;   /*没有满足该点*/
          }
      }
      else
      {
          Triden_LoopX = -1;   /*没有满足该点*/
      }

}





