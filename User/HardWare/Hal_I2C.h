/**
  ******************************************************************************
  * @File       Hal_I2C
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      I2C接口驱动

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
    uint8_t Addr;           /* 从设备的I2C设备地址 */
    uint16_t Flag;          /* 控制标志 */
    uint16_t Len;           /* 读写数据的长度 */
    uint8_t *Buf;           /* 存储读写数据的指针 */
}xI2C_Msg_t;

/******************************************************************************
 *函数名称：Hal_I2C_Master_Init
 *功能描述：I2C主机初始化
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
void Hal_I2C_Master_Init(xI2C_Chn_t eChn, uint32_t ulBaudRate);

/******************************************************************************
 *函数名称：Hal_I2C_ReadBytes
 *功能描述：IIC读取数据
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
uint32_t Hal_I2C_ReadBytes(xI2C_Chn_t eChn,uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToRead);

/******************************************************************************
 *函数名称：I2C_Transfer_Read
 *功能描述：从IIC设备中读取数据
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
uint32_t I2C_Transfer_Read(xI2C_Chn_t eChn,uint8_t ClientAddr,uint8_t* pBuffer, uint8_t RegAddrLen,uint16_t NumByteToRead);

/******************************************************************************
 *函数名称：I2C_Transfer_Write
 *功能描述：从IIC设备中写入数据
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
uint32_t I2C_Transfer_Write(xI2C_Chn_t eChn,uint8_t ClientAddr,uint8_t* pBuffer,uint16_t NumByteToWrite);

/******************************************************************************
 *函数名称：Hal_I2C_WriteBytes
 *功能描述：IIC写入数据
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
uint32_t Hal_I2C_WriteBytes(xI2C_Chn_t eChn,uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToWrite);

#ifdef __cplusplus
}
#endif



#endif
