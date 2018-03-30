/**
  ******************************************************************************
  * @File       Hal_Uart
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      Led指示灯接口驱动

  * @Modified
  ******************************************************************************
  */
  
#ifndef HAL_UART_H
#define HAL_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "SysConf.h"

typedef enum
{
    UART_CH1 = 1,
    UART_CH2,
    UART_CH3,
    UART_CH4,
    UART_CH5,
    UART_CH6,
    UART_CH7,
    UART_CH8,
}xUART_Chn_t;

extern LPUART_Type *const xLPUART_Bases[];

/******************************************************************************
 *函数名称：Hal_Uart_Init
 *功能描述：串口通用通信初始化
 *参数说明：串口通道-波特率-接收中断开关
 *返 回 值：无
 *注意事项：无
 */
void Hal_Uart_Init(xUART_Chn_t eChn,uint32_t ulBaudRate,bool intRxEnable);

/******************************************************************************
 *函数名称：Hal_Uart_BlockWrite
 *功能描述：串口阻塞式发送
 *参数说明：串口通道-数据指针-数据长度
 *返 回 值：无
 *注意事项：无
 */
void Hal_Uart_BlockWrite(xUART_Chn_t eChn,uint8_t *pucBuf,uint16_t usLen);

#ifdef __cplusplus
}
#endif

#endif
