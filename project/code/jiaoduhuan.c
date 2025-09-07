#include "zf_common_headfile.h"
#include "jiaoduhuan.h"

long error_ax=0,error_ay=0,error_az=0;
long error_gx=0,error_gy=0,error_gz=0;

float AccX=0, AccY=0, AccZ = 0;
float GyroY = 0, GyroZ = 0;
float dYaw = 0; //Z轴一个周期内转过的角度
float dRoll = 0; //Y轴一个周期内转过的角度
float gyroZangle = 0;

uint8 yaw_flag = 0;  //积分标志位，1开始积分
float yaw_angle = 0; // 开始积分的角度

uint8 flag_angle_integral = 0;
float now_angle = 0,target_angle = 0;//目标角度

extern int16 imu660ra_gyro_x, imu660ra_gyro_y, imu660ra_gyro_z;                 // 三轴陀螺仪数据      GYRO (陀螺仪)
extern int16 imu660ra_acc_x, imu660ra_acc_y, imu660ra_acc_z;                    // 三轴加速度计数据     ACC  (accelerometer 加速度计)



float d_Time = 0.005f;
void imu660ra_Init(void)
{
    imu660ra_init();//陀螺仪初始化
    Calibrate();//陀螺仪校准函数
    set_dTime(0.005);//陀螺仪积分时间,对应中断时间，括号内单位秒
}

//校准函数
void Calibrate(void){
    int i=0;
    error_gx=0,error_gy=0,error_gz=0;

    for(i=0;i<500;i++){
        imu660ra_get_gyro();
        imu660ra_get_acc();

        error_gx+=imu660ra_gyro_x;
        error_gy+=imu660ra_gyro_y;
        error_gz+=imu660ra_gyro_z;

        error_ax+=imu660ra_acc_x;
        error_ay+=imu660ra_acc_y;
        error_az+=imu660ra_acc_z;
    }
    error_gx/=500;
    error_gy/=500;
    error_gz/=500;

    error_ax/=500;
    error_ay/=500;
    error_az/=500;
}

//设定单位时间(即陀螺仪数据更新的时间周期)
void set_dTime(float d_time)
{
    d_Time = d_time;
}

//MPU数据更新
//更新数据: 1.y轴加速度 2.角速度 3.角度积分
//is_suan_angle置1开始积分, 置0清零积分
//角度积分积到最大值会保持积分最大值
float gyroz_shuzu[16] = {0} ;
float GyroZfinal = 0;
float GyroZAR[3]={0};
void GyroZ_lvbo(void)
{
    uint8 n = 0;

       for(n=0; n<14 ; n++)
             {
           gyroz_shuzu[n] = gyroz_shuzu[n+1];  //    0~14 赋值

             }
       gyroz_shuzu[n] = GyroZ;
       gyroz_shuzu[15]   = 0.33*(gyroz_shuzu[15] +gyroz_shuzu[14] +gyroz_shuzu[13] );

       GyroZAR[0] = GyroZAR[1];
       GyroZAR[1] = GyroZAR[2];
       GyroZAR[2] =( gyroz_shuzu[15]+ gyroz_shuzu[14]+ gyroz_shuzu[13]
                     + gyroz_shuzu[12]+ gyroz_shuzu[11]+ gyroz_shuzu[10]+
                     gyroz_shuzu[9]+ gyroz_shuzu[8]+ gyroz_shuzu[7]+ gyroz_shuzu[6]+
                     gyroz_shuzu[5]+ gyroz_shuzu[4]+ gyroz_shuzu[3]+ gyroz_shuzu[2]+ gyroz_shuzu[1]+ gyroz_shuzu[0])/16;

       GyroZAR[2] = 0.7*GyroZAR[2]+0.2*GyroZAR[1]+0.1*GyroZAR[0];
       GyroZfinal =GyroZAR[2];
}
void IMU_update()
{
//    static uint8 num_angle = 0;

    imu660ra_get_acc();
    imu660ra_get_gyro();

    AccX=(float)(imu660ra_acc_x)/8192;
    AccY=(float)(imu660ra_acc_y)/8192;//y轴方向加速度(向前加速度)
    AccZ=(float)(imu660ra_acc_z+ 4062)/2;

//  GyroX=(float)(mpu6050_gyro_x-error_gx)/32.768;
    GyroY=(float)(imu660ra_gyro_y-error_gy)/32.768;
    GyroZ=(float)(imu660ra_gyro_z-error_gz)/16.8  ;//角速度，原始数据转化为度每秒
    GyroZ_lvbo();
    dYaw = GyroZfinal * d_Time;//绕z轴角度微分
    dRoll = GyroY * d_Time;//绕y轴角度微分

    //开始yaw积分
    if(GyroZfinal>0.5 ||GyroZfinal<(-0.5))
    { if(yaw_flag) {yaw_angle += dYaw;}}
}

//获取Y轴加速度
float IMU_get_acc_y(void)
{
    imu660ra_get_acc();
    return (float)(imu660ra_acc_y-error_ay)/8192;
}

//获取Z轴角速度
float IMU_getGyroZ(void)
{
    return GyroZ;
}

//z轴开始积分
void start_yaw(void)
{
    yaw_flag = 1;
}

//记得使用完后清除标志位
//z轴停止积分并将积分角度清0
void stop_yaw(void)
{
    yaw_flag = 0;
    yaw_angle = 0;
}

float get_current_yaw(void)
{
    return yaw_angle;
}

//设置目标转向角度
//设置后就会转向
float angle_to = 0 ; // 目标角度
float angle_now = 0 ; // 当前角度
void setTargetAngle(float t_angle)
{
    angle_to = t_angle;
}
/*
float angle_loop()
{
 angle_to = Slope_Calculate(5, 115, center_line);
 return  angle_to;
}
*/



/*
float angle_loop()
{
 angle_to = Slope_Calculate(5, 115, center_line);
 return  angle_to;
}
*/



/*设置目标转角，用于元素判断
 * 输入：目标角度，正:右;负:左;
 */
void set_target_turn_angle(float turn_angle_)
{
    now_angle= 0;
    flag_angle_integral = 1;
    start_yaw();
    target_angle = turn_angle_;
}
/*
 * 查询是否到达
 */
uint8 Check_angle_arrived(void)
{
//    if(abs(target_angle) < abs(now_angle) )//到达目标角度
//    {
//        now_angle = 0;
//        flag_angle_integral = 0;
//        return 1;
//    }
//    else return 0;
    //换个合理写法
    now_angle=-get_current_yaw();
    if (target_angle > 0 && now_angle > target_angle) {
        // 当目标角度为正数时，当前角度大于等于目标角度表示已经到达目标角度
        now_angle = 0;
        yaw_angle=0;
        target_angle = 0;
        flag_angle_integral = 0;
        stop_yaw();
        return 1;
    } else if (target_angle < 0 && now_angle < target_angle) {
        // 当目标角度为负数时，当前角度小于等于目标角度表示已经到达目标角度
        now_angle = 0;
        target_angle = 0;
        flag_angle_integral = 0;
        stop_yaw();
        return 1;
    } else {
        return 0;
    }
}

