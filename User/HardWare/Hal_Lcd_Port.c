/**
  ******************************************************************************
  * @File       Hal_LCD_Port
  * @Author     shulai
  * @Date       2018-3-11
  * @Brief      LCD½Ó¿ÚÇý¶¯

  * @Modified
  ******************************************************************************
  */
  
#include "Hal_Lcd_Port.h"
#include "Hal_Lcd.h"

LCD_Dev_t xLCD_Dev = 
{
    .usWidth            =           LCD_WIDTH,
    .usHeight           =           LCD_HEIGHT,
    .ucDirection        =           1,

    .vLCD_HardInit      =           Hal_Lcd_Init,
    .vLCD_SetDirection  =           Hal_Lcd_SetDirection,
    .usReadPoint        =           Hal_Lcd_GetPixel,
    .vDrawPoint         =           Hal_Lcd_PutPixel,
    .vFillRect          =           Hal_Lcd_FillRect,
};


