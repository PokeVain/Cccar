




#include "LeastSquare.h"

//码表
#define TAN_MAP_SIZE    256
#define TAN_MAP_RES     0.003921569f

float fast_atan_table[257] =
{
    0.000000e+00, 3.921549e-03, 7.842976e-03, 1.176416e-02,
    1.568499e-02, 1.960533e-02, 2.352507e-02, 2.744409e-02,
    3.136226e-02, 3.527947e-02, 3.919560e-02, 4.311053e-02,
    4.702413e-02, 5.093629e-02, 5.484690e-02, 5.875582e-02,
    6.266295e-02, 6.656816e-02, 7.047134e-02, 7.437238e-02,
    7.827114e-02, 8.216752e-02, 8.606141e-02, 8.995267e-02,
    9.384121e-02, 9.772691e-02, 1.016096e-01, 1.054893e-01,
    1.093658e-01, 1.132390e-01, 1.171087e-01, 1.209750e-01,
    1.248376e-01, 1.286965e-01, 1.325515e-01, 1.364026e-01,
    1.402496e-01, 1.440924e-01, 1.479310e-01, 1.517652e-01,
    1.555948e-01, 1.594199e-01, 1.632403e-01, 1.670559e-01,
    1.708665e-01, 1.746722e-01, 1.784728e-01, 1.822681e-01,
    1.860582e-01, 1.898428e-01, 1.936220e-01, 1.973956e-01,
    2.011634e-01, 2.049255e-01, 2.086818e-01, 2.124320e-01,
    2.161762e-01, 2.199143e-01, 2.236461e-01, 2.273716e-01,
    2.310907e-01, 2.348033e-01, 2.385093e-01, 2.422086e-01,
    2.459012e-01, 2.495869e-01, 2.532658e-01, 2.569376e-01,
    2.606024e-01, 2.642600e-01, 2.679104e-01, 2.715535e-01,
    2.751892e-01, 2.788175e-01, 2.824383e-01, 2.860514e-01,
    2.896569e-01, 2.932547e-01, 2.968447e-01, 3.004268e-01,
    3.040009e-01, 3.075671e-01, 3.111252e-01, 3.146752e-01,
    3.182170e-01, 3.217506e-01, 3.252758e-01, 3.287927e-01,
    3.323012e-01, 3.358012e-01, 3.392926e-01, 3.427755e-01,
    3.462497e-01, 3.497153e-01, 3.531721e-01, 3.566201e-01,
    3.600593e-01, 3.634896e-01, 3.669110e-01, 3.703234e-01,
    3.737268e-01, 3.771211e-01, 3.805064e-01, 3.838825e-01,
    3.872494e-01, 3.906070e-01, 3.939555e-01, 3.972946e-01,
    4.006244e-01, 4.039448e-01, 4.072558e-01, 4.105574e-01,
    4.138496e-01, 4.171322e-01, 4.204054e-01, 4.236689e-01,
    4.269229e-01, 4.301673e-01, 4.334021e-01, 4.366272e-01,
    4.398426e-01, 4.430483e-01, 4.462443e-01, 4.494306e-01,
    4.526070e-01, 4.557738e-01, 4.589307e-01, 4.620778e-01,
    4.652150e-01, 4.683424e-01, 4.714600e-01, 4.745676e-01,
    4.776654e-01, 4.807532e-01, 4.838312e-01, 4.868992e-01,
    4.899573e-01, 4.930055e-01, 4.960437e-01, 4.990719e-01,
    5.020902e-01, 5.050985e-01, 5.080968e-01, 5.110852e-01,
    5.140636e-01, 5.170320e-01, 5.199904e-01, 5.229388e-01,
    5.258772e-01, 5.288056e-01, 5.317241e-01, 5.346325e-01,
    5.375310e-01, 5.404195e-01, 5.432980e-01, 5.461666e-01,
    5.490251e-01, 5.518738e-01, 5.547124e-01, 5.575411e-01,
    5.603599e-01, 5.631687e-01, 5.659676e-01, 5.687566e-01,
    5.715357e-01, 5.743048e-01, 5.770641e-01, 5.798135e-01,
    5.825531e-01, 5.852828e-01, 5.880026e-01, 5.907126e-01,
    5.934128e-01, 5.961032e-01, 5.987839e-01, 6.014547e-01,
    6.041158e-01, 6.067672e-01, 6.094088e-01, 6.120407e-01,
    6.146630e-01, 6.172755e-01, 6.198784e-01, 6.224717e-01,
    6.250554e-01, 6.276294e-01, 6.301939e-01, 6.327488e-01,
    6.352942e-01, 6.378301e-01, 6.403565e-01, 6.428734e-01,
    6.453808e-01, 6.478788e-01, 6.503674e-01, 6.528466e-01,
    6.553165e-01, 6.577770e-01, 6.602282e-01, 6.626701e-01,
    6.651027e-01, 6.675261e-01, 6.699402e-01, 6.723452e-01,
    6.747409e-01, 6.771276e-01, 6.795051e-01, 6.818735e-01,
    6.842328e-01, 6.865831e-01, 6.889244e-01, 6.912567e-01,
    6.935800e-01, 6.958943e-01, 6.981998e-01, 7.004964e-01,
    7.027841e-01, 7.050630e-01, 7.073330e-01, 7.095943e-01,
    7.118469e-01, 7.140907e-01, 7.163258e-01, 7.185523e-01,
    7.207701e-01, 7.229794e-01, 7.251800e-01, 7.273721e-01,
    7.295557e-01, 7.317307e-01, 7.338974e-01, 7.360555e-01,
    7.382053e-01, 7.403467e-01, 7.424797e-01, 7.446045e-01,
    7.467209e-01, 7.488291e-01, 7.509291e-01, 7.530208e-01,
    7.551044e-01, 7.571798e-01, 7.592472e-01, 7.613064e-01,
    7.633576e-01, 7.654008e-01, 7.674360e-01, 7.694633e-01,
    7.714826e-01, 7.734940e-01, 7.754975e-01, 7.774932e-01,
    7.794811e-01, 7.814612e-01, 7.834335e-01, 7.853983e-01,
    7.853983e-01
};


/***最小二乘法***/
 float regression(ST_2D_INT_POINT_INFO Temp[], int startline, int endline)
 {
     int i = 0;
     int sumlines = startline-endline;
     int sumX = 0;
     int sumY = 0;
     float averageX = 0;
     float averageY = 0;
     float sumUp = 0;
     float sumDown = 0;
     float parameterB= 0;

     for (i = startline; i > endline; i--)
     {
         sumX += i;
         sumY += Temp[i].m_i16x;
     }

     if (sumlines != 0)
     {
         averageX = (float)sumX / sumlines;     //x的平均值
         averageY = (float)sumY / sumlines;     //y的平均值
     }
     else
     {
         averageX = 0;     //x的平均值
         averageY = 0;     //y的平均值
     }
     for (i = startline; i > endline; i--)
     {
         sumUp += (Temp[i].m_i16x - averageY) * (i - averageX);
         sumDown += (i - averageX) * (i - averageX);
     }
     if (sumDown == 0) parameterB = 0;
     else parameterB = sumUp / sumDown;

     return parameterB;
 }


 Int16 Calculate_Add(ST_2D_INT_POINT_INFO Temp[], int startline, int endline, int YPoint)
  {
     float res;
     Int16 Result;

     int i = 0;
     int sumlines = endline - startline;

     int sumX = 0;
     int sumY = 0;

     float averageX = 0;
     float averageY = 0;

     float sumUp = 0;
     float sumDown = 0;

     float Paramterk= 0;

      for (i = startline; i > endline; i--)
      {
          sumX += i;
          sumY += Temp[i].m_i16x;
      }

      if (sumlines != 0)
      {
          averageX = (float)sumX / sumlines;     //x的平均值
          averageY = (float)sumY / sumlines;     //y的平均值
      }
      else
      {
          averageX = 0;     //x的平均值
          averageY = 0;     //y的平均值
      }

      for (i = startline; i > endline; i--)
      {
          sumUp += (Temp[i].m_i16x - averageY) * (i - averageX);
          sumDown += (i - averageX) * (i - averageX);
      }

     if (sumDown == 0) Paramterk = 0;
     else Paramterk = sumUp / sumDown;

     res =  Temp[YPoint + 1].m_i16x + Paramterk;

     Result =  res > 0 ? res : 1;
     Result =  res < IMGW - 1 ? res : IMGW - 1;

     return Result;
 }


 Int16 Calculate_CecterLine(int startline, int endline, int sprt, ST_2D_INT_POINT_INFO Temp[])
 {
     int i=0,SumX=0,SumY=0,SumLines = 0, result;
     float SumUp=0,SumDown=0,avrX=0,avrY=0,B;
     SumLines=endline-startline;

     for(i=startline;i<endline;i++)
     {
       SumX+=i;
       SumY+=Temp[i].m_i16x;
     }

     avrX=SumX/SumLines;     //X的平均值
     avrY=SumY/SumLines;     //Y的平均值

     SumUp=0;
     SumDown=0;

     for(i=startline;i<endline;i++)
     {
       SumUp+=(Temp[i].m_i16x-avrY)*(i-avrX);
       SumDown+=(i-avrX)*(i-avrX);
     }

     if(SumDown==0) B=0;
     else
     {
       B=(int)(SumUp/SumDown);
     }

     result =  Temp[sprt].m_i16x - B;

     return result;  //返回斜率
 }





 /*曲率的计算*/
 //开平方函数
 float my_sqrt(float number)
  {
      long i;
      float x, y;
      long * A;
      float * B;
      const float f = 1.5;

      x = number * 0.5;
      y = number;
      A = ( long * ) &y;
      i = * A;
      i = 0x5f3759df - ( i >> 1 );

      B = ( float * ) &i;
      y = * B;
      y = y * ( f - ( x * y * y ) );
      y = y * ( f - ( x * y * y ) );
      return number * y;
  }

 //如果 ABC 三点是顺时针方向分布，则三角形面积为负值，逆时针分布为正
float CurvatureSolution(ST_2D_INT_POINT_INFO _PointA, ST_2D_INT_POINT_INFO _PointB, ST_2D_INT_POINT_INFO _PointC)
{
     float LineCurvature, LengthAB, LengthBC, LengthAC;
     float LengthProduct = 0;
     float SumArea = 0;

     SumArea = (float)(((_PointB.m_i16x - _PointA.m_i16x) * (_PointC.m_i16y - _PointA.m_i16y)) - ((_PointC.m_i16x - _PointA.m_i16x) * (_PointB.m_i16y - _PointA.m_i16y))) / 2.0f;

     LengthAB = my_sqrt((float)(POW(_PointA.m_i16x - _PointB.m_i16x) - POW(_PointA.m_i16y - _PointB.m_i16y)));

     LengthBC = my_sqrt((float)(POW(_PointB.m_i16x - _PointC.m_i16x) - POW(_PointB.m_i16y - _PointC.m_i16y)));

     LengthAC = my_sqrt((float)(POW(_PointA.m_i16x - _PointC.m_i16x) - POW(_PointA.m_i16y - _PointC.m_i16y)));

     LengthProduct = LengthAB * LengthBC * LengthAC;

     if(LengthProduct == 0)
     {
         return 0;
     }
     else
     {
         LineCurvature = (4 * SumArea) / LengthProduct;
     }

     return LineCurvature;
}


 /*差速需要的函数*/


 float my_abs(float f)
 {
     if (f >= 0)
     {
         return f;
     }

     return -f;
 }


 double mx_sin(double rad)
 {
     double sine;
     if (rad < 0)
         sine = rad*(1.27323954 + 0.405284735 * rad);
     else
         sine = rad * (1.27323954 - 0.405284735 * rad);
     if (sine < 0)
         sine = sine*(-0.225 * (sine + 1) + 1);
     else
         sine = sine * (0.225 *( sine - 1) + 1);
     return sine;
 }




 REAL fast_atan2(REAL y, REAL x)
 {
     REAL x_abs, y_abs, z;
     REAL alpha, angle, base_angle;
     int index;

     /* don't divide by zero! */
     if ((y == 0.0) || (x == 0.0))//if ((y == 0.0f) && (x == 0.0f))
         angle = 0.0;
     else
     {
         /* normalize to +/- 45 degree range */
         y_abs = ABS(y);
         x_abs = ABS(x);
         //z = (y_abs < x_abs ? y_abs / x_abs : x_abs / y_abs);
         if (y_abs < x_abs)
             z = y_abs / x_abs;
         else
             z = x_abs / y_abs;
         /* when ratio approaches the table resolution, the angle is */
         /*      best approximated with the argument itself...       */
         if (z < TAN_MAP_RES)
             base_angle = z;
         else
         {
             /* find index and interpolation value */
             alpha = z * (REAL) TAN_MAP_SIZE - .5;
             index = (int) alpha;
             alpha -= (REAL) index;
             /* determine base angle based on quadrant and */
             /* add or subtract table value from base angle based on quadrant */
             base_angle = fast_atan_table[index];
             base_angle += (fast_atan_table[index + 1] - fast_atan_table[index]) * alpha;
         }

         if (x_abs > y_abs)
         {        /* -45 -> 45 or 135 -> 225 */
             if (x >= 0.0)
             {           /* -45 -> 45 */
                 if (y >= 0.0)
                     angle = base_angle;   /* 0 -> 45, angle OK */
                 else
                     angle = -base_angle;  /* -45 -> 0, angle = -angle */
             }
             else
             {                  /* 135 -> 180 or 180 -> -135 */
                 angle = 3.14159265358979323846;

                 if (y >= 0.0)
                     angle -= base_angle;  /* 135 -> 180, angle = 180 - angle */
                 else
                     angle = base_angle - angle;   /* 180 -> -135, angle = angle - 180 */
             }
         }
         else
         {                    /* 45 -> 135 or -135 -> -45 */
             if (y >= 0.0)
             {           /* 45 -> 135 */
                 angle = 1.57079632679489661923;

                 if (x >= 0.0)
                     angle -= base_angle;  /* 45 -> 90, angle = 90 - angle */
                 else
                     angle += base_angle;  /* 90 -> 135, angle = 90 + angle */
             }
             else
             {                  /* -135 -> -45 */
                 angle = -1.57079632679489661923;

                 if (x >= 0.0)
                     angle += base_angle;  /* -90 -> -45, angle = -90 + angle */
                 else
                     angle -= base_angle;  /* -135 -> -90, angle = -90 - angle */
             }
         }
     }

     return (angle);
 }


float fast_atan(float x, float y)
{
    return fast_atan2(y, x);
}


//最小二乘法拟合斜率

float Slope_Calculate(int begin,int end,int temp[][2])
{

    float xsum=0,ysum=0,xysum=0,x2sum=0, result;
    int i;

    for(i = begin; i < end; i++)
    {
        xsum += temp[i][0];
        ysum += temp[i][1];
        xysum +=  temp[i][0] * temp[i][1];
        x2sum += temp[i][0] * temp[i][0];
    }

    if((end-begin)*x2sum-xsum*xsum)
    {
        result=((end-begin)*xysum-xsum*ysum)/((end-begin)*x2sum-xsum*xsum);
    }
    else
    {
        result=0;
    }
    return result;
}

float KSlope_Calculate(int begin,int end,int temp[][2])
{
    float AverageSumx = 0, AverageSumy = 0, result = 0, Sumx = 0, Sumy = 0;
    int i, Num = end - begin;

    for(i = begin; i < end; i++)
    {
        AverageSumx += (float)temp[i][1];
        AverageSumy += (float)temp[i][0];
    }

    AverageSumx /= Num;
    AverageSumy /= Num;

    for(i = begin; i < end; i++)
    {
        Sumy += ((float)temp[i][0] - AverageSumy) * (temp[i][1] - AverageSumx);
        Sumx += ((float)temp[i][1] - AverageSumx) * ((float)temp[i][1] - AverageSumx);
    }

    result = Sumy / Sumx;

    return result;
}

float IncludedAngleCOS(ST_2D_INT_POINT_INFO _Upoint, ST_2D_INT_POINT_INFO _Mpoint, ST_2D_INT_POINT_INFO _Dpoint)
{
    float COSCurve, LengthMD, LengthMU;

    ST_2D_INT_POINT_INFO ModeMU, ModeMD;

    ModeMU.m_i16x = _Upoint.m_i16x - _Mpoint.m_i16x;
    ModeMU.m_i16y = _Upoint.m_i16y - _Mpoint.m_i16y;

    ModeMD.m_i16x = _Dpoint.m_i16x - _Mpoint.m_i16x;
    ModeMD.m_i16y = _Dpoint.m_i16y - _Mpoint.m_i16y;

    LengthMD = my_sqrt((float)(POW(ModeMD.m_i16x) + POW(ModeMD.m_i16y)));

    LengthMU = my_sqrt((float)(POW(ModeMU.m_i16x) + POW(ModeMU.m_i16y)));

    COSCurve =  (float)(ModeMU.m_i16x * ModeMD.m_i16x + ModeMU.m_i16y * ModeMD.m_i16y) / (LengthMD * LengthMU);

    return COSCurve;
}

float invSqrt(float x)
{

    long * A;
    float * B;
    float halfx = 0.5 * x;
    float y = x;
    A = (long*)&y;
    long i = *A;
    i = 0x5f3759df - (i>>1);
    B = (float*)&i;
    y = *B;
    y = y * (1.5 - (halfx * y * y));
    return y;
}

float FastTan(float x)
{
    long n;
    float xn;
    float f, g;
    float x_int, x_fract;
    float result;
    float xnum, xden;

    if ((x > (float)X_MAX) || (x < (float)-X_MAX)){
        return (float)0.0;
    }

    x_int = (float)((long)(x));
    x_fract = x - x_int;

    g = (float)0.5;
    if (x <= (float)0.0){
        g = -g;
    }
    n = (long)(x * (float)INV_PI_2 + g);
    xn = (float)(n);

    f = x_int - xn * PI_2_C1;
    f = f + x_fract;
    f = f - xn * PI_2_C2;
    f = f - xn * PI_2_C3;

    if (f < (float)0.0){
        g = -f;
    }
    else{
        g = f;
    }
    if (g < (float)EPS_FLOAT){
        if (n & 0x0001){
            result = -1.0f / f;
        }
        else{
            result = f;
        }
        return result;
    }

    g = f * f;
    xnum = g * TANP_COEF2;
    xnum = xnum + TANP_COEF1;
    xnum = xnum * g;
    xnum = xnum * f;
    xnum = xnum + f;

    xden = g * TANQ_COEF2;
    xden = xden + TANQ_COEF1;
    xden = xden * g;
    xden = xden + TANQ_COEF0;

    if (n & 0x0001){
        result = xnum;
        xnum = -xden;
        xden = result;
    }
    result = xnum / xden;
    return result;
}

#define FAST_SIN_TABLE_SIZE 512

const float sinTable[FAST_SIN_TABLE_SIZE + 1] = {
    0.00000000f, 0.01227154f, 0.02454123f, 0.03680722f, 0.04906767f, 0.06132074f,
    0.07356456f, 0.08579731f, 0.09801714f, 0.11022221f, 0.12241068f, 0.13458071f,
    0.14673047f, 0.15885814f, 0.17096189f, 0.18303989f, 0.19509032f, 0.20711138f,
    0.21910124f, 0.23105811f, 0.24298018f, 0.25486566f, 0.26671276f, 0.27851969f,
    0.29028468f, 0.30200595f, 0.31368174f, 0.32531029f, 0.33688985f, 0.34841868f,
    0.35989504f, 0.37131719f, 0.38268343f, 0.39399204f, 0.40524131f, 0.41642956f,
    0.42755509f, 0.43861624f, 0.44961133f, 0.46053871f, 0.47139674f, 0.48218377f,
    0.49289819f, 0.50353838f, 0.51410274f, 0.52458968f, 0.53499762f, 0.54532499f,
    0.55557023f, 0.56573181f, 0.57580819f, 0.58579786f, 0.59569930f, 0.60551104f,
    0.61523159f, 0.62485949f, 0.63439328f, 0.64383154f, 0.65317284f, 0.66241578f,
    0.67155895f, 0.68060100f, 0.68954054f, 0.69837625f, 0.70710678f, 0.71573083f,
    0.72424708f, 0.73265427f, 0.74095113f, 0.74913639f, 0.75720885f, 0.76516727f,
    0.77301045f, 0.78073723f, 0.78834643f, 0.79583690f, 0.80320753f, 0.81045720f,
    0.81758481f, 0.82458930f, 0.83146961f, 0.83822471f, 0.84485357f, 0.85135519f,
    0.85772861f, 0.86397286f, 0.87008699f, 0.87607009f, 0.88192126f, 0.88763962f,
    0.89322430f, 0.89867447f, 0.90398929f, 0.90916798f, 0.91420976f, 0.91911385f,
    0.92387953f, 0.92850608f, 0.93299280f, 0.93733901f, 0.94154407f, 0.94560733f,
    0.94952818f, 0.95330604f, 0.95694034f, 0.96043052f, 0.96377607f, 0.96697647f,
    0.97003125f, 0.97293995f, 0.97570213f, 0.97831737f, 0.98078528f, 0.98310549f,
    0.98527764f, 0.98730142f, 0.98917651f, 0.99090264f, 0.99247953f, 0.99390697f,
    0.99518473f, 0.99631261f, 0.99729046f, 0.99811811f, 0.99879546f, 0.99932238f,
    0.99969882f, 0.99992470f, 1.00000000f, 0.99992470f, 0.99969882f, 0.99932238f,
    0.99879546f, 0.99811811f, 0.99729046f, 0.99631261f, 0.99518473f, 0.99390697f,
    0.99247953f, 0.99090264f, 0.98917651f, 0.98730142f, 0.98527764f, 0.98310549f,
    0.98078528f, 0.97831737f, 0.97570213f, 0.97293995f, 0.97003125f, 0.96697647f,
    0.96377607f, 0.96043052f, 0.95694034f, 0.95330604f, 0.94952818f, 0.94560733f,
    0.94154407f, 0.93733901f, 0.93299280f, 0.92850608f, 0.92387953f, 0.91911385f,
    0.91420976f, 0.90916798f, 0.90398929f, 0.89867447f, 0.89322430f, 0.88763962f,
    0.88192126f, 0.87607009f, 0.87008699f, 0.86397286f, 0.85772861f, 0.85135519f,
    0.84485357f, 0.83822471f, 0.83146961f, 0.82458930f, 0.81758481f, 0.81045720f,
    0.80320753f, 0.79583690f, 0.78834643f, 0.78073723f, 0.77301045f, 0.76516727f,
    0.75720885f, 0.74913639f, 0.74095113f, 0.73265427f, 0.72424708f, 0.71573083f,
    0.70710678f, 0.69837625f, 0.68954054f, 0.68060100f, 0.67155895f, 0.66241578f,
    0.65317284f, 0.64383154f, 0.63439328f, 0.62485949f, 0.61523159f, 0.60551104f,
    0.59569930f, 0.58579786f, 0.57580819f, 0.56573181f, 0.55557023f, 0.54532499f,
    0.53499762f, 0.52458968f, 0.51410274f, 0.50353838f, 0.49289819f, 0.48218377f,
    0.47139674f, 0.46053871f, 0.44961133f, 0.43861624f, 0.42755509f, 0.41642956f,
    0.40524131f, 0.39399204f, 0.38268343f, 0.37131719f, 0.35989504f, 0.34841868f,
    0.33688985f, 0.32531029f, 0.31368174f, 0.30200595f, 0.29028468f, 0.27851969f,
    0.26671276f, 0.25486566f, 0.24298018f, 0.23105811f, 0.21910124f, 0.20711138f,
    0.19509032f, 0.18303989f, 0.17096189f, 0.15885814f, 0.14673047f, 0.13458071f,
    0.12241068f, 0.11022221f, 0.09801714f, 0.08579731f, 0.07356456f, 0.06132074f,
    0.04906767f, 0.03680722f, 0.02454123f, 0.01227154f, 0.00000000f, -0.01227154f,
    -0.02454123f, -0.03680722f, -0.04906767f, -0.06132074f, -0.07356456f,
    -0.08579731f, -0.09801714f, -0.11022221f, -0.12241068f, -0.13458071f,
    -0.14673047f, -0.15885814f, -0.17096189f, -0.18303989f, -0.19509032f,
    -0.20711138f, -0.21910124f, -0.23105811f, -0.24298018f, -0.25486566f,
    -0.26671276f, -0.27851969f, -0.29028468f, -0.30200595f, -0.31368174f,
    -0.32531029f, -0.33688985f, -0.34841868f, -0.35989504f, -0.37131719f,
    -0.38268343f, -0.39399204f, -0.40524131f, -0.41642956f, -0.42755509f,
    -0.43861624f, -0.44961133f, -0.46053871f, -0.47139674f, -0.48218377f,
    -0.49289819f, -0.50353838f, -0.51410274f, -0.52458968f, -0.53499762f,
    -0.54532499f, -0.55557023f, -0.56573181f, -0.57580819f, -0.58579786f,
    -0.59569930f, -0.60551104f, -0.61523159f, -0.62485949f, -0.63439328f,
    -0.64383154f, -0.65317284f, -0.66241578f, -0.67155895f, -0.68060100f,
    -0.68954054f, -0.69837625f, -0.70710678f, -0.71573083f, -0.72424708f,
    -0.73265427f, -0.74095113f, -0.74913639f, -0.75720885f, -0.76516727f,
    -0.77301045f, -0.78073723f, -0.78834643f, -0.79583690f, -0.80320753f,
    -0.81045720f, -0.81758481f, -0.82458930f, -0.83146961f, -0.83822471f,
    -0.84485357f, -0.85135519f, -0.85772861f, -0.86397286f, -0.87008699f,
    -0.87607009f, -0.88192126f, -0.88763962f, -0.89322430f, -0.89867447f,
    -0.90398929f, -0.90916798f, -0.91420976f, -0.91911385f, -0.92387953f,
    -0.92850608f, -0.93299280f, -0.93733901f, -0.94154407f, -0.94560733f,
    -0.94952818f, -0.95330604f, -0.95694034f, -0.96043052f, -0.96377607f,
    -0.96697647f, -0.97003125f, -0.97293995f, -0.97570213f, -0.97831737f,
    -0.98078528f, -0.98310549f, -0.98527764f, -0.98730142f, -0.98917651f,
    -0.99090264f, -0.99247953f, -0.99390697f, -0.99518473f, -0.99631261f,
    -0.99729046f, -0.99811811f, -0.99879546f, -0.99932238f, -0.99969882f,
    -0.99992470f, -1.00000000f, -0.99992470f, -0.99969882f, -0.99932238f,
    -0.99879546f, -0.99811811f, -0.99729046f, -0.99631261f, -0.99518473f,
    -0.99390697f, -0.99247953f, -0.99090264f, -0.98917651f, -0.98730142f,
    -0.98527764f, -0.98310549f, -0.98078528f, -0.97831737f, -0.97570213f,
    -0.97293995f, -0.97003125f, -0.96697647f, -0.96377607f, -0.96043052f,
    -0.95694034f, -0.95330604f, -0.94952818f, -0.94560733f, -0.94154407f,
    -0.93733901f, -0.93299280f, -0.92850608f, -0.92387953f, -0.91911385f,
    -0.91420976f, -0.90916798f, -0.90398929f, -0.89867447f, -0.89322430f,
    -0.88763962f, -0.88192126f, -0.87607009f, -0.87008699f, -0.86397286f,
    -0.85772861f, -0.85135519f, -0.84485357f, -0.83822471f, -0.83146961f,
    -0.82458930f, -0.81758481f, -0.81045720f, -0.80320753f, -0.79583690f,
    -0.78834643f, -0.78073723f, -0.77301045f, -0.76516727f, -0.75720885f,
    -0.74913639f, -0.74095113f, -0.73265427f, -0.72424708f, -0.71573083f,
    -0.70710678f, -0.69837625f, -0.68954054f, -0.68060100f, -0.67155895f,
    -0.66241578f, -0.65317284f, -0.64383154f, -0.63439328f, -0.62485949f,
    -0.61523159f, -0.60551104f, -0.59569930f, -0.58579786f, -0.57580819f,
    -0.56573181f, -0.55557023f, -0.54532499f, -0.53499762f, -0.52458968f,
    -0.51410274f, -0.50353838f, -0.49289819f, -0.48218377f, -0.47139674f,
    -0.46053871f, -0.44961133f, -0.43861624f, -0.42755509f, -0.41642956f,
    -0.40524131f, -0.39399204f, -0.38268343f, -0.37131719f, -0.35989504f,
    -0.34841868f, -0.33688985f, -0.32531029f, -0.31368174f, -0.30200595f,
    -0.29028468f, -0.27851969f, -0.26671276f, -0.25486566f, -0.24298018f,
    -0.23105811f, -0.21910124f, -0.20711138f, -0.19509032f, -0.18303989f,
    -0.17096189f, -0.15885814f, -0.14673047f, -0.13458071f, -0.12241068f,
    -0.11022221f, -0.09801714f, -0.08579731f, -0.07356456f, -0.06132074f,
    -0.04906767f, -0.03680722f, -0.02454123f, -0.01227154f, -0.00000000f
};


float FastSin(float x)
{
    float sinVal, fract, in; // Temporary variables for input, output
    unsigned short index; // Index variable
    float a, b; // Two nearest output values
    int n;
    float findex;

    // input x is in radians
    // Scale the input to [0 1] range from [0 2*PI] , divide input by 2*pi
    in = x * 0.159154943092f;

    // Calculation of floor value of input
    n = (int) in;

    // Make negative values towards -infinity
    if(x < 0.0f){
        n--;
    }

    // Map input value to [0 1]
    in = in - (float) n;

    // Calculation of index of the table
    findex = (float) FAST_SIN_TABLE_SIZE * in;
    index = ((unsigned short)findex) & 0x1ff;

    // fractional value calculation
    fract = findex - (float) index;

    // Read two nearest values of input value from the sin table
    a = sinTable[index];
    b = sinTable[index+1];

    // Linear interpolation process
    sinVal = (1.0f-fract)*a + fract*b;

    // Return the output value
    return (sinVal);
}

float FastCos(float x)
{
    float cosVal, fract, in; // Temporary variables for input, output
    unsigned short index; // Index variable
    float a, b; // Two nearest output values
    int n;
    float findex;

    // input x is in radians
    // Scale the input to [0 1] range from [0 2*PI] , divide input by 2*pi, add 0.25 (pi/2) to read sine table
    in = x * 0.159154943092f + 0.25;

    // Calculation of floor value of input
    n = (int) in;

    // Make negative values towards -infinity
    if(in < 0.0){
        n--;
    }

    // Map input value to [0 1]
    in = in - (float) n;

    // Calculation of index of the table
    findex = (float) FAST_SIN_TABLE_SIZE * in;
    index = ((unsigned short)findex) & 0x1ff;

    // fractional value calculation
    fract = findex - (float) index;

    // Read two nearest values of input value from the cos table
    a = sinTable[index];
    b = sinTable[index+1];

    // Linear interpolation process
    cosVal = (1.0f-fract)*a + fract*b;

    // Return the output value
    return (cosVal);
}

/*得到余弦值的平方*/
float angle_caculate(float x1, float y1, float x2, float y2, float x3, float y3)
{
    float cosA2;

    y1 = 2.115*y1;
    y2 = 2.115*y2;
    y3 = 2.115*y3;
    cosA2 = ((x2-x1)*(x3-x1)+(y2-y1)*(y3-y1))*((x2-x1)*(x3-x1)+(y2-y1)*(y3-y1))/(((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1))*((x3-x1)*(x3-x1)+(y3-y1)*(y3-y1)));

    return cosA2;
}


float kalmanFilter(KFP *kfp,float input)
{
//预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
     kfp->Now_P = kfp->LastP + kfp->Q;
//卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
     kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
//更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
     kfp->out = kfp->out + kfp->Kg * (input -kfp->out);//因为这一次的预测值就是上一次的输出值
//更新协方差方程: 本次的系统协方差付给 kfp->LastP 威下一次运算准备。
     kfp->LastP = (1-kfp->Kg) * kfp->Now_P;

     return kfp->out;
}
