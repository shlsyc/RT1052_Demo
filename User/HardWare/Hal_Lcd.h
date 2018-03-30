/**
  ******************************************************************************
  * @File       Hal_Lcd
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      LCD�ӿ�����

  * @Modified
  ******************************************************************************
  */
  
#ifndef HAL_LCD_H
#define HAL_LCD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "SysConf.h"

#define LCD_RST_GPIO                                GPIO1
#define LCD_RST_GPIO_PIN                            (2U)
#define LCD_RST_IOMUXC                              IOMUXC_GPIO_AD_B0_02_GPIO1_IO02

#define LCD_BL_GPIO                                 GPIO1
#define LCD_BL_GPIO_PIN                             (15U)
#define LCD_BL_IOMUXC                               IOMUXC_GPIO_AD_B0_15_GPIO1_IO15

/******************************************************************************
 *�������ƣ�Hal_Lcd_Init
 *����������LCD ��ʼ��
 *����˵������
 *�� �� ֵ����
 *ע�������
 */
void Hal_Lcd_Init(void);

/******************************************************************************
 *�������ƣ�Hal_Lcd_PinInit
 *����������LCD ���ų�ʼ��
 *����˵������
 *�� �� ֵ����
 *ע�������
 */
void Hal_Lcd_PinInit(void);

/******************************************************************************
 *�������ƣ�Hal_Lcd_PixelClockInit
 *����������LCD ʱ�ӳ�ʼ��
 *����˵������
 *�� �� ֵ����
 *ע�������
 */
void Hal_Lcd_PixelClockInit(void);

/******************************************************************************
 *�������ƣ�Hal_Lcd_InitRGB
 *����������LCD RGB��ʼ��
 *����˵������
 *�� �� ֵ����
 *ע�������
 */
void Hal_Lcd_InitRGB(void);

/******************************************************************************
 *�������ƣ�Hal_Lcd_Test
 *����������LCD Test
 *����˵������
 *�� �� ֵ����
 *ע�������
 */
void Hal_Lcd_Test(void);


#ifdef __cplusplus
}
#endif

#endif
