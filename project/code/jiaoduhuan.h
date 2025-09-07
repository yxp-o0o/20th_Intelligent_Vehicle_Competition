
#ifndef CODE_JIAODUHUAN_H_
#define CODE_JIAODUHUAN_H_

#include "zf_device_imu660ra.h"
#include "math.h"
#define RAD_TO_DEG 57.2957   // ����ת�Ƕȵ�ת����57.295779513082320876798154814105
#define DEG_TO_RAD 0.0174   // �Ƕ�ת���ȵ�ת����0.01745329251994329576923690768489

extern long error_gx,error_gy,error_gz;
extern float GyroY, GyroZ;//���ٶ���תΪ����תΪ��
extern float dYaw;  //X��һ��������ת���ĽǶ�
extern float dRoll; //Y��һ��������ת���ĽǶ�
extern float AccX, AccY, AccZ;
extern float yaw_angle;
extern  uint8 flag_angle_integral;
extern float now_angle ,target_angle;//Ŀ��Ƕ�
extern float gyroz_shuzu[16]  ;
extern float GyroZfinal;
extern float GyroZAR[3];
extern void GyroZ_lvbo(void);
#define MPU_getAccZ() AccZ //��ȡZ����ٶ�

//�����ǳ�ʼ��
void imu660ra_Init(void);

//У׼����
void Calibrate(void);

//�趨��λʱ��(�����������ݸ��µ�ʱ������)
void set_dTime(float d_time);

//��������������
void IMU_update(void);


void start_yaw(void);
void stop_yaw(void);
float get_current_yaw(void);
float IMU_getGyroZ(void);

void set_target_turn_angle(float turn_angle_);
uint8 Check_angle_arrived(void);
#endif /* CODE_JIAODUHUAN_H_ */



