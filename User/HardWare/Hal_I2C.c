/**
  ******************************************************************************
  * @File       Hal_I2C
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      I2C接口驱动

  * @Modified
  ******************************************************************************
  */
  
#include "Hal_I2C.h"
#include "fsl_iomuxc.h"
#include "fsl_lpi2c.h"

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define LPI2C_CLOCK_FREQUENCY ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

LPI2C_Type *const xLPI2C_Bases[] = LPI2C_BASE_PTRS;

lpi2c_master_handle_t xMasterHandle;
volatile bool bMasterCompletionFlag = false;

void LPI2C_Master_Callback(LPI2C_Type *base, lpi2c_master_handle_t *handle, status_t status, void *userData);

/******************************************************************************
 *函数名称：Hal_I2C_Master_Init
 *功能描述：I2C主机初始化
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
void Hal_I2C_Master_Init(xI2C_Chn_t eChn, uint32_t ulBaudRate)
{
    lpi2c_master_config_t masterConfig;
    
    switch(eChn)
    {
      /* 0xD8B0u
         Slew Rate Field: Slow Slew Rate
         Drive Strength Field: R0/6
         Speed Field: medium(100MHz)
         Open Drain Enable Field: Open Drain Enabled
         Pull / Keep Enable Field: Pull/Keeper Enabled
         Pull / Keep Select Field: Keeper
         Pull Up / Down Config. Field: 22K Ohm Pull Up
         Hyst. Enable Field: Hysteresis Disabled */
        case I2C_CH1:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL,1U);
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA,1U); 
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL,0xD8B0u);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA,0xD8B0u);
            break;
        case I2C_CH2:
            break;
        case I2C_CH3:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_LPI2C3_SCL,1U);
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_LPI2C3_SDA,1U); 
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_07_LPI2C3_SCL,0xD8B0u);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_06_LPI2C3_SDA,0xD8B0u);
            break;
        case I2C_CH4:
            break;
    }
    
    /*Clock setting for LPI2C*/
    CLOCK_SetMux(kCLOCK_Lpi2cMux, LPI2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);
    
    LPI2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Hz = ulBaudRate;
    
    LPI2C_MasterInit(xLPI2C_Bases[eChn], &masterConfig, LPI2C_CLOCK_FREQUENCY);
    LPI2C_MasterTransferCreateHandle(xLPI2C_Bases[eChn], &xMasterHandle, LPI2C_Master_Callback, NULL);
}

/******************************************************************************
 *函数名称：Hal_I2C_ReadBytes
 *功能描述：IIC读取数据
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
uint32_t Hal_I2C_ReadBytes(xI2C_Chn_t eChn,uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToRead)
{
    lpi2c_master_transfer_t masterXfer = {0};
    status_t reVal = kStatus_Fail;

    /* subAddress = 0x01, data = g_master_rxBuff - read from slave.
        start + slaveaddress(w) + subAddress + repeated start + slaveaddress(r) + rx data buffer + stop */
    masterXfer.slaveAddress = (ClientAddr>>1);
    masterXfer.direction = kLPI2C_Read;
    masterXfer.subaddress = (uint32_t)0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = pBuffer;
    masterXfer.dataSize = NumByteToRead;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;

    reVal = LPI2C_MasterTransferNonBlocking(xLPI2C_Bases[eChn], &xMasterHandle, &masterXfer);
    if (reVal != kStatus_Success)
    {
        return 1;
    }
    /*  Reset master completion flag to false. */
    bMasterCompletionFlag = false;

    /*  Wait for transfer completed. */
    while (!bMasterCompletionFlag)
    {
    }
    bMasterCompletionFlag = false;
    
    return 0;
}

/******************************************************************************
 *函数名称：Hal_I2C_WriteBytes
 *功能描述：IIC写入数据
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
uint32_t Hal_I2C_WriteBytes(xI2C_Chn_t eChn,uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToWrite)
{
    lpi2c_master_transfer_t masterXfer = {0};
    status_t reVal = kStatus_Fail;

    /* subAddress = 0x01, data = g_master_rxBuff - read from slave.
        start + slaveaddress(w) + subAddress + repeated start + slaveaddress(r) + rx data buffer + stop */
    masterXfer.slaveAddress = (ClientAddr>>1);
    masterXfer.direction = kLPI2C_Write;
    masterXfer.subaddress = (uint32_t)0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = pBuffer;
    masterXfer.dataSize = NumByteToWrite;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;

    reVal = LPI2C_MasterTransferNonBlocking(xLPI2C_Bases[eChn], &xMasterHandle, &masterXfer);
    if (reVal != kStatus_Success)
    {
        return 1;
    }
    /*  Reset master completion flag to false. */
    bMasterCompletionFlag = false;

    /*  Wait for transfer completed. */
    while (!bMasterCompletionFlag)
    {
    }
    bMasterCompletionFlag = false;

    return 0;
}

/******************************************************************************
 *函数名称：I2C_Transfer
 *功能描述：IIC进行数据传输
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
uint32_t I2C_Transfer(xI2C_Chn_t eChn,xI2C_Msg_t *pxI2C_Msg,uint32_t Num)
{
    uint32_t i;
    uint32_t ret = 0;
    for(i = 0; ret == 0 && i < Num; i++)
    {
        if(pxI2C_Msg[i].Flag & I2C_M_RD)
        {
            ret = Hal_I2C_ReadBytes(eChn,pxI2C_Msg[i].Addr,pxI2C_Msg[i].Buf,pxI2C_Msg[i].Len);
        }
        else
        {
            ret = Hal_I2C_WriteBytes(eChn,pxI2C_Msg[i].Addr,pxI2C_Msg[i].Buf,pxI2C_Msg[i].Len);
        }
    }
    
    if(ret) return ret;
    return i;
}

/******************************************************************************
 *函数名称：I2C_Transfer_Read
 *功能描述：从IIC设备中读取数据
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
uint32_t I2C_Transfer_Read(xI2C_Chn_t eChn,uint8_t ClientAddr,uint8_t* pBuffer, uint8_t RegAddrLen,uint16_t NumByteToRead)
{
    xI2C_Msg_t xI2C_Msg[2];
    uint8_t retry = 0;
    uint32_t ret = 0;
    
    xI2C_Msg[0].Addr = ClientAddr;
    xI2C_Msg[0].Flag = !I2C_M_RD;
    xI2C_Msg[0].Buf = &pBuffer[0];
    xI2C_Msg[0].Len = RegAddrLen;
    
    xI2C_Msg[1].Addr = ClientAddr;
    xI2C_Msg[1].Flag = I2C_M_RD;
    xI2C_Msg[1].Buf = &pBuffer[RegAddrLen];
    xI2C_Msg[1].Len = NumByteToRead - RegAddrLen;
    
    while(retry < 5)
    {
        ret = I2C_Transfer(eChn,xI2C_Msg,2);
        if(ret == 2) break;
        else retry++;
    }
    return ret;
}

/******************************************************************************
 *函数名称：I2C_Transfer_Write
 *功能描述：从IIC设备中写入数据
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
uint32_t I2C_Transfer_Write(xI2C_Chn_t eChn,uint8_t ClientAddr,uint8_t* pBuffer,uint16_t NumByteToWrite)
{
    xI2C_Msg_t xI2C_Msg;
    uint8_t retry = 0;
    uint32_t ret = 0;
    
    xI2C_Msg.Addr = ClientAddr;
    xI2C_Msg.Flag = !I2C_M_RD;
    xI2C_Msg.Buf = &pBuffer[0];
    xI2C_Msg.Len = NumByteToWrite;
    
    while(retry < 5)
    {
        ret = I2C_Transfer(eChn,&xI2C_Msg,1);
        if(ret == 2) break;
        else retry++;
    }
    return ret;
}

void LPI2C_Master_Callback(LPI2C_Type *base, lpi2c_master_handle_t *handle, status_t status, void *userData)
{
    /* Signal transfer success when received success status. */
    if (status == kStatus_Success)
    {
        bMasterCompletionFlag = true;
    }
}
