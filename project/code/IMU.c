#include "IMU.h"

_xyz_int_st Gyro,Acc;
_xyz_f_st GYRO_Real = {0,0,0};
_xyz_f_st imu_data = {0,0,0};

void Mpu6050_getdata(void)
{
    mpu6050_get_gyro();
    mpu6050_get_acc();

    Acc.x = mpu_acc_x;
    Acc.y = mpu_acc_y;
    Acc.z = mpu_acc_z;

    Gyro.x = mpu_gyro_x - 1;
    Gyro.y = mpu_gyro_y - 1;
    Gyro.z = mpu_gyro_z + 15;

    GYRO_Real.x =  (float)Gyro.x * Gyro_G;
    GYRO_Real.y =  0.4*(float)Gyro.y * Gyro_G + 0.6*GYRO_Real.y;
    GYRO_Real.z =  (float)Gyro.z * Gyro_G;

//    if((GYRO_Real.x > 1.5)||(GYRO_Real.x< -1.5))
//    {
//        imu_data.x+=0.01*GYRO_Real.x;
//    }
    if((GYRO_Real.y > 30)||(GYRO_Real.y< -30))
    {
        imu_data.y+=0.01*GYRO_Real.y;
    }
//    if((GYRO_Real.z > 1.5)||(GYRO_Real.z< -1.5))
//    {
//        imu_data.z+=0.01*GYRO_Real.z;
//    }
}
