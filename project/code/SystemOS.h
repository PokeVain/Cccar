#ifndef __SystemOS_H_
#define __SystemOS_H_

/*****************************
 *
 *          �ײ�����
 *
 ****************************/
#include "zf_common_headfile.h"

/*****************************
 *
 *          ͼ����
 *
 ****************************/

#include "ImageProcess.h"
#include "Perspective.h"
#include "ElementProcess.h"
#include "FeatureProcess.h"

#include "CrossRoad.h"
#include "CrossWalk.h"
#include "TridentRoad.h"
#include "Roundabout.h"


/*****************************
 *
 *         �㷨����
 *
 ****************************/
#include "LeastSquare.h"

/*****************************
 *
 *          ���账��
 *
 ****************************/
#include "DSPFunction.h"
#include "GUIControl.h"
#include "HMIControl.h"



/*****************************
 *
 *          ���ƺ���
 *
 ****************************/
#include "TaskFunction.h"

#include "SchemeFunction.h"
/*****************************
 *
 *          �ⲿ����
 *
 ****************************/
extern uint8 TheEdge;
extern uint8 Threshold, Threshold_Limt;
extern uint8 mt9v03x_output_image[IMGH][IMGW];

//�û�����
void RealTimeOS_OperationInital(void);
void RealTimeOS_OperationSystem(void);

//�ײ㺯��
void Parameter_StareDispose(void);
#endif


