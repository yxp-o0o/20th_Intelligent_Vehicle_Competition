#include "yuansu.h"

CarState MODE=normal;

uint8 circle_flag=0;

float error1;

int16 L_speed=0;
int16 R_speed=0;

void car_mode(void){
  
  if(MODE==nomal_circle){
    output1=Speed_PID_Increment(60-0.6*output3,motor_L.encoder_raw,&speed_pid);
     output2=Speed_PID_Incremen_tow(60+0.6*output3,motor_R.encoder_raw,&speed_pid_tow);  
     
     L_speed=output1;
     R_speed=output2;
   motor(func_limit_ab(L_speed,-5000,5000),func_limit_ab(R_speed,-5000,5000));
   
   if(circle_flag==0)MODE=normal;
   
  }

  if(MODE==normal){
     output1=Speed_PID_Increment(100-output3,motor_L.encoder_raw,&speed_pid);
     output2=Speed_PID_Incremen_tow(100+output3,motor_R.encoder_raw,&speed_pid_tow);  
     
     L_speed=output1;
     R_speed=output2;
   motor(func_limit_ab(L_speed,-5000,5000),func_limit_ab(R_speed,-5000,5000));

if(circle_flag==1 && motor_L.distance>3600)circle_flag=0;
 
   
//     if(func_abs(yaw_angle)>260){
//    if(circle_flag==1){
//     for(uint8 i=MT9V03X_H-1;i>0;i--)
//     mid_line_list[i]=right_line_list[i];
//     circle_flag=0;
//     stop_yaw();
//  }
//    else if(circle_flag==2){
//     for(uint8 i=MT9V03X_H-1;i>0;i--)
//     mid_line_list[i]=left_line_list[i];
//     circle_flag=0;
//     stop_yaw();
//  }
//}

//     if(func_abs(yaw_angle)>220){
//    if(circle_flag==1){
//       stop_yaw();
//      circle_flag=0;
//      for(uint8 i=MT9V03X_H-1;i>0;i--){
//     mid_line_list[i]=right_line_list[i];
//      }
//     }
//    
//    else if(circle_flag==2){
//       stop_yaw();
//      circle_flag=0;
//      for(uint8 i=MT9V03X_H-1;i>0;i--){
//     mid_line_list[i]=left_line_list[i];
//      }
//  }
//}
  }
  
  else if(MODE==turning_left){
     output1=Speed_PID_Increment(35,motor_L.encoder_raw,&speed_pid);
     output2=Speed_PID_Incremen_tow(125,motor_R.encoder_raw,&speed_pid_tow);  
     motor(output1,output2);
      MODE=normal;
//     start_yaw();
//  if(func_abs(yaw_angle)>5){
//    MODE=normal;
//    stop_yaw();
//    }                                                                        //直角50速度，left25,95  right 95,20
  }         
  
  else if(MODE==turning_right){
     output1=Speed_PID_Increment(125,motor_L.encoder_raw,&speed_pid);
     output2=Speed_PID_Incremen_tow(35,motor_R.encoder_raw,&speed_pid_tow);  
     motor(output1,output2);
      MODE=normal;
//      start_yaw();
//  if(yaw_angle>35){
//    MODE=normal;
//    stop_yaw();
//  }
  }
  
  else if(MODE==turning_Left_circle){
    gpio_set_level(P23_7, 1);
    motor_L.total_encoder=0;
    motor_R.total_encoder=0;
    circle_flag=1;
    

    output1=Speed_PID_Increment(20,motor_L.encoder_raw,&speed_pid);
    output2=Speed_PID_Incremen_tow(100,motor_R.encoder_raw,&speed_pid_tow); 
    motor(output1,output2);
    start_yaw();
    
  if(func_abs(yaw_angle)>30){

    gpio_set_level(P23_7, 0);
    MODE=normal;
    
    stop_yaw();                                                                //环岛50速度，left，30，85 right，85，30
  }
// if(func_abs(yaw_angle)>240){
//     for(uint8 i=MT9V03X_H-1;i>0;i--)
//     mid_line_list[i]=right_line_list[i];
//     circle_flag=0;
//     stop_yaw();
//  }
  }
  
  
  else if(MODE==turning_Right_circle){
    gpio_set_level(P23_7, 1);
    motor_L.total_encoder=0;
    motor_R.total_encoder=0;
    circle_flag=1;

    
    output1=Speed_PID_Increment(100,motor_L.encoder_raw,&speed_pid);
    output2=Speed_PID_Incremen_tow(20,motor_R.encoder_raw,&speed_pid_tow); 
    motor(output1,output2);
    start_yaw();
  if(func_abs(yaw_angle)>30){

    gpio_set_level(P23_7, 0);
    MODE=normal;
     stop_yaw();
  }
//  if(func_abs(yaw_angle)>240){
//     for(uint8 i=MT9V03X_H-1;i>0;i--)
//     mid_line_list[i]=right_line_list[i];
//     circle_flag=0;
//     stop_yaw();
//  }

  }
  
   else if(MODE==stoping){
    output1=Speed_PID_Increment(0,motor_L.encoder_raw,&speed_pid);
    output2=Speed_PID_Incremen_tow(0,motor_R.encoder_raw,&speed_pid_tow); 
    motor(output1,output2);
    
}
  
//     
//  if(huandao_flag==1){
//     output1=Speed_PID_Increment(-30,motor_L.encoder_raw,&speed_pid);
//     output2=Speed_PID_Incremen_tow(50,motor_R.encoder_raw,&speed_pid_tow);  
//     motor(output1,output2);
//     system_delay_us(250);
//     huandao_flag=0;
//  }\
  
//   if(huandao_flag==2){
//     output1=Speed_PID_Increment(50,motor_L.encoder_raw,&speed_pid);
//     output2=Speed_PID_Incremen_tow(-30,motor_R.encoder_raw,&speed_pid_tow);  
//     motor(output1,output2);
//     system_delay_us(250);
//     huandao_flag=0;
//  }
//    
//    
//    
    
else if(MODE==turning_left && circle_flag==1){
     output1=Speed_PID_Increment(105,motor_L.encoder_raw,&speed_pid);
     output2=Speed_PID_Incremen_tow(30,motor_R.encoder_raw,&speed_pid_tow);  
     motor(output1,output2);
      MODE=nomal_circle;
// if(func_abs(yaw_angle)>240){
//     for(uint8 i=MT9V03X_H-1;i>0;i--)
//     mid_line_list[i]=right_line_list[i];
//     circle_flag=0;
//     stop_yaw();
//  }
  }

else if(MODE==turning_right && circle_flag==1){
    output1=Speed_PID_Increment(30,motor_L.encoder_raw,&speed_pid);
     output2=Speed_PID_Incremen_tow(105,motor_R.encoder_raw,&speed_pid_tow);  
     motor(output1,output2);
      MODE=nomal_circle;
// if(func_abs(yaw_angle)>240){
//     for(uint8 i=MT9V03X_H-1;i>0;i--)
//     mid_line_list[i]=right_line_list[i];
//     circle_flag=0;
//     stop_yaw();
//  }
  }  
}
//
//
void detect_angle(void){
  
  uint8 find_left_angle=0;
  uint8 find_right_angle=0;
  uint8 find_track_angle=0;

  //左
  for(uint8 i=MT9V03X_H-1;i>15;i-=2){
    if(image[i][10]==255){                   // image[i][MT9V03X_W/5]==0 && 
      find_left_angle=1;     
      break;
  }
  }
  
  //右
   for(uint8 i=MT9V03X_H-1;i>15;i-=2){
    if(image[i][178]==255){                    // image[i][MT9V03X_W/5*4]==0 && 
      find_right_angle=1;
      break;
  }
   }
  
  //中
  for(uint8 i=1;i<MT9V03X_W;i++){
    if(image[25][i]==255){                   //&& image[MT9V03X_H/3*2][i-1]==0
      find_track_angle=1;
      break;
  }
  }
  if(find_left_angle==1 && find_right_angle==0 && find_track_angle==0 && MODE==normal){
    MODE=turning_left;
  }
  else if(find_left_angle==0 && find_right_angle==1 && find_track_angle==0 && MODE==normal){
    MODE=turning_right;
  }  

}

//
//CarState carmode=normal;
//
//CarState detect_angle_tow(void){
//  for(int i=10 ;i<30;i++){
//        if(track_wide[i]>0){
//            return normal;
//        }
//    }
//      //寻找左右识别点
//  int L=0; 
//  int R=0;
//  
//  if(track_wide[45]>0){
//    
//     L = left_line_list[45]-5;   // 转弯误判 改   改小 看的远
//     R = right_line_list[45]+5;  
//  }
//    //边界合法化
//    func_limit_ab(0,187,L);
//     func_limit_ab(0,187,R);
//    
//    //通过识别点计算上方引导线的位置
//    int LD = up_line_list[L];
//    int RD = up_line_list[R];
//if(LD > 30 && RD < 15) return turning_left;
//if(RD > 30 && LD < 15) return turning_right;
//}
//


























