






#ifndef __HMIControl_H_
#define __HMIControl_H_




//-------外部调用函数-------

//-------参数 数据修改------

#include "SystemOS.h"


#define         HMITaskNumber           6

#define         KEY_Up                gpio_get_level(E6)  //数据上  //移动上
#define         KEY_Down              gpio_get_level(E3)  //数据下  //移动下
#define         KEY_Left              gpio_get_level(E2)  //数据左 //退出
#define         KEY_Right             gpio_get_level(E4)  //数据右 //进入
#define         KEY_Enable            gpio_get_level(E5)  //确定    //发车

#define         _KEY_CntClear            0
#define         _KEY_LongMode            20



typedef enum
{
    //状态标志
    _KeyStatus_Reset = 0,
    _KeyStatus_Chick,
    _KeyStatus_Enable,
    _KeyStatus_Clear,


    //存储参数
    _KeyStore_NULL,
    _KeyStore_KEYUp,
    _KeyStore_KEYDown,
    _KeyStore_KEYLeft,
    _KeyStore_KEYRight,
    _KeyStore_KEYEnable,

    //选则数据
    _KeySwitch_Open,
    _KeySwitch_Close,

    //使能界面
    _HIMDisplay_Open,
    _HIMDisplay_Close,

    //刷新界面
    _HIMDisplay_Enable,
    _HIMDisplay_Resert,

    //控制界面是否有效
    _HMIControl_Enable,
    _HMIControl_Disable

}__KeyStatus_Enum;


typedef struct __HMIControl
{
    unsigned char KeyStatus_Present;
    unsigned char KeyStatus_StoreData;
    unsigned char KeyStatus_CurrentTask;
    unsigned char KeyStatus_HMIDisplay;
    unsigned char HMIControl_Refresh;
    unsigned char HMIControl_Enable;

    //长按模式
    unsigned int  HMIControl_ModelCnt;
    unsigned char HMIControl_LongData;

}__HMIControl_Struct;


typedef struct __HMIDisplay
{
    unsigned char TaskSerial;
    unsigned char SelectData;
    void (*TaskFunction) (void);

}__HMIDisplay_Struct;


typedef struct __ParamCorrect
{
    char *ParamterName;
    float *Paramtersprt;
    float Stepping;
}__ParamAlter;

typedef struct __StoreControl
{
    char *ControlName;

}__StoreControl_Struct;


typedef struct __MenuDisplay
{
    char *DSPName;
}__DSPMenu_Struct;



typedef enum
{
    _StoreParam_Defeat  = 0,
    _StoreParam_Success
}__EnumStoreParam_Emun;


typedef struct __StoreParamFlash
{
    unsigned char Flag_CheckStore;
    unsigned char StoreCountLoop;
}__StoreParamFlash_Struct;



extern __HMIControl_Struct _HMIControl;
extern __HMIDisplay_Struct _HMIDisplay[HMITaskNumber];

extern __VehicleInformation_Struct VehicleInformation;

//--------------外部调用函数----------------
void InitalParameter_HIMControl(void);
void TaskImplement_HIMControl  (void);

void BaseConfigure_HIMFunction (void);
void TaskImplement_HIMFunction (void);

//---------------底层函数-------------------
void TaskDisplay_MainFunction (void);
void TaskDisplay_ImageFunction(void);
void TaskDisplay_MotorFunction(void);
void TaskDisplay_SteerFunction(void);
void TaskDisplay_StoreFunction(void);
void TaskDisplay_OtherFunction(void);

//--------------储存函数--------------------
void StoreFunction_RunEndAuto   (void);
void StoreFunction_StareReadLast(void);


//--------------状态函数--------------------
void TaskRuning_StareupFunction(void);
void TaskStopend_StopFunction  (void);


void DisplayPage_HIMFunction(unsigned char TaskFunction, unsigned char TaskParamNumber, __ParamAlter TaskAlter[]);
#endif






