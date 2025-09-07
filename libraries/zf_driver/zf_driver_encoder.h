/*********************************************************************************************************************
* CYT2BL37CEE Opensourec Library ����CYT2BL37CEE ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2023 SEEKFREE ��ɿƼ�
*
* ���ļ��� CYT2BL37CEE ��Դ���һ����
*
* CYT2BL37CEE ��Դ�� ��������
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
* �ļ�����          zf_driver_encoder
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          MDK 5.38a
* ����ƽ̨          CYT2BL37CEE
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2023-11-30       pudding            first version
* 2023-05-21       pudding            ����֧������������
********************************************************************************************************************/

#ifndef _zf_driver_encoder_h_
#define _zf_driver_encoder_h_

#include "zf_common_typedef.h"

// ��ö�ٶ��岻�����û��޸�
typedef enum // ö�ٱ���������
{
    TC_CH07_ENCODER_CH1_P02_0,    // tcpwmģ���ͨ��7  �������ſ�ѡ��Χ
    TC_CH09_ENCODER_CH1_P05_0,    // tcpwmģ���ͨ��9  �������ſ�ѡ��Χ
    TC_CH19_ENCODER_CH1_P08_0,    // tcpwmģ���ͨ��19 �������ſ�ѡ��Χ	
    TC_CH36_ENCODER_CH1_P12_0,    // tcpwmģ���ͨ��36 �������ſ�ѡ��Χ
}encoder_channel1_enum;

// ��ö�ٶ��岻�����û��޸�
typedef enum // ö�ٱ���������
{
    TC_CH07_ENCODER_CH2_P02_1,    // tcpwmģ���ͨ��7  �������ſ�ѡ��Χ	 
    TC_CH09_ENCODER_CH2_P05_1,    // tcpwmģ���ͨ��9  �������ſ�ѡ��Χ
    TC_CH19_ENCODER_CH2_P08_1,    // tcpwmģ���ͨ��19 �������ſ�ѡ��Χ	
    TC_CH36_ENCODER_CH2_P12_1,    // tcpwmģ���ͨ��36 �������ſ�ѡ��Χ
}encoder_channel2_enum;

typedef enum  // ö�� ��ʱ�����
{
    TC_CH07_ENCODER,
    TC_CH09_ENCODER,
    TC_CH19_ENCODER,
    TC_CH36_ENCODER,
}encoder_index_enum;

//====================================================������ ��������====================================================
int16 encoder_get_count     (encoder_index_enum encoder_n);
void  encoder_clear_count   (encoder_index_enum encoder_n);

void  encoder_quad_init     (encoder_index_enum encoder_n, encoder_channel1_enum count_pin, encoder_channel2_enum dir_pin);
void  encoder_dir_init      (encoder_index_enum encoder_n, encoder_channel1_enum ch1_pin, encoder_channel2_enum ch2_pin);
//====================================================������ ��������====================================================

#endif
