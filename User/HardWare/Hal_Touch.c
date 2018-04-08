/**
  ******************************************************************************
  * @File       Hal_Touch
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      触摸接口驱动

  * @Modified
  ******************************************************************************
  */
  
#include "Hal_Touch.h"
#include "Hal_Lcd_Port.h"
#include "Touch_GT9xx.h"

Touch_Dev_t xTouch_Dev;
Touch_t xTouch;
  
/******************************************************************************
 *函数名称：Hal_Touch_Init
 *功能描述：触摸芯片 硬件初始化
 *参数说明：无
 *返 回 值：无
 *注意事项：
 */
void Hal_Touch_Init(void)
{
    xTouch.Touch_HardInit = GT9xx_Init;
    xTouch.Touch_CapScan = GT9xx_Scan;
    
    xTouch.Touch_GetKey = Hal_Touch_GetKey;
    xTouch.Touch_PutKey = Hal_Touch_PutKey;
    
    xTouch.Touch_HardInit();
}

/******************************************************************************
 *函数名称：Hal_Touch_PutKey
 *功能描述：将1个触摸点坐标值压入触摸FIFO缓冲区
 *参数说明：无
 *返 回 值：无
 *注意事项：
 */
void Hal_Touch_PutKey(uint8_t ucEvent, uint16_t usX, uint16_t usY)
{
    uint16_t xx, yy;
    uint16_t x = 0, y = 0;

    xTouch.Event[xTouch.Write] = ucEvent;

    if (xTouch_Dev.Enable == 1)
    {
        xx = usX;
        yy = usY;
    }

    /* 横屏和竖屏方向识别 */
    if (xLCD_Dev.ucDirection == 0)      /* 横屏 */
    {
        x = xx;
        y = yy;
    }
    else if (xLCD_Dev.ucDirection == 1) /* 横屏180°*/
    {
        x = xLCD_Dev.usWidth - xx - 1;
        y = xLCD_Dev.usHeight - yy - 1;
    }
    else if (xLCD_Dev.ucDirection == 2) /* 竖屏 */
    {
        y = xx;
        x = xLCD_Dev.usWidth - yy - 1;
    }
    else if (xLCD_Dev.ucDirection == 3) /* 竖屏180° */
    {
        y = xLCD_Dev.usHeight - xx - 1;
        x = yy;
    }

    xTouch.XBuf[xTouch.Write] = x;
    xTouch.YBuf[xTouch.Write] = y;

    if (++xTouch.Write  >= TOUCH_FIFO_SIZE)
    {
        xTouch.Write = 0;
    }
}

/******************************************************************************
 *函数名称：Hal_Touch_GetKey
 *功能描述：从触摸FIFO缓冲区读取一个坐标值
 *参数说明：无
 *返 回 值：无
 *注意事项：
 */
uint8_t Hal_Touch_GetKey(int16_t *psX, int16_t *psY)
{
    uint8_t ret;

    if (xTouch.Read == xTouch.Write)
    {
        return TOUCH_NONE;
    }
    else
    {
        ret = xTouch.Event[xTouch.Read];
        *psX = xTouch.XBuf[xTouch.Read];
        *psY = xTouch.YBuf[xTouch.Read];

        if (++xTouch.Read >= TOUCH_FIFO_SIZE)
        {
            xTouch.Read = 0;
        }
        return ret;
    }
}

