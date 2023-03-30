




#ifndef __TridentRoad_H_
#define __TridentRoad_H_

#include "SystemOS.h"


typedef enum __TridentStatus
{
    _Trident_Close = 0,
    _Trident_Left,
    _Trident_Right
}__EnumTridentStatus;

typedef enum __EnumTridentRoad
{
    _TridentRoad_Left = 1,
    _TridentRoad_Right,
}____EnumTridentRoadStatus;


extern float _Trident_Setup;
extern Uint32 _Trident_Record;
//extern Uint32 _Trident_Distance;

extern Uint8 _TridentDirectionStatus;

extern ST_2D_INT_POINT_INFO MSPOT;

extern float KSlope_LTop;
extern float KSlope_LBot;
extern float KSlope_RTop;
extern float KSlope_RBot;

extern Int16 Triden_LoopX;
extern Int16 Triden_LoopXStart;
extern Int16 Triden_LoopXEnd;

extern ST_2D_INT_POINT_INFO MSPOT;

extern Int32 BlackPointArea;

extern Uint8 _TridentDirectionFlag;

void TridentMiddle_TaskFunction(void);
void StatusTridentRoad_TaskFunction(void);
void RepairTrident_TaskFunction(void);
void TridentMiddle_TaskFunction(void);




#endif
