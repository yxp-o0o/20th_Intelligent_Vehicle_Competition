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
* �ļ�����          zf_driver_spi
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 9.40.1
* ����ƽ̨          CYT2BL3
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2024-1-9       pudding            first version
********************************************************************************************************************/
#ifndef _zf_driver_spi_h_
#define _zf_driver_spi_h_

#include "zf_common_typedef.h"

typedef enum        				// SPIģ���
{
    SPI_1,
    SPI_2,
    SPI_3,
    SPI_4,
}spi_index_enum;

typedef enum        				// ö�� SPI ģʽ ��ö�ٶ��岻�����û��޸�
{
    SPI_MODE0,
    SPI_MODE1,
    SPI_MODE2,
    SPI_MODE3,
}spi_mode_enum;

typedef enum                           	// ö��SPI CLK���� ��ö�ٶ��岻�����û��޸�
{
    SPI1_CLK_P06_2,                		// SPI1 CLK ����
    SPI2_CLK_P07_2,             		// SPI2 CLK ����
    SPI3_CLK_P13_2,				// SPI3 CLK ����
    SPI4_CLK_P14_2,				// SPI4 CLK ����
}spi_clk_pin_enum;

typedef enum                       		// ö��SPI MOSI���� ��ö�ٶ��岻�����û��޸�
{
    SPI1_MOSI_P06_1,                    	// SPI1 MOSI ���� 
    SPI2_MOSI_P07_1,                  	        // SPI2 MOSI ����   
    SPI3_MOSI_P13_1,                  	        // SPI3 MOSI ����     
    SPI4_MOSI_P14_1,                  	        // SPI4 MOSI ���� 
}spi_mosi_pin_enum;

typedef enum             			// ö��SPI MISO���� ��ö�ٶ��岻�����û��޸�
{
    SPI1_MISO_P06_0,	                 	// SPI1 MISO ����
    SPI2_MISO_P07_0,              		// SPI2 MISO ����
    SPI3_MISO_P13_0, 				// SPI3 MISO ����    
    SPI4_MISO_P14_0, 				// SPI4 MISO ����	
    SPI_MISO_NULL,
}spi_miso_pin_enum;

typedef enum                       		// ö��SPI CS���� ��ö�ٶ��岻�����û��޸�
{
    SPI1_CS0_P06_3,	        		// SPI1 CS ����
    SPI2_CS0_P11_0,	               		// SPI2 CS ����
    SPI3_CS0_P13_3,                 	        // SPI3 CS ����
    SPI4_CS0_P23_7,                             // SPI4 CS ����
    SPI_CS_NULL,
}spi_cs_pin_enum;

//====================================================SPI ��������====================================================
void        spi_write_8bit                  (spi_index_enum spi_n, const uint8 data);
void        spi_write_8bit_array            (spi_index_enum spi_n, const uint8 *data, uint32 len);

void        spi_write_16bit                 (spi_index_enum spi_n, const uint16 data);
void        spi_write_16bit_array           (spi_index_enum spi_n, const uint16 *data, uint32 len);

void        spi_write_8bit_register         (spi_index_enum spi_n, const uint8 register_name, const uint8 data);
void        spi_write_8bit_registers        (spi_index_enum spi_n, const uint8 register_name, const uint8 *data, uint32 len);

void        spi_write_16bit_register        (spi_index_enum spi_n, const uint16 register_name, const uint16 data);
void        spi_write_16bit_registers       (spi_index_enum spi_n, const uint16 register_name, const uint16 *data, uint32 len);

uint8       spi_read_8bit                   (spi_index_enum spi_n);
void        spi_read_8bit_array             (spi_index_enum spi_n, uint8 *data, uint32 len);

uint16      spi_read_16bit                  (spi_index_enum spi_n);
void        spi_read_16bit_array            (spi_index_enum spi_n, uint16 *data, uint32 len);

uint8       spi_read_8bit_register          (spi_index_enum spi_n, const uint8 register_name);
void        spi_read_8bit_registers         (spi_index_enum spi_n, const uint8 register_name, uint8 *data, uint32 len);

uint16      spi_read_16bit_register         (spi_index_enum spi_n, const uint16 register_name);
void        spi_read_16bit_registers        (spi_index_enum spi_n, const uint16 register_name, uint16 *data, uint32 len);

void        spi_transfer_8bit               (spi_index_enum spi_n, const uint8 *write_buffer, uint8 *read_buffer, uint32 len);
void        spi_transfer_16bit              (spi_index_enum spi_n, const uint16 *write_buffer, uint16 *read_buffer, uint32 len);

void        spi_init                        (spi_index_enum spi_n, spi_mode_enum mode, uint32 baud, spi_clk_pin_enum sck_pin, spi_mosi_pin_enum mosi_pin, spi_miso_pin_enum miso_pin, spi_cs_pin_enum cs_pin);
//====================================================SPI ��������====================================================

#endif
