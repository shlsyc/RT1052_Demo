/**
  ******************************************************************************
  * @File       RT10xx_IT
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      RT10xx 中断接口文件

  * @Modified
  ******************************************************************************
  */

#include "SysConf.h"
#include "Hal_Touch.h"
#include "fsl_lpuart.h"
#include "lv_hal/lv_hal_tick.h"
  
/******************************************************************************
 *函数名称：SysTick_Handler
 *功能描述：滴答时钟中断
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
void SysTick_Handler(void)
{
    g_SysTimeTicks++;
    lv_tick_inc(1);
    if(xTouch_Dev.Enable)
        xTouch.Touch_CapScan();
}

/******************************************************************************
 *函数名称：LPUART1_IRQHandler
 *功能描述：LPUART1中断
 *参数说明：无
 *返 回 值：无
 *注意事项：无
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

