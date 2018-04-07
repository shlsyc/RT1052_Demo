/**
  ******************************************************************************
  * @File       Hal_I2C
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      I2C�ӿ�����

  * @Modified
  ******************************************************************************
  */
  
#ifndef HAL_I2C_H
#define HAL_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "SysConf.h"

#define I2C_M_RD            0x0001

typedef enum
{
    I2C_CH1 = 1,
    I2C_CH2,
    I2C_CH3,
    I2C_CH4,
}xI2C_Chn_t;

typedef struct
{
    uint8_t Addr;           /* ���豸��I2C�豸��ַ */
    uint16_t Flag;          /* ���Ʊ�־ */
    uint16_t Len;           /* ��д���ݵĳ��� */
    uint8_t *Buf;           /* �洢��д���ݵ�ָ�� */
}xI2C_Msg_t;

/******************************************************************************
 *�������ƣ�Hal_I2C_Master_Init
 *����������I2C������ʼ��
 *����˵������
 *�� �� ֵ����
 *ע�������
 */
void Hal_I2C_Master_Init(xI2C_Chn_t eChn, uint32_t ulBaudRate);

/******************************************************************************
 *�������ƣ�Hal_I2C_ReadBytes
 *����������IIC��ȡ����
 *����˵������
 *�� �� ֵ����
 *ע�������
 */
uint32_t Hal_I2C_ReadBytes(xI2C_Chn_t eChn,uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToRead);

/******************************************************************************
 *�������ƣ�I2C_Transfer_Read
 *������������IIC�豸�ж�ȡ����
 *����˵������
 *�� �� ֵ����
 *ע�������
 */
uint32_t I2C_Transfer_Read(xI2C_Chn_t eChn,uint8_t ClientAddr,uint8_t* pBuffer, uint8_t RegAddrLen,uint16_t NumByteToRead);

/******************************************************************************
 *�������ƣ�I2C_Transfer_Write
 *������������IIC�豸��д������
 *����˵������
 *�� �� ֵ����
 *ע�������
 */
uint32_t I2C_Transfer_Write(xI2C_Chn_t eChn,uint8_t ClientAddr,uint8_t* pBuffer,uint16_t NumByteToWrite);

/******************************************************************************
 *�������ƣ�Hal_I2C_WriteBytes
 *����������IICд������
 *����˵������
 *�� �� ֵ����
 *ע�������
 */
uint32_t Hal_I2C_WriteBytes(xI2C_Chn_t eChn,uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToWrite);

#ifdef __cplusplus
}
#endif



#endif
