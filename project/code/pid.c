
#include "pid.h"





int16 target_speed = 0;

int32 output1=0;
int32 output2=0;
int32 output3=0;
//Speed_PID_TypeDef_tow speed_pid_two;
Speed_PID_TypeDef speed_pid;
Steering_PID_TypeDef steer_pid;



Speed_PID_TypeDef speed_pid_tow;







void Speed_PID_Init(Speed_PID_TypeDef *pid, float kp, float ki, float kd, float out_min, float out_max)
{  
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->ek = 0.0f;
    pid->ek1 = 0.0f;
    pid->ek2 = 0.0f;
    pid->out = 0.0f;
    pid->out_limit_min = out_min;
    pid->out_limit_max = out_max;
}
















void Steering_PID_Init(Steering_PID_TypeDef *pid, 
                      float kp, float kp2, float kd, float gkd,
                      float out_min, float out_max) 
{
    pid->KP = kp;
    pid->KP2 = kp2;
    pid->KD = kd;
    pid->GKD = gkd;
    pid->ek = 0.0f;
    pid->ek1 = 0.0f;
    pid->out = 0.0f;
    pid->out_limit_min = out_min;
    pid->out_limit_max = out_max;
}

float Speed_PID_Increment(float target_speed, float actual_speed, Speed_PID_TypeDef *pid)
{

    pid->ek = target_speed - actual_speed;
    
    
    pid->out += pid->kp * (pid->ek - pid->ek1)    
              + pid->ki * pid->ek                  
              + pid->kd * (pid->ek - 2* pid->ek1 + pid->ek2);  
    
    pid->ek2 = pid->ek1;
    pid->ek1 = pid->ek;
   
    if(pid->out < pid->out_limit_min) {
        pid->out = pid->out_limit_min;
    }
    if(pid->out > pid->out_limit_max) {
        pid->out = pid->out_limit_max;
    }
    
    return pid->out;
}






float Speed_PID_Incremen_tow(float target_speed, float actual_speed, Speed_PID_TypeDef *pid)
{

    pid->ek = target_speed - actual_speed;
    
    
    pid->out += pid->kp * (pid->ek - pid->ek1)    
              + pid->ki * pid->ek                  
              + pid->kd * (pid->ek - 2* pid->ek1 + pid->ek2);  
    
    pid->ek2 = pid->ek1;
    pid->ek1 = pid->ek;
   
    if(pid->out < pid->out_limit_min) {
        pid->out = pid->out_limit_min;
    }
    if(pid->out > pid->out_limit_max) {
        pid->out = pid->out_limit_max;
    }
    
    return pid->out;
}


















float Steering_PID_Calculate(float ind_error, float gyro_value, Steering_PID_TypeDef *pid) 
{
	float proportional,quadratic,derivative,gyro_comp; 
    pid->ek = ind_error;
     proportional = pid->KP * pid->ek;
     quadratic = pid->KP2 * pid->ek * func_abs(pid->ek);
     derivative = pid->KD * (pid->ek - pid->ek1);
     gyro_comp = pid->GKD * gyro_value;

    pid->out = proportional + quadratic + gyro_comp;


 
    pid->ek1 = pid->ek;

    
    if (pid->out < pid->out_limit_min) {
        pid->out = pid->out_limit_min;
    } else if (pid->out > pid->out_limit_max) {
        pid->out = pid->out_limit_max;
    }

    return pid->out;
}
int Incremental_PI (int Encoder,int Target)
{ 	
     float Kp=1,Ki=0.2;	
	 static int Bias,Pwm,Last_bias;
	 Bias=Target - Encoder;                //计算偏差
	 Pwm+=Kp*(Bias-Last_bias)+Ki*Bias;   //增量式PI控制器
	 Last_bias=Bias;	                 //保存上一次偏差
	 return (int32)Pwm;  
}