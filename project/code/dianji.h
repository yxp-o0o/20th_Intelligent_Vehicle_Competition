#ifndef _dianji_h_
#define _dianji_h_
#include "zf_common_headfile.h"

#define DIR1                (P00_2)
#define PWM1                (TCPWM_CH13_P00_3)
#define DIR2                (P18_6)
#define PWM2                (TCPWM_CH50_P18_7)

void motor(int32 duty1, int32 duty2);

typedef struct Motor{
  int target_speed;
  int duty;
  int encoder_speed;
  int encoder_raw;
  float distance;
  float total_encoder;
}Motor;

extern struct Motor motor_L;

extern struct Motor motor_R;

void Encoder_Data_Get(void);





#endif



