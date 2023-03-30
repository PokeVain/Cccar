




#ifndef __FeatureProcess_H_
#define __FeatureProcess_H_


#include "SystemOS.h"

#define Image_Size188           0

/*�ⲿ��������*/
extern ST_2D_INT_POINT_INFO m_stLPnt[IMGH];         /*��߱�������*/
extern ST_2D_INT_POINT_INFO m_stRPnt[IMGH];         /*�ұ߱�������*/
extern ST_2D_INT_POINT_INFO m_stCPnt[IMGH];         /*��������*/
extern ST_2D_INT_POINT_INFO m_stTPnt[IMGH];         /*ɨ������*/

extern ST_2D_INT_POINT_INFO m_stOptimalPoint;       /*ʵ����������ڵ����ŵ�*/

extern ST_2D_INT_POINT_INFO m_stLOptimalPoint;      /*ʵ��������������ڵ����ŵ�*/
extern ST_2D_INT_POINT_INFO m_stROptimalPoint;      /*ʵ���Ұ����������ڵ����ŵ�*/

extern F32 m_f32LineAllError;

extern Int8 _Cammer_Prospect;

extern Uint32 m_u32LAllArea;                        /*��߰�ɫ�������*/
extern Uint32 m_u32RAllArea;                        /*�ұ߰�ɫ�������*/

extern Int8 m_i8LMonotonicity[IMGH];                /*��ߵ����� (����:���߽׶�������������Ҫ�Ƿ���Ҫ����,���߲��ߵ�����,���)*/
extern Int8 m_i8LMonotonicityCnt;                   /*Υ������Եĵ���*/
extern Int8 m_i8RMonotonicity[IMGH];                /*�ұߵ����� (����:ͬ��,�����ұ�)*/
extern Int8 m_i8RMonotonicityCnt;                   /*Υ������Եĵ���*/

extern Int16 m_i16LPointCnt;                        /*����ҵ�����*/
extern Int16 m_i16RPointCnt;                        /*�ұ��ҵ�����*/

extern Uint16 m_u16LineBAr[IMGW];                   /*��¼ÿ�дӵײ����ϵİ׵�������*/

extern Int16 m_i16LEndNum;                          /*����ҵ�����*/
extern Int16 m_i16REndNum;                          /*�ұ��ҵ�����*/
extern Int16 m_i16LLostPointCnt;                    /*��߶�ʧ������*/
extern Int16 m_i16RLostPointCnt;                    /*�ұ߶�ʧ������*/


extern Int16 TrackWide[IMGH];

extern Uint8 Left_With;
extern Uint8 Right_With;

extern Uint8 LineSetupFlag;
extern Uint32 AllAreaWhitePoint;                  /*ȫ�ְ׵����*/


extern Int8    _Special_Prospect;
extern Int8    _Roundabout_Prospect;
extern Int8    _Trident_Prospect;


extern Uint8    _SpecialHandle_FixedDeviation;
extern float    _SpecialHandle_FixedError;

/*����Ч��־λ��
 *�����������жϣ�
 * 0��ʾ�����ұ߽�����ڣ�
 * 1��ʾ��߽�ȱʧ��
 * 2��ʾ���ұ߽�ȱʧ��
 * 3��ʾ�����ұ߽��ȱʧ��
 * -1��ʾ������Ч
   �����������жϣ�
 * 0��ʾ˫�߽�����ڣ�
 * 1��ʾ��߽�ֻ�ǿ������˵�����ͻ��ȱʧ��
 * 2��ʾ�б߽翴�����˵�����ͻ��ȱʧ��
 * 3��ʾ˫�߽綼û�У����м䶼��������һ������ʮ��·���ж�
 * 4��ʾ����ķ�ɢ���ͣ�
 * 5��ʾ��߽磨�����������߽磩ͻ��ȱʧ��
 * 6��ʾ�ұ߽磨����������ұ߽磩ͻ��ȱʧ��
 * 7��ʾ��߽�����ͻ�䣬
 * 8��ʾ�ұ߽�����ͻ�䣬
 * -1��ʾ������Ч������7��8ֻ�ڻ�����ʹ�ã�
*/

typedef enum _GeneralEdgeStatus
{

    _GeneralEdge_Null = -1,
    _GeneralEdge_Normal,
    _GeneralEdge_LLose,
    _GeneralEdge_RLose,
    _GeneralEdge_ALose

}___GeneralEdgeStatus_Enum;

typedef enum _SpecialEdgeStatus
{
    _SpecialEdge_Null = -1,
    _SpecialEdge_Normal,

    /*��ʧ��*/
    _SpecialEdge_LLose,
    _SpecialEdge_RLose,
    _SpecialEdge_ALose,

    /*Υ�������Եĵ�*/
    _SpecialEdge_LDrab,
    _SpecialEdge_RDrab,
    _SpecialEdge_ADrab,

    /*�ָ���*/
    _SpecialEdge_LRnew,
    _SpecialEdge_RRnew,
    _SpecialEdge_ARnew,
}__SpecialEdgeStatus_Enum;


void GetLineError(void);

//��ֵ������
void ElementHandle_UltraWide(void); //�������

Int16 ElementHandle_LimitEdge(Int16 Variable, Int16 LimitMin, Int16 LimitMax);

char SLine(float *x,float *y,Uint8 n,float *a0,float *a1,float *a2);
void CalculationCenterLine(void);

void SolutionOffestCenterLine(void);

void ImageHandle_OptimalPoint(Uint8 (*InImg)[IMGW]);
void ImageHandle_EdgeBorde(Uint8 (*InImg)[IMGW]);


#endif
