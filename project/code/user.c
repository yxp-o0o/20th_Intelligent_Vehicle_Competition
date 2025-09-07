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
//    const int WIDE_WIDTH_TH = 80; // 环岛触发宽度阈值（需大于正常赛道宽度）
//    const int NARROW_WIDTH_TH = 40; // 退出环岛宽度阈值
//    const int MIN_VALID_ROWS = 5; // 状态确认所需连续有效行数
//    const int INNER_OFFSET = 25; // 内切偏移量（根据车体尺寸调整）
//    const int SEARCH_EXPAND = 15; // 环岛内搜索扩展范围
//    
//    static IslandState prev_state = ISLAND_NONE;
//    static int entry_mid_line_list = 0; // 环岛入口中线位置
//    static int island_type = 0; // 0-未定 1-左环岛 2-右环岛
//    static int confirm_counter = 0; // 状态确认计数器
//    int current_mid_line_list = MT9V03X_W/2; // 当前行中线
//    
//    int left_line_list_edge = -1, right_line_list_edge = -1;
//    int search_start, search_end;
// 
//    for (int y = MT9V03X_H-1; y >= 0; y--) { // 从下往上扫描
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
//            /* 进入过渡阶段处理：
//             * 1. 持续监测宽度变化
//             * 2. 确定环岛类型（左/右） */
//            if (valid_width < NARROW_WIDTH_TH) {
//                state = ISLAND_ENTER;
//                confirm_counter = 0;
//            }
//            break;
// 
//        case ISLAND_ENTER:
//            /* 入口处理策略：
//             * 1. 强制中线向环岛内侧偏移
//             * 2. 准备切换为环岛内模式 */
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
//            /* 环岛内部策略：
//             * 1. 固定使用内切中线
//             * 2. 允许边缘不连续（虚线） */
//            if (!valid[y]) { // 处理虚线情况
//                left_line_list_edge = left_line_list[y+1];
//                right_line_list_edge = right_line_list[y+1];
//                valid[y] = 1;
//            }
//            
//            // 持续内切偏移
//            current_mid_line_list = (island_type == 1) ? 
//                           (right_line_list_edge - INNER_OFFSET) : 
//                           (left_line_list_edge + INNER_OFFSET);
// 
//            // 退出条件检测
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
//            /* 出口处理策略：
//             * 1. 渐变回正常中线
//             * 2. 状态恢复准备 */
//            current_mid_line_list = (current_mid_line_list*3 + (left_line_list_edge+right_line_list_edge)/2)/4;
//            if (++confirm_counter >= MIN_VALID_ROWS) {
//                state = ISLAND_NONE;
//                island_type = 0;
//            }
//            break;
//        }
// 
//        //============ 数据更新 ============//
//        left_line_list[y] = left_line_list_edge;
//        right_line_list[y] = right_line_list_edge;
//        mid_line_list[y] = current_mid_line_list;
// 
//        // 历史数据继承（处理无效行）
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
    
 //上左边界   
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
 //下左边界
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
    // 上右边界 
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
 //下右边界
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
    
    
    
    
    
    
    
    
    
    