/**
  ******************************************************************************
  * @Project    TestDemo
  * @Author     舒来
  * @Version    V1.0.0
  * @Date       2018-3-22
  * @Brief
  *
  * @Modified
  * @CopyRight  
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "UsrInc.h"

/******************************************************************************
 *函数名称：prvBoardInit
 *功能描述：板级外设初始化
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
static void prvBoardInit()
{
    vBoard_ConfigMPU();
    vBoard_BootClockRun();
    SysTick_Config(SystemCoreClock / 1000);
    HalLedInit();
    HalLedBlink(HAL_LED_1,0,50,1000);
    Hal_Uart_Init(UART_CH1,38400,true);
    Hal_Lcd_Init();
    Hal_Lcd_Test();
    printf("Hello World!:%d\r\n",SystemCoreClock);
}

/**
  * @brief  Main program.
  * @param  None
  * @retval : None
  */
int main(void)
{
    /* 板载外设初始化 */
    prvBoardInit();
    
    while(1)
    {
        HalLedUpdate();
        printf("Hello World!:%d\r\n",SystemCoreClock);
        SConf_DelayUS(200*1000);
    }
}


/**
  * @}
  */

/*********************************END OF FILE**********************************/
