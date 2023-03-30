



#include "ElementProcess.h"



/*������ȡ�յ�*/
ST_2D_INT_POINT_INFO LTOP;
ST_2D_INT_POINT_INFO RTOP;
ST_2D_INT_POINT_INFO LBOT;
ST_2D_INT_POINT_INFO RBOT;


/*��־��*/
Int8 m_i16LTopcornerFindFlag;                                 /*���Ͻǵ���Ѱ��־*/
Int8 m_i16LBotcornerFindFlag;                                 /*���½ǵ���Ѱ��־*/
Int8 m_i16RTopcornerFindFlag;                                 /*���Ͻǵ���Ѱ��־*/
Int8 m_i16RBotcornerFindFlag;                                 /*���½ǵ���Ѱ��־*/

Int8 m_i16CorneerPointNumber;

/*����б��*/
float KSlope_Left, KSlope_Right;
float KStemp_Left = -0.6f, KStemp_Right = 0.6f;

/*�������*/
Int16 m_i16OutWidthROWNum;                                  /*������*/
Int16 m_i16WOutWidthROWNum;                                 /*�ǳ����صĳ���״̬*/
Int16 m_i16AbsoluteOutWidth;

ST_2D_INT_POINT_INFO Temp_Point;

ST_2D_INT_POINT_INFO TopWhiteBorder[IMGW];
ST_2D_INT_POINT_INFO ChamferCorner;
ST_2D_INT_POINT_INFO EndTopofLine;

Int8  FrontCarStareDistance;
Int8  FrontCarExpect;

float _StopSpeed;



/**
 *      ��������ʼ��
 **/

void ElementInital_UltraWideParam(void)
{

    //��������
    m_i16OutWidthROWNum  = 0;
    m_i16WOutWidthROWNum = 0;

//    //����
//    _TridentDirectionStatus = _Trident_Close;
//
//    _Trident_Record = 0;
//    _Trident_Distance = 0;

//    //������
//    _CrossWalk_Record = 0;
//
    //ǰհ����
    _Cammer_Prospect = 30;
//
//    //ֹͣ���־���
//    Stop_Distance = 1500;
//
//    //��������
//    _Trident_Setup = 2000;
//
    //ֱ������
//    SetTargerSpeed = 0;

    //�����г�ʼ����
    m_stOptimalPoint.m_i16x = IMGW / 2;

    //�𲽻���
    _WaitStop_Record = 0;

    //�𲽽Ƕ�����
    _SetupAngle = 100;

    //ǰհ����
    _Special_Prospect = 50;
    _Roundabout_Prospect = 42;
    _Trident_Prospect  = 36;
    _Cammer_Prospect   = 43;

//    _StopSpeed = 40;
    //�����ٶȿ���
//    SetLMotorParam(0,260,1);
//    SetRMotorParam(0,260,1);
}


/**
 *  ���ʽ���
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
     Int16 t_i16LoopY=0;                                         /*����ѭ����*/

     Int16 t_i16TempY = 0;

     Int16 t_i16Count = 0, t_i16LoseCount = 0;

     Int16 t_i16LostPoint = 0, t_i16TargetPoint = 0;             //������Ҫ��ĵ�

     Int16 t_i16FlagStatus = 0, t_i16FinishStatus = 0;


     ST_2D_INT_POINT_INFO Temp;

     LBOT.m_i16x = 2;                                            //���µ�
     LBOT.m_i16y = IMGH - 2;

     LTOP.m_i16x = 2;
     LTOP.m_i16y = 2;

     RBOT.m_i16x = IMGW - 3;
     RBOT.m_i16y = IMGH - 2;

     RTOP.m_i16x = IMGW - 3;
     RTOP.m_i16y = 2;


     m_i16LTopcornerFindFlag=-1;    //����
     m_i16LBotcornerFindFlag=-1;    //����
     m_i16RTopcornerFindFlag=-1;    //����
     m_i16RBotcornerFindFlag=-1;    //����
     m_i16CorneerPointNumber = 0;  //��¼�ҵ��ĵ���


     /* -1 ��ʾѰ�Ҵ��� 0��ʾ�ҵ��ߵĵ����� 1 ��ʾ�ҵ��յ� 2��ʾ��ʧ�յ�*/

     /*
      * ��һ��Ѱ�����µ� ��ȥ��һ�ź����һ��
      */
     Temp.m_i16x = 2;
     Temp.m_i16y = IMGH - 2;

     t_i16LoopY = IMGH - 1;

     while(--t_i16LoopY > 0)
     {
         /*������ڵ�������ʲô��   ��ֵ��ͬʱӦ������������ʱ������*/
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
         // ��һ����� һֱɨ�赽���һ�� ��ֱ��Ϊ����ֱ��
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
         //�ײ���ȥ����ĵ�
         LBOT.m_i16x = 2;
         LBOT.m_i16y = IMGH - 2;
         m_i16LBotcornerFindFlag = 2;
     }
     else
     {
         //�ҵ��¹յ�
         /*�������㵥���� �������۵���� δ�ҵ��ĵ�*/
         t_i16LoopY = Temp.m_i16y + 8;
         t_i16LoopY = t_i16LoopY > IMGH - 2 ? IMGH - 2 : t_i16LoopY;


         t_i16LostPoint = 0;
         t_i16TargetPoint = 0;
         //���㵥����
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
         //���ϲ����㵥����
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
             m_i16LBotcornerFindFlag = 2;        //�����յ�
         }
     }


     /*
      *  Ѱ�����Ͻǵ�
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

        while((--t_i16LoopY) > 2 && m_stLPnt[t_i16LoopY].m_i16x > LBOT.m_i16x); //�ϲ���һ�� �� �յ���¹յ�С

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

        //���������

            if(t_i16FlagStatus == 2)
            {

                LOOPLSpecial:
                  t_i16LostPoint = 1;
                  t_i16LoopY++;
                  while((--t_i16LoopY) > 2)   //��ȥ������10�� ����Ҫ����
                  {
                      //�Ƿ���Ҫ�������
                      if (m_stLPnt[t_i16LoopY].m_i16x > LBOT.m_i16x && m_i8LMonotonicity[t_i16LoopY] == 0 && m_stLPnt[t_i16LoopY-1].m_i16x-m_stLPnt[t_i16LoopY].m_i16x>-1)
                      {
                          //�Ӹõ�����Ѱ�� 10������������
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
                      m_i16LTopcornerFindFlag = 2;    //�����׵�
                  }
            }
            else if(t_i16FlagStatus == 1)
            {

                //������Ҫ���㵥����
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
                    //�������㵥����
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
                        m_i16LTopcornerFindFlag = 2;    //�����׵�
                    }
                }
                else
                {
                    goto LOOPLSpecial;  //����Ѱ�ҹյ�
                }
            }
            else
            {
                LTOP.m_i16x = 2;
                LTOP.m_i16y = 2;
                m_i16LTopcornerFindFlag = 2;    //�����׵�
            }
        }
        else
        {
            LTOP.m_i16x = 2;
            LTOP.m_i16y = 2;
            m_i16LTopcornerFindFlag = 2;    //�����׵�
        }
    }

    /**************************************************************************/

    /*
     *  Ѱ�����½ǵ�
     */
    Temp.m_i16x = IMGW - 3;
    Temp.m_i16y = IMGH - 2;

    t_i16LoopY = IMGH - 1;

    t_i16Count = 0;

    /*��ż��յ�*/
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
        // ��һ����� һֱɨ�赽���һ�� ��ֱ��Ϊ����ֱ��
        t_i16Count = 58;
        while(m_i8RMonotonicity[t_i16Count] != 0)
        {
            t_i16Count--;
        }
        m_i16RBotcornerFindFlag = 0;
    }
    else if(Temp.m_i16x > IMGW - 5 || Temp.m_i16x < 5)
    {
        //�˴�Ҫ���в��߲��� ���Ͻǵ�
        RBOT.m_i16x = IMGW - 3;
        RBOT.m_i16y = IMGH - 2;
        m_i16RBotcornerFindFlag = 2;        //�����յ�

    }
    else
    {
        /*�յ��������㵥����*/
        t_i16LoopY = Temp.m_i16y + 8;
        t_i16LoopY = t_i16LoopY > IMGH - 2 ? IMGH - 2 : t_i16LoopY;


        t_i16LostPoint = 0;
        t_i16TargetPoint = 0;
        //���㵥����
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
        //��������հ�
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
            //�˴�Ҫ���в��߲��� ���Ͻǵ�
            RBOT.m_i16x = IMGW - 3;
            RBOT.m_i16y = IMGH - 2;
            m_i16RBotcornerFindFlag = 2;        //�����յ�
        }
    }


    /*
     *  Ѱ�����Ͻǵ�
     */

    if(m_i16RBotcornerFindFlag == 0)
    {
        //���㵥���Ե����
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
                while((--t_i16LoopY) > 2)   //��ȥ�������һ��
                {
                  if (m_stRPnt[t_i16LoopY].m_i16x < RBOT.m_i16x &&  m_i8RMonotonicity[t_i16LoopY] == 0 && m_stRPnt[t_i16LoopY-1].m_i16x-m_stRPnt[t_i16LoopY].m_i16x<1)
                  {
                      //�Ӹõ�����Ѱ�� 10������������
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
     Int16 t_i16LoopY=0;                                         /*����ѭ����*/

     Int16 t_i16TempY = 0;

     Int16 t_i16Count = 0, t_i16LoseCount = 0;

     Int16 t_i16LostPoint = 0, t_i16TargetPoint = 0;             //������Ҫ��ĵ�

     Int16 t_i16FlagStatus = 0, t_i16FinishStatus = 0;


     ST_2D_INT_POINT_INFO Temp;

     LBOT.m_i16x = 2;                                            //���µ�
     LBOT.m_i16y = IMGH - 2;

     LTOP.m_i16x = 2;
     LTOP.m_i16y = 2;

     RBOT.m_i16x = IMGW - 3;
     RBOT.m_i16y = IMGH - 2;

     RTOP.m_i16x = IMGW - 3;
     RTOP.m_i16y = 2;


     m_i16LTopcornerFindFlag=-1;    //����
     m_i16LBotcornerFindFlag=-1;    //����
     m_i16RTopcornerFindFlag=-1;    //����
     m_i16RBotcornerFindFlag=-1;    //����
     m_i16CorneerPointNumber = 0;  //��¼�ҵ��ĵ���


     /* -1 ��ʾѰ�Ҵ��� 0��ʾ�ҵ��ߵĵ����� 1 ��ʾ�ҵ��յ� 2��ʾ��ʧ�յ�*/

     /*
      * ��һ��Ѱ�����µ� ��ȥ��һ�ź����һ��
      */
     Temp.m_i16x = 2;
     Temp.m_i16y = IMGH - 2;

     t_i16LoopY = IMGH - 1;

     while(--t_i16LoopY > 0)
     {
         /*������ڵ�������ʲô��   ��ֵ��ͬʱӦ������������ʱ������*/
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
         // ��һ����� һֱɨ�赽���һ�� ��ֱ��Ϊ����ֱ��
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
         //�ײ���ȥ����ĵ�
         LBOT.m_i16x = 2;
         LBOT.m_i16y = IMGH - 2;
         m_i16LBotcornerFindFlag = 2;
     }
     else
     {
         //�ҵ��¹յ�
         /*�������㵥���� �������۵���� δ�ҵ��ĵ�*/
         t_i16LoopY = Temp.m_i16y + 8;
         t_i16LoopY = t_i16LoopY > IMGH - 2 ? IMGH - 2 : t_i16LoopY;


         t_i16LostPoint = 0;
         t_i16TargetPoint = 0;
         //���㵥����
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
         //���ϲ����㵥����
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
             m_i16LBotcornerFindFlag = 2;        //�����յ�
         }
     }


     /*
      *  Ѱ�����Ͻǵ�
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

        while((--t_i16LoopY) > 2 && m_stLPnt[t_i16LoopY].m_i16x > LBOT.m_i16x); //�ϲ���һ�� �� �յ���¹յ�С

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

        //���������

            if(t_i16FlagStatus == 2)
            {

                LOOPLSpecial:
                  t_i16LostPoint = 1;
                  t_i16LoopY++;
                  while((--t_i16LoopY) > 2)   //��ȥ������10�� ����Ҫ����
                  {
                      //�Ƿ���Ҫ�������
                      if (m_stLPnt[t_i16LoopY].m_i16x > LBOT.m_i16x && m_i8LMonotonicity[t_i16LoopY] == 0 && m_stLPnt[t_i16LoopY-1].m_i16x-m_stLPnt[t_i16LoopY].m_i16x>0)
                      {
                          //�Ӹõ�����Ѱ�� 10������������
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
                      m_i16LTopcornerFindFlag = 2;    //�����׵�
                  }
            }
            else if(t_i16FlagStatus == 1)
            {

                //������Ҫ���㵥����
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
                    //�������㵥����
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
                        m_i16LTopcornerFindFlag = 2;    //�����׵�
                    }
                }
                else
                {
                    goto LOOPLSpecial;  //����Ѱ�ҹյ�
                }
            }
            else
            {
                LTOP.m_i16x = 2;
                LTOP.m_i16y = 2;
                m_i16LTopcornerFindFlag = 2;    //�����׵�
            }
        }
        else
        {
            LTOP.m_i16x = 2;
            LTOP.m_i16y = 2;
            m_i16LTopcornerFindFlag = 2;    //�����׵�
        }
    }

    /**************************************************************************/

    /*
     *  Ѱ�����½ǵ�
     */
    Temp.m_i16x = IMGW - 3;
    Temp.m_i16y = IMGH - 2;

    t_i16LoopY = IMGH - 1;

    t_i16Count = 0;

    /*��ż��յ�*/
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
        // ��һ����� һֱɨ�赽���һ�� ��ֱ��Ϊ����ֱ��
        t_i16Count = 58;
        while(m_i8RMonotonicity[t_i16Count] != 0)
        {
            t_i16Count--;
        }
        m_i16RBotcornerFindFlag = 0;
    }
    else if(Temp.m_i16x > IMGW - 5 || Temp.m_i16x < 5)
    {
        //�˴�Ҫ���в��߲��� ���Ͻǵ�
        RBOT.m_i16x = IMGW - 3;
        RBOT.m_i16y = IMGH - 2;
        m_i16RBotcornerFindFlag = 2;        //�����յ�

    }
    else
    {
        /*�յ��������㵥����*/
        t_i16LoopY = Temp.m_i16y + 8;
        t_i16LoopY = t_i16LoopY > IMGH - 2 ? IMGH - 2 : t_i16LoopY;


        t_i16LostPoint = 0;
        t_i16TargetPoint = 0;
        //���㵥����
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
        //��������հ�
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
            //�˴�Ҫ���в��߲��� ���Ͻǵ�
            RBOT.m_i16x = IMGW - 3;
            RBOT.m_i16y = IMGH - 2;
            m_i16RBotcornerFindFlag = 2;        //�����յ�
        }
    }


    /*
     *  Ѱ�����Ͻǵ�
     */

    if(m_i16RBotcornerFindFlag == 0)
    {
        //���㵥���Ե����
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
                while((--t_i16LoopY) > 2)   //��ȥ�������һ��
                {
                  if (m_stRPnt[t_i16LoopY].m_i16x < RBOT.m_i16x &&  m_i8RMonotonicity[t_i16LoopY] == 0 && m_stRPnt[t_i16LoopY-1].m_i16x-m_stRPnt[t_i16LoopY].m_i16x<0)
                  {
                      //�Ӹõ�����Ѱ�� 10������������
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

    //������������
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

    //������������
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
            t_i16LoopY = t_i16LoopY + 10; //��ǰ����
            t_i16LoopY = t_i16LoopY < IMGH - 5 ? t_i16LoopY : IMGH - 5; //����
        }
        else
        {
            t_i16LoopY = IMGH - 5;
        }
    }

    //ҪѰ�����         �ȼ�С������

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
                //��һ���ж�
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

