#include "CrossRoad.h"


uint8_t CrossRoadControl = 1;
Uint32  _CrossRoad_Record;

float _CrossRoadAngle_Record;

void StatusCrossRoad_TaskFunction(void)
{
    switch(EventStatusTask[_Event_Crossroads]._EventCurrentStatus)
     {
         case _NormalStatus_Standby:
             BEEP_ON;
             _CrossRoad_Record = _Runing_Distance;
             //_CrossRoadAngle_Record = _OutAngle.yaw;
             EventStatusTask[_Event_Crossroads]._EventCurrentStatus = _Task_Crossroads_Entry;
             break;

         case _Task_Crossroads_Entry:
             if(_Runing_Distance - _CrossRoad_Record > 2000) //5ms一次 调过 20次 4次 调过5图像
             {
                 if(m_i16AbsoluteOutWidth < 5 && m_i16WOutWidthROWNum < 15 && m_i16OutWidthROWNum < 30)
                 {
                     //_StatusMonitor._fpEventControl = _Event_NormalStatus;
                     _CrossRoad_Record = _Runing_Distance;
                     EventStatusTask[_Event_Crossroads]._EventCurrentStatus = _Task_Crossroads_Waiting;

                 }
                 else if(
                         (_Runing_Distance - _CrossRoad_Record) > 4200 &&
                         m_i16AbsoluteOutWidth < 3
                 )
                 {
                     _CrossRoad_Record = _Runing_Distance;
                     EventStatusTask[_Event_Crossroads]._EventCurrentStatus = _Task_Crossroads_Waiting;
                 }
             }
             break;
         case _Task_Crossroads_Waiting:
             if(m_i16AbsoluteOutWidth > 8 && m_i16WOutWidthROWNum > 15  && m_i16CorneerPointNumber > 1 )
             {
                 _CrossRoad_Record = _Runing_Distance;
                 EventStatusTask[_Event_Crossroads]._EventCurrentStatus = _Task_Crossroads_Outing;
             }
             else if(_Runing_Distance -  _CrossRoad_Record > 6000 && (Lpt1_found == true || Lpt0_found == true)
                     && m_i16WOutWidthROWNum > 8 && m_i16CorneerPointNumber > 1
                     )
             {
                 _CrossRoad_Record = _Runing_Distance;
                 EventStatusTask[_Event_Crossroads]._EventCurrentStatus = _Task_Crossroads_Outing;
             }
             break;
         case _Task_Crossroads_Outing:
             if(_Runing_Distance - _CrossRoad_Record > 1600) //5ms一次 调过 20次 4次 调过5图像
               {
                   if(m_i16AbsoluteOutWidth < 5 && m_i16WOutWidthROWNum < 15 && m_i16OutWidthROWNum < 30)
                   {
                       BEEP_OFF;
                       _StatusMonitor._fpEventControl = _Event_NormalStatus;
                   }
                   else if(
                           (_Runing_Distance - _CrossRoad_Record) > 4000 &&
                           m_i16AbsoluteOutWidth < 3
                   )
                   {
                       BEEP_OFF;
                       _StatusMonitor._fpEventControl = _Event_NormalStatus;
                   }
                   else if(m_i16WOutWidthROWNum < 10 && m_i16OutWidthROWNum < 20)
                   {
                       BEEP_OFF;
                       _StatusMonitor._fpEventControl = _Event_NormalStatus;
                   }
                   else if(ipts0_num > 40 && ipts1_num > 40
                           && m_i16AbsoluteOutWidth < 7 && m_i16WOutWidthROWNum < 10)
                   {
                       BEEP_OFF;
                       _StatusMonitor._fpEventControl = _Event_NormalStatus;
                   }
               }
             break;
     }
}


void RepairCrossRoad_TaskFunction(void)
{

    switch(EventStatusTask[_Event_Crossroads]._EventCurrentStatus)
    {
        case _Task_Crossroads_Outing:
        case _Task_Crossroads_Entry:
            ElementHandle_NormalStatus();
            break;
    }

}







