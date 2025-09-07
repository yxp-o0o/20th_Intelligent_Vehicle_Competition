
#ifndef CODE_JIAODUHUAN_H_
#define CODE_JIAODUHUAN_H_

#include "zf_device_imu660ra.h"
#include "math.h"
#define RAD_TO_DEG 57.2957   // 弧度转角度的转换率57.295779513082320876798154814105
#define DEG_TO_RAD 0.0174   // 角度转弧度的转换率0.01745329251994329576923690768489

extern long error_gx,error_gy,error_gz;
extern float GyroY, GyroZ;//角速度左转为正右转为负
extern float dYaw;  //X轴一个周期内转过的角度
extern float dRoll; //Y轴一个周期内转过的角度
extern float AccX, AccY, AccZ;
extern float yaw_angle;
extern  uint8 flag_angle_integral;
extern float now_angle ,target_angle;//目标角度
extern float gyroz_shuzu[16]  ;
extern float GyroZfinal;
extern float GyroZAR[3];
extern void GyroZ_lvbo(void);
#define MPU_getAccZ() AccZ //获取Z轴加速度

//陀螺仪初始化
void imu660ra_Init(void);

//校准函数
void Calibrate(void);

//设定单位时间(即陀螺仪数据更新的时间周期)
void set_dTime(float d_time);

//更新陀螺仪数据
void IMU_update(void);


void start_yaw(void);
void stop_yaw(void);
float get_current_yaw(void);
float IMU_getGyroZ(void);

void set_target_turn_angle(float turn_angle_);
uint8 Check_angle_arrived(void);
#endif /* CODE_JIAODUHUAN_H_ */



