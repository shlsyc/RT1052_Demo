/**
  ******************************************************************************
  * @File       Hal_Uart
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      Ledָʾ�ƽӿ�����

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
 *�������ƣ�Hal_Uart_Init
 *��������������ͨ��ͨ�ų�ʼ��
 *����˵��������ͨ��-������-�����жϿ���
 *�� �� ֵ����
 *ע�������
 */
void Hal_Uart_Init(xUART_Chn_t eChn,uint32_t ulBaudRate,bool intRxEnable);

/******************************************************************************
 *�������ƣ�Hal_Uart_BlockWrite
 *������������������ʽ����
 *����˵��������ͨ��-����ָ��-���ݳ���
 *�� �� ֵ����
 *ע�������
 */
void Hal_Uart_BlockWrite(xUART_Chn_t eChn,uint8_t *pucBuf,uint16_t usLen);

#ifdef __cplusplus
}
#endif

#endif
