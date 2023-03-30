



#ifndef __TaskFunction_H_
#define __TaskFunction_H_

#include "SystemOS.h"

#define         EventNumber         9

typedef enum __EnumStatus
{

    /*�¼�����ָ��*/
    _Event_NormalStatus = 0,
    _Event_CurveStatus,
    _Event_Roundabout,
    _Event_TridentRoad,
    _Event_Crossroads,
    _Event_CrossWalk,
    _Event_WaitStop,
    _Event_RampWay,
    _Event_Special

}__EnumControlEventStatus;


typedef enum _AlterStatus
{

    /*��״̬��λ*/
    //�ȴ�����
    _NormalStatus_Standby = 0,



    /*���״̬*/
    _Task_CurveStatus_Straight,
    _Task_CurveStatus_Left,
    _Task_CurveStatus_Right,

    /*����״̬*/
    /*�󻷵�*/
    _Task_LRoundabout_Chick,          //����Ƿ���ֻ���
    _Task_LRoundabout_Ready,
    _Task_LRoundabout_Stare,          //������һ������

    _Task_LRoundabout_Exit,           //5
    _Task_LRoundabout_Out,            //6
    _Task_LRoundabout_Finish,            //6


    _Task_LRoundabout_Enable,         //2
    _Task_LRoundabout_Entry,          //3
    _Task_LRoundabout_Ring,           //4

    /*�һ���*/
    _Task_RRoundabout_Chick,
    _Task_RRoundabout_Ready,
    _Task_RRoundabout_Stare,

    _Task_RRoundabout_Exit,
    _Task_RRoundabout_Out,
    _Task_RRoundabout_Finish,

    _Task_RRoundabout_Enable,
    _Task_RRoundabout_Entry,
    _Task_RRoundabout_Ring,

    /*ʮ��·��*/
    _Task_Crossroads_Entry,
    _Task_Crossroads_Waiting,
    _Task_Crossroads_Outing,

    /*����·ռʱ��������*/


    /*�ڻ�������������*/
    _Task_TridentRoad_LEntry,
    _Task_TridentRoad_REntry,
    _Task_TridentRoad_Runing,


    _Task_TridentRoad_Chick,
    _Task_TridentRoad_Stare,
    /*�������*/
    _Task_TridentRoad_LEning,
    /*�ҽ�����*/
    _Task_TridentRoad_REning,

    /*��������*/
    _Task_TridentRoad_LExit,
    _Task_TridentRoad_RExit,

    //�����߼��
    _Task_CrossWalk_Stop,
    _Task_CrossWalk_Chick,
    _Task_CrossWalk_Enable,
    _Task_CrossWalk_Waking,
    _Task_CrossWalk_Waiting,
    _Task_CrossWalk_Outing,
    _Task_CrossWalk_Exit,
    _Task_CrossWalk_Finish,
    _Task_CrossWalk_Disable,

    _Task_CrossWalk_LCheck,
    _Task_CrossWalk_RCheck,

    //����
    _Task_WaitStop_Ready,
    _Task_WaitStop_Start,
    _Task_WaitStop_Stop,
    _Task_WaitStop_Sucessful,

    //�µ�
    _Task_RampWay_Chick,
    _Task_RampWay_Ring,
    _Task_RampWay_Dowm,
    _Task_RampWay_Exit


}__EnumAlterEventStatus;


typedef struct __EventStatus
{
    unsigned char _EventCurrentStatus;
    void (*fpStatusFunction)(void);
    void (*fpEventFunction)(void);
}__EventSatsusStruct;


typedef struct __StatusControl
{

    unsigned char _fpEventControl;
    __EventSatsusStruct _EventStatus;
    void (*fpSpecialEvent)(void);

}__StatusMonitorStruct;

//----------------------------------------------------------------------------------------

typedef enum _CodeMessage_
{

    //������ʼ
    _LeaderSignal_MainControl_Stare   = 0x00,

    //β��Ӧ��
    _TailerSignal_LeaderHandle_reply   = 0x01,

    //β������
    _LeaderSignal_TailerHandle_firing  = 0x02,

    //β��ֹͣ ȫ��ֹͣ
    _LeaderSignal_TailerHandle_Stopped = 0x04,

    //β������
    _LeaderSignal_TailerHandle_RoundEntry = 0x03,

    //�м䳵����
    _LeaderSignal_MiddleHandle_firing  = 0x05,

    _MiddleSignal_MiddleHandle_SpeedUp = 0x08,

    //β���ź�ͷ������
    _TailerSignal_LeaderHandle_firing = 0x10,

    //�ȴ�β���ź�
    _LeaderSignal_TailerHandle_CrossWalkWait = 0x11,

    //�ڶ���β��ͣ������
    _LeaderSignal_TailerHandle_CrossWalkDouble = 0x12,

    //�䳵������
    _MiddleSignal_TailerHandle_TridentEntry = 0x60,

    //��������
    _TailerSignal_LeaderHandle_SpeedUp = 0x61,

    //������־
    _HeaderSignal_TailerHandle_Victory = 0x66,

    //β��ֹͣ�ɹ�
    _MainControl_MiddleHandle_Stopped = 0x56,

    //ͷ��ֹͣ�ɹ�
    _MainControl_LeaderHandle_Stopped = 0x57,

    //β��ֹͣ�ɹ�
    _MainControl_TailerHandle_Stopped = 0x58,

    //ǿ��ͣ��
    _MainControl_AllStop = 0x55,
}_CodeMessage_Enum;

typedef enum _MessageEnum_
{

    //������Ϣ
    _SendMessageStatus_Reset = 0,

    //��������
    _SendMessageStatus_Stare,

    //������Ϣ
    _DealMessageStatus_Reset

}_MessageSatus_Enum;

typedef enum
{
    _MessageElement_Normal = 0,
    _MessageElement_TeidentEnable,
}_MessageElement;

typedef enum
{
    _ElementMessage_Normal = 0,
    _ElementMessage_TeidentEnable,
}_ElementMessage;



typedef enum _SignalEnum_
{

    //�����ź�
    _MessageSignal_Normal = 0,
    _MessageSignal_Waiting,
    _MessageSignal_Contact,
    _MessageSignal_Stopped,
    _MessageSignal_Reset,

    _MessageSignal_Defeat,

    //�����ź�
    _MessageHandle_Answer,

    //���渴λ
    _MessageTrident_Stare,
    _MessageTrident_SetUp,


    _MessageRoundabout_Stare,


    //�����߼��
    _MessageCrossWalk_Double,
    _MessageCrossWalk_Complete,


}_MessageSignal_Enum;


typedef struct MessageControl
{
    unsigned char _ControlSprt;
    void (*MessageFunction)(void);

}__MessageControl_Struct;


typedef struct MessageMonitor
{
    unsigned char MessageSendSprt_Current;
    unsigned char MessageDealSprt_Current;

}__MessageMonitor_Struct;
//----------------------------------------------------------------------------------------


extern float _TridentRoadCosA;
extern Uint16 _Measure_Distance;

extern __StatusMonitorStruct _StatusMonitor;
extern __EventSatsusStruct EventStatusTask[EventNumber];



void DisplayParam_TaskFunction(void);

/*�ⲿ���ú���*/
void InitalParam_TaskFunction(void);
void TaskImplement_TaskFunction(void);


void ObserverStatus_TaskFunction(void);
void SpecialEvent_TaskFunction(void);

void NullFunction_TaskFunction(void);

/*Ԫ�غ���*/
void ResetElement_TaskFunction(void);
void CurveStatus_TaskFunction(void);
void NormalRoad_TaskFunction(void);
#endif





