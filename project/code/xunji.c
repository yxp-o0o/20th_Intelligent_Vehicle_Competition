#include "zf_common_headfile.h"
#include "math.h"


uint8 base_image[MT9V03X_H][MT9V03X_W];
uint8 image[MT9V03X_H][MT9V03X_W];

float P[GrayScale];
float PK[GrayScale];
float MK[GrayScale];
uint8  img_threshold;

uint16 hist[GrayScale]={0};

uint8 Ostu(uint8 index[MT9V03X_H][MT9V03X_W]){
 
  uint8 threshold;
  uint8 images_value_temp;
  
  float sumPK=0;
  float sumMK=0;
  float var=0;
  float vartmp=0;
  
  for(uint16 i=0;i<GrayScale;i++){
    hist[i]=0;
    P[i]=0;
    PK[i]=0;
    MK[i]=0;
  }
  
  for(uint8 i=0;i<MT9V03X_H;i++){
    for(uint8 j=0;j<MT9V03X_W;j++){
       images_value_temp=index[i][j];
       hist[images_value_temp]++;
    }
  }
  
  for(uint16 i=0;i<GrayScale;i++){
    P[i]=(float)hist[i]/MT9V03X_IMAGE_SIZE;
    PK[i]=sumPK+P[i];
    sumPK=PK[i];
    MK[i]=sumMK+i*P[i];
    sumMK=MK[i];
  }
  for(uint8 i=5;i<245;i++){
    vartmp=((MK[GrayScale-1]*PK[i]-MK[i])*(MK[GrayScale-1]*PK[i]-MK[i]))/(PK[i]*(1-PK[i]));
    if(vartmp>var){
      var=vartmp;
      threshold=i;
    }
  }
  return threshold;
 }
  
 



//二值化函数
void towzhi(uint8 value){
  uint8 temp_value;
  
  for(uint8 i=0;i<MT9V03X_H;i++){ 
    for(uint8 j=0;j<MT9V03X_W;j++){
      temp_value=mt9v03x_image[i][j];
      if(temp_value<value){
        image[i][j]=0;
      }
      else{
        image[i][j]=255;
      }
    }
  }
}




//巡线函数
uint16 left_line_list[MT9V03X_H];
uint16 right_line_list[MT9V03X_H];
uint16 mid_line_list[MT9V03X_H];
uint16 track_wide[MT9V03X_H];
uint8 saikuan=0;

uint16 up_line_list[MT9V03X_W];
uint16 down_line_list[MT9V03X_W];
uint16 track_wide_high[MT9V03X_W];
uint8 saikuan_high=0;


void qing_kong_shu_zu(void){
  for(uint8 i=0;i<MT9V03X_H;i++){
     left_line_list[i]=0;
     right_line_list[i]=0;
     mid_line_list[i]=0;
     track_wide[i]=0;
  }
}



//左右搜线
void find_mid_Line_tow(void){
 
  uint8 left_find_flag=0;
  uint8 right_find_flag=0;
  
  
  uint8 left_point=1;
  uint8 right_point=MT9V03X_W-1;
  
  qing_kong_shu_zu();
   
  
  for(uint8 i=MT9V03X_H-1;i>0;i--){
    
     left_point=1;
     right_point=MT9V03X_W-1;
     
     left_find_flag=0;
     right_find_flag=0;
     saikuan=0;
  
     
     
       for(uint8 j=1;j<MT9V03X_W;j++){
      if(image[i][j]==255){
          left_point=j;
          left_find_flag=1;
          break;
        }
       }
       
       
      for(uint8 j=MT9V03X_W-1;j>0;j--){
         if(image[i][j-1]==255){
          right_point=j;
          right_find_flag=1;
          break;
      }
     }
    
    if(left_find_flag==1 && right_find_flag==1){
      
      saikuan=func_abs(right_point-left_point);
      
      if(saikuan<100 && saikuan>1){
    left_line_list[i]=left_point;//
    right_line_list[i]=right_point;//
    mid_line_list[i]=(left_point+right_point)/2;
    track_wide[i]=saikuan;
      }
      else{
        mid_line_list[i]=mid_line_list[i+1];
      }
   //track_wide[i]=func_abs(right_point-left_point);
   
    }
    
  else if(left_find_flag==0 && right_find_flag==1){
       right_line_list[i]=right_point;
      
   for(uint8 j=1;j<MT9V03X_W;j++){
      if(image[i][j]==255){
          left_point=j;
          break;
        }
      }   
    
   
   saikuan=func_abs(right_point-left_point);
   
   if(saikuan<100 && saikuan>1){
    mid_line_list[i]=(left_point+right_point)/2;
    }  
  else{
        mid_line_list[i]=mid_line_list[i+1];
      }
  } 
    
    else if(left_find_flag==1 && right_find_flag==0){
     left_line_list[i]=left_point;
      
      for(uint8 j=MT9V03X_W-1;j>0 ;j--){
         if(image[i][j]==255){
          right_point=j;
          break;
      }
       
       
       saikuan=func_abs(right_point-left_point);
   
   if(saikuan<100 && saikuan>1){
      
    mid_line_list[i]=(left_point+right_point)/2;
    }  
  else{
        mid_line_list[i]=mid_line_list[i+1];
      }
    }
    }   
    
    
    else if(left_find_flag==0 && right_find_flag==0 && i<115)
    {
     mid_line_list[i]=mid_line_list[i+1];
    }
  }   
//    else if((left_find_flag==0 || right_find_flag==0) && i<119){
//      left_line_list [i]=left_line_list [i+1];
//      right_line_list[i]=right_line_list[i+1];
//      track_wide[i]=track_wide[i+1];
//     mid_line_list[i]=mid_line_list[i+1];
//  }
}


void qing_kong_shu_zu_tow(void){
  for(uint8 i=0;i<MT9V03X_H;i++){
    
up_line_list[i]=0;
down_line_list[i]=0;
track_wide_high[i]=0;

  }
}




void find_mid_Line_three(void){
 
  uint8 up_find_flag=0;
  uint8 down_find_flag=0;
  
  
  uint8 up_point=1;
  uint8 down_point=MT9V03X_H-1;
  qing_kong_shu_zu_tow();
 
   
  
 for(uint8 j=0;j<MT9V03X_W;j++){
    
     up_find_flag=0;
     down_find_flag=0;
     saikuan_high=0;
  
     
     
       for(uint8 i=MT9V03X_H-1;i>1;i--){
      if(image[i-1][j]==255){
          down_point=i;
          down_find_flag=1;
          break;
        }
       }
       
       
      for(uint8 i=1;i<MT9V03X_H;i++){
         if(image[i][j]==255){
          up_point=i;
          up_find_flag=1;
          break;
      }
      }
      if(down_find_flag==1 && up_find_flag==1){
      saikuan_high=func_abs(up_point-down_point);
      
      
    up_line_list[j]=up_point;//
    down_line_list[j]=down_point;//
    track_wide_high[j]=saikuan_high;
      }
   
      }
}


//起始点
//  uint8 qi_shi_left[2] = { 0 };//左边起点的x，y值
//  uint8 qi_shi_right[2] = { 0 };//右边起点的x，y值
//
//uint8 qi_shi_dian(uint8 qi_shi_row){
//    uint8 i = 0,left_found_flag = 0,right_found_flag = 0;
//    //清零
//    qi_shi_left[0] = 0;//x
//    qi_shi_left[1] = 0;//y
// 
//    qi_shi_right[0] = 0;//x
//    qi_shi_right[1] = 0;//y
// 
//                     
//            
//     for(i=0;i<MT9V03X_W-1;i++) 
//    {
//        qi_shi_left[0] = i;//x
//        qi_shi_left[1] = qi_shi_row;//y
//        if (image[qi_shi_row][i] == 255 && image[qi_shi_row][i - 1] == 0)
//        {
//            //printf("找到左边起点image[%d][%d]\n", qi_shi_row,i);
//            left_found_flag = 1;
//            break;
//        }
//    }
// 
//    
//     for(i=MT9V03X_W-1;i>0;i--)
//    {
//        qi_shi_right[0] = i;//x
//        qi_shi_right[1] = qi_shi_row;//y
//        if (image[qi_shi_row][i] == 255 && image[qi_shi_row][i + 1] == 0)
//        {
//            //printf("找到右边起点image[%d][%d]\n",qi_shi_row, i);
//            right_found_flag = 1;
//            break;
//        }
//    }
//    if(left_found_flag && right_found_flag)return 1;
//    else {
//        //printf("未找到起点\n");
//        return 0;
//    }
//}  

//  
////巡线函数
//uint8 zuo_shu_zu[MT9V03X_H];
//uint8 you_shu_zu[MT9V03X_H];
//uint8 zhong_shu_zu[MT9V03X_H];
//
//                                                 //func_limit();
//uint8 last_mid_point=94;
//
//void find_mid_Line_four(void){
// 
//  last_mid_point=(qi_shi_left[0]+qi_shi_right[0])/2;
//  
//  uint8 left_find_flag=0;
//
//  uint8 right_find_flag=0;
// 
//  uint8 left_pointd=1;
//  uint8 right_pointd=MT9V03X_W-1;
//  uint8 j=func_limit(1,187);
//  
//  for(uint8 i=MT9V03X_H-2;i>0;i--){
//    
//   left_find_flag=0;
//    right_find_flag=0;
//    
//    for(j=last_mid_point-20;j<last_mid_point+20;j++){
//      if(image[i][j]==255 && image[i][j-1]==0){
//          left_pointd=j;
//          left_find_flag=1;
//          break;
//        }
//      
//    }
//     for(j=last_mid_point+20;j>last_mid_point-20 ;j--){
//         if(image[i][j]==0 && image[i][j-1]==255){
//          right_pointd=j;
//          right_find_flag=1;
//          break;
//      }
//     }
//    if(left_find_flag && right_find_flag){
//    
//      
//      zuo_shu_zu [i]=left_pointd;//
//   you_shu_zu[i]=right_pointd;//
// 
//   track_wide[i]=func_abs(right_pointd-left_pointd);
//   
//    zhong_shu_zu[i]=(left_pointd+right_pointd)/2;
//    }
//    
//    else if(left_find_flag==1 && right_find_flag==0){
//      zhong_shu_zu[i]=left_pointd+10;
//    }
//    
//    else if(left_find_flag==0 && right_find_flag==1){
//      zhong_shu_zu[i]=right_pointd-10;
//    }
//    
//    else{
//      zhong_shu_zu[i]=zhong_shu_zu[i+1];
//      
//      
//      
//      
//      
//      
//      
//      
//      
//      
////    else if((left_find_flag==0 || right_find_flag==0) && i<119){
////      left_line_list [i]=left_line_list [i+1];
////      right_line_list[i]=right_line_list[i+1];
////      track_wide[i]=track_wide[i+1];
////     mid_line_list[i]=mid_line_list[i+1];
////  }
//}
//}
//
//}


//权重
uint8 weight_midline[120]={
                               1,1,1,1,1,1,1,1,1,1,//图像0-10行权重
                               1,1,1,1,1,1,1,1,1,1,//图像10-20行权重  23--60cm
                               1,1,1,1,1,1,1,1,1,1,//图像20-30行权重
                               1,1,1,1,1,1,1,1,1,1,//图像30-40行权重
                               20,19,19,18,18,15,15,14,13,12,//图像40-50行权重
                               10,10,10,9,9,9,9,8,8,8,//图像50-60行权重
                               8,8,7,7,7,7,6,6,6,6,//图像60-70行权重
                               5,5,5,4,4,1,1,1,1,1,//图像70-80行权重
                               1,1,1,1,1,1,1,1,1,1,
                               1,1,1,1,1,1,1,1,1,1,
                               1,1,1,1,1,1,1,1,1,1,
                               1,1,1,1,1,1,1,1,1,1,
//  1,1,1,1,1,1,1,1,1,1,    
//  1,1,1,1,1,2,2,2,2,2,
//  2,2,2,2,2,2,2,2,2,2,
//  3,3,3,3,3,4,4,4,4,4,
//  4,4,4,4,4,4,4,4,4,4,
//  5,5,5,5,5,5,5,5,5,5,
//  7,7,8,8,9,9,10,10,10,10,
//  10,10,10,10,10,10,10,10,10,10,    //9,9,8,8,7,7,6,6,5,5,                                       
//  9,9,8,8,7,7,6,6,5,5,
//  3,3,3,3,3,3,3,3,3,3,
//  2,2,2,2,2,2,2,2,2,2,
//  1,1,1,1,1,1,1,1,1,1,
  
};



//权重赋值计算误差
//uint16 mid_line_error=0;
//int16 error=0;
//uint16 mid_line=94; 
//int16 error_line[MT9V03X_H];
//
//void find_mid_Line_weight(void){
//  uint16 weight_sum=0;
//  
//  uint8 mid_diuxian_flag=0;
//  uint32 weight_midline_sum=0;
//  
//  
//  for(uint8 i=MT9V03X_H-1;i>0;i--){
//    if(i==MT9V03X_H-1){
//      error_line[i]=(94-mid_line_list[i])*weight_midline[i];
//      weight_midline_sum+=mid_line_list[i]*weight_midline[i];
//      weight_sum+=weight_midline[i];
//    }
//   else{
//      mid_line_error=func_abs(mid_line_list[i]-mid_line_list[i+1]);
//      
//      if(mid_diuxian_flag==1){
//        continue;
//      }
//      else if(mid_line_error>25 && mid_diuxian_flag==0){
//        mid_diuxian_flag=1;
//        continue;
//      }
//      else{
//     
//     
//     error_line[i]=(94-mid_line_list[i])*weight_midline[i];
//       weight_midline_sum+=mid_line_list[i]*weight_midline[i];
//       weight_sum+=weight_midline[i];
//      }
//   }
//  }
//  
//  
//  
//  mid_line=weight_midline_sum/weight_sum;
//
//  error=94-mid_line;
//
//}
//   

uint16 mid_line_error=0;
int16 error=0;
uint16 mid_line=94; 


void find_mid_Line_weight(void){
  uint16 weight_sum=0;
  
 
  uint32 weight_midline_sum=0;
  
  
  for(uint8 i=MT9V03X_H-1;i>0;i--){
   
    
      weight_midline_sum+=mid_line_list[i]*weight_midline[i];
      weight_sum+=weight_midline[i];
    }

  
  
  mid_line=weight_midline_sum/weight_sum;

  if(mid_line>0)error=94-mid_line;

}
   

  

//
//
//
//  
////划线
//void draw_line(void){
//  for(uint8 i=MT9V03X_H-1;i>0;i--){
//    tft180_draw_point((int)(0.5*left_line_list[i]),(int)(0.5*i), RGB565_RED);//显示起点 显示中线
//       tft180_draw_point((int)(0.5*right_line_list[i]), (int)(0.5*i), RGB565_BLUE);//显示起点 显示左边线
//       tft180_draw_point((int)(0.5*mid_line_list[i]), (int)(0.5*i), RGB565_GREEN);//显示起点 显示右边线
//    
//      
//      
//  }
//}
//
//
//
//
//
//bool searchGuidelineTop(int startX, int endX) {
//    int topY =  MT9V03X_H/ 4; // 搜索上方1/4区域
//    for (int x = startX; x <= endX; x++) {
//        for (int y = 0; y < topY; y++) {
//            if (image[y][x] > 150) {
//                return true;
//            }
//        }
//        
//      
//        
//    }
//    return false;
//}
//
//
//float zhuanjiao=0; 
//float zhuanjiao_sum=0;
//
//
//void zhuanxiang(int W){
//  while(W==1){
//    motor(1000,-1000);
//     imu660ra_get_gyro();
//     
//  zhuanjiao=imu660ra_gyro_z/100;
//  
//  zhuanjiao_sum+=zhuanjiao;
//  
//  zhuanjiao=0;
//  if(zhuanjiao_sum>100)
//    zhuanjiao_sum=0;
//  W=0;
//    break;
//  }
// 
//  while(W==2){
//     motor(1000,-1000);
//   imu660ra_get_gyro();
//   
//  zhuanjiao=imu660ra_gyro_z/100;
//  
//  zhuanjiao_sum+=zhuanjiao;
//  zhuanjiao=0;
//  if(zhuanjiao_sum<-100){
//    zhuanjiao_sum=0;
//    W=0;
//    break;
//  }  
//  }
// 
//}
// 
//
//
//void yanshi(int Y){
//  if(Y==1){
//    motor(800,-800);
//      system_delay_us(500);
//  }
//   if(Y==2){
//    motor(-800,800);
//      system_delay_us(500);
//  }
//  Y=0;
//}
//
//
//
//
//
//
//
//
//
//
//










