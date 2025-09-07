
#ifndef _imu_h_
#define _imu_h_
 
 
#include "zf_common_headfile.h"

 
#define uesr_RED     0XF800    //红色
#define uesr_GREEN   0X07E0    //绿色
#define uesr_BLUE    0X001F    //蓝色
 
#define image_h MT9V03X_H//图像高度
#define image_w MT9V03X_W//图像宽度
 
#define white_pixel 255
#define black_pixel 0
 
#define bin_jump_num    1//跳过的点数
#define border_max  image_w-2 //边界最大值
#define border_min  1   //边界最小值
extern uint8 original_image[image_h][image_w];
extern uint8 bin_image[image_h][image_w];//图像数组
 
extern int image_process(void); //直接在中断或循环里调用此程序就可以循环执行了
 
//extern uint8 l_border[image_h];//左线数组
//extern uint8 r_border[image_h];//右线数组
//extern uint8 center_line[image_h];//中线数组
 
//extern current_error;

extern int error_tow;


 
#endif
