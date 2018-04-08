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
#define TOUCH_MAX_POINTS        5

typedef struct
{
    uint16_t ChipID;
    uint8_t Enable;
    uint8_t TimerCount;
    
    uint8_t Count;                          /* 几个点按下 */
    
    uint8_t id[TOUCH_MAX_POINTS];
    uint16_t X[TOUCH_MAX_POINTS];
    uint16_t Y[TOUCH_MAX_POINTS];
}Touch_Dev_t;
extern Touch_Dev_t xTouch_Dev;

typedef struct
{
    uint8_t Event[TOUCH_FIFO_SIZE];         /* 触摸事件 */
    int16_t XBuf[TOUCH_FIFO_SIZE];          /* 触摸坐标缓冲区 */
    int16_t YBuf[TOUCH_FIFO_SIZE];          /* 触摸坐标缓冲区 */
    uint8_t Read;                           /* 缓冲区读指针 */
    uint8_t Write;                          /* 缓冲区写指针 */
    
    void (*Touch_HardInit)();               /* 触控硬件初始化 */
    void (*Touch_PutKey)(uint8_t ucEvent, uint16_t usX, uint16_t usY);
    uint8_t (*Touch_GetKey)(int16_t *psX, int16_t *psY);   
    void (*Touch_CapScan)(void);
}Touch_t;
extern Touch_t xTouch;

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

/******************************************************************************
 *函数名称：Hal_Touch_PutKey
 *功能描述：将1个触摸点坐标值压入触摸FIFO缓冲区
 *参数说明：无
 *返 回 值：无
 *注意事项：
 */
void Hal_Touch_PutKey(uint8_t ucEvent, uint16_t usX, uint16_t usY);

/******************************************************************************
 *函数名称：Hal_Touch_GetKey
 *功能描述：从触摸FIFO缓冲区读取一个坐标值
 *参数说明：无
 *返 回 值：无
 *注意事项：
 */
uint8_t Hal_Touch_GetKey(int16_t *psX, int16_t *psY);

#ifdef __cplusplus
}
#endif


#endif
