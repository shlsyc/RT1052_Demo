/**
  ******************************************************************************
  * @File       Hal_Touch
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      触摸接口驱动

  * @Modified
  ******************************************************************************
  */
  
#ifndef HAL_TOUCH_H
#define HAL_TOUCH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "SysConf.h"

#define TOUCH_FIFO_SIZE         20

typedef struct
{
    uint8_t Event[TOUCH_FIFO_SIZE];         /* 触摸事件 */
    int16_t XBuf[TOUCH_FIFO_SIZE];          /* 触摸坐标缓冲区 */
    int16_t YBuf[TOUCH_FIFO_SIZE];          /* 触摸坐标缓冲区 */
    uint8_t Read;                           /* 缓冲区读指针 */
    uint8_t Write;                          /* 缓冲区写指针 */
}TOUCH_t;

enum
{
    TOUCH_NONE = 0,                         /* 无触摸 */
    TOUCH_DOWN = 1,                         /* 按下 */
    TOUCH_MOVE = 2,                         /* 移动 */
    TOUCH_RELEASE = 3                       /* 释放 */
};

/******************************************************************************
 *函数名称：Hal_Touch_Init
 *功能描述：触摸芯片 硬件初始化
 *参数说明：无
 *返 回 值：无
 *注意事项：
 */
void Hal_Touch_Init(void);

#ifdef __cplusplus
}
#endif


#endif
