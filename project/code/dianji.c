
#include "dianji.h"
                                             // 最大 MAX_DUTY% 占空比
#define min -8000
#define max  8000
void motor(int32 duty1, int32 duty2)
{
  if (duty1 > max) duty1 = max;
    if (duty1 < min) duty1 = min; 
      if (duty2 > max) duty2 = max; 
        if (duty2 < min) duty2 = min; 
  
  
  if(duty1 >= 0)                                                           // 正转
        {
            gpio_set_level(DIR1, GPIO_LOW);                                    // DIR输出高电平
            pwm_set_duty(PWM1, duty1);                    // 计算占空比
           
        }
        else                                                                    // 反转
        {
            gpio_set_level(DIR1, GPIO_HIGH);                                     // DIR输出低电平
            pwm_set_duty(PWM1, -duty1);                 // 计算占空比
                   // 计算占空比
        }
  
   if(duty2 >= 0)                                                           // 正转
        {
            gpio_set_level(DIR2, GPIO_LOW);                                    // DIR输出高电平
            pwm_set_duty(PWM2, duty2);                    // 计算占空比
           
        }
        else                                                                    // 反转
        {
            gpio_set_level(DIR2, GPIO_HIGH);                                     // DIR输出低电平
            pwm_set_duty(PWM2, -duty2);                 // 计算占空比
         }
}

Motor motor_L;
Motor motor_R;










void Encoder_Data_Get(void){
  motor_L.encoder_raw=encoder_get_count(TC_CH07_ENCODER);
  motor_L.encoder_speed=(int)(0.2*motor_L.encoder_speed+0.8*motor_L.encoder_raw);
  motor_L.total_encoder+=(2*3.14*24)/1024*motor_L.encoder_raw;
  encoder_clear_count(TC_CH07_ENCODER);
  
  
  motor_R.encoder_raw=-encoder_get_count(TC_CH09_ENCODER);
  motor_R.encoder_speed=(int)(0.2*motor_R.encoder_speed+0.8*motor_R.encoder_raw);
  motor_R.total_encoder+=(2*3.14*24)/1024*motor_R.encoder_raw;
  encoder_clear_count(TC_CH09_ENCODER);
  motor_L.distance=(motor_L.total_encoder+motor_R.total_encoder)/2;
}



/*#include <stdio.h>
#include <stdbool.h>

// 图像尺寸定义
#define IMAGE_WIDTH   160
#define IMAGE_HEIGHT  120

// 引导线像素值定义
#define GUIDE_LINE_COLOR 255  // 假设引导线是白色(255)
#define BACKGROUND_COLOR 0    // 假设背景是黑色(0)

// 差速控制参数
#define NORMAL_SPEED_DIFF 30  // 正常循迹时的差速
#define TURN_SPEED_DIFF   80  // 直角转弯时的固定差速

// 图像数据数组(模拟摄像头采集的图像)
unsigned char image[IMAGE_HEIGHT][IMAGE_WIDTH];

// 函数声明
bool searchLeftGuideLine();
bool searchRightGuideLine();
bool searchTopGuideLine();
void normalTracking();
void rightAngleTurn();
void processImage();


 * 从图像左侧自下向上搜索引导线
 * @return true 如果找到引导线, false 否则
 
bool searchLeftGuideLine() {
    // 从底部开始向上搜索(假设底部是图像的最后一行)
    for (int y = IMAGE_HEIGHT - 1; y >= IMAGE_HEIGHT / 2; y--) {
        // 检查左侧一定范围内的像素(例如左侧1/4宽度)
        for (int x = 0; x < IMAGE_WIDTH / 4; x++) {
            if (image[y][x] == GUIDE_LINE_COLOR) {
                return true;  // 找到引导线
            }
        }
    }
    return false;  // 未找到引导线
}


 * 从图像右侧自下向上搜索引导线
 * @return true 如果找到引导线, false 否则
 
bool searchRightGuideLine() {
    // 从底部开始向上搜索
    for (int y = IMAGE_HEIGHT - 1; y >= IMAGE_HEIGHT / 2; y--) {
        // 检查右侧一定范围内的像素(例如右侧1/4宽度)
        for (int x = IMAGE_WIDTH * 3 / 4; x < IMAGE_WIDTH; x++) {
            if (image[y][x] == GUIDE_LINE_COLOR) {
                return true;  // 找到引导线
            }
        }
    }
    return false;  // 未找到引导线
}


 * 从图像上方自左向右搜索引导线
 * @return true 如果找到引导线, false 否则
 
bool searchTopGuideLine() {
    // 在图像上半部分搜索(例如上半部分1/3高度)
    for (int y = 0; y < IMAGE_HEIGHT / 3; y++) {
        // 从左到右搜索整个宽度
        for (int x = 0; x < IMAGE_WIDTH; x++) {
            if (image[y][x] == GUIDE_LINE_COLOR) {
                return true;  // 找到引导线
            }
        }
    }
    return false;  // 未找到引导线
}


 * 正常循迹模式

void normalTracking() {
    // 设置正常差速
    printf("正常循迹模式 - 差速: %d\n", NORMAL_SPEED_DIFF);
    // 这里可以添加具体的电机控制代码
}


 * 直角转弯处理
 
void rightAngleTurn() {
    printf("进入直角转弯模式\n");
    
    // 设置固定差速进行转弯
    printf("设置固定差速: %d\n", TURN_SPEED_DIFF);
    // 这里可以添加具体的电机控制代码
    
    // 持续检测图像上部是否出现引导线
    while (true) {
        // 获取新一帧图像(在实际应用中需要从摄像头获取)
        // processNewImage();  // 假设有这个函数
        
        if (searchTopGuideLine()) {
            printf("检测到上部引导线，返回正常循迹模式\n");
            normalTracking();
            break;
        }
        
        // 添加适当的延时，避免过于频繁的检测
        // delay(50);  // 假设有延时函数
    }
}


 * 处理图像并判断直角弯
 
void processImage() {
    // 检查直角弯的三个条件
    bool leftFound = searchLeftGuideLine();
    bool rightFound = searchRightGuideLine();
    bool topFound = searchTopGuideLine();
    
    printf("检测结果 - 左侧: %d, 右侧: %d, 上部: %d\n", 
           leftFound, rightFound, topFound);
    
    // 判断是否为直角弯的条件:
    // 1. 左侧自下向上搜索到引导线
    // 2. 右侧自下向上没有搜到引导线
    // 3. 图像上方自左向右没有搜到引导线
    if (leftFound && !rightFound && !topFound) {
        printf("检测到直角弯\n");
        rightAngleTurn();  // 进入直角弯处理
    } else {
        normalTracking();   // 正常循迹模式
    }
}

int main() {
    // 模拟主循环
    while (1) {
        // 这里应该从摄像头获取图像数据填充到image数组中
        // captureImage(image);  // 假设有这个函数
        
        // 处理图像并判断直角弯
        processImage();
        
        // 控制循环速度
        // delay(100);  // 假设有延时函数
    }
    
    return 0;
}
*/



