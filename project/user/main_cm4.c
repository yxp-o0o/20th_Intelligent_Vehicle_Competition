/*********************************************************************************************************************
* CYT2BL3 Opensourec Library 即（ CYT2BL3 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 CYT2BL3 开源库的一部分
*
* CYT2BL3 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          main_cm4
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          IAR 9.40.1
* 适用平台          CYT2BL3
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2024-11-19       pudding            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"

// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完

// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设

// **************************** 代码区域 ****************************


int main(void)
{


    clock_init(SYSTEM_CLOCK_160M); 	// 时钟配置及系统初始化<务必保留>
    debug_info_init();                  // 调试串口信息初始化
    
    // 此处编写用户代码 例如外设初始化代码等
   
    tft180_set_dir(TFT180_PORTAIT_180);                                           // 需要先横屏 不然显示不下
    tft180_init();
    mt9v03x_init();
    tft180_show_string(0, 0, "mt9v03x init.");
     while(1)
    {
        if(mt9v03x_init())
            tft180_show_string(0, 16, "mt9v03x reinit.");
        else
            break;
        system_delay_ms(500);                                                   // 闪灯表示异常
    }

    tft180_show_string(0, 16, "init success.");
    system_delay_ms(100);
    tft180_clear();
    
    //蜂鸣器
    gpio_init(P23_7, GPO, GPIO_LOW, GPO_PUSH_PULL);
    
    //电机
    pwm_init(TCPWM_CH13_P00_3, 17000, 0);
    pwm_init(TCPWM_CH50_P18_7, 17000, 0);
    gpio_init(P00_2,GPO,GPIO_HIGH,GPO_PUSH_PULL);
    gpio_init(P18_6, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    
    
    //陀螺仪
    imu660ra_Init();
    
    
    //编码器
    encoder_dir_init(TC_CH09_ENCODER,TC_CH09_ENCODER_CH1_P05_0,TC_CH09_ENCODER_CH2_P05_1);//右
    encoder_dir_init(TC_CH07_ENCODER,TC_CH07_ENCODER_CH1_P02_0,TC_CH07_ENCODER_CH2_P02_1);//左
    
  
    //速度环50速度
//    Speed_PID_Init(&speed_pid,30,4.5,1,-4000,4000);   //左
//    Speed_PID_Init(&speed_pid_tow,30,4.3,1,-4000,4000);   //右
//    Steering_PID_Init(&steer_pid,0.33,0.003,0,0.1,-100,100);
    //80速度
//    Speed_PID_Init(&speed_pid,31,4.53,1.5,-6000,6000);   //左
//    Speed_PID_Init(&speed_pid_tow,31,4.53,1.5,-6000,6000);   //右
//    Steering_PID_Init(&steer_pid,1.8,0.003,1.5,0.3,-120,120);
//    
    
    Speed_PID_Init(&speed_pid,31,4.4,1.8,-5000,5000);   //左
    Speed_PID_Init(&speed_pid_tow,31,4.4,1.8,-5000,5000);   //右
    Steering_PID_Init(&steer_pid,2.3548,0.0033,1.3,0.33,-100,100);
//       
    //中断
    pit_ms_init(PIT_CH1,10);      //速度模式中断
    pit_ms_init(PIT_CH2,5);      //编码器数据读取中断
    pit_ms_init(PIT_CH10,6);      // 陀螺仪中断 
   
   
    
   
    
    // 此处编写用户代码 例如外设初始化代码等
    while(true)  
    {  
    
       
      if(mt9v03x_finish_flag)
        {
          
 

//          
         memcpy(base_image[0],mt9v03x_image[0],MT9V03X_IMAGE_SIZE);
//         
        // img_threshold=Ostu(base_image);
//         
         towzhi(185);
      
       
//       image_process();
//         
    
         
          find_mid_Line_tow();
        
//         find_mid_Line_three();
////
       find_mid_Line_weight();
       
       

//         
         output3=Steering_PID_Calculate(error,GyroZ,&steer_pid);
//         
         

         
  
//       
          mt9v03x_finish_flag =0;
//          
//      }
    // 此处编写需要循环执行的代码
    }
    //    
//        
         //printf("%d,%d,%d,%d\n",motor_L.encoder_raw,motor_R.encoder_raw,40,40);
      
     // send_data_to_uart("%d\n",motor_L.encoder_raw );
//        
//         tft180_show_int(0,140,(int)yaw_angle,3);
    
    tft180_show_uint(100,0,L_up_time_flag,1);
    tft180_show_uint(100,20,L_down_time_flag,1);
    tft180_show_uint(100,40,R_up_time_flag,1);
    tft180_show_uint(100,60,R_down_time_flag,1);
    tft180_show_int(90,80,yaw_angle,3); 
    tft180_show_int(60,80, circle_flag,1); 
    tft180_show_int(0,80, motor_L.distance,5); 
//     tft180_show_uint(0,80,huandao_flag,6);
//     tft180_show_uint(40,80,motor_L.distance,6);//func_limit_ab((left_line_list[118]+10),0,187)
//     tft180_show_uint(80,80,,3);//func_limit_ab((right_line_list[118]+10),0,187)
         
         tft180_show_uint(0,100,mid_line,4);
          tft180_show_uint(40,100,MODE,1);

       
       
//         
         tft180_show_int(0,120,error,4);   
         tft180_show_int(40,120,motor_L.encoder_raw,5);
         tft180_show_int(80,120,motor_R.encoder_raw,5);
       
         tft180_show_int(0,140,output3,5);
//       tft180_show_int(50,140,L_speed,5);
//       tft180_show_int(0,140,R_speed,3);
          
          //draw_line();
     tft180_show_gray_image(0, 0, image[0], MT9V03X_W, MT9V03X_H, 94, 60,0);
    
    
     uint8 heidian=0;
     for(uint8 i=MT9V03X_H-1;i>40;i--){
       for(uint8 j=50;j<138;j++){
         if(image[i][j]==0){
           heidian++;
           break;
       }
       }
       if(heidian>0)break;
     }
     if(heidian==0)MODE=stoping;
    
    
    
    
    
    
}
}
// **************************** 代码区域 ****************************
