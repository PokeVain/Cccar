



#include "ElementProcess.h"



/*用来提取拐点*/
ST_2D_INT_POINT_INFO LTOP;
ST_2D_INT_POINT_INFO RTOP;
ST_2D_INT_POINT_INFO LBOT;
ST_2D_INT_POINT_INFO RBOT;


/*标志量*/
Int8 m_i16LTopcornerFindFlag;                                 /*左上角点找寻标志*/
Int8 m_i16LBotcornerFindFlag;                                 /*左下角点找寻标志*/
Int8 m_i16RTopcornerFindFlag;                                 /*右上角点找寻标志*/
Int8 m_i16RBotcornerFindFlag;                                 /*右下角点找寻标志*/

Int8 m_i16CorneerPointNumber;

/*赛道斜率*/
float KSlope_Left, KSlope_Right;
float KStemp_Left = -0.6f, KStemp_Right = 0.6f;

/*赛道半宽*/
Int16 m_i16OutWidthROWNum;                                  /*超宽函数*/
Int16 m_i16WOutWidthROWNum;                                 /*非常严重的超宽状态*/
Int16 m_i16AbsoluteOutWidth;

ST_2D_INT_POINT_INFO Temp_Point;

ST_2D_INT_POINT_INFO TopWhiteBorder[IMGW];
ST_2D_INT_POINT_INFO ChamferCorner;
ST_2D_INT_POINT_INFO EndTopofLine;

Int8  FrontCarStareDistance;
Int8  FrontCarExpect;

float _StopSpeed;



/**
 *      赛道半宽初始化
 **/

void ElementInital_UltraWideParam(void)
{

    //超宽设置
    m_i16OutWidthROWNum  = 0;
    m_i16WOutWidthROWNum = 0;

//    //三叉
//    _TridentDirectionStatus = _Trident_Close;
//
//    _Trident_Record = 0;
//    _Trident_Distance = 0;

//    //斑马线
//    _CrossWalk_Record = 0;
//
    //前瞻参数
    _Cammer_Prospect = 30;
//
//    //停止积分距离
//    Stop_Distance = 1500;
//
//    //发车距离
//    _Trident_Setup = 2000;
//
    //直道加速
//    SetTargerSpeed = 0;

    //最优列初始设置
    m_stOptimalPoint.m_i16x = IMGW / 2;

    //起步积分
    _WaitStop_Record = 0;

    //起步角度限制
    _SetupAngle = 100;

    //前瞻控制
    _Special_Prospect = 50;
    _Roundabout_Prospect = 42;
    _Trident_Prospect  = 36;
    _Cammer_Prospect   = 43;

//    _StopSpeed = 40;
    //启动速度控制
//    SetLMotorParam(0,260,1);
//    SetRMotorParam(0,260,1);
}


/**
 *  曲率解算
 **/




void ElementHandle_SegmentCurve(void)
{

}



float LBotTariangleCosCornor;
float RBotTariangleCosCornor;

void ElementHandle_JudgeTriangle(void)
{

//    ST_2D_INT_POINT_INFO UTemp, DTemp;
//
//    if(m_i16LBotcornerFindFlag == 1)
//    {
//        UTemp.m_i16x = ipts0[ipts0_num - 1][0];
//        UTemp.m_i16y = ipts0[ipts0_num - 1][1];
//
//        DTemp.m_i16x = ipts0[0][0];
//        DTemp.m_i16y = ipts0[0][1];
//
//        LBotTariangleCosCornor = IncludedAngleCOS(UTemp, LBOT, DTemp);
//    }
//    else
//    {
//        LBotTariangleCosCornor = -2.0f;
//    }
//
//
//    if(m_i16RBotcornerFindFlag == 1)
//    {
//        UTemp.m_i16x = ipts1[ipts1_num - 1][0];
//        UTemp.m_i16y = ipts1[ipts1_num - 1][1];
//
//        DTemp.m_i16x = ipts1[0][0];
//        DTemp.m_i16y = ipts1[0][1];
//
//        RBotTariangleCosCornor = IncludedAngleCOS(UTemp, RBOT, DTemp);
//    }
//    else
//    {
//        RBotTariangleCosCornor = -2.0f;
//    }

}

#if 1

void ElementHandle_SpecialPoint(void)
{
     Int16 t_i16LoopY=0;                                         /*用来循环行*/

     Int16 t_i16TempY = 0;

     Int16 t_i16Count = 0, t_i16LoseCount = 0;

     Int16 t_i16LostPoint = 0, t_i16TargetPoint = 0;             //测满足要求的点

     Int16 t_i16FlagStatus = 0, t_i16FinishStatus = 0;


     ST_2D_INT_POINT_INFO Temp;

     LBOT.m_i16x = 2;                                            //左下点
     LBOT.m_i16y = IMGH - 2;

     LTOP.m_i16x = 2;
     LTOP.m_i16y = 2;

     RBOT.m_i16x = IMGW - 3;
     RBOT.m_i16y = IMGH - 2;

     RTOP.m_i16x = IMGW - 3;
     RTOP.m_i16y = 2;


     m_i16LTopcornerFindFlag=-1;    //左上
     m_i16LBotcornerFindFlag=-1;    //左下
     m_i16RTopcornerFindFlag=-1;    //右上
     m_i16RBotcornerFindFlag=-1;    //右下
     m_i16CorneerPointNumber = 0;  //记录找到的点数


     /* -1 表示寻找错误 0表示找到线的单调性 1 表示找到拐点 2表示丢失拐点*/

     /*
      * 第一步寻找左下点 舍去第一排和最后一排
      */
     Temp.m_i16x = 2;
     Temp.m_i16y = IMGH - 2;

     t_i16LoopY = IMGH - 1;

     while(--t_i16LoopY > 0)
     {
         /*这里等于的作用是什么呢   当值相同时应该是最上面暂时忘记了*/
         //if(m_stLPnt[t_i16LoopY].m_i16x > Temp.m_i16x && m_i8LMonotonicity[t_i16LoopY] == 0)
         if(m_stLPnt[t_i16LoopY].m_i16x >= Temp.m_i16x && m_i8LMonotonicity[t_i16LoopY] == 0)
         {
             Temp = m_stLPnt[t_i16LoopY];
             t_i16Count = 0;
         }
         else
         {
             t_i16Count++;
             if(t_i16Count > 4)
                 break;
         }
     }

     if(t_i16LoopY == 0 && t_i16Count == 0)
     {
         // 第一种情况 一直扫描到最后一排 该直线为单调直线
         t_i16Count = 58;
         while(m_i8LMonotonicity[t_i16Count] != 0)
         {
             t_i16Count--;
         }
         LBOT = m_stLPnt[t_i16Count];
         m_i16LBotcornerFindFlag = 0;
     }
     else if(Temp.m_i16x < 5 || Temp.m_i16x > IMGW - 5)
     {
         //底部舍去角落的点
         LBOT.m_i16x = 2;
         LBOT.m_i16y = IMGH - 2;
         m_i16LBotcornerFindFlag = 2;
     }
     else
     {
         //找到下拐点
         /*往下满足单调性 往上是折点或者 未找到的点*/
         t_i16LoopY = Temp.m_i16y + 8;
         t_i16LoopY = t_i16LoopY > IMGH - 2 ? IMGH - 2 : t_i16LoopY;


         t_i16LostPoint = 0;
         t_i16TargetPoint = 0;
         //满足单调性
         while((--t_i16LoopY) > Temp.m_i16y)
         {
             if(
//                     m_stLPnt[t_i16LoopY].m_i16x - m_stLPnt[t_i16LoopY + 1].m_i16x < 3 &&
//                     m_stLPnt[t_i16LoopY].m_i16x - m_stLPnt[t_i16LoopY + 1].m_i16x > -1
                     m_i8LMonotonicity[t_i16LoopY] == 0
               )
             {
                 t_i16TargetPoint++;
             }
         }
         //往上不满足单调性
         t_i16LoopY = Temp.m_i16y;
         while((--t_i16LoopY) > Temp.m_i16y - 8 && t_i16LoopY > 0)
         {
             if(m_i8LMonotonicity[t_i16LoopY] != 0)
             {
                 t_i16LostPoint++;
             }
         }

         if(t_i16TargetPoint > 2 && t_i16LostPoint > 2)
         {
             LBOT = Temp;
             m_i16LBotcornerFindFlag = 1;
             m_i16CorneerPointNumber++;
         }
         else
         {
             LBOT.m_i16x = 2;
             LBOT.m_i16y = IMGH - 2;
             m_i16LBotcornerFindFlag = 2;        //丢掉拐点
         }
     }


     /*
      *  寻找左上角点
      */


    if(m_i16LBotcornerFindFlag == 0)
    {
        LTOP = m_stLPnt[1];
        m_i16LTopcornerFindFlag = 0;
    }
    else
    {
        Temp = LTOP;
        t_i16FlagStatus = 0;
        t_i16LoopY = LBOT.m_i16y;

        while((--t_i16LoopY) > 2 && m_stLPnt[t_i16LoopY].m_i16x > LBOT.m_i16x); //合并到一起 上 拐点比下拐点小

        if(t_i16LoopY > 2)
        {
            t_i16LoopY += 8;
            t_i16LoopY = t_i16LoopY > IMGH - 2 ? IMGH - 2 : t_i16LoopY;

            while((--t_i16LoopY) > 2)
            {
                if(
                    m_i8LMonotonicity[t_i16LoopY] == 0         &&
                    m_i8LMonotonicity[t_i16LoopY - 1] == 0     &&
                    m_i8LMonotonicity[t_i16LoopY - 2] == 0     )

                 {
                    if(
                            m_i8LMonotonicity[t_i16LoopY + 2] != 0     &&
                            m_i8LMonotonicity[t_i16LoopY + 1] != 0
                    )
                    {
                        if(m_i8LMonotonicity[t_i16LoopY + 1] == -1)
                        {
                            t_i16FlagStatus = 2;
                        }
                        else
                        {
                            t_i16FlagStatus = 1;
                        }
                        Temp = m_stLPnt[t_i16LoopY];
                        break;
                    }
                 }
             }

        //分情况讨论

            if(t_i16FlagStatus == 2)
            {

                LOOPLSpecial:
                  t_i16LostPoint = 1;
                  t_i16LoopY++;
                  while((--t_i16LoopY) > 2)   //舍去最上面10排 不需要补线
                  {
                      //是否需要这个条件
                      if (m_stLPnt[t_i16LoopY].m_i16x > LBOT.m_i16x && m_i8LMonotonicity[t_i16LoopY] == 0 && m_stLPnt[t_i16LoopY-1].m_i16x-m_stLPnt[t_i16LoopY].m_i16x>-1)
                      {
                          //从该点往后寻找 10个的坐标相差不大
                          t_i16TempY = t_i16LoopY;
                          t_i16LoseCount = 0;

                          while((--t_i16LoopY) > t_i16TempY - 5 && t_i16LoopY > 0)
                          {
                              if(m_stLPnt[t_i16LoopY].m_i16x - m_stLPnt[t_i16LoopY + 1].m_i16x > 3)
                              {
                                  t_i16LoseCount++;
                                  break;
                              }
                          }

                          if(t_i16LoseCount == 0)
                          {
                              t_i16LostPoint = 0;
                              m_i16CorneerPointNumber++;
                              m_i16LTopcornerFindFlag = 1;
                              LTOP = m_stLPnt[t_i16TempY];
                              break;
                          }

                      }
                  }

                  if(t_i16LostPoint == 1)
                  {
                      LTOP.m_i16x = 2;
                      LTOP.m_i16y = 2;
                      m_i16LTopcornerFindFlag = 2;    //丢掉白点
                  }
            }
            else if(t_i16FlagStatus == 1)
            {

                //往上需要满足单调性
                t_i16TargetPoint = 0;
                t_i16TempY = t_i16LoopY;

                while((--t_i16TempY) > t_i16LoopY  - 5 && t_i16TempY > 2)
                {
                    if(m_i8LMonotonicity[t_i16TempY] == 0)
                    {
                        t_i16TargetPoint++;
                    }
                }

                if(t_i16TargetPoint > 2 && ABS(m_stLPnt[t_i16TempY].m_i16x - m_stLPnt[t_i16TempY - 1].m_i16x) < 3)
                {
                    //往上满足单调性
                    t_i16TempY = t_i16LoopY;

                    while( ABS(m_stLPnt[t_i16LoopY].m_i16x - m_stLPnt[t_i16LoopY - 1].m_i16x) > 2 && t_i16LoopY > t_i16TempY - 8)
                    {
                        t_i16LoopY--;
                    }

                    t_i16TargetPoint = 0;
                    t_i16LostPoint = 0;
                    t_i16FinishStatus = 0;
                    t_i16TempY = t_i16LoopY;

                    while(++t_i16LoopY < t_i16TempY + 8 && t_i16LoopY < IMGH - 2)
                    {
                        if(m_stLPnt[t_i16TempY].m_i16x - m_stLPnt[t_i16LoopY].m_i16x > 4)
                        {
                            if(++t_i16LostPoint > 2)
                            {
                              t_i16FinishStatus = 1;
                              m_i16LTopcornerFindFlag = 1;
                              m_i16CorneerPointNumber++;
                              LTOP = m_stLPnt[t_i16TempY];
                              break;
                            }
                        }
                    }

                    if(t_i16FinishStatus == 0)
                    {
                        LTOP.m_i16x = 2;
                        LTOP.m_i16y = 2;
                        m_i16LTopcornerFindFlag = 2;    //丢掉白点
                    }
                }
                else
                {
                    goto LOOPLSpecial;  //继续寻找拐点
                }
            }
            else
            {
                LTOP.m_i16x = 2;
                LTOP.m_i16y = 2;
                m_i16LTopcornerFindFlag = 2;    //丢掉白点
            }
        }
        else
        {
            LTOP.m_i16x = 2;
            LTOP.m_i16y = 2;
            m_i16LTopcornerFindFlag = 2;    //丢掉白点
        }
    }

    /**************************************************************************/

    /*
     *  寻找右下角点
     */
    Temp.m_i16x = IMGW - 3;
    Temp.m_i16y = IMGH - 2;

    t_i16LoopY = IMGH - 1;

    t_i16Count = 0;

    /*大概检测拐点*/
    while(--t_i16LoopY > 0)
    {
        if(m_stRPnt[t_i16LoopY].m_i16x <= Temp.m_i16x && m_i8RMonotonicity[t_i16LoopY] == 0)
        {
            Temp = m_stRPnt[t_i16LoopY];
            t_i16Count = 0;
        }
        else
        {
            t_i16Count++;
            if(t_i16Count > 4)
                break;
        }
    }

    if(t_i16LoopY == 0 && t_i16Count == 0)
    {
        // 第一种情况 一直扫描到最后一排 该直线为单调直线
        t_i16Count = 58;
        while(m_i8RMonotonicity[t_i16Count] != 0)
        {
            t_i16Count--;
        }
        m_i16RBotcornerFindFlag = 0;
    }
    else if(Temp.m_i16x > IMGW - 5 || Temp.m_i16x < 5)
    {
        //此处要进行补线操作 有上角点
        RBOT.m_i16x = IMGW - 3;
        RBOT.m_i16y = IMGH - 2;
        m_i16RBotcornerFindFlag = 2;        //丢掉拐点

    }
    else
    {
        /*拐点以下满足单调性*/
        t_i16LoopY = Temp.m_i16y + 8;
        t_i16LoopY = t_i16LoopY > IMGH - 2 ? IMGH - 2 : t_i16LoopY;


        t_i16LostPoint = 0;
        t_i16TargetPoint = 0;
        //满足单调性
        while((--t_i16LoopY) > Temp.m_i16y)
        {
            if(
//                    m_stRPnt[t_i16LoopY + 1].m_i16x - m_stRPnt[t_i16LoopY].m_i16x < 3 &&
//                    m_stRPnt[t_i16LoopY + 1].m_i16x - m_stRPnt[t_i16LoopY].m_i16x > -1
                    m_i8RMonotonicity[t_i16LoopY] == 0
              )
            {
                t_i16TargetPoint++;
            }
        }
        //往上满足空白
        t_i16LoopY = Temp.m_i16y;

        while((--t_i16LoopY) > Temp.m_i16y - 8 && t_i16LoopY > 0)
        {
            if(m_i8RMonotonicity[t_i16LoopY] != 0)
            {
                t_i16LostPoint++;
            }
        }

        if(t_i16TargetPoint > 2 && t_i16LostPoint > 2)
        {
            RBOT = Temp;
            m_i16RBotcornerFindFlag = 1;
            m_i16CorneerPointNumber++;
        }
        else
        {
            //此处要进行补线操作 有上角点
            RBOT.m_i16x = IMGW - 3;
            RBOT.m_i16y = IMGH - 2;
            m_i16RBotcornerFindFlag = 2;        //丢掉拐点
        }
    }


    /*
     *  寻找右上角点
     */

    if(m_i16RBotcornerFindFlag == 0)
    {
        //满足单调性的情况
        RTOP = m_stRPnt[1];
        m_i16RTopcornerFindFlag = 0;
    }
    else
    {
        Temp = RTOP;
        t_i16FlagStatus = 0;
        t_i16LoopY = RBOT.m_i16y;


        while((--t_i16LoopY) > 2 && m_stRPnt[t_i16LoopY].m_i16x < RBOT.m_i16x);

        if(t_i16LoopY > 2)
        {

            t_i16LoopY += 10;
            t_i16LoopY = t_i16LoopY > IMGH - 2 ? IMGH - 2 : t_i16LoopY;

            while((--t_i16LoopY) > 2)
            {

                if(
                m_i8RMonotonicity[t_i16LoopY] == 0         &&
                m_i8RMonotonicity[t_i16LoopY - 1] == 0     &&
                m_i8RMonotonicity[t_i16LoopY - 2] == 0     )

                {
                    if(
                            m_i8RMonotonicity[t_i16LoopY + 2] != -1     &&
                            m_i8RMonotonicity[t_i16LoopY + 1] != -1     )
                    {

                        if(m_i8RMonotonicity[t_i16LoopY + 1] > 0)
                        {
                            t_i16FlagStatus = 1;
                        }
                        else
                        {
                            t_i16FlagStatus = 2;
                        }
                        Temp = m_stRPnt[t_i16LoopY];
                        break;

                    }
                }
            }

            if(t_i16FlagStatus == 2)
            {
                LOOPRSpecial:
                t_i16LostPoint = 1;
                t_i16LoopY++;
                while((--t_i16LoopY) > 2)   //舍去最上面的一排
                {
                  if (m_stRPnt[t_i16LoopY].m_i16x < RBOT.m_i16x &&  m_i8RMonotonicity[t_i16LoopY] == 0 && m_stRPnt[t_i16LoopY-1].m_i16x-m_stRPnt[t_i16LoopY].m_i16x<1)
                  {
                      //从该点往后寻找 10个的坐标相差不大
                      t_i16TempY = t_i16LoopY;
                      t_i16LoseCount = 0;

                      while((--t_i16LoopY) > t_i16TempY - 5 && t_i16LoopY > 0)
                      {
                          if(m_stRPnt[t_i16LoopY + 1].m_i16x - m_stRPnt[t_i16LoopY].m_i16x > 3)
                          {
                             t_i16LoseCount++;
                             break;
                          }
                      }
                      if(t_i16LoseCount == 0)
                      {
                          t_i16LostPoint = 0;
                          m_i16RTopcornerFindFlag = 1;
                          m_i16CorneerPointNumber++;
                          RTOP = m_stRPnt[t_i16TempY];
                          break;
                      }
                  }
                }

                if(t_i16LostPoint == 1)
                {
                    RTOP.m_i16x = IMGW - 3;
                    RTOP.m_i16y = 2;
                    m_i16RTopcornerFindFlag = 2;
                }

            }
            else if(t_i16FlagStatus == 1)
            {
                t_i16TargetPoint = 0;
                t_i16TempY = t_i16LoopY;

                while((--t_i16TempY) > t_i16LoopY  - 8 && t_i16TempY > 2)
                {
                    if(m_i8RMonotonicity[t_i16TempY] == 0)
                    {
                    t_i16TargetPoint++;
                    }
                }

                t_i16TempY = t_i16LoopY;

                while( ABS(m_stRPnt[t_i16LoopY].m_i16x - m_stRPnt[t_i16LoopY - 1].m_i16x) > 2 && t_i16LoopY > t_i16TempY - 8)
                {
                    t_i16LoopY--;
                }

                if(t_i16TargetPoint > 2 && ABS(m_stRPnt[t_i16TempY].m_i16x - m_stRPnt[t_i16TempY - 1].m_i16x) < 3)
                {
                    t_i16TargetPoint = 0;
                    t_i16LostPoint = 0;
                    t_i16FinishStatus = 0;

                    t_i16LoopY++;
                    t_i16TempY = t_i16LoopY;

                    while(++t_i16LoopY < t_i16TempY + 8)
                    {
                        if(ABS(m_stRPnt[t_i16LoopY].m_i16x - m_stRPnt[t_i16TempY].m_i16x) > 4)
                        {
                            if(++t_i16LostPoint > 1)
                            {
                            t_i16FinishStatus = 1;
                            m_i16RTopcornerFindFlag = 1;
                            m_i16CorneerPointNumber++;
                            RTOP = m_stRPnt[t_i16TempY];
                            break;
                            }
                        }
                    }

                    if(t_i16FinishStatus == 0)
                    {
                        RTOP.m_i16x = IMGW - 3;
                        RTOP.m_i16y = 2;
                        m_i16RTopcornerFindFlag = 2;
                    }
                }
                else
                {

                    goto LOOPRSpecial;
                }
            }
            else
            {
                RTOP.m_i16x = IMGW - 3;
                RTOP.m_i16y = 2;
                m_i16RTopcornerFindFlag = 2;
            }
        }
        else
        {
            RTOP.m_i16x = IMGW - 3;
            RTOP.m_i16y = 2;
            m_i16RTopcornerFindFlag = 2;
        }
    }
}

#else

void ElementHandle_SpecialPoint(void)
{
     Int16 t_i16LoopY=0;                                         /*用来循环行*/

     Int16 t_i16TempY = 0;

     Int16 t_i16Count = 0, t_i16LoseCount = 0;

     Int16 t_i16LostPoint = 0, t_i16TargetPoint = 0;             //测满足要求的点

     Int16 t_i16FlagStatus = 0, t_i16FinishStatus = 0;


     ST_2D_INT_POINT_INFO Temp;

     LBOT.m_i16x = 2;                                            //左下点
     LBOT.m_i16y = IMGH - 2;

     LTOP.m_i16x = 2;
     LTOP.m_i16y = 2;

     RBOT.m_i16x = IMGW - 3;
     RBOT.m_i16y = IMGH - 2;

     RTOP.m_i16x = IMGW - 3;
     RTOP.m_i16y = 2;


     m_i16LTopcornerFindFlag=-1;    //左上
     m_i16LBotcornerFindFlag=-1;    //左下
     m_i16RTopcornerFindFlag=-1;    //右上
     m_i16RBotcornerFindFlag=-1;    //右下
     m_i16CorneerPointNumber = 0;  //记录找到的点数


     /* -1 表示寻找错误 0表示找到线的单调性 1 表示找到拐点 2表示丢失拐点*/

     /*
      * 第一步寻找左下点 舍去第一排和最后一排
      */
     Temp.m_i16x = 2;
     Temp.m_i16y = IMGH - 2;

     t_i16LoopY = IMGH - 1;

     while(--t_i16LoopY > 0)
     {
         /*这里等于的作用是什么呢   当值相同时应该是最上面暂时忘记了*/
         if(m_stLPnt[t_i16LoopY].m_i16x > Temp.m_i16x && m_i8LMonotonicity[t_i16LoopY] == 0)
//         if(m_stLPnt[t_i16LoopY].m_i16x >= Temp.m_i16x && m_i8LMonotonicity[t_i16LoopY] == 0)
         {
             Temp = m_stLPnt[t_i16LoopY];
             t_i16Count = 0;
         }
         else
         {
             t_i16Count++;
             if(t_i16Count > 4)
                 break;
         }
     }

     if(t_i16LoopY == 0 && t_i16Count == 0)
     {
         // 第一种情况 一直扫描到最后一排 该直线为单调直线
         t_i16Count = 58;
         while(m_i8LMonotonicity[t_i16Count] != 0)
         {
             t_i16Count--;
         }
         LBOT = m_stLPnt[t_i16Count];
         m_i16LBotcornerFindFlag = 0;
     }
     else if(Temp.m_i16x < 5 || Temp.m_i16x > IMGW - 5)
     {
         //底部舍去角落的点
         LBOT.m_i16x = 2;
         LBOT.m_i16y = IMGH - 2;
         m_i16LBotcornerFindFlag = 2;
     }
     else
     {
         //找到下拐点
         /*往下满足单调性 往上是折点或者 未找到的点*/
         t_i16LoopY = Temp.m_i16y + 8;
         t_i16LoopY = t_i16LoopY > IMGH - 2 ? IMGH - 2 : t_i16LoopY;


         t_i16LostPoint = 0;
         t_i16TargetPoint = 0;
         //满足单调性
         while((--t_i16LoopY) > Temp.m_i16y)
         {
             if(
//                     m_stLPnt[t_i16LoopY].m_i16x - m_stLPnt[t_i16LoopY + 1].m_i16x < 3 &&
//                     m_stLPnt[t_i16LoopY].m_i16x - m_stLPnt[t_i16LoopY + 1].m_i16x > -1
                     m_i8LMonotonicity[t_i16LoopY] == 0
               )
             {
                 t_i16TargetPoint++;
             }
         }
         //往上不满足单调性
         t_i16LoopY = Temp.m_i16y;
         while((--t_i16LoopY) > Temp.m_i16y - 8 && t_i16LoopY > 0)
         {
             if(m_i8LMonotonicity[t_i16LoopY] != 0)
             {
                 t_i16LostPoint++;
             }
         }

         if(t_i16TargetPoint > 2 && t_i16LostPoint > 2)
         {
             LBOT = Temp;
             m_i16LBotcornerFindFlag = 1;
             m_i16CorneerPointNumber++;
         }
         else
         {
             LBOT.m_i16x = 2;
             LBOT.m_i16y = IMGH - 2;
             m_i16LBotcornerFindFlag = 2;        //丢掉拐点
         }
     }


     /*
      *  寻找左上角点
      */


    if(m_i16LBotcornerFindFlag == 0)
    {
        LTOP = m_stLPnt[1];
        m_i16LTopcornerFindFlag = 0;
    }
    else
    {
        Temp = LTOP;
        t_i16FlagStatus = 0;
        t_i16LoopY = LBOT.m_i16y;

        while((--t_i16LoopY) > 2 && m_stLPnt[t_i16LoopY].m_i16x > LBOT.m_i16x); //合并到一起 上 拐点比下拐点小

        if(t_i16LoopY > 2)
        {
            t_i16LoopY += 8;
            t_i16LoopY = t_i16LoopY > IMGH - 2 ? IMGH - 2 : t_i16LoopY;

            while((--t_i16LoopY) > 2)
            {
                if(
                    m_i8LMonotonicity[t_i16LoopY] == 0         &&
                    m_i8LMonotonicity[t_i16LoopY - 1] == 0     &&
                    m_i8LMonotonicity[t_i16LoopY - 2] == 0     )

                 {
                    if(
                            m_i8LMonotonicity[t_i16LoopY + 2] != 0     &&
                            m_i8LMonotonicity[t_i16LoopY + 1] != 0
                    )
                    {
                        if(m_i8LMonotonicity[t_i16LoopY + 1] == -1)
                        {
                            t_i16FlagStatus = 2;
                        }
                        else
                        {
                            t_i16FlagStatus = 1;
                        }
                        Temp = m_stLPnt[t_i16LoopY];
                        break;
                    }
                 }
             }

        //分情况讨论

            if(t_i16FlagStatus == 2)
            {

                LOOPLSpecial:
                  t_i16LostPoint = 1;
                  t_i16LoopY++;
                  while((--t_i16LoopY) > 2)   //舍去最上面10排 不需要补线
                  {
                      //是否需要这个条件
                      if (m_stLPnt[t_i16LoopY].m_i16x > LBOT.m_i16x && m_i8LMonotonicity[t_i16LoopY] == 0 && m_stLPnt[t_i16LoopY-1].m_i16x-m_stLPnt[t_i16LoopY].m_i16x>0)
                      {
                          //从该点往后寻找 10个的坐标相差不大
                          t_i16TempY = t_i16LoopY;
                          t_i16LoseCount = 0;

                          while((--t_i16LoopY) > t_i16TempY - 5 && t_i16LoopY > 0)
                          {
                              if(m_stLPnt[t_i16LoopY].m_i16x - m_stLPnt[t_i16LoopY + 1].m_i16x > 3)
                              {
                                  t_i16LoseCount++;
                                  break;
                              }
                          }

                          if(t_i16LoseCount == 0)
                          {
                              t_i16LostPoint = 0;
                              m_i16CorneerPointNumber++;
                              m_i16LTopcornerFindFlag = 1;
                              LTOP = m_stLPnt[t_i16TempY];
                              break;
                          }

                      }
                  }

                  if(t_i16LostPoint == 1)
                  {
                      LTOP.m_i16x = 2;
                      LTOP.m_i16y = 2;
                      m_i16LTopcornerFindFlag = 2;    //丢掉白点
                  }
            }
            else if(t_i16FlagStatus == 1)
            {

                //往上需要满足单调性
                t_i16TargetPoint = 0;
                t_i16TempY = t_i16LoopY;

                while((--t_i16TempY) > t_i16LoopY  - 5 && t_i16TempY > 2)
                {
                    if(m_i8LMonotonicity[t_i16TempY] == 0)
                    {
                        t_i16TargetPoint++;
                    }
                }

                if(t_i16TargetPoint > 2 && ABS(m_stLPnt[t_i16TempY].m_i16x - m_stLPnt[t_i16TempY - 1].m_i16x) < 3)
                {
                    //往上满足单调性
                    t_i16TempY = t_i16LoopY;

                    while( ABS(m_stLPnt[t_i16LoopY].m_i16x - m_stLPnt[t_i16LoopY - 1].m_i16x) > 2 && t_i16LoopY > t_i16TempY - 8)
                    {
                        t_i16LoopY--;
                    }

                    t_i16TargetPoint = 0;
                    t_i16LostPoint = 0;
                    t_i16FinishStatus = 0;
                    t_i16TempY = t_i16LoopY;

                    while(++t_i16LoopY < t_i16TempY + 8 && t_i16LoopY < IMGH - 2)
                    {
                        if(m_stLPnt[t_i16TempY].m_i16x - m_stLPnt[t_i16LoopY].m_i16x > 4)
                        {
                            if(++t_i16LostPoint > 2)
                            {
                              t_i16FinishStatus = 1;
                              m_i16LTopcornerFindFlag = 1;
                              m_i16CorneerPointNumber++;
                              LTOP = m_stLPnt[t_i16TempY];
                              break;
                            }
                        }
                    }

                    if(t_i16FinishStatus == 0)
                    {
                        LTOP.m_i16x = 2;
                        LTOP.m_i16y = 2;
                        m_i16LTopcornerFindFlag = 2;    //丢掉白点
                    }
                }
                else
                {
                    goto LOOPLSpecial;  //继续寻找拐点
                }
            }
            else
            {
                LTOP.m_i16x = 2;
                LTOP.m_i16y = 2;
                m_i16LTopcornerFindFlag = 2;    //丢掉白点
            }
        }
        else
        {
            LTOP.m_i16x = 2;
            LTOP.m_i16y = 2;
            m_i16LTopcornerFindFlag = 2;    //丢掉白点
        }
    }

    /**************************************************************************/

    /*
     *  寻找右下角点
     */
    Temp.m_i16x = IMGW - 3;
    Temp.m_i16y = IMGH - 2;

    t_i16LoopY = IMGH - 1;

    t_i16Count = 0;

    /*大概检测拐点*/
    while(--t_i16LoopY > 0)
    {
        if(m_stRPnt[t_i16LoopY].m_i16x < Temp.m_i16x && m_i8RMonotonicity[t_i16LoopY] == 0)
        {
            Temp = m_stRPnt[t_i16LoopY];
            t_i16Count = 0;
        }
        else
        {
            t_i16Count++;
            if(t_i16Count > 4)
                break;
        }
    }

    if(t_i16LoopY == 0 && t_i16Count == 0)
    {
        // 第一种情况 一直扫描到最后一排 该直线为单调直线
        t_i16Count = 58;
        while(m_i8RMonotonicity[t_i16Count] != 0)
        {
            t_i16Count--;
        }
        m_i16RBotcornerFindFlag = 0;
    }
    else if(Temp.m_i16x > IMGW - 5 || Temp.m_i16x < 5)
    {
        //此处要进行补线操作 有上角点
        RBOT.m_i16x = IMGW - 3;
        RBOT.m_i16y = IMGH - 2;
        m_i16RBotcornerFindFlag = 2;        //丢掉拐点

    }
    else
    {
        /*拐点以下满足单调性*/
        t_i16LoopY = Temp.m_i16y + 8;
        t_i16LoopY = t_i16LoopY > IMGH - 2 ? IMGH - 2 : t_i16LoopY;


        t_i16LostPoint = 0;
        t_i16TargetPoint = 0;
        //满足单调性
        while((--t_i16LoopY) > Temp.m_i16y)
        {
            if(
//                    m_stRPnt[t_i16LoopY + 1].m_i16x - m_stRPnt[t_i16LoopY].m_i16x < 3 &&
//                    m_stRPnt[t_i16LoopY + 1].m_i16x - m_stRPnt[t_i16LoopY].m_i16x > -1
                    m_i8RMonotonicity[t_i16LoopY] == 0
              )
            {
                t_i16TargetPoint++;
            }
        }
        //往上满足空白
        t_i16LoopY = Temp.m_i16y;

        while((--t_i16LoopY) > Temp.m_i16y - 8 && t_i16LoopY > 0)
        {
            if(m_i8RMonotonicity[t_i16LoopY] != 0)
            {
                t_i16LostPoint++;
            }
        }

        if(t_i16TargetPoint > 2 && t_i16LostPoint > 2)
        {
            RBOT = Temp;
            m_i16RBotcornerFindFlag = 1;
            m_i16CorneerPointNumber++;
        }
        else
        {
            //此处要进行补线操作 有上角点
            RBOT.m_i16x = IMGW - 3;
            RBOT.m_i16y = IMGH - 2;
            m_i16RBotcornerFindFlag = 2;        //丢掉拐点
        }
    }


    /*
     *  寻找右上角点
     */

    if(m_i16RBotcornerFindFlag == 0)
    {
        //满足单调性的情况
        RTOP = m_stRPnt[1];
        m_i16RTopcornerFindFlag = 0;
    }
    else
    {
        Temp = RTOP;
        t_i16FlagStatus = 0;
        t_i16LoopY = RBOT.m_i16y;


        while((--t_i16LoopY) > 2 && m_stRPnt[t_i16LoopY].m_i16x < RBOT.m_i16x);

        if(t_i16LoopY > 2)
        {

            t_i16LoopY += 10;
            t_i16LoopY = t_i16LoopY > IMGH - 2 ? IMGH - 2 : t_i16LoopY;

            while((--t_i16LoopY) > 2)
            {

                if(
                m_i8RMonotonicity[t_i16LoopY] == 0         &&
                m_i8RMonotonicity[t_i16LoopY - 1] == 0     &&
                m_i8RMonotonicity[t_i16LoopY - 2] == 0     )

                {
                    if(
                            m_i8RMonotonicity[t_i16LoopY + 2] != -1     &&
                            m_i8RMonotonicity[t_i16LoopY + 1] != -1     )
                    {

                        if(m_i8RMonotonicity[t_i16LoopY + 1] > 0)
                        {
                            t_i16FlagStatus = 1;
                        }
                        else
                        {
                            t_i16FlagStatus = 2;
                        }
                        Temp = m_stRPnt[t_i16LoopY];
                        break;

                    }
                }
            }

            if(t_i16FlagStatus == 2)
            {
                LOOPRSpecial:
                t_i16LostPoint = 1;
                t_i16LoopY++;
                while((--t_i16LoopY) > 2)   //舍去最上面的一排
                {
                  if (m_stRPnt[t_i16LoopY].m_i16x < RBOT.m_i16x &&  m_i8RMonotonicity[t_i16LoopY] == 0 && m_stRPnt[t_i16LoopY-1].m_i16x-m_stRPnt[t_i16LoopY].m_i16x<0)
                  {
                      //从该点往后寻找 10个的坐标相差不大
                      t_i16TempY = t_i16LoopY;
                      t_i16LoseCount = 0;

                      while((--t_i16LoopY) > t_i16TempY - 5 && t_i16LoopY > 0)
                      {
                          if(m_stRPnt[t_i16LoopY + 1].m_i16x - m_stRPnt[t_i16LoopY].m_i16x > 3)
                          {
                             t_i16LoseCount++;
                             break;
                          }
                      }
                      if(t_i16LoseCount == 0)
                      {
                          t_i16LostPoint = 0;
                          m_i16RTopcornerFindFlag = 1;
                          m_i16CorneerPointNumber++;
                          RTOP = m_stRPnt[t_i16TempY];
                          break;
                      }
                  }
                }

                if(t_i16LostPoint == 1)
                {
                    RTOP.m_i16x = IMGW - 3;
                    RTOP.m_i16y = 2;
                    m_i16RTopcornerFindFlag = 2;
                }

            }
            else if(t_i16FlagStatus == 1)
            {
                t_i16TargetPoint = 0;
                t_i16TempY = t_i16LoopY;

                while((--t_i16TempY) > t_i16LoopY  - 8 && t_i16TempY > 2)
                {
                    if(m_i8RMonotonicity[t_i16TempY] == 0)
                    {
                    t_i16TargetPoint++;
                    }
                }

                t_i16TempY = t_i16LoopY;

                while( ABS(m_stRPnt[t_i16LoopY].m_i16x - m_stRPnt[t_i16LoopY - 1].m_i16x) > 2 && t_i16LoopY > t_i16TempY - 8)
                {
                    t_i16LoopY--;
                }

                if(t_i16TargetPoint > 2 && ABS(m_stRPnt[t_i16TempY].m_i16x - m_stRPnt[t_i16TempY - 1].m_i16x) < 3)
                {
                    t_i16TargetPoint = 0;
                    t_i16LostPoint = 0;
                    t_i16FinishStatus = 0;

                    t_i16LoopY++;
                    t_i16TempY = t_i16LoopY;

                    while(++t_i16LoopY < t_i16TempY + 8)
                    {
                        if(ABS(m_stRPnt[t_i16LoopY].m_i16x - m_stRPnt[t_i16TempY].m_i16x) > 4)
                        {
                            if(++t_i16LostPoint > 1)
                            {
                            t_i16FinishStatus = 1;
                            m_i16RTopcornerFindFlag = 1;
                            m_i16CorneerPointNumber++;
                            RTOP = m_stRPnt[t_i16TempY];
                            break;
                            }
                        }
                    }

                    if(t_i16FinishStatus == 0)
                    {
                        RTOP.m_i16x = IMGW - 3;
                        RTOP.m_i16y = 2;
                        m_i16RTopcornerFindFlag = 2;
                    }
                }
                else
                {

                    goto LOOPRSpecial;
                }
            }
            else
            {
                RTOP.m_i16x = IMGW - 3;
                RTOP.m_i16y = 2;
                m_i16RTopcornerFindFlag = 2;
            }
        }
        else
        {
            RTOP.m_i16x = IMGW - 3;
            RTOP.m_i16y = 2;
            m_i16RTopcornerFindFlag = 2;
        }
    }
}


#endif

void ElementHandle_NormalStatus(void)
{

    float Temp_KSlope = 0;
    Int16 Count_I1 = 0;

    //左面正常补线
    if(     ( m_i16LTopcornerFindFlag == 1 ) &&
            ( m_i16LBotcornerFindFlag == 1 ) )
     {
         Temp_KSlope = m_stLPnt[LBOT.m_i16y].m_i16x;
         KSlope_Left  = (float)(LTOP.m_i16x - LBOT.m_i16x)/(float)(LTOP.m_i16y-LBOT.m_i16y);

         if(KSlope_Left < -0.36 && KSlope_Left > -0.86)
         {
             KStemp_Left = KSlope_Left;
         }
         else
         {
             KSlope_Left = KStemp_Left;
         }

         for(Count_I1 = LBOT.m_i16y - 1; Count_I1 > LTOP.m_i16y; Count_I1--)
         {
             Temp_KSlope -= KSlope_Left;
             m_stLPnt[Count_I1].m_i16x = (int)Temp_KSlope;
             m_stLPnt[Count_I1].m_i16y = Count_I1;
             if(m_stLPnt[Count_I1].m_i16x < 1)
             {
                 m_stLPnt[Count_I1].m_i16x = 1;
             }
             else if(m_stLPnt[Count_I1].m_i16x > IMGW - 2)
             {
                 m_stLPnt[Count_I1].m_i16x = IMGW - 2;
             }

         }
     }
     else if(
             ( m_i16LTopcornerFindFlag == 1 ) &&
             ( m_i16LBotcornerFindFlag == 2 ) )
     {
         Temp_KSlope = m_stLPnt[LTOP.m_i16y].m_i16x;
         KSlope_Left = regression(m_stLPnt, LTOP.m_i16y, LTOP.m_i16y - 6);

         if(KSlope_Left < -0.36 && KSlope_Left > -0.86)
         {
             KStemp_Left = KSlope_Left;
         }
         else
         {
             KSlope_Left = KStemp_Left;
         }

         for(Count_I1 = LTOP.m_i16y + 1; Count_I1 < IMGH; Count_I1++)
         {
             Temp_KSlope += KSlope_Left;
             m_stLPnt[Count_I1].m_i16x = (int)Temp_KSlope;
             m_stLPnt[Count_I1].m_i16y = Count_I1;
             if(m_stLPnt[Count_I1].m_i16x < 1)
             {
                 m_stLPnt[Count_I1].m_i16x = 1;
             }
             else if(m_stLPnt[Count_I1].m_i16x > IMGW - 2)
             {
                 m_stLPnt[Count_I1].m_i16x = IMGW - 2;
             }
         }
     }
     else if(( m_i16LTopcornerFindFlag == 2 ) &&
             ( m_i16LBotcornerFindFlag == 1 )  )
     {
         Temp_KSlope = LBOT.m_i16x;
         KSlope_Left = regression(m_stLPnt, LBOT.m_i16y + 6, LBOT.m_i16y);

         for(Count_I1 = LBOT.m_i16y - 1; Count_I1 > 0; Count_I1--)
         {
             Temp_KSlope -= KSlope_Left;
             m_stLPnt[Count_I1].m_i16x = (int)Temp_KSlope;
             m_stLPnt[Count_I1].m_i16y = Count_I1;
             if(m_stLPnt[Count_I1].m_i16x < 1)
             {
                 m_stLPnt[Count_I1].m_i16x = 1;
             }
             else if(m_stLPnt[Count_I1].m_i16x > IMGW - 2)
             {
                 m_stLPnt[Count_I1].m_i16x = IMGW - 2;
             }
         }
     }

   if (
            ( m_i16RTopcornerFindFlag == 1 ) &&
            ( m_i16RBotcornerFindFlag == 1 ) )
   {
       Temp_KSlope = m_stRPnt[RBOT.m_i16y].m_i16x;
       KSlope_Right = (float)(RTOP.m_i16x - RBOT.m_i16x)/(float)(RTOP.m_i16y-RBOT.m_i16y);

       if(KSlope_Right > 0.36 && KSlope_Right < 0.86)
       {
           KStemp_Right = KSlope_Right;
       }
       else
       {
           KSlope_Right = KStemp_Right;
       }


       for(Count_I1 = RBOT.m_i16y - 1; Count_I1 > RTOP.m_i16y; Count_I1--)
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
   else if(
           ( m_i16RTopcornerFindFlag == 1 ) &&
           ( m_i16RBotcornerFindFlag == 2 ) )
   {
       Temp_KSlope = m_stRPnt[RTOP.m_i16y].m_i16x;
       KSlope_Right = regression(m_stRPnt, RTOP.m_i16y, RTOP.m_i16y - 6);

       if(KSlope_Right > 0.36 && KSlope_Right < 0.86)
       {
           KStemp_Right = KSlope_Right;
       }
       else
       {
           KSlope_Right = KStemp_Right;
       }

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
   else if(( m_i16RTopcornerFindFlag == 2 ) &&
           ( m_i16RBotcornerFindFlag == 1 ) )
   {
       Temp_KSlope = RBOT.m_i16x;
       KSlope_Right = regression(m_stRPnt, RBOT.m_i16y + 6, RBOT.m_i16y);

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



void ElementHandle_SpecialStatus(void)
{

    float Temp_KSlope = 0;
    Int16 Count_I1 = 0;

    //左面正常补线
    if(     ( m_i16LTopcornerFindFlag == 1 ) &&
            ( m_i16LBotcornerFindFlag == 1 ) )
     {
         Temp_KSlope = m_stLPnt[LBOT.m_i16y].m_i16x;
         KSlope_Left  = (float)(LTOP.m_i16x - LBOT.m_i16x)/(float)(LTOP.m_i16y-LBOT.m_i16y);

         for(Count_I1 = LBOT.m_i16y - 1; Count_I1 > LTOP.m_i16y; Count_I1--)
         {
             Temp_KSlope -= KSlope_Left;
             m_stLPnt[Count_I1].m_i16x = (int)Temp_KSlope;
             m_stLPnt[Count_I1].m_i16y = Count_I1;
             if(m_stLPnt[Count_I1].m_i16x < 1)
             {
                 m_stLPnt[Count_I1].m_i16x = 1;
             }
             else if(m_stLPnt[Count_I1].m_i16x > IMGW - 2)
             {
                 m_stLPnt[Count_I1].m_i16x = IMGW - 2;
             }

         }
     }
     else if(
             ( m_i16LTopcornerFindFlag == 1 ) &&
             ( m_i16LBotcornerFindFlag == 2 ) )
     {
         Temp_KSlope = m_stLPnt[LTOP.m_i16y].m_i16x;
         KSlope_Left = regression(m_stLPnt, LTOP.m_i16y, LTOP.m_i16y - 6);

         for(Count_I1 = LTOP.m_i16y + 1; Count_I1 < IMGH; Count_I1++)
         {
             Temp_KSlope += KSlope_Left;
             m_stLPnt[Count_I1].m_i16x = (int)Temp_KSlope;
             m_stLPnt[Count_I1].m_i16y = Count_I1;
             if(m_stLPnt[Count_I1].m_i16x < 1)
             {
                 m_stLPnt[Count_I1].m_i16x = 1;
             }
             else if(m_stLPnt[Count_I1].m_i16x > IMGW - 2)
             {
                 m_stLPnt[Count_I1].m_i16x = IMGW - 2;
             }
         }
     }
     else if(( m_i16LTopcornerFindFlag == 2 ) &&
             ( m_i16LBotcornerFindFlag == 1 )  )
     {
         Temp_KSlope = LBOT.m_i16x;
         KSlope_Left = regression(m_stLPnt, LBOT.m_i16y + 6, LBOT.m_i16y);

         for(Count_I1 = LBOT.m_i16y - 1; Count_I1 > 0; Count_I1--)
         {
             Temp_KSlope -= KSlope_Left;
             m_stLPnt[Count_I1].m_i16x = (int)Temp_KSlope;
             m_stLPnt[Count_I1].m_i16y = Count_I1;
             if(m_stLPnt[Count_I1].m_i16x < 1)
             {
                 m_stLPnt[Count_I1].m_i16x = 1;
             }
             else if(m_stLPnt[Count_I1].m_i16x > IMGW - 2)
             {
                 m_stLPnt[Count_I1].m_i16x = IMGW - 2;
             }
         }
     }

   if (
            ( m_i16RTopcornerFindFlag == 1 ) &&
            ( m_i16RBotcornerFindFlag == 1 ) )
   {
       Temp_KSlope = m_stRPnt[RBOT.m_i16y].m_i16x;
       KSlope_Right = (float)(RTOP.m_i16x - RBOT.m_i16x)/(float)(RTOP.m_i16y-RBOT.m_i16y);

       for(Count_I1 = RBOT.m_i16y - 1; Count_I1 > RTOP.m_i16y; Count_I1--)
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
   else if(
           ( m_i16RTopcornerFindFlag == 1 ) &&
           ( m_i16RBotcornerFindFlag == 2 ) )
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
   else if(( m_i16RTopcornerFindFlag == 2 ) &&
           ( m_i16RBotcornerFindFlag == 1 ) )
   {
       Temp_KSlope = RBOT.m_i16x;
       KSlope_Right = regression(m_stRPnt, RBOT.m_i16y + 6, RBOT.m_i16y);

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


void SpecialTopCorner_TaskFunction(void)
{

    uint8_t (*InImg)[IMGW];
    InImg = mt9v03x_output_image;

    Int16 t_i16LoopX = IMGW;

    Int16 t_i16LoopY = IMGH - 5;

    Int16 FindTopWhiteSignal  = 0;

    Int16 t_i16TempX;
    Int16 t_i16Equal;
    Int16 LTargetPointCount;
    Int16 RTargetPointCount;

    ChamferCorner.m_i16x = 0;
    ChamferCorner.m_i16y = 0;

    EndTopofLine.m_i16x = 0;
    EndTopofLine.m_i16y = 0;

     memset(TopWhiteBorder, 0, sizeof(TopWhiteBorder));

    while((--t_i16LoopX) > -1)
    {
        FindTopWhiteSignal  = 0;

        while((--t_i16LoopY) > 5)
        {
            if( InImg[t_i16LoopY - 1][t_i16LoopX] == B_BLACK   &&
                InImg[t_i16LoopY    ][t_i16LoopX] == B_WHITE   &&
                InImg[t_i16LoopY + 1][t_i16LoopX] == B_WHITE   )
            {
                TopWhiteBorder[t_i16LoopX].m_i16x = t_i16LoopX;
                TopWhiteBorder[t_i16LoopX].m_i16y = t_i16LoopY;
                FindTopWhiteSignal  = 1;
                break;
            }
        }

        if(InImg[t_i16LoopY][t_i16LoopX] == B_WHITE && InImg[58][t_i16LoopX] == B_WHITE)
        {
            TopWhiteBorder[t_i16LoopX].m_i16x = t_i16LoopX;
            TopWhiteBorder[t_i16LoopX].m_i16y = t_i16LoopY;
            FindTopWhiteSignal  = 1;
        }

        if(FindTopWhiteSignal  == 1)
        {
            t_i16LoopY = t_i16LoopY + 10; //往前搜索
            t_i16LoopY = t_i16LoopY < IMGH - 5 ? t_i16LoopY : IMGH - 5; //保护
        }
        else
        {
            t_i16LoopY = IMGH - 5;
        }
    }

    //要寻找棱角         先减小后增大

    t_i16LoopX = 10;
    while((++t_i16LoopX) < IMGW - 11)
    {
        if(TopWhiteBorder[t_i16LoopX - 3].m_i16x && TopWhiteBorder[t_i16LoopX].m_i16x && TopWhiteBorder[t_i16LoopX + 3].m_i16x)
        {
            if(
                    TopWhiteBorder[t_i16LoopX - 3].m_i16y < TopWhiteBorder[t_i16LoopX].m_i16y  &&
                    TopWhiteBorder[t_i16LoopX + 3].m_i16y < TopWhiteBorder[t_i16LoopX].m_i16y
                    )
            {
                //进一步判断
                LTargetPointCount = 0;
                RTargetPointCount = 0;

                t_i16Equal = 0;
                t_i16TempX = t_i16LoopX;

                while((--t_i16TempX) > t_i16LoopX - 4)
                {
                    if(TopWhiteBorder[t_i16TempX].m_i16x)
                    {
                        if(TopWhiteBorder[t_i16TempX].m_i16y < TopWhiteBorder[t_i16LoopX].m_i16y)
                        {
                            LTargetPointCount++;
                        }
                    }
                }


                while((++t_i16TempX) < t_i16LoopX + 4)
                {
                    if(TopWhiteBorder[t_i16TempX].m_i16x)
                    {
                        if(TopWhiteBorder[t_i16TempX].m_i16y < TopWhiteBorder[t_i16LoopX].m_i16y)
                        {
                            RTargetPointCount++;
                        }
                        else if(TopWhiteBorder[t_i16TempX].m_i16y == TopWhiteBorder[t_i16LoopX].m_i16y)
                        {
                            t_i16Equal = t_i16TempX;
                            RTargetPointCount++;
                        }
                    }
                }

                if(LTargetPointCount > 2 && RTargetPointCount > 2)
                {
                    if(t_i16Equal)  t_i16LoopX = (t_i16LoopX + t_i16Equal) / 2;
                    ChamferCorner = TopWhiteBorder[t_i16LoopX];
                    break;
                }
            }
        }
    }
}

void DropLine_ProtectFunction(void)
{

    static unsigned char ImageTime_Count = 0;
    if((AllAreaWhitePoint) < 200 && VehicleInformation.StartDeparture == _VehicleInformation_Stare )
    {
        if(++ImageTime_Count > 3)
        {
            VehicleInformation.StartDeparture = _VehicleInformation_Stop;
            ImageTime_Count = 0;
        }
    }
    else
    {

        ImageTime_Count = 0;
    }

}

