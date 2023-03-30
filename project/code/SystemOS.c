





#include  "SystemOS.h"


#define         TimeDebug           0
#define         PrintInfo           0
#define         SendImage           0


#define         SendInfor           0


#define         Motor               0
#define         Steer               0
#define         Sensor_MPU6050      1
#define         Task                0



/*****
 **     外部调用
 *****/



/****
 **  全局变量定义
 ****/
uint8 Threshold, Threshold_Limt = 120;
uint8 mt9v03x_output_image[IMGH][IMGW];

uint8 TheEdge = 100;


/**
 *  @name  Parameter_StareDispose
 *  @brief 参数初始化    外设初始化   传入参数：无 返回参数：无
 *  @note  参数 标志 信号 数据初始化
 **/

void Parameter_StareDispose(void)
{




}


/**
 *  @name  RealTimeOS_OperationInital
 *  @brief 参数初始化    外设初始化   传入参数：无 返回参数：无
 *  @note  串口Debug定义到USART8 使用USART3 发送采集处理图像
 **/

void RealTimeOS_OperationInital(void)
{
    /*以下是外设初始化*/
}


void RealTimeOS_OperationSystem(void)
{
    get_using_img(mt9v03x_image_dvp,mt9v03x_output_image);

    Threshold = GetThreshold(mt9v03x_output_image); //1ms
    /**大津法计算实现动态阈值必须进行限幅操作！！！！**/
    if(Threshold < Threshold_Limt)Threshold = Threshold_Limt;
    /**图像二值化**/
    ImageHandle_threshold(mt9v03x_output_image,mt9v03x_output_image,Threshold); //2.8ms


    ImageHandle_OptimalPoint(mt9v03x_output_image);

    if(AllAreaWhitePoint > 200)
    {
        ImageHandle_LineProcess(mt9v03x_output_image,Threshold); //3ms

        ImageHandle_EdgeBorde(mt9v03x_output_image); //1ms

        ElementHandle_UltraWide();

        ElementHandle_SpecialPoint();

        TaskImplement_TaskFunction();

        CalculationCenterLine();

        SolutionOffestCenterLine();
    }
}


