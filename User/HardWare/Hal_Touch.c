/**
  ******************************************************************************
  * @File       Hal_Touch
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      �����ӿ�����

  * @Modified
  ******************************************************************************
  */
  
#include "Hal_Touch.h"
#include "Hal_Lcd_Port.h"
#include "Touch_GT9xx.h"

Touch_Dev_t xTouch_Dev;
Touch_t xTouch;
  
/******************************************************************************
 *�������ƣ�Hal_Touch_Init
 *��������������оƬ Ӳ����ʼ��
 *����˵������
 *�� �� ֵ����
 *ע�����
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
 *�������ƣ�Hal_Touch_PutKey
 *������������1������������ֵѹ�봥��FIFO������
 *����˵������
 *�� �� ֵ����
 *ע�����
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

    /* ��������������ʶ�� */
    if (xLCD_Dev.ucDirection == 0)      /* ���� */
    {
        x = xx;
        y = yy;
    }
    else if (xLCD_Dev.ucDirection == 1) /* ����180��*/
    {
        x = xLCD_Dev.usWidth - xx - 1;
        y = xLCD_Dev.usHeight - yy - 1;
    }
    else if (xLCD_Dev.ucDirection == 2) /* ���� */
    {
        y = xx;
        x = xLCD_Dev.usWidth - yy - 1;
    }
    else if (xLCD_Dev.ucDirection == 3) /* ����180�� */
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
 *�������ƣ�Hal_Touch_GetKey
 *�����������Ӵ���FIFO��������ȡһ������ֵ
 *����˵������
 *�� �� ֵ����
 *ע�����
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

