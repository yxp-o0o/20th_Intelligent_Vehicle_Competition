#ifndef _user_h_
#define _user_h_
#include "zf_common_headfile.h"

extern uint8 huandao_flag;
void huandao(void);

typedef enum {
    ISLAND_NONE, // δ��⵽����
    ISLAND_APPROACH,// �ӽ���������⵽������
    ISLAND_ENTER, // ���ڽ��뻷��
    ISLAND_INNER, // �����ڲ���ʻ
    ISLAND_EXIT // �����˳�����
} IslandState;

void island_handler(const uint8_t* binary_img,
    IslandState state);

extern IslandState zhuang_tai;
extern uint8 valid[MT9V03X_H];

void yuan_huan(void);
//
//void circle(void);
//
//void circle_handle(void);

extern uint8 L_up_time_flag;
extern uint8 L_down_time_flag;

extern uint8 R_up_time_flag;
extern uint8 R_down_time_flag;




//
//if(MODE==huandao){
//  start_yaw();
//  if(yaw_angle>30){
//    MODE=normal;
//    stop_yaw();
//    zuo_turning_flag=1;
//  }
//}
//
//if(zuo_turning_flag=1){
//  start_yaw();
//  if(yaw_angle>320){
//    you_shuzu_






 #endif