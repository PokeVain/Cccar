



#ifndef __ElementProcess_H_
#define __ElementProcess_H_

#include "SystemOS.h"


/*������ȡ�յ�*/
extern ST_2D_INT_POINT_INFO LTOP;
extern ST_2D_INT_POINT_INFO RTOP;
extern ST_2D_INT_POINT_INFO LBOT;
extern ST_2D_INT_POINT_INFO RBOT;


/*��־��*/
extern Int8 m_i16LTopcornerFindFlag;                                 /*���Ͻǵ���Ѱ��־*/
extern Int8 m_i16LBotcornerFindFlag;                                 /*���½ǵ���Ѱ��־*/
extern Int8 m_i16RTopcornerFindFlag;                                 /*���Ͻǵ���Ѱ��־*/
extern Int8 m_i16RBotcornerFindFlag;                                 /*���½ǵ���Ѱ��־*/

/*��������*/
extern Int16 m_i16OutWidthROWNum;                                  /*������*/
extern Int16 m_i16WOutWidthROWNum;                                 /*�ǳ����صĳ���״̬*/
extern Int16 m_i16AbsoluteOutWidth;

extern ST_2D_INT_POINT_INFO ChamferCorner;
extern ST_2D_INT_POINT_INFO EndTopofLine;


extern uint8_t  mt9v03x_output_image[IMGH][IMGW];

extern float KSlope_Left, KSlope_Right;

extern Int8 m_i16CorneerPointNumber;



extern float LBotTariangleCosCornor;
extern float RBotTariangleCosCornor;


extern float _StopSpeed;
void SpeicalPoint(void);

void DropLine_ProtectFunction(void);

void ElementInital_UltraWideParam(void);
void ElementHandle_JudgeTriangle(void);
void ElementHandle_SpecialPoint(void);
void ElementHandle_SegmentCurve(void);
void ElementHandle_NormalStatus(void);
void ElementHandle_SpecialStatus(void);
void SpecialTopCorner_TaskFunction(void);


#endif
