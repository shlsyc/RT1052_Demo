/**
  ******************************************************************************
  * @File       Hal_Lcd
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      LCD接口驱动

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
 *函数名称：Hal_Lcd_Init
 *功能描述：LCD 初始化
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
void Hal_Lcd_Init(void);

/******************************************************************************
 *函数名称：Hal_Lcd_PinInit
 *功能描述：LCD 引脚初始化
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
void Hal_Lcd_PinInit(void);

/******************************************************************************
 *函数名称：Hal_Lcd_PixelClockInit
 *功能描述：LCD 时钟初始化
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
void Hal_Lcd_PixelClockInit(void);

/******************************************************************************
 *函数名称：Hal_Lcd_InitRGB
 *功能描述：LCD RGB初始化
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
void Hal_Lcd_InitRGB(void);

/******************************************************************************
 *函数名称：Hal_Lcd_Test
 *功能描述：LCD Test
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
void Hal_Lcd_Test(void);


#ifdef __cplusplus
}
#endif

#endif
