
#ifndef _yuansu_h_
#define _yuansu_h_
#include "zf_common_headfile.h"


typedef enum{
    normal,          // 正常循迹模式
    turning_left,   //左直角转弯中
    turning_right, // 右直角转弯中   
    turning_Left_circle,
    turning_Right_circle,
    stoping,
    nomal_circle,
}CarState;


extern CarState carmode;

extern CarState MODE;

void car_mode(void);

void detect_angle(void);

CarState detect_angle_tow(void);

extern int16 L_speed;
extern int16 R_speed;

extern uint8 circle_flag;
#endif