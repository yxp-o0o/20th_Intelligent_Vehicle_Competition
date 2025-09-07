
#ifndef _imu_h_
#define _imu_h_
 
 
#include "zf_common_headfile.h"

 
#define uesr_RED     0XF800    //��ɫ
#define uesr_GREEN   0X07E0    //��ɫ
#define uesr_BLUE    0X001F    //��ɫ
 
#define image_h MT9V03X_H//ͼ��߶�
#define image_w MT9V03X_W//ͼ����
 
#define white_pixel 255
#define black_pixel 0
 
#define bin_jump_num    1//�����ĵ���
#define border_max  image_w-2 //�߽����ֵ
#define border_min  1   //�߽���Сֵ
extern uint8 original_image[image_h][image_w];
extern uint8 bin_image[image_h][image_w];//ͼ������
 
extern int image_process(void); //ֱ�����жϻ�ѭ������ô˳���Ϳ���ѭ��ִ����
 
//extern uint8 l_border[image_h];//��������
//extern uint8 r_border[image_h];//��������
//extern uint8 center_line[image_h];//��������
 
//extern current_error;

extern int error_tow;


 
#endif
