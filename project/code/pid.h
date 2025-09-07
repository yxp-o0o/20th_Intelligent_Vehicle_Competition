#ifndef _pid_h_
#define _pid_h_
#include "zf_common_headfile.h"


extern float kp ;
extern float Ki ;
extern float Kd ;

//extern float mortor1,mortor2;



extern int16 last_chazhi;
extern int out_dn,error_fangxiang;
extern int last_error_fnagxiang;


extern int16 Limt(int16 value, int16 low, int16 high);



typedef struct {
    float kp;          
    float ki;           
    float kd;           
    float ek;           
    float ek1;          
    float ek2;         
    float out;          
    float out_limit_max; 
    float out_limit_min; 
} Speed_PID_TypeDef;


typedef struct {
    float KP;      
    float KP2;     
    float KD;      
    float GKD;      
    float ek;       
    float ek1;     
    float out;      
    float out_limit_min;  
    float out_limit_max;  
} Steering_PID_TypeDef;




//extern Speed_PID_TypeDef_tow 

extern int32 output1;

extern int32 output2;

extern int32 output3;

extern int16 target_speed;

extern int16 target_speed_haundao ;

extern float Speed_PID_Increment(float target_speed, float actual_speed, Speed_PID_TypeDef *pid);

extern void Speed_PID_Init(Speed_PID_TypeDef *pid, float kp, float ki, float kd, float out_min, float out_max);

extern Speed_PID_TypeDef speed_pid;

extern Steering_PID_TypeDef steer_pid;

extern void Steering_PID_Init(Steering_PID_TypeDef *pid, 
                      float kp, float kp2, float kd, float gkd,
                      float out_min, float out_max);

extern float Steering_PID_Calculate(float ind_error, float gyro_value, Steering_PID_TypeDef *pid); 	

extern void Send_PID_DebugData(Steering_PID_TypeDef *pid, float gyro);
 
int Incremental_PI (int Encoder,int Target);







float Speed_PID_Incremen_tow(float target_speed, float actual_speed, Speed_PID_TypeDef *pid);

extern Speed_PID_TypeDef speed_pid_tow;





 #endif