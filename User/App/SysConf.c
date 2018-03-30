/**
  ******************************************************************************
  * @File       SysConf
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      ϵͳ���ýӿ������ļ�

  * @Modified
  ******************************************************************************
  */

#include "SysConf.h"
#include "Hal_Uart.h"
#include "fsl_clock.h"

/* ARM PLL configuration for RUN mode */
const clock_arm_pll_config_t armPllConfig = {.loopDivider = 100U};
/* SYS PLL configuration for RUN mode */
const clock_sys_pll_config_t sysPllConfig = {.loopDivider = 1U};
/* USB1 PLL configuration for RUN mode */
const clock_usb_pll_config_t usb1PllConfig = {.loopDivider = 0U};

volatile uint32_t g_SysTimeTicks = 0;

/******************************************************************************
 *�������ƣ�vBoard_ConfigMPU
 *������������ʼ���ڴ����Ԫ 
 *����˵������
 *�� �� ֵ����
 *ע�������
 */
void vBoard_ConfigMPU()
{
    /* Disable I cache and D cache */ 
    SCB_DisableICache();
    SCB_DisableDCache();

    /* Disable MPU */ 
    ARM_MPU_Disable();

    /* Region 0 setting */    
    MPU->RBAR = ARM_MPU_RBAR(0, 0xC0000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

    /* Region 1 setting */
    MPU->RBAR = ARM_MPU_RBAR(1, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

    /* Region 2 setting */
    MPU->RBAR = ARM_MPU_RBAR(2, 0x60000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

    /* Region 3 setting */
    MPU->RBAR = ARM_MPU_RBAR(3, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

    /* Region 4 setting */
    MPU->RBAR = ARM_MPU_RBAR(4, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128KB);

    /* Region 5 setting */
    MPU->RBAR = ARM_MPU_RBAR(5, 0x20000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128KB);    

    /* Region 6 setting */
    MPU->RBAR = ARM_MPU_RBAR(6, 0x20200000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_256KB);    

#if defined(SDRAM_MPU_INIT)
    /* Region 7 setting */
    MPU->RBAR = ARM_MPU_RBAR(7, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_32MB);    

    /* Region 8 setting */
    MPU->RBAR = ARM_MPU_RBAR(8, 0x81E00000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_2MB);   
#endif

    /* Enable MPU */ 
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

    /* Enable I cache and D cache */ 
    SCB_EnableDCache();
    SCB_EnableICache();
}

/******************************************************************************
 *�������ƣ�vBoard_BootClockGate
 *�����������ر�δʹ�õ�ʱ�� 
 *����˵������
 *�� �� ֵ����
 *ע�������
 */
static void vBoard_BootClockGate(void)
{
    /* Disable all unused peripheral clock */
    CCM->CCGR0 = 0x00C0000FU;
    CCM->CCGR1 = 0x30000000U;
    CCM->CCGR2 = 0xFF3F303FU;
    CCM->CCGR3 = 0xF0000330U;
    CCM->CCGR4 = 0x0000FF3CU;
    CCM->CCGR5 = 0xF003330FU;
    CCM->CCGR6 = 0x00FC0F00U;
}

/******************************************************************************
 *�������ƣ�vBoard_BootClockRun
 *������������ʼ��ʱ�� 
 *����˵������
 *�� �� ֵ����
 *ע�������
 */
void vBoard_BootClockRun()
{
    /* Boot ROM did initialize the XTAL, here we only sets external XTAL OSC freq */
    CLOCK_SetXtalFreq(24000000U);
    CLOCK_SetRtcXtalFreq(32768U);

    CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 0x1); /* Set PERIPH_CLK2 MUX to OSC */
    CLOCK_SetMux(kCLOCK_PeriphMux, 0x1);     /* Set PERIPH_CLK MUX to PERIPH_CLK2 */

    /* Setting the VDD_SOC to 1.5V. It is necessary to config AHB to 600Mhz */
    DCDC->REG3 = (DCDC->REG3 & (~DCDC_REG3_TRG_MASK)) | DCDC_REG3_TRG(0x12);

    CLOCK_InitArmPll(&armPllConfig); /* Configure ARM PLL to 1200M */
#ifndef SKIP_SYSCLK_INIT
    CLOCK_InitSysPll(&sysPllConfig); /* Configure SYS PLL to 528M */
#endif
#ifndef SKIP_USB_PLL_INIT
    CLOCK_InitUsb1Pll(&usb1PllConfig); /* Configure USB1 PLL to 480M */
#endif
    CLOCK_SetDiv(kCLOCK_ArmDiv, 0x1); /* Set ARM PODF to 0, divide by 2 */
    CLOCK_SetDiv(kCLOCK_AhbDiv, 0x0); /* Set AHB PODF to 0, divide by 1 */
    CLOCK_SetDiv(kCLOCK_IpgDiv, 0x3); /* Set IPG PODF to 3, divede by 4 */

    CLOCK_SetMux(kCLOCK_PrePeriphMux, 0x3); /* Set PRE_PERIPH_CLK to PLL1, 1200M */
    CLOCK_SetMux(kCLOCK_PeriphMux, 0x0);    /* Set PERIPH_CLK MUX to PRE_PERIPH_CLK */

    /* Disable unused clock */
    vBoard_BootClockGate();

    /* Power down all unused PLL */
    CLOCK_DeinitAudioPll();
    CLOCK_DeinitVideoPll();
    CLOCK_DeinitEnetPll();
    CLOCK_DeinitUsb2Pll();

    /* Configure UART divider to default */
    CLOCK_SetMux(kCLOCK_UartMux, 0); /* Set UART source to PLL3 80M */
    CLOCK_SetDiv(kCLOCK_UartDiv, 0); /* Set UART divider to 1 */

    /* Update core clock */
    SystemCoreClockUpdate();
    
    /* Enable iomuxc clock */
    CLOCK_EnableClock(kCLOCK_Iomuxc);
}

/******************************************************************************
 *�������ƣ�SConf_DelayUS
 *����������΢�뼶��ʱ
 *����˵����us:΢����
 *�� �� ֵ����
 *ע�������ʹ��ucOS
 */
void SConf_DelayUS(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told,tnow,tcnt=0;
    uint16_t fac_us = SystemCoreClock/1000000;
    uint32_t reload = SysTick->LOAD;        //LOAD��ֵ
    ticks = nus*fac_us;                     //��Ҫ�Ľ�����
    tcnt = 0;
    told = SysTick->VAL;                    //�ս���ʱ�ļ�����ֵ
    while(1)
    {
        tnow=SysTick->VAL;  
        if(tnow!=told)
        {       
            if(tnow<told)tcnt+=told-tnow;   //����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
            else tcnt+=reload-tnow+told;
            told=tnow;
            if(tcnt>=ticks)break;           //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
        }  
    }
}

/********************************************************************************
 *��׼����Ҫ��֧�ֺ���
 *�ض���printf()����
 ********************************************************************************/
#pragma import(__use_no_semihosting)    //ȷ��û�д� C ������ʹ�ð������ĺ���

struct __FILE
{
int handle;
/* Whatever you require here. If the only file you are using is */
/* standard output using printf() for debugging, no file handling */
/* is required. */
};
/* FILE is typedef' d in stdio.h. */
FILE __stdout;
FILE __stdin;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
x=x;
}

void _ttywrch(int ch)
{
ch = ch;
}

/*******************************************************************************
* Function Name  : int fputc(int ch,FILE *f)
* Description    : Retargets the C library printf function to the USART
* Input          : None
* Output         : None
* Return         : ������ַ�
*******************************************************************************/
int fputc(int ch,FILE *f)
{
    while (!(xLPUART_Bases[UART_CH1]->STAT & LPUART_STAT_TDRE_MASK))
    {
    }
    xLPUART_Bases[UART_CH1]->DATA = ch;
    return ch;
}


