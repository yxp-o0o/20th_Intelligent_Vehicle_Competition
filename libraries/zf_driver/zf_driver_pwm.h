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
* �ļ�����          zf_driver_pwm
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 9.40.1
* ����ƽ̨          CYT2BL3
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2024-1-8       pudding            first version
********************************************************************************************************************/

#ifndef _zf_driver_pwm_h_
#define _zf_driver_pwm_h_

#include "zf_common_typedef.h"

#define PWM_DUTY_MAX     10000                 // PWM���ռ�ձ�  ���ռ�ձ�Խ��ռ�ձȵĲ���ֵԽС

// ��ö�ٶ��岻�����û��޸�
typedef enum // ö��PWM����
{
	TCPWM_CH00_P06_1 ,
	TCPWM_CH01_P06_3 ,
	TCPWM_CH02_P06_5 ,
	TCPWM_CH06_P02_1 , 
	TCPWM_CH07_P02_0 , 
	TCPWM_CH09_P05_0 , 
	TCPWM_CH10_P05_1 , 
	TCPWM_CH11_P05_2 , 	
	TCPWM_CH12_P05_3 , 	
	TCPWM_CH13_P00_3 ,			
	TCPWM_CH14_P00_2 , 
	TCPWM_CH15_P07_1 ,
	TCPWM_CH17_P00_1 ,
        TCPWM_CH18_P00_0 , 	
	TCPWM_CH19_P08_0 ,	
	TCPWM_CH20_P08_1 ,	
	TCPWM_CH22_P23_7 ,			 
	TCPWM_CH25_P23_4 ,                       
	TCPWM_CH36_P12_0 ,		
	TCPWM_CH37_P12_1 ,		            
	TCPWM_CH39_P21_3 ,		
	TCPWM_CH40_P21_2 ,		
	TCPWM_CH41_P21_1 ,		
	TCPWM_CH42_P21_0 ,	                 
	TCPWM_CH44_P13_1 ,	                 
	TCPWM_CH45_P13_3 ,		
	TCPWM_CH48_P14_0 ,		
	TCPWM_CH49_P14_1 ,		 	
	TCPWM_CH50_P18_7 , 	                 
	TCPWM_CH51_P18_6 ,	                 
	TCPWM_CH52_P18_5 ,		
	TCPWM_CH53_P18_4 ,		
	TCPWM_CH54_P18_3 ,		
	
	TCPWM_CH_NUM	 ,
}pwm_channel_enum;


//====================================================PWM ��������====================================================
void pwm_all_channel_close      (void);
void pwm_set_duty               (pwm_channel_enum pwmch, uint32 duty);
void pwm_init                   (pwm_channel_enum pwmch, uint32 freq, uint32 duty);
//====================================================PWM ��������====================================================

#endif
