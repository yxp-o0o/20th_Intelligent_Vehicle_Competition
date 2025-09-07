/*********************************************************************************************************************
* CYT2BL3 Opensourec Library ���� CYT2BL3 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� CYT2BL3 ��Դ���һ����
*
* CYT2BL3 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          main_cm4
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 9.40.1
* ����ƽ̨          CYT2BL3
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2024-11-19       pudding            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"

// ���µĹ��̻��߹����ƶ���λ�����ִ�����²���
// ��һ�� �ر��������д򿪵��ļ�
// �ڶ��� project->clean  �ȴ��·�����������

// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������

// **************************** �������� ****************************


int main(void)
{


    clock_init(SYSTEM_CLOCK_160M); 	// ʱ�����ü�ϵͳ��ʼ��<��ر���>
    debug_info_init();                  // ���Դ�����Ϣ��ʼ��
    
    // �˴���д�û����� ���������ʼ�������
   
    tft180_set_dir(TFT180_PORTAIT_180);                                           // ��Ҫ�Ⱥ��� ��Ȼ��ʾ����
    tft180_init();
    mt9v03x_init();
    tft180_show_string(0, 0, "mt9v03x init.");
     while(1)
    {
        if(mt9v03x_init())
            tft180_show_string(0, 16, "mt9v03x reinit.");
        else
            break;
        system_delay_ms(500);                                                   // ���Ʊ�ʾ�쳣
    }

    tft180_show_string(0, 16, "init success.");
    system_delay_ms(100);
    tft180_clear();
    
    //������
    gpio_init(P23_7, GPO, GPIO_LOW, GPO_PUSH_PULL);
    
    //���
    pwm_init(TCPWM_CH13_P00_3, 17000, 0);
    pwm_init(TCPWM_CH50_P18_7, 17000, 0);
    gpio_init(P00_2,GPO,GPIO_HIGH,GPO_PUSH_PULL);
    gpio_init(P18_6, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    
    
    //������
    imu660ra_Init();
    
    
    //������
    encoder_dir_init(TC_CH09_ENCODER,TC_CH09_ENCODER_CH1_P05_0,TC_CH09_ENCODER_CH2_P05_1);//��
    encoder_dir_init(TC_CH07_ENCODER,TC_CH07_ENCODER_CH1_P02_0,TC_CH07_ENCODER_CH2_P02_1);//��
    
  
    //�ٶȻ�50�ٶ�
//    Speed_PID_Init(&speed_pid,30,4.5,1,-4000,4000);   //��
//    Speed_PID_Init(&speed_pid_tow,30,4.3,1,-4000,4000);   //��
//    Steering_PID_Init(&steer_pid,0.33,0.003,0,0.1,-100,100);
    //80�ٶ�
//    Speed_PID_Init(&speed_pid,31,4.53,1.5,-6000,6000);   //��
//    Speed_PID_Init(&speed_pid_tow,31,4.53,1.5,-6000,6000);   //��
//    Steering_PID_Init(&steer_pid,1.8,0.003,1.5,0.3,-120,120);
//    
    
    Speed_PID_Init(&speed_pid,31,4.4,1.8,-5000,5000);   //��
    Speed_PID_Init(&speed_pid_tow,31,4.4,1.8,-5000,5000);   //��
    Steering_PID_Init(&steer_pid,2.3548,0.0033,1.3,0.33,-100,100);
//       
    //�ж�
    pit_ms_init(PIT_CH1,10);      //�ٶ�ģʽ�ж�
    pit_ms_init(PIT_CH2,5);      //���������ݶ�ȡ�ж�
    pit_ms_init(PIT_CH10,6);      // �������ж� 
   
   
    
   
    
    // �˴���д�û����� ���������ʼ�������
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
    // �˴���д��Ҫѭ��ִ�еĴ���
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
// **************************** �������� ****************************
