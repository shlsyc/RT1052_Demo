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
#include "lv_porting.h"
#include <rthw.h>
#include <rtthread.h>

/******************************************************************************
 *函数名称：prvBoardInit
 *功能描述：板级外设初始化
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
static void prvBoardInit()
{
    HalLedInit();
    HalLedBlink(HAL_LED_1,0,50,1000);
    Hal_Lcd_Init();
    Hal_Touch_Init();
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
//    lv_porting();

    while (1)
    {
        HalLedUpdate();
        rt_thread_delay(RT_TICK_PER_SECOND/10);
    }
}


/**
  * @}
  */

/*********************************END OF FILE**********************************/
