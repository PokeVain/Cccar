#ifndef IMU_H_
#define IMU_H_

#include "zf_device_mpu6050.h"

#define RtA             57.295779      //弧度->角度
#define AtR             0.0174533      //角度->弧度
#define Acc_G           0.0011963      /*      */
#define Gyro_G          0.0610351      /*  4000 / 65536    */
#define Gyro_Gr         0.0010653      /*  π / 180 * LSBg  */

typedef struct
{
    float x;
    float y;
    float z;
} _xyz_f_st;

typedef struct
{
    int16 x;
    int16 y;
    int16 z;
} _xyz_int_st;

extern _xyz_int_st Gyro,Acc;
extern _xyz_f_st GYRO_Real, imu_data;

void Mpu6050_getdata(void);

#endif /* IMU_H_ */
