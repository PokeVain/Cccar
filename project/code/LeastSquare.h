


#ifndef __LeastSquare_H_
#define __LeastSquare_H_


#include "ImageProcess.h"

typedef float REAL;

#define ABS(x) ( (x)>0?(x):-(x) )
#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )
#define POW(a) ((a)*(a))

float regression(ST_2D_INT_POINT_INFO Temp[], int startline, int endline);
float my_sqrt(float number);
float CurvatureSolution(ST_2D_INT_POINT_INFO _PointA, ST_2D_INT_POINT_INFO _PointB, ST_2D_INT_POINT_INFO _PointC);

float my_abs(float f);
double mx_sin(double rad);

Int16 Calculate_Add(ST_2D_INT_POINT_INFO Temp[], int startline, int endline, int YPoint);
Int16 Calculate_CecterLine(int startline, int endline, int sprt, ST_2D_INT_POINT_INFO Temp[]);
REAL fast_atan2(REAL y, REAL x);
float fast_atan(float x, float y);
float Slope_Calculate(int begin,int end,int temp[][2]);
float IncludedAngleCOS(ST_2D_INT_POINT_INFO _Upoint, ST_2D_INT_POINT_INFO _Mpoint, ST_2D_INT_POINT_INFO _Dpoint);
float invSqrt(float x);

float KSlope_Calculate(int begin,int end,int temp[][2]);
#define EPS_FLOAT (+3.452669830012e-4)

#define X_MAX (+9.099024257348e3)
#define INV_PI_2 ( 0.63661977236758134307553505349006)
#define PI_2_C1             ( 1.5703125)
#define PI_2_C2             ( 4.84466552734375e-4)
#define PI_2_C3 (-6.39757837755768678308360248557e-7)

#define TANP_COEF1    (-1.113614403566e-1)
#define TANP_COEF2    (+1.075154738488e-3)
#define TANQ_COEF0    (+1.000000000000)
#define TANQ_COEF1    (-4.446947720281e-1)
#define TANQ_COEF2    (+1.597339213300e-2)

float FastTan(float x);
float FastSin(float x);
float FastCos(float x);


float angle_caculate(float x1, float y1, float x2, float y2, float x3, float y3);

//�ṹ�����Ͷ���
typedef struct
{
    float LastP;//�ϴι���Э���� ��ʼ��ֵΪ0.02
    float Now_P;//��ǰ����Э���� ��ʼ��ֵΪ0
    float out;//�������˲������ ��ʼ��ֵΪ0
    float Kg;//���������� ��ʼ��ֵΪ0
    float Q;//��������Э���� ��ʼ��ֵΪ0.001
    float R;//�۲�����Э���� ��ʼ��ֵΪ0.543

}KFP;//Kalman Filter parameter

float kalmanFilter(KFP *kfp,float input);

#endif
