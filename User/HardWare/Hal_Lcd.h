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

#define LCD_LAYER_1                                 0x0001      /* 顶层 */
#define LCD_LAYER_2                                 0x0002      /* 第2层 */

#define IMG_HEIGHT                                  480
#define IMG_WIDTH                                   800

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
 *函数名称：Hal_Lcd_InitPXP
 *功能描述：LCD PXP初始化
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
void Hal_Lcd_InitPXP(void);

/******************************************************************************
 *函数名称：Hal_Lcd_InitBuffer
 *功能描述：LCD 初始化缓存
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
void Hal_Lcd_InitBuffer(void);

/******************************************************************************
 *函数名称：Hal_Lcd_SetLayer
 *功能描述：切换层。只是更改程序变量，以便于后面的代码更改相关寄存器。硬件支持2层。
 *参数说明：无
 *返 回 值：无
 *注意事项：
 */
void Hal_Lcd_SetLayer(uint8_t _ucLayer);

/******************************************************************************
 *函数名称：Hal_Lcd_SetDirection
 *功能描述：设置显示屏显示方向（横屏 竖屏）
 *参数说明：显示方向代码 0 横屏正常, 1=横屏180度翻转, 2=竖屏, 3=竖屏180度翻转
 *返 回 值：无
 *注意事项：
 */
void Hal_Lcd_SetDirection(uint8_t _dir);

/******************************************************************************
 *函数名称：Hal_Lcd_GetPixel
 *功能描述：读1个像素
 *参数说明：无
 *返 回 值：无
 *注意事项：
 */
uint32_t Hal_Lcd_GetPixel(uint16_t _usX, uint16_t _usY);

/******************************************************************************
 *函数名称：Hal_Lcd_PutPixel
 *功能描述：画1个像素
 *参数说明：无
 *返 回 值：无
 *注意事项：
 */
void Hal_Lcd_PutPixel(uint16_t _usX, uint16_t _usY, uint32_t _usColor);

/******************************************************************************
 *函数名称：Hal_Lcd_FillRect
 *功能描述：用一个颜色值填充一个矩形
 *参数说明：无
 *返 回 值：无
 *注意事项：
 */
void Hal_Lcd_FillRect(uint16_t usX, uint16_t usY, uint16_t usHeight, uint16_t usWidth, uint32_t usColor);

/******************************************************************************
 *函数名称：Hal_Lcd_DrawCircle
 *功能描述：绘制一个圆，笔宽为1个像素
 *参数说明：圆心的坐标-圆的半径
 *返 回 值：无
 *注意事项：
 */
void Hal_Lcd_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint32_t _usColor);

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
