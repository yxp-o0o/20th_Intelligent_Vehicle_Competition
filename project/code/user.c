#include "user.h"
uint8 huandao_flag=0;
uint8 circle_handle_flag=0;
//void huandao(void){
//  uint8 zuo_flag=0;
//  uint8 you_flag=0;
//
//  if(track_wide[5]>40 && track_wide[114]>40){
//    if(left_line_list[118]>0 && right_line_list[118]>0){
//      for(uint8 i=MT9V03X_H-1;i>0;i--){
//        if(image[i][func_limit_ab((left_line_list[118]+10),0,187)]==255){
//          zuo_flag=1;
//          break;
//        }
//      } 
//    
//      for(uint8 i=MT9V03X_H-1;i>0;i--){
//        
//          if(image[i][func_limit_ab((right_line_list[118]+10),0,187)]==255){
//          you_flag=1;
//          break;
//        }
//      }
//      if(zuo_flag==1 && you_flag==0){
//      huandao_flag=1;
//      output1=Speed_PID_Increment(-30,motor_L.encoder_raw,&speed_pid);
//      output2=Speed_PID_Incremen_tow(50,motor_R.encoder_raw,&speed_pid_tow);  
//      motor(output1,output2);
//      system_delay_us(250);
//     
//      }
//      else if(zuo_flag==0 && you_flag==1){
//        huandao_flag=2;
//     output1=Speed_PID_Increment(50,motor_L.encoder_raw,&speed_pid);
//     output2=Speed_PID_Incremen_tow(-30,motor_R.encoder_raw,&speed_pid_tow);  
//     motor(output1,output2);
//     system_delay_us(250);
//    }
//    }
//}
//}

//uint8 valid[MT9V03X_H];
//
//
//IslandState zhuang_tai;
//
//void island_handler(
//    const uint8_t* binary_img,
//    IslandState state
//) {
//
//    const int WIDE_WIDTH_TH = 80; // �������������ֵ�����������������ȣ�
//    const int NARROW_WIDTH_TH = 40; // �˳����������ֵ
//    const int MIN_VALID_ROWS = 5; // ״̬ȷ������������Ч����
//    const int INNER_OFFSET = 25; // ����ƫ���������ݳ���ߴ������
//    const int SEARCH_EXPAND = 15; // ������������չ��Χ
//    
//    static IslandState prev_state = ISLAND_NONE;
//    static int entry_mid_line_list = 0; // �����������λ��
//    static int island_type = 0; // 0-δ�� 1-�󻷵� 2-�һ���
//    static int confirm_counter = 0; // ״̬ȷ�ϼ�����
//    int current_mid_line_list = MT9V03X_W/2; // ��ǰ������
//    
//    int left_line_list_edge = -1, right_line_list_edge = -1;
//    int search_start, search_end;
// 
//    for (int y = MT9V03X_H-1; y >= 0; y--) { // ��������ɨ��
//        const uint8_t* row = binary_img + y*MT9V03X_W;
//        
//        search_start = (left_line_list[y+1] - SEARCH_EXPAND > 0) ? 
//                      left_line_list[y+1] - SEARCH_EXPAND : 0;
//        search_end = (state == ISLAND_INNER) ? 
//                    left_line_list[y+1] + SEARCH_EXPAND*2 : 
//                    left_line_list[y+1] + SEARCH_EXPAND;
//        search_end = (search_end < MT9V03X_W) ? search_end : MT9V03X_W-1;
//        
//        for (int x = search_start; x <= search_end; x++) {
//            if (row[x]) { left_line_list_edge = x; break; }
//        }
// 
//        search_start = (right_line_list[y+1] + SEARCH_EXPAND < MT9V03X_W) ? 
//                      right_line_list[y+1] + SEARCH_EXPAND : MT9V03X_W-1;
//        search_end = (state == ISLAND_INNER) ? 
//                    right_line_list[y+1] - SEARCH_EXPAND*2 : 
//                    right_line_list[y+1] - SEARCH_EXPAND;
//        search_end = (search_end >= 0) ? search_end : 0;
//        
//        for (int x = search_start; x >= search_end; x--) {
//            if (row[x]) { right_line_list_edge = x; break; }
//        }
//
//        int valid_width = (right_line_list_edge - left_line_list_edge);
//        valid[y] = (left_line_list_edge != -1 && right_line_list_edge != -1 && 
//                   valid_width > 3 && valid_width < MT9V03X_W-10);
//
//        switch (state) {
//        case ISLAND_NONE:
//
//            if (valid_width > WIDE_WIDTH_TH) {
//                if (++confirm_counter >= MIN_VALID_ROWS) {
//                    state = ISLAND_APPROACH;
//                    entry_mid_line_list = (left_line_list_edge + right_line_list_edge)/2;
//                    island_type = (entry_mid_line_list < MT9V03X_W/2) ? 1 : 2;
//                }
//            } else {
//                confirm_counter = 0;
//            }
//            break;
// 
//        case ISLAND_APPROACH:
//            /* ������ɽ׶δ���
//             * 1. ��������ȱ仯
//             * 2. ȷ���������ͣ���/�ң� */
//            if (valid_width < NARROW_WIDTH_TH) {
//                state = ISLAND_ENTER;
//                confirm_counter = 0;
//            }
//            break;
// 
//        case ISLAND_ENTER:
//            /* ��ڴ�����ԣ�
//             * 1. ǿ�������򻷵��ڲ�ƫ��
//             * 2. ׼���л�Ϊ������ģʽ */
//            current_mid_line_list = (island_type == 1) ? 
//                           (right_line_list_edge - INNER_OFFSET) : 
//                           (left_line_list_edge + INNER_OFFSET);
//            
//            if (++confirm_counter >= MIN_VALID_ROWS) {
//                state = ISLAND_INNER;
//            }
//            break;
// 
//        case ISLAND_INNER:
//            /* �����ڲ����ԣ�
//             * 1. �̶�ʹ����������
//             * 2. �����Ե�����������ߣ� */
//            if (!valid[y]) { // �����������
//                left_line_list_edge = left_line_list[y+1];
//                right_line_list_edge = right_line_list[y+1];
//                valid[y] = 1;
//            }
//            
//            // ��������ƫ��
//            current_mid_line_list = (island_type == 1) ? 
//                           (right_line_list_edge - INNER_OFFSET) : 
//                           (left_line_list_edge + INNER_OFFSET);
// 
//            // �˳��������
//            if (valid_width < NARROW_WIDTH_TH) {
//                if (++confirm_counter >= MIN_VALID_ROWS) {
//                    state = ISLAND_EXIT;
//                }
//            } else {
//                confirm_counter = 0;
//            }
//            break;
// 
//        case ISLAND_EXIT:
//            /* ���ڴ�����ԣ�
//             * 1. �������������
//             * 2. ״̬�ָ�׼�� */
//            current_mid_line_list = (current_mid_line_list*3 + (left_line_list_edge+right_line_list_edge)/2)/4;
//            if (++confirm_counter >= MIN_VALID_ROWS) {
//                state = ISLAND_NONE;
//                island_type = 0;
//            }
//            break;
//        }
// 
//        //============ ���ݸ��� ============//
//        left_line_list[y] = left_line_list_edge;
//        right_line_list[y] = right_line_list_edge;
//        mid_line_list[y] = current_mid_line_list;
// 
//        // ��ʷ���ݼ̳У�������Ч�У�
//        if (!valid[y] && y < MT9V03X_H-1) {
//            left_line_list[y] = left_line_list[y+1];
//            right_line_list[y] = right_line_list[y+1];
//            mid_line_list[y] = mid_line_list[y+1];
//        }
// 
//    }
//
//    for (int y = MT9V03X_H-2; y > 0; y--) {
//        if (valid[y] && valid[y-1] && valid[y+1]) {
//            mid_line_list[y] = (mid_line_list[y-1] + 2*mid_line_list[y] + mid_line_list[y+1])/4;
//        }
//    }
//}
uint8 prepare_flag=0;


uint8 zuo_up[2];     uint8 you_up[2];
uint8 zuo_down[2];   uint8 you_down[2];

uint8 L_up_time_flag=0;
uint8 L_down_time_flag=0;

uint8 R_up_time_flag=0;
uint8 R_down_time_flag=0;




void yuan_huan(void){
  int L_up_time=0;int L_down_time=0;
  int R_up_time=0;int R_down_time=0;
  
    L_up_time_flag=0;
    L_down_time_flag=0;

    R_up_time_flag=0;
    R_down_time_flag=0;
    
 //����߽�   
 for(uint8 j=1;j<MT9V03X_W;j++){
   if(L_up_time>1){
     L_up_time_flag=1;
     break;
   }
      if(image[20][j]==255 && image[20][j-1]==0){
         zuo_up[L_up_time]=j;
         L_up_time++;
         }
 }
 //����߽�
 for(uint8 j=1;j<MT9V03X_W;j++){
   if(L_down_time>1){
     L_down_time_flag=1;
     break;
   }
      if(image[119][j]==255 && image[119][j-1]==0){
         zuo_down[L_down_time]=j;
         L_down_time++;
         }
 }
    // ���ұ߽� 
   for(uint8 j=1;j<MT9V03X_W;j++){
     if(R_up_time>1){
     R_up_time_flag=1;
     break;
     }
      if(image[20][j]==0 && image[20][j-1]==255){
         you_up[R_up_time]=j;
         R_up_time++;
         }
   }
 //���ұ߽�
 for(uint8 j=1;j<MT9V03X_W;j++){
   if(R_down_time>1){
     R_down_time_flag=1;
     break;
   }
      if(image[119][j]==0 && image[119][j-1]==255){
         you_down[R_down_time]=j;
         R_down_time++;
         } 
 }
      
      if(L_up_time_flag==1 && L_down_time_flag==1 && R_up_time_flag==1 && R_down_time_flag==1 && circle_flag==0 ){
            prepare_flag=1;
      }
 
 
 
   if(prepare_flag==1 && L_up_time_flag==0 && L_down_time_flag==1 && R_up_time_flag==0 && R_down_time_flag==1){
     for(uint8 i=MT9V03X_H;i>0;i--){
       if(image[i][4]==255 && image[i][183]==0){
         MODE=turning_Left_circle;
         prepare_flag=0;
         break;
       }
    
      else if(image[i][183]==255 && image[i][4]==0){
         MODE=turning_Right_circle;
         prepare_flag=0;
         break;
       }
}

}
}

   





//
//
//void circle(void){
//  huandao_flag=0;
//  uint8 zuo_circle=0;
//  uint8 you_circle=0;
//  uint8 qian_circle=0;
//  for(uint8 i=0;i<MT9V03X_H;i++){
//    if(image[i][4]==255){
//      zuo_circle=1;
//      break;
//    }
//  }
//   for(uint8 i=0;i<MT9V03X_H;i++){
//    if(image[i][183]==255){
//      you_circle=1;
//      break;
//    }
//   }
//  for(uint8 j=0;j<MT9V03X_W;j++){
//    if(image[0][j]==255){
//      qian_circle=1;
//      break;
//    }
//   }
//  if(qian_circle==1 && zuo_circle==1 && you_circle==0 &&
//    func_abs(right_line_list[20]+right_line_list[30]-right_line_list[25]*2<5) &&
//    func_abs(right_line_list[30]+right_line_list[35]-right_line_list[40]*2<5) &&  
//    func_abs(right_line_list[40]+right_line_list[45]-right_line_list[50]*2<5)){
//    huandao_flag=1;
//    
//  }
//  if(qian_circle==1 && zuo_circle==0 && you_circle==1 && 
//    func_abs(left_line_list[20]+left_line_list[30]-left_line_list[25]*2<5)&&
//    func_abs(left_line_list[30]+left_line_list[35]-left_line_list[40]*2<5)&&
//    func_abs(left_line_list[40]+left_line_list[45]-left_line_list[50]*2<5)){
//    huandao_flag=2;
//  }
//}
//
//void circle_handle(void){
//  if( huandao_flag==1){
//    if(circle_handle_flag!=1 && circle_handle_flag!=2){
//     motor_L.distance=0;
//     circle_handle_flag=1;
//     huandao_flag=0;
//    }
//  }
//     if(circle_handle_flag==1 && motor_L.distance>500){
//     output1=Speed_PID_Increment(-50,motor_L.encoder_raw,&speed_pid);
//     output2=Speed_PID_Incremen_tow(100,motor_R.encoder_raw,&speed_pid_tow);  
//     
//     L_speed=output1;
//     R_speed=output2;
//   motor(func_limit_ab(L_speed,-5000,5000),func_limit_ab(R_speed,-5000,5000));
//   system_delay_ms(1000);
//   circle_handle_flag=0;
//     }
//  
//    if( huandao_flag==2){
//    if(circle_handle_flag!=1 && circle_handle_flag!=2){
//     motor_L.distance=0;
//     circle_handle_flag=2;
//     huandao_flag=0;
//    }
//  }
//     if(circle_handle_flag==2 && motor_L.distance>500){
//     output1=Speed_PID_Increment(100,motor_L.encoder_raw,&speed_pid);
//     output2=Speed_PID_Incremen_tow(-50,motor_R.encoder_raw,&speed_pid_tow);  
//     
//     
//     L_speed=output1;
//     R_speed=output2;
//   motor(func_limit_ab(L_speed,-5000,5000),func_limit_ab(R_speed,-5000,5000));
//   system_delay_ms(1000);
//   circle_handle_flag=0;
//     }
//  
//  
//  
//  
//}
//    
//    
    
    
    
    
    
    
    
    
    
    