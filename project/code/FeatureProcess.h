




#ifndef __FeatureProcess_H_
#define __FeatureProcess_H_


#include "SystemOS.h"

#define Image_Size188           0

/*外部调用数据*/
extern ST_2D_INT_POINT_INFO m_stLPnt[IMGH];         /*左边边线数组*/
extern ST_2D_INT_POINT_INFO m_stRPnt[IMGH];         /*右边边线数组*/
extern ST_2D_INT_POINT_INFO m_stCPnt[IMGH];         /*中线数组*/
extern ST_2D_INT_POINT_INFO m_stTPnt[IMGH];         /*扫线数组*/

extern ST_2D_INT_POINT_INFO m_stOptimalPoint;       /*实际最大列所在的最优点*/

extern ST_2D_INT_POINT_INFO m_stLOptimalPoint;      /*实际左半边最大列所在的最优点*/
extern ST_2D_INT_POINT_INFO m_stROptimalPoint;      /*实际右半边最大列所在的最优点*/

extern F32 m_f32LineAllError;

extern Int8 _Cammer_Prospect;

extern Uint32 m_u32LAllArea;                        /*左边白色点总面积*/
extern Uint32 m_u32RAllArea;                        /*右边白色点总面积*/

extern Int8 m_i8LMonotonicity[IMGH];                /*左边单调性 (复用:找线阶段用它来描述需要是否需要补线,或者补线的类型,左边)*/
extern Int8 m_i8LMonotonicityCnt;                   /*违反点调性的点数*/
extern Int8 m_i8RMonotonicity[IMGH];                /*右边单调性 (复用:同上,描述右边)*/
extern Int8 m_i8RMonotonicityCnt;                   /*违反点调性的点数*/

extern Int16 m_i16LPointCnt;                        /*左边找到点数*/
extern Int16 m_i16RPointCnt;                        /*右边找到点数*/

extern Uint16 m_u16LineBAr[IMGW];                   /*记录每列从底部向上的白点数数组*/

extern Int16 m_i16LEndNum;                          /*左边找到点数*/
extern Int16 m_i16REndNum;                          /*右边找到点数*/
extern Int16 m_i16LLostPointCnt;                    /*左边丢失点总数*/
extern Int16 m_i16RLostPointCnt;                    /*右边丢失点总数*/


extern Int16 TrackWide[IMGH];

extern Uint8 Left_With;
extern Uint8 Right_With;

extern Uint8 LineSetupFlag;
extern Uint32 AllAreaWhitePoint;                  /*全局白点面积*/


extern Int8    _Special_Prospect;
extern Int8    _Roundabout_Prospect;
extern Int8    _Trident_Prospect;


extern Uint8    _SpecialHandle_FixedDeviation;
extern float    _SpecialHandle_FixedError;

/*行有效标志位，
 *（初步类型判断）
 * 0表示行左右边界均存在，
 * 1表示左边界缺失，
 * 2表示行右边界缺失，
 * 3表示行左右边界均缺失，
 * -1表示该行无效
   （完整类型判断）
 * 0表示双边界均存在，
 * 1表示左边界只是看不见了但不是突变缺失，
 * 2表示有边界看不见了但不是突变缺失，
 * 3表示双边界都没有，但中间都是赛道，一般用于十字路口判断
 * 4表示三岔的发散类型，
 * 5表示左边界（相对于正常左边界）突变缺失，
 * 6表示右边界（相对于正常右边界）突变缺失，
 * 7表示左边界向内突变，
 * 8表示右边界向内突变，
 * -1表示该行无效（其中7和8只在环岛内使用）
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

    /*丢失点*/
    _SpecialEdge_LLose,
    _SpecialEdge_RLose,
    _SpecialEdge_ALose,

    /*违反单调性的点*/
    _SpecialEdge_LDrab,
    _SpecialEdge_RDrab,
    _SpecialEdge_ADrab,

    /*恢复点*/
    _SpecialEdge_LRnew,
    _SpecialEdge_RRnew,
    _SpecialEdge_ARnew,
}__SpecialEdgeStatus_Enum;


void GetLineError(void);

//二值化解算
void ElementHandle_UltraWide(void); //赛道宽度

Int16 ElementHandle_LimitEdge(Int16 Variable, Int16 LimitMin, Int16 LimitMax);

char SLine(float *x,float *y,Uint8 n,float *a0,float *a1,float *a2);
void CalculationCenterLine(void);

void SolutionOffestCenterLine(void);

void ImageHandle_OptimalPoint(Uint8 (*InImg)[IMGW]);
void ImageHandle_EdgeBorde(Uint8 (*InImg)[IMGW]);


#endif
