



#include "FeatureProcess.h"

ST_2D_INT_POINT_INFO m_stLPnt[IMGH];         /*��߱�������*/
ST_2D_INT_POINT_INFO m_stRPnt[IMGH];         /*�ұ߱�������*/
ST_2D_INT_POINT_INFO m_stCPnt[IMGH];         /*��������*/
ST_2D_INT_POINT_INFO m_stTPnt[IMGH];         /*ɨ������*/

ST_2D_INT_POINT_INFO m_stLMaxPoint;          /*�������,�����������,���㲻Ӧ�ó����ڱ��ߵ��м�,ֻ������ڱ��߽���,�������м�,��ô�������Ӧ�þ�*/
ST_2D_INT_POINT_INFO m_stRMinPoint;
ST_2D_INT_POINT_INFO m_stOptimalPoint;       /*ʵ����������ڵ����ŵ�*/
ST_2D_INT_POINT_INFO m_stLOptimalPoint;      /*ʵ��������������ڵ����ŵ�*/
ST_2D_INT_POINT_INFO m_stROptimalPoint;      /*ʵ���Ұ����������ڵ����ŵ�*/
ST_2D_INT_POINT_INFO m_stCenterLinePoint;    /*ͼ���м������ڵ����ŵ�*/
Int16 m_i16LPointCnt;                        /*����ҵ�����*/
Int16 m_i16RPointCnt;                        /*�ұ��ҵ�����*/
Int16 m_i16LEndNum;                          /*����ҵ�����*/
Int16 m_i16REndNum;                          /*�ұ��ҵ�����*/
Int16 m_i16LLostPointCnt;                    /*��߶�ʧ������*/
Int16 m_i16RLostPointCnt;                    /*�ұ߶�ʧ������*/
Uint32 m_u32LAllArea;                        /*��߰�ɫ�������*/
Uint32 m_u32RAllArea;                        /*�ұ߰�ɫ�������*/
Uint32 m_u32TALLArea;                        /*����ͼ���ɫ�����*/
Int8 m_i8LMonotonicity[IMGH];                /*��ߵ����� (����:���߽׶�������������Ҫ�Ƿ���Ҫ����,���߲��ߵ�����,���)*/
Int8 m_i8LMonotonicityCnt;                   /*Υ������Եĵ���*/
Int8 m_i8RMonotonicity[IMGH];                /*�ұߵ����� (����:ͬ��,�����ұ�)*/
Int8 m_i8RMonotonicityCnt;                   /*Υ������Եĵ���*/

Uint16 m_u16LineBAr[IMGW];                   /*��¼ÿ�дӵײ����ϵİ׵�������*/

ST_2D_INT_POINT_INFO m_stErPointBuffer[7];
F32 m_f32LineAllError;



Int8 _CammerGeneralEdgeType[IMGH];
Int8 _CammerSpecialEdgeType[IMGH];

Uint8 _CammerLEdgeNumber;
Uint8 _CammerREdgeNumber;

float _CammerSimilar;


Int16 TrackWide[IMGH] =
{
        28,
        28,
        30,
        30,
        32,
        34,
        34,
        36,
        36,
        38,
        38,
        40,
        42,
        42,
        44,
        44,
        46,
        46,
        48,
        49,
        50,
        51,
        52,
        54,
        54,
        56,
        56,
        58,
        59,
        60,
        62,
        62,
        64,
        64,
        66,
        66,
        68,
        68,
        70,
        72,
        72,
        74,
        74,
        76,
        76,
        78,
        79,
        80,
        82,
        82,
        84,
        84,
        86,
        86,
        88,
        88,
        90,
        92,
        92,
        94
};


const float OffestWeight[6] =
{
//        {1.2f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f}, //1
//        {0.7f, 0.5f, 0.0f, 0.0f,  0.0f,  0.0f}, //2
//        {0.6f, 0.4f, 0.2f, 0.0f,  0.0f,  0.0f}, //3
//        {0.5f, 0.4f, 0.2f, 0.1f,  0.0f,  0.0f}, //4
//        {0.4f, 0.3f, 0.3f, 0.1f,  0.1f,  0.0f}, //5
        0.4, 0.3, 0.2, 0.15, 0.1, 0.05 //6
};


Int16 ElementHandle_LimitEdge(Int16 Variable, Int16 LimitMin, Int16 LimitMax)
{
    return ((Variable)<(LimitMin)?(LimitMin):((Variable)>(LimitMax)?(LimitMax):(Variable)));
}

//�������
char SLine(float *x,float *y,Uint8 n,float *a0,float *a1,float *a2)
{
    float temp=0;
    float x0=n,x1=0,x2=0,x3=0,x4=0,x0y1=0,x1y1=0,x2y1=0;//��ʾ��������
    float x0x2,x0x3,x0x4,x1x1,x1x2,x1x3,x1x4,x2x2,x2x3,x2x4,x3x3;
    float a_banshui[3][3],a_ni[3][3];//����ǰ������������
    float a_abs;//����a������ʽ
    for(int i=1;i<n-1;i++)
    {
        x0y1+=*(y+i);
        temp=*(x+i);
        x1+=temp;
        x1y1+=temp*(*(y+i));
        temp*=*(x+i);
        x2+=temp;
        x2y1+=temp*(*(y+i));
        temp*=*(x+i);
        x3+=temp;
        temp*=*(x+i);
        x4+=temp;
    }
    //����Ҫ�õ������ݣ��������
    x0x2=x0*x2;
    x0x3=x0*x3;
    x0x4=x0*x4;
    x1x1=x1*x1;
    x1x2=x1*x2;
    x1x3=x1*x3;
    x1x4=x1*x4;
    x2x2=x2*x2;
    x2x3=x2*x3;
    x2x4=x2*x4;
    x3x3=x3*x3;
    //����������,����ʽ�����������ʵ�������öԳ����ټ�������
    a_banshui[0][0]= (x2x4-x3x3);  a_banshui[0][1]=-(x1x4-x2x3);  a_banshui[0][2]= (x1x3-x2x2);
    a_banshui[1][0]=-(x1x4-x2x3);  a_banshui[1][1]= (x0x4-x2x2);  a_banshui[1][2]=-(x0x3-x1x2);
    a_banshui[2][0]= (x1x3-x2x2);  a_banshui[2][1]=-(x0x3-x1x2);  a_banshui[2][2]= (x0x2-x1x1);
    //��������Ӧ����ʽ��ֵ
    a_abs=(x0*a_banshui[0][0]+x1*a_banshui[0][1]+x2*a_banshui[0][2]);
    //���������
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            a_ni[i][j]=a_banshui[i][j]/a_abs;
        }
    }
    *a0=a_ni[0][0]*x0y1+a_ni[0][1]*x1y1+a_ni[0][2]*x2y1;
    *a1=a_ni[1][0]*x0y1+a_ni[1][1]*x1y1+a_ni[1][2]*x2y1;
    *a2=a_ni[2][0]*x0y1+a_ni[2][1]*x1y1+a_ni[2][2]*x2y1;
    return 1;
}


void ElementHandle_UltraWide(void)
{
    Int16 t_i16LoopY=0;                                         /*����ѭ����*/
    Int16 t_i16Width = 0;

    m_i16OutWidthROWNum   = 0;
    m_i16WOutWidthROWNum  = 0;
    m_i16AbsoluteOutWidth = 0;



#if 1

    t_i16LoopY = IMGH - 1;
    while((--t_i16LoopY) > 0)   /*�������������һ���޷����*/
    {
        t_i16Width = m_stRPnt[t_i16LoopY].m_i16x - m_stLPnt[t_i16LoopY].m_i16x;
        if(t_i16Width > TrackWide[t_i16LoopY] + 8)
        {
            m_i16OutWidthROWNum++;
        }
        if(t_i16Width > 120)
        {
            m_i16WOutWidthROWNum++;
        }
        if(t_i16Width > 160 )
        {
            m_i16AbsoluteOutWidth++;
        }
    }

#else

    t_i16LoopY = -1;

    printf("************************************************************\r\n");

    while((++t_i16LoopY) < IMGH)   /*�������������һ���޷����*/
    {
        t_i16Width = ABS(m_stRPnt[t_i16LoopY].m_i16x - m_stLPnt[t_i16LoopY].m_i16x);

        printf("%d,\r\n",t_i16Width);

        if(t_i16Width > TrackWide[t_i16LoopY])
        {
            m_i16OutWidthROWNum++;
        }

        if(t_i16Width > IMGW * 0.75f)
        {
            m_i16WOutWidthROWNum++;
        }

    }
    printf("************************************************************\r\n");

#endif

}



float _Similar;
Int16  _Result;
Uint16 _Width;
Uint16 _CalculationEdge;
Uint16 _CalculationLine;

 void CalculationCenterLine(void)
 {
    Int16 t_i16LoopY;
    Int16 MapLth;
    Int16 Curve;
    Int16 Last_LoopY;
//    Int16 FlagBend;

    t_i16LoopY = IMGH;

    _CalculationEdge = TrackWide[IMGH - 1];
    _CalculationLine = IMGH - 1;


    /*��һ����ֵ�ж����*/
//    if(ipts0_num == 0 && ipts1_num > 60 && ReftLoopYangle < 0.5f && ipts1[ipts1_num - 1][0] < 60)
//    {
//        FlagBend = 50;
//        while((--FlagBend) > 1)
//        {
//            m_stLPnt[FlagBend].m_i16y = 0;
//        }
//    }
//    else if(ipts1_num == 0 && ipts0_num > 60 &&  LeftLoopYangle < 0.5f && ipts0[ipts0_num - 1][0] > IMGW - 60)
//    {
//        FlagBend = 50;
//        while((--FlagBend) > 1)
//        {
//            m_stRPnt[FlagBend].m_i16y = 0;
//        }
//    }
//    else
//    {
//        FlagBend = 0;
//    }


    while ((--t_i16LoopY) > -1)
    {
        Last_LoopY = t_i16LoopY + 1;

        if      (m_stLPnt[t_i16LoopY].m_i16y && m_stRPnt[t_i16LoopY].m_i16y)
        {
            m_stCPnt[t_i16LoopY].m_i16x = (m_stLPnt[t_i16LoopY].m_i16x + m_stRPnt[t_i16LoopY].m_i16x) / 2;
            m_stCPnt[t_i16LoopY].m_i16y = t_i16LoopY;

            _CalculationEdge = m_stRPnt[t_i16LoopY].m_i16x - m_stLPnt[t_i16LoopY].m_i16x;
            _CalculationLine = t_i16LoopY;
        }
        else if (m_stLPnt[t_i16LoopY].m_i16y && !m_stRPnt[t_i16LoopY].m_i16y)
        {
            //�����Ϊ�ұ߶���

            //��ֹ��ʼ�͵��߶��ߵ����
            if(t_i16LoopY < IMGH - 3)
            {
                //��һ�ε���ߺ��ұ߾�����
                if      (m_stLPnt[Last_LoopY].m_i16y && m_stRPnt[Last_LoopY].m_i16y)
                {
                    MapLth = ABS(m_stRPnt[Last_LoopY].m_i16x-m_stLPnt[Last_LoopY].m_i16x) / 2;
                    m_stCPnt[t_i16LoopY].m_i16x = m_stLPnt[t_i16LoopY].m_i16x + MapLth - 1;
                    m_stCPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                }
                else if (m_stLPnt[Last_LoopY].m_i16y)
                {
                    //��һ����ߴ���   //����Ȼ��ۻ�
                    Curve = m_stLPnt[t_i16LoopY].m_i16x - m_stLPnt[Last_LoopY].m_i16x;
                    if      (m_i8LMonotonicity[t_i16LoopY] == 0 && Curve > -1 && Curve < 4)
                    {
                        //���ֺ��������ͬ������
                        m_stCPnt[t_i16LoopY].m_i16x = m_stCPnt[Last_LoopY].m_i16x + Curve;
                        m_stCPnt[t_i16LoopY].m_i16y = t_i16LoopY;

                    }
                    else
                    {
                        //����������㵥����  ������ͻ��  �����
                        _Width = m_stCPnt[Last_LoopY].m_i16x - m_stLPnt[Last_LoopY].m_i16x;
                        _Similar = (float)(_Width)/ TrackWide[Last_LoopY];
                        _Result = (Int16)(TrackWide[t_i16LoopY] * _Similar);

                        m_stCPnt[t_i16LoopY].m_i16x = m_stLPnt[t_i16LoopY].m_i16x + _Result;
                        m_stCPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                    }

                }
                else if(m_stRPnt[Last_LoopY].m_i16y)
                {
                    //��һ������� ֱ��ͨ�������
                    _Width = m_stRPnt[Last_LoopY].m_i16x - m_stCPnt[Last_LoopY].m_i16x;
                    _Similar = (float)(_Width)/ TrackWide[Last_LoopY];
                    _Result = (Int16)(TrackWide[t_i16LoopY] * _Similar);

                    m_stCPnt[t_i16LoopY].m_i16x = m_stLPnt[t_i16LoopY].m_i16x + _Result;
                    m_stCPnt[t_i16LoopY].m_i16y = t_i16LoopY;

                }
                else
                {
                    //���ܻ���ִ���ĵ� �����ڴ���

                    _Similar = (float)_CalculationEdge / TrackWide[_CalculationLine];
                    _Result = (Int16)(TrackWide[t_i16LoopY] * _Similar);

                    m_stCPnt[t_i16LoopY].m_i16x = m_stLPnt[t_i16LoopY].m_i16x + (_Result / 2);
                    m_stCPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                }
            }
            else
            {
                //�տ�ʼ����ͨ��ǰ��Ԥ��  ��ֹ����Խ��
                m_stCPnt[t_i16LoopY].m_i16x = m_stLPnt[t_i16LoopY].m_i16x + (TrackWide[IMGH - 1] / 2 + 10);
                m_stCPnt[t_i16LoopY].m_i16y = t_i16LoopY;
            }

        }
        else if (m_stRPnt[t_i16LoopY].m_i16y && !m_stLPnt[t_i16LoopY].m_i16y)
        {
            if(t_i16LoopY < IMGH - 3)
            {
                //��һ�ε���ߺ��ұ߾�����
                if      (m_stLPnt[Last_LoopY].m_i16y && m_stRPnt[Last_LoopY].m_i16y)
                {
                    MapLth = ABS(m_stRPnt[Last_LoopY].m_i16x-m_stLPnt[Last_LoopY].m_i16x) / 2;
                    m_stCPnt[t_i16LoopY].m_i16x = m_stRPnt[t_i16LoopY].m_i16x - MapLth + 1;
                    m_stCPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                }
                else if (m_stRPnt[Last_LoopY].m_i16y)
                {
                    Curve = m_stRPnt[Last_LoopY].m_i16x - m_stRPnt[t_i16LoopY].m_i16x;

                    if      (m_i8RMonotonicity[t_i16LoopY] == 0 && Curve > -1 && Curve < 4)
                    {
                        //�����ǰ����������㵥����
                        //���ֺ��ұ�����ͬ������
                        m_stCPnt[t_i16LoopY].m_i16x = m_stCPnt[Last_LoopY].m_i16x - Curve;
                        m_stCPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                    }
                    else
                    {
                        //����������㵥����  ������ͻ��  �����
                        _Width = m_stRPnt[Last_LoopY].m_i16x - m_stCPnt[Last_LoopY].m_i16x;
                        _Similar = (float)(_Width)/ TrackWide[Last_LoopY];
                        _Result = (Int16)(TrackWide[t_i16LoopY] * _Similar);

                        m_stCPnt[t_i16LoopY].m_i16x = m_stRPnt[t_i16LoopY].m_i16x - _Result;
                        m_stCPnt[t_i16LoopY].m_i16y = t_i16LoopY;

                    }

                }
                else if(m_stLPnt[Last_LoopY].m_i16y)
                {
                    //��һ���ұ��� ֱ��ͨ�������
                    _Width = m_stCPnt[Last_LoopY].m_i16x - m_stLPnt[Last_LoopY].m_i16x;
                    _Similar = (float)(_Width)/ TrackWide[Last_LoopY];
                    _Result = (Int16)(TrackWide[t_i16LoopY] * _Similar);

                    m_stCPnt[t_i16LoopY].m_i16x = m_stRPnt[t_i16LoopY].m_i16x - _Result;
                    m_stCPnt[t_i16LoopY].m_i16y = t_i16LoopY;

                }
                else
                {
                    //�������ֱ��ͨ��ǰ������ʼ���

                    //���ܻ���ִ���ĵ� �����ڴ���
                    _Similar = (float)_CalculationEdge / TrackWide[_CalculationLine];
                    _Result = (Int16)(TrackWide[t_i16LoopY] * _Similar);

                    m_stCPnt[t_i16LoopY].m_i16x = m_stRPnt[t_i16LoopY].m_i16x - (_Result / 2);
                    m_stCPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                }
            }
            else
            {
                //�տ�ʼ����ͨ��ǰ��Ԥ��  ��ֹ����Խ��
                m_stCPnt[t_i16LoopY].m_i16x = m_stRPnt[t_i16LoopY].m_i16x - (TrackWide[IMGH - 1] / 2 + 10);
                m_stCPnt[t_i16LoopY].m_i16y = t_i16LoopY;
            }

        }
        else
        {
            if(t_i16LoopY < IMGH - 5)
            {
                if(m_stCPnt[Last_LoopY].m_i16x < 25)
                {
                    m_stCPnt[t_i16LoopY].m_i16x = m_stCPnt[t_i16LoopY + 1].m_i16x - 4;
                    m_stCPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                }
                else if(m_stCPnt[Last_LoopY].m_i16x > IMGW - 25)
                {
                    m_stCPnt[t_i16LoopY].m_i16x = m_stCPnt[t_i16LoopY + 1].m_i16x + 4;
                    m_stCPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                }
                else
                {
                    if(t_i16LoopY < 30)
                    {
                        Curve = m_stCPnt[40].m_i16x - m_stCPnt[55].m_i16x;
                        if(Curve > 0)
                        {
                            m_stCPnt[t_i16LoopY].m_i16x = m_stCPnt[t_i16LoopY + 1].m_i16x + 2;
                        }
                        else
                        {
                            m_stCPnt[t_i16LoopY].m_i16x = m_stCPnt[t_i16LoopY + 1].m_i16x - 2;
                        }
                        m_stCPnt[t_i16LoopY].m_i16y = t_i16LoopY;

                    }
                    else
                    {
                        Curve = m_stCPnt[t_i16LoopY + 1].m_i16x - m_stCPnt[t_i16LoopY + 2].m_i16x;
                        m_stCPnt[t_i16LoopY].m_i16x = m_stCPnt[t_i16LoopY + 1].m_i16x + Curve;
                        m_stCPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                    }

                    if(AT_IMAGE(&img_raw,m_stCPnt[t_i16LoopY].m_i16x, t_i16LoopY) == B_BLACK)
                    {
                        if(m_stOptimalPoint.m_i16x > IMGW / 2)
                        {
                            m_stCPnt[t_i16LoopY].m_i16x = IMGW - 2;
                        }
                        else
                        {
                            m_stCPnt[t_i16LoopY].m_i16x = 1;
                        }
                    }
                }

            }
            else
            {
                 m_stCPnt[t_i16LoopY].m_i16x = m_stOptimalPoint.m_i16x;
                 m_stCPnt[t_i16LoopY].m_i16y = t_i16LoopY;
            }
        }


        //���⴦��
        if(t_i16LoopY < 50)
        {
            if      (m_stLPnt[t_i16LoopY].m_i16y || m_stRPnt[t_i16LoopY].m_i16y)
            {
                if      (m_stCPnt[Last_LoopY].m_i16x < 10 && (m_stCPnt[Last_LoopY].m_i16x - m_stCPnt[t_i16LoopY].m_i16x) < 0)
                {
                    m_stCPnt[t_i16LoopY].m_i16x = m_stCPnt[Last_LoopY].m_i16x - 4;
                }
                else if(m_stCPnt[Last_LoopY].m_i16x > IMGW - 10 && (m_stCPnt[Last_LoopY].m_i16x - m_stCPnt[t_i16LoopY].m_i16x) > 0)
                {
                    m_stCPnt[t_i16LoopY].m_i16x = m_stCPnt[Last_LoopY].m_i16x + 4;
                }
            }
            else
            {
                if      (m_stCPnt[Last_LoopY].m_i16x < 25 && (m_stCPnt[Last_LoopY].m_i16x - m_stCPnt[t_i16LoopY].m_i16x) < 0)
                {
                    m_stCPnt[t_i16LoopY].m_i16x = m_stCPnt[Last_LoopY].m_i16x - 4;
                }
                else if(m_stCPnt[Last_LoopY ].m_i16x > IMGW - 25 && (m_stCPnt[Last_LoopY].m_i16x - m_stCPnt[t_i16LoopY].m_i16x) > 0)
                {
                    m_stCPnt[t_i16LoopY].m_i16x = m_stCPnt[Last_LoopY].m_i16x + 4;
                }
            }
        }

        //---------------�޷�����-----------------
        if (m_stCPnt[t_i16LoopY].m_i16x > IMGW - 2)
        {
            m_stCPnt[t_i16LoopY].m_i16x = IMGW - 2;
        }
        else if (m_stCPnt[t_i16LoopY].m_i16x < 1)
        {
            m_stCPnt[t_i16LoopY].m_i16x = 1;
        }
    }

    //---------------�������-----------------

    for(int i=IMGH-10;i>20;i--)
    {
        m_stCPnt[i].m_i16x=(m_stCPnt[i-1].m_i16x+
                m_stCPnt[i-2].m_i16x+
                m_stCPnt[i+1].m_i16x+
                m_stCPnt[i+2].m_i16x)>>2;
    }
 }

Int8    _Cammer_Prospect;
Int8    _Special_Prospect;
Int8    _Roundabout_Prospect;
Int8    _Trident_Prospect;

Uint8    _SpecialHandle_FixedDeviation;
float    _SpecialHandle_FixedError;
float    _OffestMeasure;
float    _OffestSimilar;
float    _OffestResulte;

void SolutionOffestCenterLine(void)
{
    Int16 t_i16LineMax; //ȡ���������
    Int16 t_i16LineMin; //ȡ�㿪ʼ����
    Int16 t_i16LoopY;   //ѭ��Y����
    Int8  t_i8Count = 0; //�ҵ��ĵ��

    /*����ѡ��8���������*/
    memset(m_stErPointBuffer, 0, sizeof(m_stErPointBuffer));

    if(_SpecialHandle_FixedDeviation)
    {
        m_f32LineAllError = _SpecialHandle_FixedError;
    }
    else
    {
        if(AllAreaWhitePoint < 300)
        {
            m_f32LineAllError = m_f32LineAllError*1; //������һ�ε����
        }
        else
        {
            /*����ʶ��ѡ��ǰհ*/
            if(_StatusMonitor._fpEventControl == _Event_Roundabout)
            {
                //SteerParam_RateKp = 0.12f;
//                ErrorSpeedRatio = 0.336;
                t_i16LineMin = _Roundabout_Prospect;
                t_i16LineMax = t_i16LineMin + 8;
            }
            else if(_StatusMonitor._fpEventControl == _Event_TridentRoad)
            {
                //SteerParam_RateKp = 0.12f;
//                ErrorSpeedRatio = 0.325;
                t_i16LineMin = _Trident_Prospect;
                t_i16LineMax = t_i16LineMin + 8;
            }
            else if(_StatusMonitor._fpEventControl == _Event_RampWay)
            {
                t_i16LineMin = _Special_Prospect;
                t_i16LineMax = t_i16LineMin + 8;
            }
            else
            {
                //SteerParam_RateKp = 0.12f;
//                ErrorSpeedRatio = 0.325;
                t_i16LineMin = _Cammer_Prospect;
                t_i16LineMax = t_i16LineMin + 8;
            }

            t_i16LoopY = t_i16LineMin - 1;

            while((++t_i16LoopY) < t_i16LineMax && t_i8Count < 6)
            {
                m_stErPointBuffer[t_i8Count] = m_stCPnt[t_i16LoopY];
                t_i8Count++;
            }

            m_f32LineAllError = 0.0;

            /*�����Ŵ�ͬһ��Χ*/
            t_i16LoopY = -1;

            while((++t_i16LoopY) < t_i8Count)
            {
                _OffestResulte = (float)m_stErPointBuffer[t_i16LoopY].m_i16x - 93;
                m_f32LineAllError += OffestWeight[t_i16LoopY] * _OffestResulte;
            }
        }
    }
}





 ST_2D_INT_POINT_INFO          GloballyOptimalPoint;
 ST_2D_INT_POINT_INFO          LeftOptimalPoint;
 ST_2D_INT_POINT_INFO          RightOptimalPoint;

 //ͼ���������
// Uint32                  LeftAreaWhitePoint;
// Uint32                  RightAreaWhitePoint;
Uint32                  AllAreaWhitePoint;
#define          LeftAreaWhitePoint         m_u32LAllArea
#define          RightAreaWhitePoint        m_u32RAllArea


 //ÿһ�еİ׵���
 //Uint8                   EachColumnWhitePoint[IMGW];
#define         EachColumnWhitePoint            m_u16LineBAr

Uint8           LineSetupFlag;


 void ImageHandle_OptimalPoint(Uint8 (*InImg)[IMGW])
 {
     Int16 t_i16LoopY;
     Int16 t_i16LoopX;
     Int16 t_i16CompeletCount = 0;

     ST_2D_INT_POINT_INFO  ReturnPoint = {0, 0};

     LeftAreaWhitePoint           = 0;
     RightAreaWhitePoint          = 0;
     AllAreaWhitePoint            = 0;

     GloballyOptimalPoint.m_i16x = 0;
     GloballyOptimalPoint.m_i16y = 0;

     LeftOptimalPoint.m_i16x     = 0;
     LeftOptimalPoint.m_i16y     = 0;

     RightOptimalPoint.m_i16x    = 0;
     RightOptimalPoint.m_i16y    = 0;

     memset(EachColumnWhitePoint, 0, sizeof(EachColumnWhitePoint));
     t_i16LoopX = IMGW;

     /*���ÿһ�еİ׵���*/
     while((--t_i16LoopX) > -1)
     {
         t_i16LoopY = IMGH;
         t_i16CompeletCount = 0;
         while((--t_i16LoopY) > -1)
         {
             if(B_WHITE == InImg[t_i16LoopY][t_i16LoopX])
             {
                (EachColumnWhitePoint[t_i16LoopX])++;
                 t_i16CompeletCount = 0;
             }
             else
             {
                 if(++t_i16CompeletCount > 4)
                     break;
             }
         }
     }

     t_i16LoopX = -1;

     while((++t_i16LoopX) < IMGW)
     {
         /*��벿��ͼ��*/
         if(t_i16LoopX <= IMGW / 2)
         {
             if(EachColumnWhitePoint[t_i16LoopX] > LeftOptimalPoint.m_i16y)
             {
                 LeftOptimalPoint.m_i16x = t_i16LoopX;
                 LeftOptimalPoint.m_i16y = EachColumnWhitePoint[t_i16LoopX];
             }

             LeftAreaWhitePoint += EachColumnWhitePoint[t_i16LoopX];
         }
         else
         {
             if(EachColumnWhitePoint[t_i16LoopX] > RightOptimalPoint.m_i16y)
             {
                 RightOptimalPoint.m_i16x = t_i16LoopX;
                 RightOptimalPoint.m_i16y = EachColumnWhitePoint[t_i16LoopX];
             }

             RightAreaWhitePoint += EachColumnWhitePoint[t_i16LoopX];
         }

         /*ȫ�������*/
         if(EachColumnWhitePoint[t_i16LoopX] > ReturnPoint.m_i16y)
         {
             ReturnPoint.m_i16x = t_i16LoopX;
             ReturnPoint.m_i16y = EachColumnWhitePoint[t_i16LoopX];
         }
     }

     AllAreaWhitePoint = LeftAreaWhitePoint + RightAreaWhitePoint;


     if(LeftOptimalPoint.m_i16y == 0 && RightOptimalPoint.m_i16y != 0)
     {
         GloballyOptimalPoint.m_i16x = (RightOptimalPoint.m_i16x + ReturnPoint.m_i16x) / 2;
         GloballyOptimalPoint.m_i16y = EachColumnWhitePoint[GloballyOptimalPoint.m_i16x];
     }
     else if(LeftOptimalPoint.m_i16y != 0 && RightOptimalPoint.m_i16y == 0)
     {
         GloballyOptimalPoint.m_i16x = (LeftOptimalPoint.m_i16x + ReturnPoint.m_i16x) / 2;
         GloballyOptimalPoint.m_i16y = EachColumnWhitePoint[GloballyOptimalPoint.m_i16x];
     }
     else if(LeftOptimalPoint.m_i16y != 0 && RightOptimalPoint.m_i16y != 0)
     {
         if(ABS(LeftOptimalPoint.m_i16x - RightOptimalPoint.m_i16x) > IMGW * 0.6)
         {
             if(LeftAreaWhitePoint - RightAreaWhitePoint > 3000)
             {
                 GloballyOptimalPoint = LeftOptimalPoint;
             }
             else if(RightAreaWhitePoint - LeftAreaWhitePoint > 3000)
             {
                 GloballyOptimalPoint = RightOptimalPoint;
             }
             else if(LeftAreaWhitePoint > RightAreaWhitePoint)
             {
                 GloballyOptimalPoint.m_i16x = (LeftOptimalPoint.m_i16x + ReturnPoint.m_i16x) / 2;
                 GloballyOptimalPoint.m_i16y = EachColumnWhitePoint[GloballyOptimalPoint.m_i16x];
             }
             else if(RightAreaWhitePoint > LeftAreaWhitePoint)
             {
                 GloballyOptimalPoint.m_i16x = (RightOptimalPoint.m_i16x + ReturnPoint.m_i16x) / 2;
                 GloballyOptimalPoint.m_i16y = EachColumnWhitePoint[GloballyOptimalPoint.m_i16x];
             }
             else if(RightAreaWhitePoint == 0 && LeftAreaWhitePoint == 0)
             {
                 AllAreaWhitePoint = 0;
             }
             else
             {
                 if(m_stOptimalPoint.m_i16x > IMGH / 2)
                 {
                     GloballyOptimalPoint = RightOptimalPoint;
                 }
                 else
                 {
                     GloballyOptimalPoint = LeftOptimalPoint;
                 }
             }
         }
         else if(ReturnPoint.m_i16y != 0 && ReturnPoint.m_i16x == LeftOptimalPoint.m_i16x)
         {
             //���Ϊ������
             GloballyOptimalPoint.m_i16x = LeftOptimalPoint.m_i16x;
             GloballyOptimalPoint.m_i16y = LeftOptimalPoint.m_i16y;
         }
         else if(ReturnPoint.m_i16y != 0 && ReturnPoint.m_i16x == RightOptimalPoint.m_i16x)
         {
             //�ұ�Ϊ������
             GloballyOptimalPoint.m_i16x = RightOptimalPoint.m_i16x;
             GloballyOptimalPoint.m_i16y = RightOptimalPoint.m_i16y;
         }
         else
         {
             /*��ֹ��������BUG*/
             GloballyOptimalPoint.m_i16x = (LeftOptimalPoint.m_i16x + RightOptimalPoint.m_i16x) / 2;
             GloballyOptimalPoint.m_i16y = EachColumnWhitePoint[GloballyOptimalPoint.m_i16x];
         }

     }
     else if(ReturnPoint.m_i16y)
     {
         GloballyOptimalPoint.m_i16x = ReturnPoint.m_i16x;
         GloballyOptimalPoint.m_i16y = ReturnPoint.m_i16y;
     }
     else
     {
         AllAreaWhitePoint = 0;
     }

     //�������ŵ�
     if(_StatusMonitor._fpEventControl == _Event_Crossroads)
     {
         if(m_i16LTopcornerFindFlag == 1 && m_i16RTopcornerFindFlag == 1)
         {
             m_stOptimalPoint.m_i16x = (LTOP.m_i16x + RTOP.m_i16x) / 2;
         }
         else if(m_i16LBotcornerFindFlag == 1 && m_i16RBotcornerFindFlag == 1)
         {
             m_stOptimalPoint.m_i16x = (LBOT.m_i16x + RBOT.m_i16x) / 2;
         }
         else if(ABS(GloballyOptimalPoint.m_i16x - m_stOptimalPoint.m_i16x) < 80)
         {
             m_stOptimalPoint = GloballyOptimalPoint;
         }
     }
     else if(EventStatusTask[_Event_TridentRoad]._EventCurrentStatus > _Task_TridentRoad_Runing)
     {
         if(_TridentDirectionFlag == _TridentRoad_Right)
         {
             m_stOptimalPoint = RightOptimalPoint;
         }
         else
         {
             m_stOptimalPoint = LeftOptimalPoint;
         }
     }
     else if(EventStatusTask[_Event_Roundabout]._EventCurrentStatus > _Task_RRoundabout_Enable)
             m_stOptimalPoint= RightOptimalPoint;
     else if(EventStatusTask[_Event_Roundabout]._EventCurrentStatus > _Task_LRoundabout_Enable)
             m_stOptimalPoint = LeftOptimalPoint;

     else if(_StatusMonitor._fpEventControl == _Event_WaitStop)
     {
         if(_SetupAngle < 0)
             m_stOptimalPoint = LeftOptimalPoint;
         else
             m_stOptimalPoint = RightOptimalPoint;
     }
     else if(ABS(GloballyOptimalPoint.m_i16x - m_stOptimalPoint.m_i16x) < 120 &&
             GloballyOptimalPoint.m_i16x > 10 && GloballyOptimalPoint.m_i16x < IMGW -10 &&
             AllAreaWhitePoint > 201 && GloballyOptimalPoint.m_i16y > 8)
             m_stOptimalPoint = GloballyOptimalPoint;

 }




Uint32 Pridect_With;

Uint8 t_u8LEdgeFindPoint = 0;
Uint8 t_u8REdgeFindPoint = 0;

Int16 _CammerWideMeasure;
Int16 _CammerEdgeMeasure;
Int16 _CammerWideSolution;
Int16 _CammerCurrentEdge;
Int16 _CammerCurrentLine;


void ImageHandle_EdgeBorde(Uint8 (*InImg)[IMGW])
{
    const Uint8 t_u8OptPntSetoffX = 6;
    Int16 t_i16LoopX = 0;

    Int16 t_i16LoopY = 0; //ѭ�������X
    Int16 t_i16Count = 0;

    Int16 Last_LoopY = 0;
    Int16 Next_LoopY = 0;

    Int16 t_i16RoopY = 0; //ѭ���ұ���X
    Int16 t_i16Number = 0;

    Int16 t_i16LStartX = 0;
    Int16 t_i16RStartX = 0;
    Int16 t_i16LEndX = 0;
    Int16 t_i16REndX = 0;

    Int16 LBlackPointNum = 0;
    Int16 RBlackPointNum = 0;

    Int16 LWhitePointNum = 0;
    Int16 RWhitePointNum = 0;

    Uint8 t_u8LFindFlag = 0;
    Uint8 t_u8RFindFlag = 0;

    /*�����־�ź�����ʹ����������Ѱ��*/
//    Uint8 t_u8LFinishFlag = 0;
//    Uint8 t_u8RFinishFlag = 0;
//
//    Int16 Curve;

    m_i16LPointCnt = 0;

    t_u8LEdgeFindPoint = IMGH - 2;
    t_u8REdgeFindPoint = IMGH - 2;

    if(ipts0_num != 0 || ipts1_num != 0)
    {
        t_i16Count = -1;
        t_i16LoopY = LLoopY;
        //�ȴ��������
        while((++t_i16Count) < ipts0_num && ipts0[t_i16Count][1] - t_i16LoopY < 1) //С���ҵ��ĵ���
        {
           if(ipts0[t_i16Count][1] != t_i16LoopY)
           {
               t_i16LoopY--;
               m_stLPnt[t_i16LoopY].m_i16x = ipts0[t_i16Count][0];
               m_stLPnt[t_i16LoopY].m_i16y = t_i16LoopY;
           }
           else
           {
               if(ipts0[t_i16Count][0] < m_stLPnt[t_i16LoopY].m_i16x)
               {
                   m_stLPnt[t_i16LoopY].m_i16x = ipts0[t_i16Count][0];
               }
           }
        }

        t_u8LEdgeFindPoint = m_stLPnt[t_i16LoopY].m_i16y;

        //����ұ���
        t_i16Number = -1;
        t_i16RoopY = RLoopY;
        while((++t_i16Number) < ipts1_num && ipts1[t_i16Number][1] - t_i16RoopY < 1) //С���ҵ��ĵ���
        {
           if(ipts1[t_i16Number][1] != t_i16RoopY)
           {
               t_i16RoopY--;
               m_stRPnt[t_i16RoopY].m_i16x = ipts1[t_i16Number][0];
               m_stRPnt[t_i16RoopY].m_i16y = t_i16RoopY;
           }
           else
           {
               if(ipts1[t_i16Number][0] > m_stRPnt[t_i16RoopY].m_i16x)
               {
                   m_stRPnt[t_i16RoopY].m_i16x = ipts1[t_i16Number][0];
               }
           }
        }

        t_u8REdgeFindPoint = m_stRPnt[t_i16RoopY].m_i16y;
    }

    /*�߽�������*/

//    if(m_stLPnt[t_i16LoopY].m_i16y && m_stRPnt[t_i16LoopY].m_i16y)
//    {
//        //���Ҿ��ҵ�
//        m_stTPnt[t_i16LoopY].m_i16x = (m_stLPnt[t_i16LoopY].m_i16x + m_stRPnt[t_i16LoopY].m_i16x) / 2;
//        m_stTPnt[t_i16LoopY].m_i16y = t_i16LoopY;
//
//        _CammerGeneralEdgeType[t_i16LoopY] = _GeneralEdge_Normal;
//        _CammerSpecialEdgeType[t_i16LoopY] = _SpecialEdge_Normal;
//    }
//    else if(m_stLPnt[t_i16LoopY].m_i16y && !m_stRPnt[t_i16LoopY].m_i16y)
//    {
//        m_stTPnt[t_i16LoopY].m_i16x = (m_stLPnt[t_i16LoopY].m_i16x + (TrackWide[IMGH - 1] / 2));
//        m_stTPnt[t_i16LoopY].m_i16y = t_i16LoopY;
//
//        _CammerGeneralEdgeType[t_i16LoopY] = _GeneralEdge_RLose;
//        _CammerSpecialEdgeType[t_i16LoopY] = _SpecialEdge_RLose;
//    }
//    else if(!m_stLPnt[t_i16LoopY].m_i16y && m_stRPnt[t_i16LoopY].m_i16y)
//    {
//        m_stTPnt[t_i16LoopY].m_i16x = (m_stRPnt[t_i16LoopY].m_i16x - (TrackWide[IMGH - 1] / 2));
//        m_stTPnt[t_i16LoopY].m_i16y = t_i16LoopY;
//
//        _CammerGeneralEdgeType[t_i16LoopY] = _GeneralEdge_LLose;
//        _CammerSpecialEdgeType[t_i16LoopY] = _SpecialEdge_LLose;
//    }
//    else
//    {
//        m_stTPnt[t_i16LoopY].m_i16x = m_stOptimalPoint.m_i16x;
//        m_stTPnt[t_i16LoopY].m_i16y = t_i16LoopY;
//
//        _CammerGeneralEdgeType[t_i16LoopY] = _GeneralEdge_ALose;
//        _CammerSpecialEdgeType[t_i16LoopY] = _SpecialEdge_ALose;
//    }
//
//    /*�����޷�*/
//    m_stTPnt[t_i16LoopY].m_i16x = m_stTPnt[t_i16LoopY].m_i16x > IMGW - 2 ? IMGW - 2 : m_stTPnt[t_i16LoopY].m_i16x;
//    m_stTPnt[t_i16LoopY].m_i16x = m_stTPnt[t_i16LoopY].m_i16x < 2 ? 2 : m_stTPnt[t_i16LoopY].m_i16x;
//
//
//    _CammerCurrentEdge = TrackWide[IMGH - 1];
//    _CammerCurrentLine = IMGH - 1;

    //�����Ϊ��ͨ����
    t_i16LoopY = 59;
    while((--t_i16LoopY) > -1)
    {
        Last_LoopY = t_i16LoopY + 1; //�ӿ������ٶ�

        if(m_stLPnt[t_i16LoopY].m_i16y > t_u8LEdgeFindPoint)
        {
            //��߽����
            m_i16LPointCnt++;
        }
        else //if(m_stTPnt[Last_LoopY].m_i16x > 10 && m_stTPnt[Last_LoopY].m_i16x < IMGW - 10)
        {
//            t_u8LFinishFlag = 0;

            LBlackPointNum = 0;
            LWhitePointNum = 0;

            //��߲�����ʱ    ������һ���е���Ѱ�ڰ������
            //�е������������
//            if(m_stTPnt[Last_LoopY].m_i16y)
//            {
//                t_i16LStartX = m_stTPnt[Last_LoopY].m_i16x + t_u8OptPntSetoffX;
//            }
//            else
//            {
                t_i16LStartX = m_stOptimalPoint.m_i16x + t_u8OptPntSetoffX;
//            }

            t_i16LStartX = ElementHandle_LimitEdge(t_i16LStartX, 2, IMGW - 2);

//            if(m_stLPnt[Last_LoopY].m_i16y)
//            {
//                t_i16LEndX = m_stLPnt[Last_LoopY].m_i16x - t_u8OptPntSetoffX;
//                t_i16LEndX = ElementHandle_LimitEdge(t_i16LEndX, 2, IMGW - 2);
//            }
//            else
//            {
                t_i16LEndX = 2;
//            }

            t_i16LoopX = t_i16LStartX;
            t_u8LFindFlag = 0;
            while((--t_i16LoopX) > t_i16LEndX)
            {
                if(
                        InImg[t_i16LoopY][t_i16LoopX + 1] == B_WHITE &&
                        InImg[t_i16LoopY][t_i16LoopX]     == B_BLACK &&
                        InImg[t_i16LoopY][t_i16LoopX - 1] == B_BLACK
                        )
                {
                    m_i16LPointCnt++;
                    m_stLPnt[t_i16LoopY].m_i16x = t_i16LoopX + 1;
                    m_stLPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                    t_u8LFindFlag = 1;
                    break;
                }
                else
                {

                    if(InImg[t_i16LoopY][t_i16LoopX] == B_BLACK)
                    {
                        LWhitePointNum = -1;
                        if(++LBlackPointNum > 8)
                        {
                            //���������Ϊ���Ǻ�ɫ�����޷��ҵ���
                            LBlackPointNum = -1;
                            break;
                        }
                    }
                    else if(m_stLPnt[Last_LoopY].m_i16x == 1 && m_stLPnt[Last_LoopY].m_i16y == 0 && LWhitePointNum != -1)
                    {
                        if(InImg[t_i16LoopY][t_i16LoopX] == B_WHITE)
                        {
                            if(++LWhitePointNum > TrackWide[t_i16LoopY])
                            {
                                m_stLPnt[t_i16LoopY].m_i16x = 1;
                                m_stLPnt[t_i16LoopY].m_i16y = 0;            //��ʧ�߽��
                                t_u8LFindFlag = 1;
                                break;
                            }
                        }
                    }
                    else
                    {
                        LBlackPointNum = 0;
                    }

                }
            }

            /*����ɨ�߶���*/
//            if(LBlackPointNum == -1 && InImg[t_i16LoopY][2] == B_WHITE)
//            {
//                /*Ϊ�˽�� û���ҵ��� �����а�ɫ������ ���ܺ����ɾ�� */
//                t_u8LFindFlag = 0;
//                LBlackPointNum = 0;
//                t_i16LoopX = 2;
//            }

            //û���ҵ��� �����д������Ӱ��
//            if(t_u8LFindFlag == 0 && LBlackPointNum != -1 && t_i16LoopX > 1)
//            {
//                //���¹涨��Χ �ٴ�Ѱ��
//                t_i16LStartX = m_stOptimalPoint.m_i16x + t_u8OptPntSetoffX;
//                t_i16LStartX = t_i16LStartX > IMGW - 2 ? IMGW - 2 : t_i16LStartX;
//                t_i16LEndX = 2;
//
//                t_i16LoopX = t_i16LStartX;
//
//                while((--t_i16LoopX) > t_i16LEndX)
//                {
//                    if(
//                            InImg[t_i16LoopY][t_i16LoopX + 1] == B_WHITE &&
//                            InImg[t_i16LoopY][t_i16LoopX]     == B_BLACK &&
//                            InImg[t_i16LoopY][t_i16LoopX - 1] == B_BLACK
//                            )
//                    {
//                        m_i16LPointCnt++;
//                        m_stLPnt[t_i16LoopY].m_i16x = t_i16LoopX + 1;
//                        m_stLPnt[t_i16LoopY].m_i16y = t_i16LoopY;
//                        t_u8LFindFlag = 1;
//                        break;
//                    }
//                }
//            }

            //������Ҳ���ж�
            if(t_u8LFindFlag == 0 && LBlackPointNum == -1 && InImg[t_i16LoopY][2] == B_BLACK)
            {
                m_stLPnt[t_i16LoopY].m_i16x = 0;            //Ϊ�ҵ���
                m_stLPnt[t_i16LoopY].m_i16y = 0;
            }
            else if(t_u8LFindFlag == 0 && LWhitePointNum == 0 && InImg[t_i16LoopY][t_i16LEndX] == B_WHITE)
            {
                m_stLPnt[t_i16LoopY].m_i16x = 1;
                m_stLPnt[t_i16LoopY].m_i16y = 0;            //��ʧ�߽��
            }
//            else if(t_u8LFindFlag == 0 && LBlackPointNum != -1 && InImg[t_i16LoopY][2] == B_WHITE)
//            {
////                t_u8LFinishFlag = 1;
//                m_stLPnt[t_i16LoopY].m_i16x = 1;
//                m_stLPnt[t_i16LoopY].m_i16y = 0;            //��ʧ�߽��
//            }

        }
//        else if(t_u8LFinishFlag == 1)
//        {
//            t_i16LStartX = m_stOptimalPoint.m_i16x + t_u8OptPntSetoffX;
//            t_i16LStartX = t_i16LStartX > IMGW - 2 ? IMGW - 2 : t_i16LStartX;
//            t_i16LEndX = 2;
//
//            t_i16LoopX = t_i16LStartX;
//            LWhitePointNum = 0;
//
//            while((--t_i16LoopX) > t_i16LEndX)
//            {
//                if(
//                        InImg[t_i16LoopY][t_i16LoopX + 1] == B_WHITE &&
//                        InImg[t_i16LoopY][t_i16LoopX]     == B_BLACK &&
//                        InImg[t_i16LoopY][t_i16LoopX - 1] == B_BLACK
//                        )
//                {
//                    m_i16LPointCnt++;
//                    m_stLPnt[t_i16LoopY].m_i16x = t_i16LoopX + 1;
//                    m_stLPnt[t_i16LoopY].m_i16y = t_i16LoopY;
//                    t_u8RFinishFlag = 0;
//                    break;
//                }
//                else
//                {
//                    if(InImg[t_i16LoopY][t_i16LoopX] == B_WHITE)
//                    {
//                        if(++LWhitePointNum > TrackWide[t_i16LoopY])
//                        {
//                            m_stLPnt[t_i16LoopY].m_i16x = 1;
//                            m_stLPnt[t_i16LoopY].m_i16y = 0;            //��ʧ�߽��
//                            break;
//                        }
//                    }
//                }
//            }
//        }

        //--------------------------------------------------------------------------
        //�ұ��ߴ���

        if(m_stRPnt[t_i16LoopY].m_i16y > t_u8REdgeFindPoint)
        {
            //�ұ߽����
            m_i16RPointCnt++;
        }
        else //if(m_stTPnt[Last_LoopY].m_i16x > 10 && m_stTPnt[Last_LoopY].m_i16x < IMGW - 10)
        {
//            t_u8RFinishFlag = 0;

            RBlackPointNum = 0;
            RWhitePointNum = 0;

//            if(m_stTPnt[Last_LoopY].m_i16y)
//            {
//                t_i16RStartX = m_stTPnt[Last_LoopY].m_i16x - t_u8OptPntSetoffX;
//            }
//            else
//            {
                t_i16RStartX = m_stOptimalPoint.m_i16x - t_u8OptPntSetoffX;
//            }

            t_i16RStartX = ElementHandle_LimitEdge(t_i16RStartX, 2, IMGW - 2);

//            if(m_stRPnt[Last_LoopY].m_i16y)
//            {
//                t_i16REndX = m_stRPnt[Last_LoopY].m_i16x + t_u8OptPntSetoffX;
//                t_i16REndX = ElementHandle_LimitEdge(t_i16REndX, 2, IMGW - 2);
//            }
//
//            else
//            {
                t_i16REndX = IMGW - 2;
//            }

            t_i16LoopX = t_i16RStartX;
            t_u8RFindFlag = 0;

            while((++t_i16LoopX) < t_i16REndX)
            {
                if (B_BLACK == InImg[t_i16LoopY][t_i16LoopX] &&
                    B_WHITE == InImg[t_i16LoopY][t_i16LoopX - 1] &&
                    B_BLACK == InImg[t_i16LoopY][t_i16LoopX + 1]) //�жϵ��׺ں�,�м�ڵ�Ϊ�߽�
                {
                    m_i16RPointCnt++;
                    m_stRPnt[t_i16LoopY].m_i16x = t_i16LoopX - 1; //��ֵ�߽�����,�ȸ�ֵ��X,�ٸ�ֵ��Y
                    m_stRPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                    t_u8RFindFlag = 1; //�򿪴���������ɱ�־λ
                    break;
                }
                else
                {

                    if(InImg[t_i16LoopY][t_i16LoopX] == B_BLACK)
                    {
                        RWhitePointNum = -1;
                        if(++RBlackPointNum > 8)
                        {
                            RBlackPointNum = -1;
                            break;
                        }
                    }
                    else if(m_stRPnt[Last_LoopY].m_i16x == IMGW - 2 && m_stRPnt[Last_LoopY].m_i16y == 0 && RWhitePointNum != -1)
                    {
                        if(InImg[t_i16LoopY][t_i16LoopX] == B_WHITE)
                        {
                            if(++RWhitePointNum > TrackWide[t_i16LoopY])
                            {
                                m_stRPnt[t_i16LoopY].m_i16x = IMGW - 2;
                                m_stRPnt[t_i16LoopY].m_i16y = 0;            //��ʧ�߽��
                                t_u8RFindFlag = 1;
                                break;
                            }
                        }
                    }
                    else
                    {
                        RBlackPointNum = 0;
                    }
                }
            }
//
//            if(RBlackPointNum == -1 && InImg[t_i16LoopY][IMGW - 2] == B_WHITE)
//            {
//                t_u8RFindFlag = 0;
//                RBlackPointNum = 0;
//                t_i16LoopX = IMGW - 2;
//            }
//
//            /*����Ѱ��*/
//            if(t_u8RFindFlag == 0 && RBlackPointNum != -1 && t_i16LoopX < IMGW - 2)
//            {
//                t_i16RStartX = m_stOptimalPoint.m_i16x - t_u8OptPntSetoffX;
//                t_i16REndX = IMGW - 2;
//
//                t_i16RStartX = t_i16RStartX < 1 ? 1 : t_i16RStartX;
//
//                t_i16LoopX = t_i16RStartX;
//
//                while((++t_i16LoopX) < t_i16REndX)
//                {
//                   if (B_BLACK == InImg[t_i16LoopY][t_i16LoopX]     &&
//                       B_WHITE == InImg[t_i16LoopY][t_i16LoopX - 1] &&
//                       B_BLACK == InImg[t_i16LoopY][t_i16LoopX + 1]) //�жϵ��׺ں�,�м�ڵ�Ϊ�߽�
//                   {
//                       m_i16RPointCnt++;
//                       m_stRPnt[t_i16LoopY].m_i16x = t_i16LoopX - 1; //��ֵ�߽�����,�ȸ�ֵ��X,�ٸ�ֵ��Y
//                       m_stRPnt[t_i16LoopY].m_i16y = t_i16LoopY;
//                       t_u8RFindFlag = 1; //�򿪴���������ɱ�־λ
//                       break;
//                   }
//                }
//            }

            if(t_u8RFindFlag == 0 && RBlackPointNum == -1 && InImg[t_i16LoopY][IMGW - 2] == B_BLACK)
            {
                m_stRPnt[t_i16LoopY].m_i16x = 0;            //Ϊ�ҵ���
                m_stRPnt[t_i16LoopY].m_i16y = 0;
            }
            else if(t_u8RFindFlag == 0 && RWhitePointNum == 0 && InImg[t_i16LoopY][t_i16REndX] == B_WHITE)
            {
                m_stRPnt[t_i16LoopY].m_i16x = IMGW - 2;
                m_stRPnt[t_i16LoopY].m_i16y = 0;            //��ʧ�߽��
            }
//            else if(t_u8RFindFlag == 0 && RBlackPointNum != -1 && InImg[t_i16LoopY][IMGW - 2] == B_WHITE)
//            {
////                t_u8RFinishFlag = 1;
//                m_stRPnt[t_i16LoopY].m_i16x = IMGW - 2;
//                m_stRPnt[t_i16LoopY].m_i16y = 0;            //��ʧ�߽��
//            }
        }
//        else if(t_u8RFinishFlag == 1)
//        {
//            //���ʱ��Ҫ����������
//            t_i16RStartX = m_stOptimalPoint.m_i16x - t_u8OptPntSetoffX;
//            t_i16REndX = IMGW - 2;
//
//            t_i16RStartX = t_i16RStartX < 1 ? 1 : t_i16RStartX;
//
//            t_i16LoopX = t_i16RStartX;
//            RWhitePointNum = 0;
//
//            while((++t_i16LoopX) < t_i16REndX)
//            {
//               if (B_BLACK == InImg[t_i16LoopY][t_i16LoopX]     &&
//                   B_WHITE == InImg[t_i16LoopY][t_i16LoopX - 1] &&
//                   B_BLACK == InImg[t_i16LoopY][t_i16LoopX + 1]) //�жϵ��׺ں�,�м�ڵ�Ϊ�߽�
//               {
//                   m_i16RPointCnt++;
//                   m_stRPnt[t_i16LoopY].m_i16x = t_i16LoopX - 1; //��ֵ�߽�����,�ȸ�ֵ��X,�ٸ�ֵ��Y
//                   m_stRPnt[t_i16LoopY].m_i16y = t_i16LoopY;
//                   t_u8RFinishFlag = 0; //�򿪴���������ɱ�־λ
//                   break;
//               }
//               else
//               {
//                   if(InImg[t_i16LoopY][t_i16LoopX] == B_WHITE)
//                   {
//                       if(++RWhitePointNum > TrackWide[t_i16LoopY])
//                       {
//                           m_stRPnt[t_i16LoopY].m_i16x = IMGW - 2;
//                           m_stRPnt[t_i16LoopY].m_i16y = 0;            //��ʧ�߽��
//                           break;
//                       }
//                   }
//               }
//            }
//        }
        //--------------------------------------------------------------------------
        /*�������Ƚ�����ֱ��
                         ��������ȱ�����һ�������У�֮���ٶ��߲����ص�����Ѱ�Ҷ��߱��ߵĻָ�
                         �У�����ָ��еĿ���������ֱ��������ȵı�ֵ���ٶ��������������
                         ֵ��ƽ����֪���ߣ���ϳ���Ҫ�����������ߡ�*/
        /*
        if(m_stLPnt[t_i16LoopY].m_i16y && m_stRPnt[t_i16LoopY].m_i16y)
        {
            //�����������
            m_stTPnt[t_i16LoopY].m_i16x = (m_stLPnt[t_i16LoopY].m_i16x + m_stRPnt[t_i16LoopY].m_i16x) / 2;
            m_stTPnt[t_i16LoopY].m_i16y = t_i16LoopY;

            _CammerCurrentEdge = (m_stRPnt[t_i16LoopY].m_i16x - m_stLPnt[t_i16LoopY].m_i16x);
        }
        else if(m_stLPnt[t_i16LoopY].m_i16y && !m_stRPnt[t_i16LoopY].m_i16y)
        {
            //��ߵ����
            if(m_stLPnt[Last_LoopY].m_i16y && m_stRPnt[Last_LoopY].m_i16y)
            {
                _CammerWideMeasure = m_stRPnt[Last_LoopY].m_i16x - m_stLPnt[Last_LoopY].m_i16x;
                _CammerSimilar = (float)_CammerWideMeasure / TrackWide[Last_LoopY];
                _CammerWideSolution = (Int16)(TrackWide[t_i16LoopY] * _CammerSimilar);

                m_stTPnt[t_i16LoopY].m_i16x = m_stLPnt[t_i16LoopY].m_i16x + (_CammerWideSolution / 2);
                m_stTPnt[t_i16LoopY].m_i16y = t_i16LoopY;
            }
            else if(m_stLPnt[Last_LoopY].m_i16y && !m_stRPnt[Last_LoopY].m_i16y)
            {
                _CammerEdgeMeasure = m_stLPnt[t_i16LoopY].m_i16x - m_stLPnt[Last_LoopY].m_i16x;
                if(_CammerEdgeMeasure > -1 && _CammerEdgeMeasure < 3)
                {
                    m_stTPnt[t_i16LoopY].m_i16x = m_stTPnt[Last_LoopY].m_i16x + _CammerEdgeMeasure;
                    m_stTPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                }
                else
                {
                    _CammerWideMeasure = m_stTPnt[Last_LoopY].m_i16x - m_stLPnt[Last_LoopY].m_i16x;
                    _CammerSimilar = (float)(_CammerWideMeasure)/ TrackWide[Last_LoopY];
                    _CammerWideSolution = (Int16)(TrackWide[t_i16LoopY] * _CammerSimilar);

                    m_stTPnt[t_i16LoopY].m_i16x = m_stLPnt[t_i16LoopY].m_i16x + _CammerWideSolution;
                    m_stTPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                }
            }
            else if(!m_stLPnt[Last_LoopY].m_i16y && m_stRPnt[Last_LoopY].m_i16y)
            {
                _CammerWideMeasure = m_stRPnt[Last_LoopY].m_i16x - m_stTPnt[Last_LoopY].m_i16x;
                _CammerSimilar = (float)(_CammerWideMeasure)/ TrackWide[Last_LoopY];
                _CammerWideSolution = (Int16)(TrackWide[t_i16LoopY] * _CammerSimilar);

                m_stTPnt[t_i16LoopY].m_i16x = m_stLPnt[t_i16LoopY].m_i16x + _CammerWideSolution;
                m_stTPnt[t_i16LoopY].m_i16y = t_i16LoopY;
            }
            else
            {

                _CammerSimilar = (float)_CammerCurrentEdge / TrackWide[_CammerCurrentLine];
                _CammerWideSolution = (Int16)(TrackWide[t_i16LoopY] * _CammerSimilar);

                m_stTPnt[t_i16LoopY].m_i16x = m_stLPnt[t_i16LoopY].m_i16x + (_CammerWideSolution / 2);
                m_stTPnt[t_i16LoopY].m_i16y = t_i16LoopY;
            }
        }
        else if(!m_stLPnt[t_i16LoopY].m_i16y && m_stRPnt[t_i16LoopY].m_i16y)
        {
            //�б߽����
            if(m_stLPnt[Last_LoopY].m_i16y && m_stRPnt[Last_LoopY].m_i16y)
            {
                _CammerWideMeasure = m_stRPnt[Last_LoopY].m_i16x - m_stLPnt[Last_LoopY].m_i16x;
                _CammerSimilar = (float)_CammerWideMeasure / TrackWide[t_i16LoopY + 1];
                _CammerWideSolution = (Int16)(TrackWide[t_i16LoopY] * _CammerSimilar);

                m_stTPnt[t_i16LoopY].m_i16x = (m_stRPnt[t_i16LoopY].m_i16x - (_CammerWideSolution / 2));
                m_stTPnt[t_i16LoopY].m_i16y = t_i16LoopY;
            }
            else if(!m_stLPnt[Last_LoopY].m_i16y && m_stRPnt[Last_LoopY].m_i16y)
            {
                _CammerEdgeMeasure = m_stRPnt[Last_LoopY].m_i16x - m_stRPnt[t_i16LoopY].m_i16x;
                if(_CammerEdgeMeasure > -1 && _CammerEdgeMeasure < 3)
                {
                    m_stTPnt[t_i16LoopY].m_i16x = m_stTPnt[Last_LoopY].m_i16x - _CammerEdgeMeasure;
                    m_stTPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                }
                else
                {
                    _CammerWideMeasure = m_stRPnt[Last_LoopY].m_i16x - m_stTPnt[Last_LoopY].m_i16x;
                    _CammerSimilar = (float)(_CammerWideMeasure)/ TrackWide[Last_LoopY];
                    _CammerWideSolution = (Int16)(TrackWide[t_i16LoopY] * _CammerSimilar);

                    m_stTPnt[t_i16LoopY].m_i16x = m_stRPnt[t_i16LoopY].m_i16x - _CammerWideSolution;
                    m_stTPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                }
            }
            else if(m_stLPnt[Last_LoopY].m_i16y && !m_stRPnt[Last_LoopY].m_i16y)
            {
                _CammerWideMeasure = m_stTPnt[Last_LoopY].m_i16x - m_stLPnt[Last_LoopY].m_i16x;
                _CammerSimilar = (float)(_CammerWideMeasure)/ TrackWide[Last_LoopY];
                _CammerWideSolution = (Int16)(TrackWide[t_i16LoopY] * _CammerSimilar);

                m_stTPnt[t_i16LoopY].m_i16x = m_stRPnt[t_i16LoopY].m_i16x - _CammerWideSolution;
                m_stTPnt[t_i16LoopY].m_i16y = t_i16LoopY;
            }
            else
            {


                _CammerSimilar = (float)_CammerCurrentEdge / TrackWide[_CammerCurrentLine];
                _CammerWideSolution = (Int16)(TrackWide[t_i16LoopY] * _CammerSimilar);

                m_stTPnt[t_i16LoopY].m_i16x = m_stRPnt[t_i16LoopY].m_i16x - (_CammerWideSolution / 2);
                m_stTPnt[t_i16LoopY].m_i16y = t_i16LoopY;
            }
        }
        else
        {
            //�������������
            if(t_i16LoopY > IMGH - 5)
            {
                m_stTPnt[t_i16LoopY].m_i16x = m_stOptimalPoint.m_i16x;
                m_stTPnt[t_i16LoopY].m_i16y = t_i16LoopY;
            }
            else
            {
                 if(InImg[t_i16LoopY][2] == B_WHITE && InImg[t_i16LoopY][IMGW - 2] == B_WHITE)
                 {
                     m_stTPnt[t_i16LoopY].m_i16x = m_stOptimalPoint.m_i16x;
                     m_stTPnt[t_i16LoopY].m_i16y = 0;
                 }
                else if(m_stTPnt[Last_LoopY].m_i16x < 20)
                 {
                     m_stTPnt[t_i16LoopY].m_i16x = m_stTPnt[t_i16LoopY + 1].m_i16x - 2;
                 }
                 else if(m_stTPnt[Last_LoopY].m_i16x > IMGW - 20)
                 {
//                     Curve = m_stTPnt[t_i16LoopY + 1].m_i16x - m_stTPnt[t_i16LoopY + 2].m_i16x;
                     m_stTPnt[t_i16LoopY].m_i16x = m_stTPnt[t_i16LoopY + 1].m_i16x + 2;
                 }

                 else if(t_i16LoopY < IMGH / 2)
                 {
                     //ע������Ҫ�޸�
                    Curve = m_stOptimalPoint.m_i16x - m_stTPnt[IMGH - 2].m_i16x;
                    if(Curve > 0)
                    {
//                        Curve = m_stTPnt[t_i16LoopY + 1].m_i16x - m_stTPnt[t_i16LoopY + 3].m_i16x;
                        m_stTPnt[t_i16LoopY].m_i16x = m_stTPnt[t_i16LoopY + 1].m_i16x + 2;
                    }
                    else
                    {
//                        Curve = m_stTPnt[t_i16LoopY + 1].m_i16x - m_stTPnt[t_i16LoopY + 3].m_i16x;
                        m_stTPnt[t_i16LoopY].m_i16x = m_stTPnt[t_i16LoopY + 1].m_i16x - 2;
                    }
                    m_stTPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                 }
                 else
                 {
                     Curve = m_stTPnt[t_i16LoopY + 1].m_i16x - m_stTPnt[t_i16LoopY + 2].m_i16x;

                     Curve = LIMIT(Curve,-3,3);
                     m_stTPnt[t_i16LoopY].m_i16x = m_stTPnt[t_i16LoopY + 1].m_i16x + Curve;
                     m_stTPnt[t_i16LoopY].m_i16y = t_i16LoopY;
                 }

            }
        }

        m_stTPnt[t_i16LoopY].m_i16x = ElementHandle_LimitEdge(m_stTPnt[t_i16LoopY].m_i16x, 2 , IMGW - 2);

        if(t_i16LoopY < IMGH / 2 && ABS(m_stTPnt[t_i16LoopY].m_i16x - m_stOptimalPoint.m_i16x) > TrackWide[t_i16LoopY] && InImg[t_i16LoopY][m_stOptimalPoint.m_i16x] == B_WHITE)
        {
            m_stTPnt[t_i16LoopY].m_i16x = m_stOptimalPoint.m_i16x;
            m_stTPnt[t_i16LoopY].m_i16y = t_i16LoopY;
        }

        */
        //--------------------------------------------------------------------------
        //������ ��ֱͨ�Ӳ������� ���� �Ƚ�����һ���й�
//        if(m_stLPnt[t_i16LoopY].m_i16y && m_stRPnt[t_i16LoopY].m_i16y)
//        {
//            _CammerGeneralEdgeType[t_i16LoopY] = _GeneralEdge_Normal;
//
//            /*����ֵ������ ��һ�εı߽������*/
//            if(m_stLPnt[Last_LoopY].m_i16y && m_stRPnt[Last_LoopY].m_i16y)
//            {
//                if(m_stLPnt[t_i16LoopY].m_i16x >= m_stLPnt[Last_LoopY].m_i16x &&
//                   m_stRPnt[t_i16LoopY].m_i16x <= m_stRPnt[Last_LoopY].m_i16x )
//                {
//                    _CammerSpecialEdgeType[t_i16LoopY] = _SpecialEdge_Normal;
//                }
//                else if(m_stLPnt[t_i16LoopY].m_i16x < m_stLPnt[Last_LoopY].m_i16x &&
//                        m_stRPnt[t_i16LoopY].m_i16x <= m_stRPnt[Last_LoopY].m_i16x)
//                {
//                    //���Υ��������
//                    _CammerSpecialEdgeType[t_i16LoopY] = _SpecialEdge_LDrab;
//                }
//                else if(m_stLPnt[t_i16LoopY].m_i16x >= m_stLPnt[Last_LoopY].m_i16x &&
//                        m_stRPnt[t_i16LoopY].m_i16x >  m_stRPnt[Last_LoopY].m_i16x )
//                {
//                    //�ұ�Υ��������
//                    _CammerSpecialEdgeType[t_i16LoopY] = _SpecialEdge_RDrab;
//                }
//                else
//                {
//                    //���Ҷ�Υ��������
//                    _CammerSpecialEdgeType[t_i16LoopY] = _SpecialEdge_ADrab;
//                }
//            }
//            else if(!m_stLPnt[Last_LoopY].m_i16y && m_stRPnt[Last_LoopY].m_i16y)
//            {
//                _CammerSpecialEdgeType[t_i16LoopY] = _SpecialEdge_LRnew;
//            }
//            else if(m_stLPnt[Last_LoopY].m_i16y && !m_stRPnt[Last_LoopY].m_i16y)
//            {
//                _CammerSpecialEdgeType[t_i16LoopY] = _SpecialEdge_RRnew;
//            }
//            else
//            {
//                _CammerSpecialEdgeType[t_i16LoopY] = _SpecialEdge_ARnew;
//            }
//
//        }
//        else if(!m_stLPnt[t_i16LoopY].m_i16y && m_stRPnt[t_i16LoopY].m_i16y)
//        {
//            _CammerGeneralEdgeType[t_i16LoopY] = _GeneralEdge_LLose;
//
//            if(m_stRPnt[Last_LoopY].m_i16y)
//            {
//                _CammerSpecialEdgeType[t_i16LoopY] = _SpecialEdge_LLose;
//            }
//            else
//            {
//                _CammerSpecialEdgeType[t_i16LoopY] = _SpecialEdge_ALose;
//            }
//        }
//        else if(m_stLPnt[t_i16LoopY].m_i16y && !m_stRPnt[t_i16LoopY].m_i16y)
//        {
//            _CammerGeneralEdgeType[t_i16LoopY] = _GeneralEdge_RLose;
//
//            if(m_stLPnt[Last_LoopY].m_i16y)
//            {
//                _CammerSpecialEdgeType[t_i16LoopY] = _SpecialEdge_RLose;
//            }
//            else
//            {
//                _CammerSpecialEdgeType[t_i16LoopY] = _SpecialEdge_ALose;
//            }
//        }
//        else
//        {
//            _CammerGeneralEdgeType[t_i16LoopY] = _GeneralEdge_ALose;
//            _CammerSpecialEdgeType[t_i16LoopY] = _SpecialEdge_ALose;
//        }
    }

    //�߽�������ж�
    /*���Ͷ��� -1 ������ 0���� 1�۵� 2Υ�������� ���ڹյ�*/


    t_i16LoopY = IMGH - 1;
    while((--t_i16LoopY) > 1)
    {
        Next_LoopY = t_i16LoopY - 2;
        Last_LoopY = t_i16LoopY + 2;

        if(m_stLPnt[t_i16LoopY].m_i16y)
        {
            if(m_stLPnt[t_i16LoopY].m_i16x < 5 || m_stLPnt[t_i16LoopY].m_i16x > IMGW - 5)
            {
                m_i8LMonotonicity[t_i16LoopY] = -1;
            }
            else if(m_stLPnt[Last_LoopY].m_i16y && m_stLPnt[Next_LoopY].m_i16y)
            {
                if(
                        m_stLPnt[Next_LoopY].m_i16x >= m_stLPnt[t_i16LoopY].m_i16x &&
                        m_stLPnt[t_i16LoopY].m_i16x > m_stLPnt[Last_LoopY].m_i16x
                  )
                {
                    m_i8LMonotonicity[t_i16LoopY] = 0;
                }
                else if(
                        m_stLPnt[Next_LoopY].m_i16x < m_stLPnt[t_i16LoopY].m_i16x &&
                        m_stLPnt[t_i16LoopY].m_i16x > m_stLPnt[Last_LoopY].m_i16x
                        )
                {
                    m_i8LMonotonicity[t_i16LoopY] = 1;
                }
                else if(
                        m_stLPnt[Next_LoopY].m_i16x < m_stLPnt[t_i16LoopY].m_i16x &&
                        m_stLPnt[t_i16LoopY].m_i16x < m_stLPnt[Last_LoopY].m_i16x
                        )
                {
                    m_i8LMonotonicity[t_i16LoopY] = 2;
                }
                else
                {
                    m_i8LMonotonicity[t_i16LoopY] = -1;
                }
            }
            else if(m_stLPnt[Last_LoopY].m_i16y)
            {
                if(m_stLPnt[t_i16LoopY].m_i16x > m_stLPnt[Last_LoopY].m_i16x)
                {
                    m_i8LMonotonicity[t_i16LoopY] = 1;
                }
                else
                {
                    m_i8LMonotonicity[t_i16LoopY] = 2;
                }
            }
            else if(m_stLPnt[Next_LoopY].m_i16y)
            {
                if(m_stLPnt[t_i16LoopY].m_i16x < m_stLPnt[Next_LoopY].m_i16x)
                {
                    m_i8LMonotonicity[t_i16LoopY] = 0;
                }
                else
                {
                    m_i8LMonotonicity[t_i16LoopY] = 2;
                }
            }
            else
            {
                m_i8LMonotonicity[t_i16LoopY] = -1;
            }
        }
        else
        {
            //���ε��޷��ҵ�
            m_i8LMonotonicity[t_i16LoopY] = -1;
        }

        if(m_stRPnt[t_i16LoopY].m_i16y)
        {

            if(m_stRPnt[t_i16LoopY].m_i16x < 5 || m_stRPnt[t_i16LoopY].m_i16x > IMGW - 5)
            {
                m_i8RMonotonicity[t_i16LoopY] = -1;
            }
            else if(m_stRPnt[Last_LoopY].m_i16y && m_stRPnt[Next_LoopY].m_i16y)
            {
                if(
                        m_stRPnt[t_i16LoopY].m_i16x >= m_stRPnt[Next_LoopY].m_i16x &&
                        m_stRPnt[t_i16LoopY].m_i16x <  m_stRPnt[Last_LoopY].m_i16x
                        )
                {
                    m_i8RMonotonicity[t_i16LoopY] = 0;
                }
                else if(
                        m_stRPnt[t_i16LoopY].m_i16x < m_stRPnt[Next_LoopY].m_i16x &&
                        m_stRPnt[t_i16LoopY].m_i16x < m_stRPnt[Last_LoopY].m_i16x
                        )
                {
                    m_i8RMonotonicity[t_i16LoopY] = 1;
                }
                else if(
                        m_stRPnt[t_i16LoopY].m_i16x < m_stRPnt[Next_LoopY].m_i16x &&
                        m_stRPnt[t_i16LoopY].m_i16x > m_stRPnt[Last_LoopY].m_i16x
                        )
                {
                    m_i8RMonotonicity[t_i16LoopY] = 2;
                }
                else
                {
                    m_i8RMonotonicity[t_i16LoopY] = -1;
                }
            }
            else if(m_stRPnt[Last_LoopY].m_i16y)
            {
                if(m_stRPnt[t_i16LoopY].m_i16x < m_stRPnt[Last_LoopY].m_i16x)
                {
                    m_i8RMonotonicity[t_i16LoopY] = 1;
                }
                else
                {
                    m_i8RMonotonicity[t_i16LoopY] = 2;
                }
            }
            else if(m_stRPnt[Next_LoopY].m_i16y)
            {
                if(m_stRPnt[t_i16LoopY].m_i16x > m_stRPnt[Next_LoopY].m_i16x)
                {
                    m_i8RMonotonicity[t_i16LoopY] = 0;
                }
                else
                {
                    m_i8RMonotonicity[t_i16LoopY] = 2;
                }
            }
            else
            {
                m_i8RMonotonicity[t_i16LoopY] = -1;
            }
        }
        else
        {
            m_i8RMonotonicity[t_i16LoopY] = -1;
        }
    }
}


