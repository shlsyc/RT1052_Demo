/**
  ******************************************************************************
  * @File       Hal_Uart
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      Ledָʾ�ƽӿ�����

  * @Modified
  ******************************************************************************
  */
  
#include "Hal_Uart.h"
#include "fsl_iomuxc.h"
#include "fsl_lpuart.h"

LPUART_Type *const xLPUART_Bases[] = LPUART_BASE_PTRS;

uint32_t GetSrcClockFreq(void)
{
    uint32_t freq;

    /* To make it simple, we assume default PLL and divider settings, and the only variable
       from application is use PLL3 source or OSC source */
    if (CLOCK_GetMux(kCLOCK_UartMux) == 0) /* PLL3 div6 80M */
    {
        freq = (CLOCK_GetPllFreq(kCLOCK_PllUsb1) / 6U) / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);
    }
    else
    {
        freq = CLOCK_GetOscFreq() / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);
    }

    return freq;
} 

/******************************************************************************
 *�������ƣ�Hal_Uart_Init
 *��������������ͨ��ͨ�ų�ʼ��
 *����˵��������ͨ��-������-�����жϿ���
 *�� �� ֵ����
 *ע�������
 */
void Hal_Uart_Init(xUART_Chn_t eChn,uint32_t ulBaudRate,bool intRxEnable)
{
    lpuart_config_t lpuart_config;
    IRQn_Type irqn;

    LPUART_GetDefaultConfig(&lpuart_config);
    lpuart_config.baudRate_Bps = ulBaudRate;
    if(intRxEnable)
        lpuart_config.enableRx = true;
    lpuart_config.enableTx = true;
    
    CLOCK_EnableClock(kCLOCK_Iomuxc);
    
    switch(eChn)
    {
    /* 0x10B0u
       Slew Rate Field: Slow Slew Rate
       Drive Strength Field: R0/6
       Speed Field: medium(100MHz)
       Open Drain Enable Field: Open Drain Disabled
       Pull / Keep Enable Field: Pull/Keeper Enabled
       Pull / Keep Select Field: Keeper
       Pull Up / Down Config. Field: 100K Ohm Pull Down
       Hyst. Enable Field: Hysteresis Disabled  */
        case UART_CH1:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_12_LPUART1_TX, 0U);                                 
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_LPUART1_RX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_12_LPUART1_TX,0x10B0u);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_13_LPUART1_RX,0x10B0u);
            irqn = LPUART1_IRQn;
            break;
        case UART_CH2:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_02_LPUART2_TX, 0U);                                 
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_03_LPUART2_RX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_02_LPUART2_TX,0x10B0u);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_03_LPUART2_RX,0x10B0u);
            irqn = LPUART2_IRQn;
            break;
        case UART_CH3:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_LPUART3_TX, 0U);                                 
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_LPUART3_RX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_06_LPUART3_TX,0x10B0u);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_07_LPUART3_RX,0x10B0u);
            irqn = LPUART3_IRQn;
            break;
        case UART_CH4:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_00_LPUART4_TX, 0U);                                 
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_01_LPUART4_RX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_00_LPUART4_TX,0x10B0u);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_01_LPUART4_RX,0x10B0u);
            irqn = LPUART4_IRQn;
            break;
        case UART_CH5:
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_12_LPUART5_TX, 0U);                                 
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_13_LPUART5_RX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_12_LPUART5_TX,0x10B0u);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_13_LPUART5_RX,0x10B0u);
            irqn = LPUART5_IRQn;
            break;
        case UART_CH6:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_02_LPUART6_TX, 0U);                                 
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_03_LPUART6_RX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_02_LPUART6_TX,0x10B0u);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_03_LPUART6_RX,0x10B0u);
            irqn = LPUART6_IRQn;
            break;
        case UART_CH7:
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_31_LPUART7_TX, 0U);                                 
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_32_LPUART7_RX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_31_LPUART7_TX,0x10B0u);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_32_LPUART7_RX,0x10B0u);
            irqn = LPUART7_IRQn;
            break;
        case UART_CH8:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_10_LPUART8_TX, 0U);                                 
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_11_LPUART8_RX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_10_LPUART8_TX,0x10B0u);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_11_LPUART8_RX,0x10B0u);
            irqn = LPUART8_IRQn;
            break;
        default:
            break;
    }
    
    LPUART_Init(xLPUART_Bases[eChn],&lpuart_config,GetSrcClockFreq());
    
    if(intRxEnable)
    {
        LPUART_EnableInterrupts(xLPUART_Bases[eChn], kLPUART_RxDataRegFullInterruptEnable);
        EnableIRQ(irqn);       
    }
}

/******************************************************************************
 *�������ƣ�Hal_Uart_BlockWrite
 *������������������ʽ����
 *����˵��������ͨ��-����ָ��-���ݳ���
 *�� �� ֵ����
 *ע�������
 */
void Hal_Uart_BlockWrite(xUART_Chn_t eChn,uint8_t *pucBuf,uint16_t usLen)
{
    LPUART_WriteBlocking(xLPUART_Bases[eChn],pucBuf,usLen);
}
