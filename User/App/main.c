/**
  ******************************************************************************
  * @Project    TestDemo
  * @Author     ����
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
 *�������ƣ�prvBoardInit
 *�����������弶�����ʼ��
 *����˵������
 *�� �� ֵ����
 *ע�������
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
}

/**
  * @brief  Main program.
  * @param  None
  * @retval : None
  */
int main(void)
{
    /* ���������ʼ�� */
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
