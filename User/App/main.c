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
#include "lv_porting.h"

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
    Hal_Uart_Init(UART_CH1,115200,true);
    Hal_Lcd_Init();
    Hal_Touch_Init();
//    Hal_Lcd_Test();
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
    lv_porting();
    
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
