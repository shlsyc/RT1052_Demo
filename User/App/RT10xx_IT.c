/**
  ******************************************************************************
  * @File       RT10xx_IT
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      RT10xx �жϽӿ��ļ�

  * @Modified
  ******************************************************************************
  */

#include "SysConf.h"
#include "fsl_lpuart.h"
  
/******************************************************************************
 *�������ƣ�SysTick_Handler
 *�����������δ�ʱ���ж�
 *����˵������
 *�� �� ֵ����
 *ע�������
 */
void SysTick_Handler(void)
{
    g_SysTimeTicks++;
}

/******************************************************************************
 *�������ƣ�LPUART1_IRQHandler
 *����������LPUART1�ж�
 *����˵������
 *�� �� ֵ����
 *ע�������
 */
void LPUART1_IRQHandler(void)
{
    /* UART in mode Receiver -------------------------------------------------*/
    if (LPUART_GetStatusFlags(LPUART1) & kLPUART_RxDataRegFullFlag)
    {
        LPUART_ReadByte(LPUART1);
    }

    /* If RX overrun. */
    if (LPUART_STAT_OR_MASK & LPUART1->STAT)
    {
        /* Clear overrun flag, otherwise the RX does not work. */
        LPUART1->STAT = ((LPUART1->STAT & 0x3FE00000U) | LPUART_STAT_OR_MASK);
    }
}
