#ifndef __CrossWalk_H_
#define __CrossWalk_H_

#include "SystemOS.h"

typedef enum __CrossWalkStatus
{
    _CrossWalk_Left = 0,
    _CrossWalk_Right,

}__EnumCrossWaklStatus;

#define         _CrossWalk_UpperLine          20
#define         _CrossWalk_BottowLine         50
#define         _CrossWalk_Circle             2

#define         _CrossWalkSignal              1

#define         _CrossWalkStarLine            30
#define         _CrossWalkStopLine            40

extern Uint8 _CrossWalkErrorControl;

extern Uint8 CrossWalk_SetupPoint;

extern Uint32 _WaitStop_Record;

extern Uint8 _CrossWalkCircle;

extern float Stop_Distance;

extern float stop_dis;

extern float _SetupAngle;
extern float _StopAngle;

void StatusWaitStop_TaskFunction(void);
void RepairWaitStop_TaskFunction(void);

void StatusCrossWalk_TaskFunction(void);
void RepairCrossWalk_TaskFunction(void);


void AD(void);
#endif

