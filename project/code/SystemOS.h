#ifndef __SystemOS_H_
#define __SystemOS_H_

/*****************************
 *
 *          底层配置
 *
 ****************************/
#include "zf_common_headfile.h"

/*****************************
 *
 *          图像处理
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
 *         算法控制
 *
 ****************************/
#include "LeastSquare.h"

/*****************************
 *
 *          外设处理
 *
 ****************************/
#include "DSPFunction.h"
#include "GUIControl.h"
#include "HMIControl.h"



/*****************************
 *
 *          控制函数
 *
 ****************************/
#include "TaskFunction.h"

#include "SchemeFunction.h"
/*****************************
 *
 *          外部调用
 *
 ****************************/
extern uint8 TheEdge;
extern uint8 Threshold, Threshold_Limt;
extern uint8 mt9v03x_output_image[IMGH][IMGW];

//用户函数
void RealTimeOS_OperationInital(void);
void RealTimeOS_OperationSystem(void);

//底层函数
void Parameter_StareDispose(void);
#endif


