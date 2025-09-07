
#include "dianji.h"
                                             // ��� MAX_DUTY% ռ�ձ�
#define min -8000
#define max  8000
void motor(int32 duty1, int32 duty2)
{
  if (duty1 > max) duty1 = max;
    if (duty1 < min) duty1 = min; 
      if (duty2 > max) duty2 = max; 
        if (duty2 < min) duty2 = min; 
  
  
  if(duty1 >= 0)                                                           // ��ת
        {
            gpio_set_level(DIR1, GPIO_LOW);                                    // DIR����ߵ�ƽ
            pwm_set_duty(PWM1, duty1);                    // ����ռ�ձ�
           
        }
        else                                                                    // ��ת
        {
            gpio_set_level(DIR1, GPIO_HIGH);                                     // DIR����͵�ƽ
            pwm_set_duty(PWM1, -duty1);                 // ����ռ�ձ�
                   // ����ռ�ձ�
        }
  
   if(duty2 >= 0)                                                           // ��ת
        {
            gpio_set_level(DIR2, GPIO_LOW);                                    // DIR����ߵ�ƽ
            pwm_set_duty(PWM2, duty2);                    // ����ռ�ձ�
           
        }
        else                                                                    // ��ת
        {
            gpio_set_level(DIR2, GPIO_HIGH);                                     // DIR����͵�ƽ
            pwm_set_duty(PWM2, -duty2);                 // ����ռ�ձ�
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

// ͼ��ߴ綨��
#define IMAGE_WIDTH   160
#define IMAGE_HEIGHT  120

// ����������ֵ����
#define GUIDE_LINE_COLOR 255  // �����������ǰ�ɫ(255)
#define BACKGROUND_COLOR 0    // ���豳���Ǻ�ɫ(0)

// ���ٿ��Ʋ���
#define NORMAL_SPEED_DIFF 30  // ����ѭ��ʱ�Ĳ���
#define TURN_SPEED_DIFF   80  // ֱ��ת��ʱ�Ĺ̶�����

// ͼ����������(ģ������ͷ�ɼ���ͼ��)
unsigned char image[IMAGE_HEIGHT][IMAGE_WIDTH];

// ��������
bool searchLeftGuideLine();
bool searchRightGuideLine();
bool searchTopGuideLine();
void normalTracking();
void rightAngleTurn();
void processImage();


 * ��ͼ�����������������������
 * @return true ����ҵ�������, false ����
 
bool searchLeftGuideLine() {
    // �ӵײ���ʼ��������(����ײ���ͼ������һ��)
    for (int y = IMAGE_HEIGHT - 1; y >= IMAGE_HEIGHT / 2; y--) {
        // ������һ����Χ�ڵ�����(�������1/4���)
        for (int x = 0; x < IMAGE_WIDTH / 4; x++) {
            if (image[y][x] == GUIDE_LINE_COLOR) {
                return true;  // �ҵ�������
            }
        }
    }
    return false;  // δ�ҵ�������
}


 * ��ͼ���Ҳ�������������������
 * @return true ����ҵ�������, false ����
 
bool searchRightGuideLine() {
    // �ӵײ���ʼ��������
    for (int y = IMAGE_HEIGHT - 1; y >= IMAGE_HEIGHT / 2; y--) {
        // ����Ҳ�һ����Χ�ڵ�����(�����Ҳ�1/4���)
        for (int x = IMAGE_WIDTH * 3 / 4; x < IMAGE_WIDTH; x++) {
            if (image[y][x] == GUIDE_LINE_COLOR) {
                return true;  // �ҵ�������
            }
        }
    }
    return false;  // δ�ҵ�������
}


 * ��ͼ���Ϸ�������������������
 * @return true ����ҵ�������, false ����
 
bool searchTopGuideLine() {
    // ��ͼ���ϰ벿������(�����ϰ벿��1/3�߶�)
    for (int y = 0; y < IMAGE_HEIGHT / 3; y++) {
        // �����������������
        for (int x = 0; x < IMAGE_WIDTH; x++) {
            if (image[y][x] == GUIDE_LINE_COLOR) {
                return true;  // �ҵ�������
            }
        }
    }
    return false;  // δ�ҵ�������
}


 * ����ѭ��ģʽ

void normalTracking() {
    // ������������
    printf("����ѭ��ģʽ - ����: %d\n", NORMAL_SPEED_DIFF);
    // ���������Ӿ���ĵ�����ƴ���
}


 * ֱ��ת�䴦��
 
void rightAngleTurn() {
    printf("����ֱ��ת��ģʽ\n");
    
    // ���ù̶����ٽ���ת��
    printf("���ù̶�����: %d\n", TURN_SPEED_DIFF);
    // ���������Ӿ���ĵ�����ƴ���
    
    // �������ͼ���ϲ��Ƿ����������
    while (true) {
        // ��ȡ��һ֡ͼ��(��ʵ��Ӧ������Ҫ������ͷ��ȡ)
        // processNewImage();  // �������������
        
        if (searchTopGuideLine()) {
            printf("��⵽�ϲ������ߣ���������ѭ��ģʽ\n");
            normalTracking();
            break;
        }
        
        // ����ʵ�����ʱ���������Ƶ���ļ��
        // delay(50);  // ��������ʱ����
    }
}


 * ����ͼ���ж�ֱ����
 
void processImage() {
    // ���ֱ�������������
    bool leftFound = searchLeftGuideLine();
    bool rightFound = searchRightGuideLine();
    bool topFound = searchTopGuideLine();
    
    printf("����� - ���: %d, �Ҳ�: %d, �ϲ�: %d\n", 
           leftFound, rightFound, topFound);
    
    // �ж��Ƿ�Ϊֱ���������:
    // 1. �����������������������
    // 2. �Ҳ���������û���ѵ�������
    // 3. ͼ���Ϸ���������û���ѵ�������
    if (leftFound && !rightFound && !topFound) {
        printf("��⵽ֱ����\n");
        rightAngleTurn();  // ����ֱ���䴦��
    } else {
        normalTracking();   // ����ѭ��ģʽ
    }
}

int main() {
    // ģ����ѭ��
    while (1) {
        // ����Ӧ�ô�����ͷ��ȡͼ��������䵽image������
        // captureImage(image);  // �������������
        
        // ����ͼ���ж�ֱ����
        processImage();
        
        // ����ѭ���ٶ�
        // delay(100);  // ��������ʱ����
    }
    
    return 0;
}
*/



