





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
 **     �ⲿ����
 *****/



/****
 **  ȫ�ֱ�������
 ****/
uint8 Threshold, Threshold_Limt = 120;
uint8 mt9v03x_output_image[IMGH][IMGW];

uint8 TheEdge = 100;


/**
 *  @name  Parameter_StareDispose
 *  @brief ������ʼ��    �����ʼ��   ����������� ���ز�������
 *  @note  ���� ��־ �ź� ���ݳ�ʼ��
 **/

void Parameter_StareDispose(void)
{




}


/**
 *  @name  RealTimeOS_OperationInital
 *  @brief ������ʼ��    �����ʼ��   ����������� ���ز�������
 *  @note  ����Debug���嵽USART8 ʹ��USART3 ���Ͳɼ�����ͼ��
 **/

void RealTimeOS_OperationInital(void)
{
    /*�����������ʼ��*/
}


void RealTimeOS_OperationSystem(void)
{
    get_using_img(mt9v03x_image_dvp,mt9v03x_output_image);

    Threshold = GetThreshold(mt9v03x_output_image); //1ms
    /**��򷨼���ʵ�ֶ�̬��ֵ��������޷�������������**/
    if(Threshold < Threshold_Limt)Threshold = Threshold_Limt;
    /**ͼ���ֵ��**/
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


