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
* 2024-3-6       pudding            �޸��봮�ڵ�ʱ�ӳ�ͻ����
********************************************************************************************************************/

#include "scb/cy_scb_spi.h"
#include "sysclk/cy_sysclk.h"
#include "gpio/cy_gpio.h"
#include "zf_common_debug.h"
#include "zf_driver_gpio.h"
#include "zf_driver_delay.h"
#include "zf_driver_spi.h"


#define SPI_FREQ       CY_INITIAL_TARGET_PERI_FREQ                             // ����ģ��ʱ�� Ĭ��80M

volatile stc_SCB_t*        spi_module[4] = {SCB4, SCB5, SCB3, SCB2};
spi_cs_pin_enum             cs_pin_save[4];
//-------------------------------------------------------------------------------------------------------------------
// �������       SPI��ȡʱ�����ź�
// ����˵��       clk_pin     ʱ������ ���� zf_driver_spi.h �� spi_clk_pin_enum ö���嶨��
// ���ز���       void
// ʹ��ʾ��       spi_get_clk_pin(SPI0_CLK_P02_2);
// ��ע��Ϣ       �ڲ����ã��û��������
//-------------------------------------------------------------------------------------------------------------------
static gpio_pin_enum spi_get_clk_pin (spi_clk_pin_enum clk_pin)
{
    gpio_pin_enum temp_clk_pin = NC;
    
    switch(clk_pin)
    {
        case SPI1_CLK_P06_2: temp_clk_pin = P06_2; break;
        case SPI2_CLK_P07_2: temp_clk_pin = P07_2; break;
        case SPI3_CLK_P13_2: temp_clk_pin = P13_2; break;
        case SPI4_CLK_P14_2: temp_clk_pin = P14_2; break;
    }
    
    return temp_clk_pin;
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI��ȡMOSI���ź�
// ����˵��       mosi_pin     ʱ������ ���� zf_driver_spi.h �� spi_mosi_pin_enum ö���嶨��
// ���ز���       void
// ʹ��ʾ��       spi_get_mosi_pin(SPI0_MOSI_P02_1);
// ��ע��Ϣ       �ڲ����ã��û��������
//-------------------------------------------------------------------------------------------------------------------
static gpio_pin_enum spi_get_mosi_pin (spi_mosi_pin_enum mosi_pin)
{
    gpio_pin_enum temp_mosi_pin = NC;
    
    switch(mosi_pin)
    {
        case SPI1_MOSI_P06_1: temp_mosi_pin = P06_1; break;
        case SPI2_MOSI_P07_1: temp_mosi_pin = P07_1; break;
        case SPI3_MOSI_P13_1: temp_mosi_pin = P13_1; break;
        case SPI4_MOSI_P14_1: temp_mosi_pin = P14_1; break;
    }
    
    return temp_mosi_pin;
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI��ȡMISO���ź�
// ����˵��       miso_pin     ʱ������ ���� zf_driver_spi.h �� spi_miso_pin_enum ö���嶨��
// ���ز���       void
// ʹ��ʾ��       spi_get_miso_pin(SPI0_MISO_P02_0);
// ��ע��Ϣ       �ڲ����ã��û��������
//-------------------------------------------------------------------------------------------------------------------
static gpio_pin_enum spi_get_miso_pin (spi_miso_pin_enum miso_pin)
{
    gpio_pin_enum temp_miso_pin = NC;
    
    switch(miso_pin)
    {
        case SPI1_MISO_P06_0: temp_miso_pin = P06_0; break;
        case SPI2_MISO_P07_0: temp_miso_pin = P07_0; break;
        case SPI3_MISO_P13_0: temp_miso_pin = P13_0; break;
        case SPI4_MISO_P14_0: temp_miso_pin = P14_0; break;
        case SPI_MISO_NULL: break;
    }
    
    return temp_miso_pin;
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI��ȡCS���ź�
// ����˵��       clk_pin     ʱ������ ���� zf_driver_spi.h �� spi_clk_pin_enum ö���嶨��
// ���ز���       void
// ʹ��ʾ��       spi_get_clk_pin(SPI0_CS0_P02_3);
// ��ע��Ϣ       �ڲ����ã��û��������
//-------------------------------------------------------------------------------------------------------------------
static gpio_pin_enum spi_get_cs_pin (spi_cs_pin_enum cs_pin)
{
    gpio_pin_enum temp_cs_pin = NC;
    switch(cs_pin)
    {
        case SPI1_CS0_P06_3: temp_cs_pin = P06_3; break;
        case SPI2_CS0_P11_0: temp_cs_pin = P11_0; break;
        case SPI3_CS0_P13_3: temp_cs_pin = P13_3; break;
        case SPI4_CS0_P23_7: temp_cs_pin = P23_7; break;
        case SPI_CS_NULL: break;
    }
    return temp_cs_pin;
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI��ȡʱ�����Ÿ��ù�ϵ
// ����˵��       clk_pin     ʱ������ ���� zf_driver_spi.h �� spi_clk_pin_enum ö���嶨��
// ���ز���       void
// ʹ��ʾ��       spi_get_clk_pin(SPI0_CLK_P02_2);
// ��ע��Ϣ       �ڲ����ã��û��������
//-------------------------------------------------------------------------------------------------------------------
static en_hsiom_sel_t spi_get_clk_hsiom (spi_clk_pin_enum clk_pin)
{
    en_hsiom_sel_t temp_clk_hsiom = HSIOM_SEL_GPIO;
    
    switch(clk_pin)
    {
        case SPI1_CLK_P06_2: temp_clk_hsiom = P6_2_SCB4_SPI_CLK; break;     
        case SPI2_CLK_P07_2: temp_clk_hsiom = P7_2_SCB5_SPI_CLK; break;    
        case SPI3_CLK_P13_2: temp_clk_hsiom = P13_2_SCB3_SPI_CLK; break;    
        case SPI4_CLK_P14_2: temp_clk_hsiom = P14_2_SCB2_SPI_CLK; break;    
    }
    
    return temp_clk_hsiom;
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI��ȡMOSI���Ÿ��ù�ϵ
// ����˵��       clk_pin     ʱ������ ���� zf_driver_spi.h �� spi_mosi_pin_enum ö���嶨��
// ���ز���       void
// ʹ��ʾ��       spi_get_mosi_pin(SPI0_MOSI_P02_1);
// ��ע��Ϣ       �ڲ����ã��û��������
//-------------------------------------------------------------------------------------------------------------------
static en_hsiom_sel_t spi_get_mosi_hsiom (spi_mosi_pin_enum mosi_pin)
{
    en_hsiom_sel_t temp_mosi_hsiom = HSIOM_SEL_GPIO;
    
    switch(mosi_pin)
    {
        case SPI1_MOSI_P06_1: temp_mosi_hsiom =  P6_1_SCB4_SPI_MOSI; break;      
        case SPI2_MOSI_P07_1: temp_mosi_hsiom =  P7_1_SCB5_SPI_MOSI; break;     
        case SPI3_MOSI_P13_1: temp_mosi_hsiom =  P13_1_SCB3_SPI_MOSI; break;     
        case SPI4_MOSI_P14_1: temp_mosi_hsiom =  P14_1_SCB2_SPI_MOSI; break;     
    }
    
    return temp_mosi_hsiom;
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI��ȡMISO���Ÿ��ù�ϵ
// ����˵��       miso_pin     ʱ������ ���� zf_driver_spi.h �� spi_miso_pin_enum ö���嶨��
// ���ز���       void
// ʹ��ʾ��       spi_get_miso_pin(SPI0_MISO_P02_0);
// ��ע��Ϣ       �ڲ����ã��û��������
//-------------------------------------------------------------------------------------------------------------------
static en_hsiom_sel_t spi_get_miso_hsiom (spi_miso_pin_enum miso_pin)
{
    en_hsiom_sel_t temp_miso_hsiom = HSIOM_SEL_GPIO;
    
    switch(miso_pin)
    {
        case SPI1_MISO_P06_0: temp_miso_hsiom =  P6_0_SCB4_SPI_MISO; break;       
        case SPI2_MISO_P07_0: temp_miso_hsiom =  P7_0_SCB5_SPI_MISO; break;     
        case SPI3_MISO_P13_0: temp_miso_hsiom =  P13_0_SCB3_SPI_MISO; break;  
        case SPI4_MISO_P14_0: temp_miso_hsiom =  P14_0_SCB2_SPI_MISO; break; 
        case SPI_MISO_NULL: break;
    }
    
    return temp_miso_hsiom;
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI��ȡCS���Ÿ��ù�ϵ
// ����˵��       cs_pin     ʱ������ ���� zf_driver_spi.h �� spi_clk_pin_enum ö���嶨��
// ���ز���       void
// ʹ��ʾ��       spi_get_clk_pin(SPI0_CS0_P02_3);
// ��ע��Ϣ       �ڲ����ã��û��������
//-------------------------------------------------------------------------------------------------------------------
static en_hsiom_sel_t spi_get_cs_hsiom (spi_cs_pin_enum cs_pin)
{
    en_hsiom_sel_t temp_cs_hsiom = HSIOM_SEL_GPIO;
    switch(cs_pin)
    {
        case SPI1_CS0_P06_3: temp_cs_hsiom =  P6_3_SCB4_SPI_SELECT0;  break;     
        case SPI2_CS0_P11_0: temp_cs_hsiom =  P11_0_GPIO;  break;      
        case SPI3_CS0_P13_3: temp_cs_hsiom =  P13_3_SCB3_SPI_SELECT0; break;     
        case SPI4_CS0_P23_7: temp_cs_hsiom =  P23_7_GPIO; break; 
        case SPI_CS_NULL: break;
    }
    return temp_cs_hsiom;
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI��ȡCS���Ÿ��ù�ϵ
// ����˵��       cs_pin     ʱ������ ���� zf_driver_spi.h �� spi_clk_pin_enum ö���嶨��
// ���ز���       void
// ʹ��ʾ��       spi_get_clk_pin(SPI0_CS0_P02_3);
// ��ע��Ϣ       �ڲ����ã��û��������
//-------------------------------------------------------------------------------------------------------------------
static en_clk_dst_t spi_get_module_clock (spi_index_enum spi_n)
{
    en_clk_dst_t temp_spi_module;
    switch(spi_n)
    {
        case SPI_1: temp_spi_module =  PCLK_SCB4_CLOCK;  break;     
        case SPI_2: temp_spi_module =  PCLK_SCB5_CLOCK;  break;      
        case SPI_3: temp_spi_module =  PCLK_SCB3_CLOCK; break;     
        case SPI_4: temp_spi_module =  PCLK_SCB2_CLOCK; break; 
    }
    return temp_spi_module;
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI���÷��ͳ���
// ����˵��       spi_n     SPIģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// ����˵��       length		��Ҫ���õĳ��� ��֧�� 8 16
// ���ز���       void
// ʹ��ʾ��       spi_get_clk_pin(SPI_0, 8);
// ��ע��Ϣ       �ڲ����ã��û��������
//-------------------------------------------------------------------------------------------------------------------
static void switch_transition_length(spi_index_enum spi_n, uint8 length)
{
  static uint8 now_length[SPI_4 + 1] = {0};
  
    if(now_length[spi_n] != length)
    {
        switch(length)
        {
            case 8:
            {
                spi_module[spi_n]->unCTRL.u32Register &= 0xffff3fff;
                spi_module[spi_n]->unTX_CTRL.u32Register &= 0xffffffe0;
                spi_module[spi_n]->unTX_CTRL.u32Register |= 0x00000007;
                spi_module[spi_n]->unRX_CTRL.u32Register &= 0xffffffe0;
                spi_module[spi_n]->unRX_CTRL.u32Register |= 0x00000007;
            }break;
            case 16:
            {
                spi_module[spi_n]->unCTRL.u32Register &= 0xffff3fff;
                spi_module[spi_n]->unCTRL.u32Register |= 0x00004000;
                spi_module[spi_n]->unTX_CTRL.u32Register &= 0xffffffe0;
                spi_module[spi_n]->unTX_CTRL.u32Register |= 0x0000000F;
                spi_module[spi_n]->unRX_CTRL.u32Register &= 0xffffffe0;
                spi_module[spi_n]->unRX_CTRL.u32Register |= 0x0000000F;
            }break;
        }
        now_length[spi_n] = length;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI �ӿ�д 8bit ����
// ����˵��       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// ����˵��       data            ����
// ���ز���       void
// ʹ��ʾ��       spi_write_8bit(SPI_0, 0x11);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void spi_write_8bit (spi_index_enum spi_n, const uint8 data)
{
    switch_transition_length(spi_n, 8);					        // �л�ͨ�ų���Ϊ8λ
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_low(cs_pin_save[spi_n]);
    }
    Cy_SCB_SPI_ClearRxFifo(spi_module[spi_n]);					// ������ջ�����
    Cy_SCB_WriteTxFifo(spi_module[spi_n], data);                                // ��������
    while(Cy_SCB_GetFifoSize(spi_module[spi_n]) == Cy_SCB_GetNumInTxFifo(spi_module[spi_n]));       // ����������ȴ�
    
	while(Cy_SCB_IsTxComplete(spi_module[spi_n]) == 0);                         // �ȴ����ݷ������
    while(Cy_SCB_SPI_GetNumInRxFifo(spi_module[spi_n]) == 0);		        // �ȴ����յ�����
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_high(cs_pin_save[spi_n]);
    }	
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI �ӿ�д 8bit ����
// ����˵��       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// ����˵��       *data           ���ݴ�Ż�����
// ����˵��       len             ����������
// ���ز���       void
// ʹ��ʾ��       spi_write_8bit_array(SPI_0, data, 64);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void spi_write_8bit_array (spi_index_enum spi_n, const uint8 *data, uint32 len)
{    
    switch_transition_length(spi_n, 8);					        // �л�ͨ�ų���Ϊ8λ
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
            gpio_low(cs_pin_save[spi_n]);
    } 
    
    do
    {
        Cy_SCB_WriteTxFifo(spi_module[spi_n], *data ++);                        // ��������
        while(Cy_SCB_GetFifoSize(spi_module[spi_n]) == Cy_SCB_GetNumInTxFifo(spi_module[spi_n]));       // ����������ȴ�
        len -= 1;						                // ���ͳ����Լ�	
    }while(len);
    
    while(Cy_SCB_IsTxComplete(spi_module[spi_n]) == 0);                         // �ȴ����ݷ������
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)				        // ��CS��Ϊ�� ������CS
    {
        gpio_high(cs_pin_save[spi_n]);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI �ӿ�д 16bit ����
// ����˵��       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// ����˵��       data            ����
// ���ز���       void
// ʹ��ʾ��       spi_write_16bit(SPI_0, 0x1101);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void spi_write_16bit (spi_index_enum spi_n, const uint16 data)
{
    switch_transition_length(spi_n, 16);				        // �л�ͨ�ų���Ϊ16λ
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_low(cs_pin_save[spi_n]);
    }
    
    Cy_SCB_WriteTxFifo(spi_module[spi_n], data);                                // ��������
    while(Cy_SCB_GetFifoSize(spi_module[spi_n]) == Cy_SCB_GetNumInTxFifo(spi_module[spi_n]));       // ����������ȴ�
    
    while(Cy_SCB_IsTxComplete(spi_module[spi_n]) == 0);                         // �ȴ����ݷ������
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_high(cs_pin_save[spi_n]);
    }		
}


//-------------------------------------------------------------------------------------------------------------------
// �������       SPI �ӿ�д 16bit ����
// ����˵��       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// ����˵��       *data           ���ݴ�Ż�����
// ����˵��       len             ����������
// ���ز���       void
// ʹ��ʾ��       spi_write_16bit_array(SPI_0, data, 64);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void spi_write_16bit_array (spi_index_enum spi_n, const uint16 *data, uint32 len)
{
    switch_transition_length(spi_n, 16);					// �л�ͨ�ų���Ϊ16λ
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
            gpio_low(cs_pin_save[spi_n]);
    } 
    
    do
    {
        Cy_SCB_WriteTxFifo(spi_module[spi_n], *data ++);                        // ��������
        while(Cy_SCB_GetFifoSize(spi_module[spi_n]) == Cy_SCB_GetNumInTxFifo(spi_module[spi_n]));       // ����������ȴ�
        len -= 1;						                // ���ͳ����Լ�	
    }while(len);
    
    while(Cy_SCB_IsTxComplete(spi_module[spi_n]) == 0);                         // �ȴ����ݷ������
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)				        // ��CS��Ϊ�� ������CS
    {
        gpio_high(cs_pin_save[spi_n]);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI �ӿ��򴫸����ļĴ���д 8bit ����
// ����˵��       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// ����˵��       register_name   �Ĵ�����ַ
// ����˵��       data            ����
// ���ز���       void
// ʹ��ʾ��        spi_write_8bit_register(SPI_0, 0x11, 0x01);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void spi_write_8bit_register (spi_index_enum spi_n, const uint8 register_name, const uint8 data)
{
    switch_transition_length(spi_n, 8);						// �л�ͨ�ų���Ϊ8λ
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
            gpio_low(cs_pin_save[spi_n]);
    }
    
    Cy_SCB_WriteTxFifo(spi_module[spi_n], register_name);                        // ���ͼĴ�����ַ
    while(Cy_SCB_GetFifoSize(spi_module[spi_n]) == Cy_SCB_GetNumInTxFifo(spi_module[spi_n]));           // ����������ȴ�

    Cy_SCB_WriteTxFifo(spi_module[spi_n], data);                                // ��������
    while(Cy_SCB_GetFifoSize(spi_module[spi_n]) == Cy_SCB_GetNumInTxFifo(spi_module[spi_n]));           // ����������ȴ�
    
    while(Cy_SCB_IsTxComplete(spi_module[spi_n]) == 0);                         // �ȴ����ݷ������
    while(spi_module[spi_n]->unSPI_STATUS.stcField.u1BUS_BUSY);
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
            gpio_high(cs_pin_save[spi_n]);
    }	
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI �ӿ��򴫸����ļĴ���д 8bit ����
// ����˵��       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// ����˵��       register_name   �Ĵ�����ַ
// ����˵��       *data           ���ݴ�Ż�����
// ����˵��       len             ����������
// ���ز���       void
// ʹ��ʾ��       spi_write_8bit_registers(SPI_0, 0x11, data, 32);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void spi_write_8bit_registers (spi_index_enum spi_n, const uint8 register_name, const uint8 *data, uint32 len)
{
    switch_transition_length(spi_n, 8);					        // �л�ͨ�ų���Ϊ8λ
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
            gpio_low(cs_pin_save[spi_n]);
    } 
    
    Cy_SCB_WriteTxFifo(spi_module[spi_n], register_name);                        // ���ͼĴ�����ַ
    while(Cy_SCB_GetFifoSize(spi_module[spi_n]) == Cy_SCB_GetNumInTxFifo(spi_module[spi_n]));           // ����������ȴ�
    
    do
    {
        Cy_SCB_WriteTxFifo(spi_module[spi_n], *data ++);                        // ��������
        while(Cy_SCB_GetFifoSize(spi_module[spi_n]) == Cy_SCB_GetNumInTxFifo(spi_module[spi_n]));       // ����������ȴ�
        len -= 1;						                // ���ͳ����Լ�	
    }while(len);
    
    while(Cy_SCB_IsTxComplete(spi_module[spi_n]) == 0);                         // �ȴ����ݷ������
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)				        // ��CS��Ϊ�� ������CS
    {
        gpio_high(cs_pin_save[spi_n]);
    }	
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI �ӿ��򴫸����ļĴ���д 16bit ����
// ����˵��       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// ����˵��       register_name   �Ĵ�����ַ
// ����˵��       data            ����
// ���ز���       void
// ʹ��ʾ��       spi_write_16bit_register(SPI_0, 0x1011, 0x0101);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void spi_write_16bit_register (spi_index_enum spi_n, const uint16 register_name, const uint16 data)
{
    switch_transition_length(spi_n, 16);				        // �л�ͨ�ų���Ϊ16λ
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
            gpio_low(cs_pin_save[spi_n]);
    }
    
    Cy_SCB_WriteTxFifo(spi_module[spi_n], register_name);                        // ���ͼĴ�����ַ
    while(Cy_SCB_GetFifoSize(spi_module[spi_n]) == Cy_SCB_GetNumInTxFifo(spi_module[spi_n]));           // ����������ȴ�

    Cy_SCB_WriteTxFifo(spi_module[spi_n], data);                                // ��������
    while(Cy_SCB_GetFifoSize(spi_module[spi_n]) == Cy_SCB_GetNumInTxFifo(spi_module[spi_n]));           // ����������ȴ�
    
    while(Cy_SCB_IsTxComplete(spi_module[spi_n]) == 0);                         // �ȴ����ݷ������
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
            gpio_high(cs_pin_save[spi_n]);
    }	
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI �ӿ��򴫸����ļĴ���д 16bit ����
// ����˵��       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// ����˵��       register_name   �Ĵ�����ַ
// ����˵��       *data           ���ݴ�Ż�����
// ����˵��       len             ����������
// ���ز���       void
// ʹ��ʾ��       spi_write_16bit_registers(SPI_0, 0x1011, data, 32);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void spi_write_16bit_registers (spi_index_enum spi_n, const uint16 register_name, const uint16 *data, uint32 len)
{
    switch_transition_length(spi_n, 16);					// �л�ͨ�ų���Ϊ16λ
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
            gpio_low(cs_pin_save[spi_n]);
    } 
    
    Cy_SCB_WriteTxFifo(spi_module[spi_n], register_name);                        // ���ͼĴ�����ַ
    while(Cy_SCB_GetFifoSize(spi_module[spi_n]) == Cy_SCB_GetNumInTxFifo(spi_module[spi_n]));           // ����������ȴ�
    
    do
    {
        Cy_SCB_WriteTxFifo(spi_module[spi_n], *data ++);                        // ��������
        while(Cy_SCB_GetFifoSize(spi_module[spi_n]) == Cy_SCB_GetNumInTxFifo(spi_module[spi_n]));       // ����������ȴ�
        len -= 1;						                // ���ͳ����Լ�	
    }while(len);
    
    while(Cy_SCB_IsTxComplete(spi_module[spi_n]) == 0);                         // �ȴ����ݷ������
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)				        // ��CS��Ϊ�� ������CS
    {
        gpio_high(cs_pin_save[spi_n]);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI �ӿڶ� 8bit ����
// ����˵��       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// ����˵��       register_name   �Ĵ�����ַ
// ���ز���       uint8           ����
// ʹ��ʾ��       spi_read_8bit(SPI_0);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint8 spi_read_8bit (spi_index_enum spi_n)
{
    uint8 read_data = 0;
    
    Cy_SCB_SPI_ClearRxFifo(spi_module[spi_n]);					// ������ջ�����
    
    switch_transition_length(spi_n, 8);						// �л�����ͨ�ų���Ϊ8λ
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_low(cs_pin_save[spi_n]);
    }
    
    Cy_SCB_WriteTxFifo(spi_module[spi_n], 0);                                   // ���Ϳ�����
    while(Cy_SCB_IsTxComplete(spi_module[spi_n]) == 0);                         // �ȴ����ݷ������
    while(Cy_SCB_SPI_GetNumInRxFifo(spi_module[spi_n]) == 0);		        // �ȴ����յ�����
    
    read_data = (uint8)(spi_module[spi_n]->unRX_FIFO_RD.u32Register);		// ��ȡ����
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_high(cs_pin_save[spi_n]);
    }	
    
    return read_data;
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI �ӿڶ� 8bit ����
// ����˵��       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// ����˵��       *data           ���ݴ�Ż�����
// ����˵��       len             ���ͻ���������
// ���ز���       void
// ʹ��ʾ��       spi_read_8bit_array(SPI_0, data, 64);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void spi_read_8bit_array (spi_index_enum spi_n, uint8 *data, uint32 len)
{
    Cy_SCB_SPI_ClearRxFifo(spi_module[spi_n]);					// ������ջ�����
    
    switch_transition_length(spi_n, 8);						// �л�����ͨ�ų���Ϊ8λ
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_low(cs_pin_save[spi_n]);
    }
    
    do{
        Cy_SCB_WriteTxFifo(spi_module[spi_n], 0);                               // ���Ϳ�����
        while(Cy_SCB_IsTxComplete(spi_module[spi_n]) == 0);                     // �ȴ����ݷ������
        while(Cy_SCB_SPI_GetNumInRxFifo(spi_module[spi_n]) == 0);		// �ȴ����յ�����
        *data ++ = (uint8)(spi_module[spi_n]->unRX_FIFO_RD.u32Register);	// ��ȡ����
        len -= 1;
    }while(len);
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_high(cs_pin_save[spi_n]);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI �ӿڶ� 16bit ����
// ����˵��       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// ����˵��       register_name   �Ĵ�����ַ
// ���ز���       uint16          ����
// ʹ��ʾ��       spi_read_16bit(SPI_0);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint16 spi_read_16bit (spi_index_enum spi_n)
{
    uint16 read_data = 0;
    
    Cy_SCB_SPI_ClearRxFifo(spi_module[spi_n]);					// ������ջ�����
    
    switch_transition_length(spi_n, 16);					// �л�����ͨ�ų���Ϊ16λ
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_low(cs_pin_save[spi_n]);
    }
    
    Cy_SCB_WriteTxFifo(spi_module[spi_n], 0);                                   // ���Ϳ�����
    while(Cy_SCB_IsTxComplete(spi_module[spi_n]) == 0);                         // �ȴ����ݷ������
    while(Cy_SCB_SPI_GetNumInRxFifo(spi_module[spi_n]) == 0);		        // �ȴ����յ�����
    read_data = (uint16)(spi_module[spi_n]->unRX_FIFO_RD.u32Register);		// ��ȡ����
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_high(cs_pin_save[spi_n]);
    }	
    
    return read_data;
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI �ӿڶ� 16bit ����
// ����˵��       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// ����˵��       *data           ���ݴ�Ż�����
// ����˵��       len             ���ͻ���������
// ���ز���       void
// ʹ��ʾ��       spi_read_16bit_array(SPI_0, data, 64);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void spi_read_16bit_array (spi_index_enum spi_n, uint16 *data, uint32 len)
{
    Cy_SCB_SPI_ClearRxFifo(spi_module[spi_n]);					// ������ջ�����
    
    switch_transition_length(spi_n, 16);					// �л�����ͨ�ų���Ϊ16λ
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_low(cs_pin_save[spi_n]);
    }
    
    do{
        Cy_SCB_WriteTxFifo(spi_module[spi_n], 0);                               // ���Ϳ�����
        while(Cy_SCB_IsTxComplete(spi_module[spi_n]) == 0);                     // �ȴ����ݷ������
        while(Cy_SCB_SPI_GetNumInRxFifo(spi_module[spi_n]) == 0);		// �ȴ����յ�����
        *data ++ = (uint16)(spi_module[spi_n]->unRX_FIFO_RD.u32Register);	// ��ȡ����
        len -= 1;
    }while(len);
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_high(cs_pin_save[spi_n]);
    }	
}


//-------------------------------------------------------------------------------------------------------------------
// �������       SPI �ӿڴӴ������ļĴ����� 8bit ����
// ����˵��       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// ����˵��       register_name   �Ĵ�����ַ
// ���ز���       uint8           ����
// ʹ��ʾ��       spi_read_8bit_register(SPI_0, 0x11);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint8 spi_read_8bit_register (spi_index_enum spi_n, const uint8 register_name)
{
    uint8 read_data = 0;
    
    switch_transition_length(spi_n, 8);						// �л�����ͨ�ų���Ϊ8λ
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_low(cs_pin_save[spi_n]);
    }
    
    Cy_SCB_WriteTxFifo(spi_module[spi_n], register_name);                       // ���ͼĴ�����ַ

	while(Cy_SCB_IsTxComplete(spi_module[spi_n]) == 0);                     // �ȴ����ݷ������ 
    while(Cy_SCB_SPI_GetNumInRxFifo(spi_module[spi_n]) == 0);		        // �ȴ����յ�����    
    
    Cy_SCB_SPI_ClearRxFifo(spi_module[spi_n]);					// ������ջ�����
    
    Cy_SCB_WriteTxFifo(spi_module[spi_n], 0);                                   // ���Ϳ�����
    
    while(Cy_SCB_IsTxComplete(spi_module[spi_n]) == 0);                         // �ȴ����ݷ������
    
    while(Cy_SCB_SPI_GetNumInRxFifo(spi_module[spi_n]) == 0);		        // �ȴ����յ�����
    read_data = (uint8)(spi_module[spi_n]->unRX_FIFO_RD.u32Register);		// ��ȡ����
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_high(cs_pin_save[spi_n]);
    }	
    
    return read_data;
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI �ӿڴӴ������ļĴ����� 8bit ����
// ����˵��       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// ����˵��       register_name   �Ĵ�����ַ
// ����˵��       *data           ���ݴ�Ż�����
// ����˵��       len             ���ͻ���������
// ���ز���       void
// ʹ��ʾ��       spi_read_8bit_registers(SPI_0, 0x11, data, 32);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void spi_read_8bit_registers (spi_index_enum spi_n, const uint8 register_name, uint8 *data, uint32 len)
{
    switch_transition_length(spi_n, 8);						// �л�����ͨ�ų���Ϊ8λ
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_low(cs_pin_save[spi_n]);
    }
    
    Cy_SCB_WriteTxFifo(spi_module[spi_n], register_name);                       // ���ͼĴ�����ַ
	while(Cy_SCB_IsTxComplete(spi_module[spi_n]) == 0);                     // �ȴ����ݷ������
    while(Cy_SCB_SPI_GetNumInRxFifo(spi_module[spi_n]) == 0);		        // �ȴ����յ�����  
    
    Cy_SCB_SPI_ClearRxFifo(spi_module[spi_n]);					// ������ջ�����
    
    do{
        Cy_SCB_WriteTxFifo(spi_module[spi_n], 0);                               // ���Ϳ�����
        
while(Cy_SCB_IsTxComplete(spi_module[spi_n]) == 0);                     // �ȴ����ݷ������
        while(Cy_SCB_SPI_GetNumInRxFifo(spi_module[spi_n]) == 0);		// �ȴ����յ�����
        *data ++ = (uint8)(spi_module[spi_n]->unRX_FIFO_RD.u32Register);	// ��ȡ����
        len -= 1;
    }while(len);
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_high(cs_pin_save[spi_n]);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI �ӿڴӴ������ļĴ����� 16bit ����
// ����˵��       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// ����˵��       register_name   �Ĵ�����ַ
// ���ز���       uint16          ����
// ʹ��ʾ��       spi_read_16bit_register(SPI_0, 0x1011);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint16 spi_read_16bit_register (spi_index_enum spi_n, const uint16 register_name)
{
    uint16 read_data = 0;
    
    switch_transition_length(spi_n, 16);					// �л�����ͨ�ų���Ϊ16λ
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_low(cs_pin_save[spi_n]);
    }
    
    Cy_SCB_WriteTxFifo(spi_module[spi_n], register_name);                       // ���ͼĴ�����ַ
    
    while(Cy_SCB_IsTxComplete(spi_module[spi_n]) == 0);                         // �ȴ����ݷ������
    
    while(Cy_SCB_SPI_GetNumInRxFifo(spi_module[spi_n]) == 0);		        // �ȴ����յ�����    
    
    Cy_SCB_SPI_ClearRxFifo(spi_module[spi_n]);					// ������ջ�����
    
    Cy_SCB_WriteTxFifo(spi_module[spi_n], 0);                                   // ���Ϳ�����
    
    while(Cy_SCB_IsTxComplete(spi_module[spi_n]) == 0);                         // �ȴ����ݷ������
    
    while(Cy_SCB_SPI_GetNumInRxFifo(spi_module[spi_n]) == 0);		        // �ȴ����յ�����
    
    read_data = (uint16)(spi_module[spi_n]->unRX_FIFO_RD.u32Register);		// ��ȡ����
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_high(cs_pin_save[spi_n]);
    }	
    
    return read_data;
}


//-------------------------------------------------------------------------------------------------------------------
// �������       SPI �ӿڴӴ������ļĴ����� 16bit ����
// ����˵��       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// ����˵��       register_name   �Ĵ�����ַ
// ����˵��       *data           ���ݴ�Ż�����
// ����˵��       len             ���ͻ���������
// ���ز���       void
// ʹ��ʾ��       spi_read_16bit_registers(SPI_0, 0x1101, data, 32);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void spi_read_16bit_registers (spi_index_enum spi_n, const uint16 register_name, uint16 *data, uint32 len)
{
    switch_transition_length(spi_n, 16);					// �л�����ͨ�ų���Ϊ16λ
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_low(cs_pin_save[spi_n]);
    }
    
    Cy_SCB_WriteTxFifo(spi_module[spi_n], register_name);                       // ���ͼĴ�����ַ
    while(Cy_SCB_IsTxComplete(spi_module[spi_n]) == 0);                         // �ȴ����ݷ������
    while(Cy_SCB_SPI_GetNumInRxFifo(spi_module[spi_n]) == 0);		        // �ȴ����յ�����  
    
    Cy_SCB_SPI_ClearRxFifo(spi_module[spi_n]);					// ������ջ�����
    
    do{
        Cy_SCB_WriteTxFifo(spi_module[spi_n], 0);                               // ���Ϳ�����
        while(Cy_SCB_IsTxComplete(spi_module[spi_n]) == 0);                     // �ȴ����ݷ������
        while(Cy_SCB_SPI_GetNumInRxFifo(spi_module[spi_n]) == 0);		// �ȴ����յ�����
        *data ++ = (uint16)(spi_module[spi_n]->unRX_FIFO_RD.u32Register);	// ��ȡ����
        len -= 1;
    }while(len);
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_high(cs_pin_save[spi_n]);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI 8bit ���ݴ��� ���������������ͬʱ���е�
// ����˵��       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// ����˵��       write_buffer    ���͵����ݻ�������ַ
// ����˵��       read_buffer     ��������ʱ���յ������ݵĴ洢��ַ(����Ҫ������ NULL)
// ����˵��       len             ����������
// ���ز���       void
// ʹ��ʾ��       spi_transfer_8bit(SPI_0, buf, buf, 1);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void spi_transfer_8bit (spi_index_enum spi_n, const uint8 *write_buffer, uint8 *read_buffer, uint32 len)
{
    switch_transition_length(spi_n, 8);						// �л�����ͨ�ų���Ϊ8λ
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_low(cs_pin_save[spi_n]);
    }
    
    Cy_SCB_SPI_ClearRxFifo(spi_module[spi_n]);					// ������ջ�����
    
    do{
        Cy_SCB_WriteTxFifo(spi_module[spi_n], *write_buffer ++);                // ��������
        while(Cy_SCB_IsTxComplete(spi_module[spi_n]) == 0);                     // �ȴ����ݷ������
        while(Cy_SCB_SPI_GetNumInRxFifo(spi_module[spi_n]) == 0);		// �ȴ����յ�����  
        *read_buffer ++ = (uint8)(spi_module[spi_n]->unRX_FIFO_RD.u32Register);	// ��ȡ����
        len -= 1;
    }while(len);
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_high(cs_pin_save[spi_n]);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������       SPI 16bit ���ݴ��� ���������������ͬʱ���е�
// ����˵��       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// ����˵��       write_buffer    ���͵����ݻ�������ַ
// ����˵��       read_buffer     ��������ʱ���յ������ݵĴ洢��ַ(����Ҫ������ NULL)
// ����˵��       len             ����������
// ���ز���       void
// ʹ��ʾ��       spi_transfer_16bit(SPI_0, buf, buf, 1);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void spi_transfer_16bit (spi_index_enum spi_n, const uint16 *write_buffer, uint16 *read_buffer, uint32 len)
{
    switch_transition_length(spi_n, 16);				        // �л�����ͨ�ų���Ϊ16λ
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_low(cs_pin_save[spi_n]);
    }
    
    Cy_SCB_SPI_ClearRxFifo(spi_module[spi_n]);					// ������ջ�����
    
    do{
        Cy_SCB_WriteTxFifo(spi_module[spi_n], *write_buffer ++);                // ��������
        while(Cy_SCB_IsTxComplete(spi_module[spi_n]) == 0);                     // �ȴ����ݷ������
        while(Cy_SCB_SPI_GetNumInRxFifo(spi_module[spi_n]) == 0);		// �ȴ����յ�����  
        *read_buffer ++ = (uint16)(spi_module[spi_n]->unRX_FIFO_RD.u32Register);// ��ȡ����
        len -= 1;
    }while(len);
    
    if(cs_pin_save[spi_n] != SPI_CS_NULL)					// ��CS��Ϊ�� ������CS
    {
        gpio_high(cs_pin_save[spi_n]);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  �������      SPI��ʼ��
//  ����˵��      spi_n           ѡ��SPIģ��(SPI_0-SPI_2)
//  ����˵��      mode            SPIģʽ 0��CPOL=0 CPHA=0    1��CPOL=0 CPHA=1   2��CPOL=1 CPHA=0   3��CPOL=1 CPHA=1  // ����ϸ�ڿ����в�������
//  ����˵��      baud            ����SPI�Ĳ�����
//  ����˵��      cs_pin          ѡ��SPIƬѡ����
//  ����˵��      sck_pin         ѡ��SPIʱ������
//  ����˵��      mosi_pin        ѡ��SPI MOSI����
//  ����˵��      miso_pin        ѡ��SPI MISO����
//  ���ز���      void
//  ʹ��ʾ��      spi_init(SPI_0, SPI_MODE0, 1*1000*1000, SPI0_CLK_P2_2, SPI0_MOSI_P2_1, SPI0_MISO_P2_0, SPI0_CS0_P2_3); // Ӳ��SPI��ʼ��  ģʽ0 ������Ϊ1Mhz
//  ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void spi_init (spi_index_enum spi_n, spi_mode_enum mode, uint32 baud, spi_clk_pin_enum clk_pin, spi_mosi_pin_enum mosi_pin, spi_miso_pin_enum miso_pin, spi_cs_pin_enum cs_pin)
{
    uint64_t                    targetFreq                      = 4 * baud;
    uint64_t                    sourceFreq_fp5                  = ((uint64_t)SPI_FREQ << 6ull);
    uint32_t                    divSetting_fp5                  = (uint32_t)(sourceFreq_fp5 / targetFreq);
    cy_stc_gpio_pin_config_t    spi_pin_cfg                     = {0};
    cy_stc_scb_spi_config_t     spi_config                      = {0};
    
    // ���ѣ�ģ��źͶ˿ڶ�����Ӧ��ô�ܳ�ʼ���أ�
    zf_assert((uint8)spi_n == (uint8)clk_pin ? 1 : 0);
    zf_assert((uint8)clk_pin == (uint8)mosi_pin ? 1 : 0);

    spi_pin_cfg.driveMode = CY_GPIO_DM_STRONG_IN_OFF;
    spi_pin_cfg.hsiom = spi_get_mosi_hsiom(mosi_pin);
    Cy_GPIO_Pin_Init(get_port(spi_get_mosi_pin(mosi_pin)), (spi_get_mosi_pin(mosi_pin) % 8), &spi_pin_cfg);

    spi_pin_cfg.driveMode = CY_GPIO_DM_STRONG_IN_OFF;
    spi_pin_cfg.hsiom = spi_get_clk_hsiom(clk_pin);
    Cy_GPIO_Pin_Init(get_port(spi_get_clk_pin(clk_pin)), (spi_get_clk_pin(clk_pin) % 8), &spi_pin_cfg);
    
    if(SPI_MISO_NULL != miso_pin)
    {
        spi_pin_cfg.driveMode = CY_GPIO_DM_HIGHZ;
        spi_pin_cfg.hsiom = spi_get_miso_hsiom(miso_pin);
        Cy_GPIO_Pin_Init(get_port(spi_get_miso_pin(miso_pin)), (spi_get_miso_pin(miso_pin) % 8), &spi_pin_cfg);
    }
    if(SPI_CS_NULL != cs_pin)
    {
        spi_pin_cfg.driveMode = CY_GPIO_DM_STRONG_IN_OFF;
        spi_pin_cfg.hsiom = spi_get_cs_hsiom(cs_pin);
        Cy_GPIO_Pin_Init(get_port(spi_get_cs_pin(cs_pin)), (spi_get_cs_pin(cs_pin) % 8), &spi_pin_cfg);
    }
    
    Cy_SysClk_PeriphAssignDivider(spi_get_module_clock(spi_n), CY_SYSCLK_DIV_24_5_BIT, ((uint8)spi_n + 1));
    Cy_SysClk_PeriphSetFracDivider(CY_SYSCLK_DIV_24_5_BIT, ((uint8)spi_n + 1), ((divSetting_fp5 & 0x1FFFFFE0ul) >> 5ul), (divSetting_fp5 & 0x0000001Ful));
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_24_5_BIT, ((uint8)spi_n + 1));
    
    switch(mode)
    {
        case SPI_MODE0:spi_config.sclkMode = CY_SCB_SPI_CPHA0_CPOL0;      break;
        case SPI_MODE1:spi_config.sclkMode = CY_SCB_SPI_CPHA0_CPOL1;      break;
        case SPI_MODE2:spi_config.sclkMode = CY_SCB_SPI_CPHA1_CPOL0;      break;
        case SPI_MODE3:spi_config.sclkMode = CY_SCB_SPI_CPHA1_CPOL1;      break;
    }
    
    spi_config.spiMode                    = CY_SCB_SPI_MASTER     ;
    spi_config.subMode                    = CY_SCB_SPI_MOTOROLA   ;
    spi_config.oversample                 = 4                     ;
    spi_config.rxDataWidth                = 8                     ;
    spi_config.txDataWidth                = 8                     ;
    spi_config.enableMsbFirst             = true                  ;
    spi_config.enableMisoLateSample       = true                  ;
    
    Cy_SCB_SPI_Init(spi_module[spi_n], &spi_config, NULL);
    Cy_SCB_SPI_SetActiveSlaveSelect(spi_module[spi_n], 0ul);
    Cy_SCB_SPI_Enable(spi_module[spi_n]);
    
    cs_pin_save[spi_n] = cs_pin;    
}

