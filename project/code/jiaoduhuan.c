#include "zf_common_headfile.h"
#include "jiaoduhuan.h"

long error_ax=0,error_ay=0,error_az=0;
long error_gx=0,error_gy=0,error_gz=0;

float AccX=0, AccY=0, AccZ = 0;
float GyroY = 0, GyroZ = 0;
float dYaw = 0; //Z��һ��������ת���ĽǶ�
float dRoll = 0; //Y��һ��������ת���ĽǶ�
float gyroZangle = 0;

uint8 yaw_flag = 0;  //���ֱ�־λ��1��ʼ����
float yaw_angle = 0; // ��ʼ���ֵĽǶ�

uint8 flag_angle_integral = 0;
float now_angle = 0,target_angle = 0;//Ŀ��Ƕ�

extern int16 imu660ra_gyro_x, imu660ra_gyro_y, imu660ra_gyro_z;                 // ��������������      GYRO (������)
extern int16 imu660ra_acc_x, imu660ra_acc_y, imu660ra_acc_z;                    // ������ٶȼ�����     ACC  (accelerometer ���ٶȼ�)



float d_Time = 0.005f;
void imu660ra_Init(void)
{
    imu660ra_init();//�����ǳ�ʼ��
    Calibrate();//������У׼����
    set_dTime(0.005);//�����ǻ���ʱ��,��Ӧ�ж�ʱ�䣬�����ڵ�λ��
}

//У׼����
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

//�趨��λʱ��(�����������ݸ��µ�ʱ������)
void set_dTime(float d_time)
{
    d_Time = d_time;
}

//MPU���ݸ���
//��������: 1.y����ٶ� 2.���ٶ� 3.�ǶȻ���
//is_suan_angle��1��ʼ����, ��0�������
//�ǶȻ��ֻ������ֵ�ᱣ�ֻ������ֵ
float gyroz_shuzu[16] = {0} ;
float GyroZfinal = 0;
float GyroZAR[3]={0};
void GyroZ_lvbo(void)
{
    uint8 n = 0;

       for(n=0; n<14 ; n++)
             {
           gyroz_shuzu[n] = gyroz_shuzu[n+1];  //    0~14 ��ֵ

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
    AccY=(float)(imu660ra_acc_y)/8192;//y�᷽����ٶ�(��ǰ���ٶ�)
    AccZ=(float)(imu660ra_acc_z+ 4062)/2;

//  GyroX=(float)(mpu6050_gyro_x-error_gx)/32.768;
    GyroY=(float)(imu660ra_gyro_y-error_gy)/32.768;
    GyroZ=(float)(imu660ra_gyro_z-error_gz)/16.8  ;//���ٶȣ�ԭʼ����ת��Ϊ��ÿ��
    GyroZ_lvbo();
    dYaw = GyroZfinal * d_Time;//��z��Ƕ�΢��
    dRoll = GyroY * d_Time;//��y��Ƕ�΢��

    //��ʼyaw����
    if(GyroZfinal>0.5 ||GyroZfinal<(-0.5))
    { if(yaw_flag) {yaw_angle += dYaw;}}
}

//��ȡY����ٶ�
float IMU_get_acc_y(void)
{
    imu660ra_get_acc();
    return (float)(imu660ra_acc_y-error_ay)/8192;
}

//��ȡZ����ٶ�
float IMU_getGyroZ(void)
{
    return GyroZ;
}

//z�Ὺʼ����
void start_yaw(void)
{
    yaw_flag = 1;
}

//�ǵ�ʹ����������־λ
//z��ֹͣ���ֲ������ֽǶ���0
void stop_yaw(void)
{
    yaw_flag = 0;
    yaw_angle = 0;
}

float get_current_yaw(void)
{
    return yaw_angle;
}

//����Ŀ��ת��Ƕ�
//���ú�ͻ�ת��
float angle_to = 0 ; // Ŀ��Ƕ�
float angle_now = 0 ; // ��ǰ�Ƕ�
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



/*����Ŀ��ת�ǣ�����Ԫ���ж�
 * ���룺Ŀ��Ƕȣ���:��;��:��;
 */
void set_target_turn_angle(float turn_angle_)
{
    now_angle= 0;
    flag_angle_integral = 1;
    start_yaw();
    target_angle = turn_angle_;
}
/*
 * ��ѯ�Ƿ񵽴�
 */
uint8 Check_angle_arrived(void)
{
//    if(abs(target_angle) < abs(now_angle) )//����Ŀ��Ƕ�
//    {
//        now_angle = 0;
//        flag_angle_integral = 0;
//        return 1;
//    }
//    else return 0;
    //��������д��
    now_angle=-get_current_yaw();
    if (target_angle > 0 && now_angle > target_angle) {
        // ��Ŀ��Ƕ�Ϊ����ʱ����ǰ�Ƕȴ��ڵ���Ŀ��Ƕȱ�ʾ�Ѿ�����Ŀ��Ƕ�
        now_angle = 0;
        yaw_angle=0;
        target_angle = 0;
        flag_angle_integral = 0;
        stop_yaw();
        return 1;
    } else if (target_angle < 0 && now_angle < target_angle) {
        // ��Ŀ��Ƕ�Ϊ����ʱ����ǰ�Ƕ�С�ڵ���Ŀ��Ƕȱ�ʾ�Ѿ�����Ŀ��Ƕ�
        now_angle = 0;
        target_angle = 0;
        flag_angle_integral = 0;
        stop_yaw();
        return 1;
    } else {
        return 0;
    }
}

