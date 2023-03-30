




#ifndef __SchemeFunction_H_
#define __SchemeFunction_H_



#include "SystemOS.h"

//extern float _StopPiont;


void DefaultScheme_SchemeFunction   (void);
void ProgrammeB_SchemeFunction      (void);

void PlanBStatus_TaskFunction       (void);
void PlanBEvent_TaskFunction        (void);

void ProgrammeC_SchemeFunction      (void);
void PlanCStatus_TaskFunction       (void);
void PlanCEvent_TaskFunction        (void);

void DefaultScheme_ParamFunction(void);


void LRoundabout_ElementLock(void);
void RRoundabout_ElementLock(void);
void CrossRoad_ElementLock  (void);
void CrossWalk_ElementLock  (void);
void TridentRoad_ElementLock(void);
void RampWay_ElementLock    (void);
void NullEvent_ElementLock  (void);

#endif

















