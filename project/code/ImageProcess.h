


#ifndef __ImageProcess_H_
#define __ImageProcess_H_

#include "stdbool.h"

#include "string.h"
#include "stdio.h"

#include "zf_driver_gpio.h"
#include "zf_driver_uart.h"

#define         BEEP_ON          gpio_set_level(E1, 1)
#define         BEEP_OFF         gpio_set_level(E1, 0)

//LED�ź� ��ʾ

#define         ImageActiveInformation          gpio_toggle_level(C14)

#define         MotorActiveInformation          gpio_set_level(C14, 0)
#define         MotorStopedInformation          gpio_set_level(C14, 1)

#define         SenorActiveInformation          gpio_set_level(C15, 0)
#define         SenorStopedInformation          gpio_set_level(C15, 1)

/*�����������(�����ͻ,����ȥ��)*/
typedef unsigned char       Uint8;
typedef signed   char       Int8;
typedef unsigned short      Uint16;
typedef signed   short      Int16;
typedef unsigned long       Uint32;
typedef signed   long       Int32;
typedef unsigned long long  Uint64;
typedef signed   long long  Int64;
typedef float               F32;
typedef double              F64;

/*����ͼ��ĸ߶ȺͿ��*/
#define IMGH 60
#define IMGW 188

/*�����ֵ��ͼ���ɫֵ���ɫֵ*/
#define B_BLACK 0
#define B_WHITE 255


typedef enum
{
    _VehicleInformation_Stare = 0,

    _VehicleInformation_Drop,

    _VehicleInformation_Stop

}__EnumSignal;

typedef struct __VehicleInformation
{

    unsigned char StartDeparture;

    float         SpeedSpecialPolicy;

    float         SpeedDeparture;

}__VehicleInformation_Struct;


/*��άint�͵�����ṹ�嶨��*/
typedef struct _st_2d_int_point_info_
{
        Int16 m_i16x;
        Int16 m_i16y;
} ST_2D_INT_POINT_INFO;


/*********��������*************/
void get_using_img(Uint8 (*InImg)[IMGW], Uint8 (*OutImg)[IMGW]);
Uint8 GetThreshold(Uint8 (*Img)[IMGW]);
void GetBinaryImage(Uint8 (*InImg)[IMGW], Uint8 (*OutImg)[IMGW], Uint8 Threshold);
void BinaryImage(Uint8 (*InImg)[IMGW], Uint8 (*OutImg)[IMGW], Uint8 Threshold);

void ImageHandle_threshold(Uint8 (*InImg)[IMGW], Uint8 (*OutImg)[IMGW], Uint8 Threshold);
void ImageHandle_Copyimage(Uint8 *img_data, Uint8 *output_data);
#endif
