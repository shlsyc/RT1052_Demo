/**
  ******************************************************************************
  * @File       Hal_Touch
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      �����ӿ�����

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
    
    uint8_t Count;                          /* �����㰴�� */
    
    uint8_t id[TOUCH_MAX_POINTS];
    uint16_t X[TOUCH_MAX_POINTS];
    uint16_t Y[TOUCH_MAX_POINTS];
}Touch_Dev_t;
extern Touch_Dev_t xTouch_Dev;

typedef struct
{
    uint8_t Event[TOUCH_FIFO_SIZE];         /* �����¼� */
    int16_t XBuf[TOUCH_FIFO_SIZE];          /* �������껺���� */
    int16_t YBuf[TOUCH_FIFO_SIZE];          /* �������껺���� */
    uint8_t Read;                           /* ��������ָ�� */
    uint8_t Write;                          /* ������дָ�� */
    
    void (*Touch_HardInit)();               /* ����Ӳ����ʼ�� */
    void (*Touch_PutKey)(uint8_t ucEvent, uint16_t usX, uint16_t usY);
    uint8_t (*Touch_GetKey)(int16_t *psX, int16_t *psY);   
    void (*Touch_CapScan)(void);
}Touch_t;
extern Touch_t xTouch;

enum
{
    TOUCH_NONE = 0,                         /* �޴��� */
    TOUCH_DOWN = 1,                         /* ���� */
    TOUCH_MOVE = 2,                         /* �ƶ� */
    TOUCH_RELEASE = 3                       /* �ͷ� */
};

/******************************************************************************
 *�������ƣ�Hal_Touch_Init
 *��������������оƬ Ӳ����ʼ��
 *����˵������
 *�� �� ֵ����
 *ע�����
 */
void Hal_Touch_Init(void);

/******************************************************************************
 *�������ƣ�Hal_Touch_PutKey
 *������������1������������ֵѹ�봥��FIFO������
 *����˵������
 *�� �� ֵ����
 *ע�����
 */
void Hal_Touch_PutKey(uint8_t ucEvent, uint16_t usX, uint16_t usY);

/******************************************************************************
 *�������ƣ�Hal_Touch_GetKey
 *�����������Ӵ���FIFO��������ȡһ������ֵ
 *����˵������
 *�� �� ֵ����
 *ע�����
 */
uint8_t Hal_Touch_GetKey(int16_t *psX, int16_t *psY);

#ifdef __cplusplus
}
#endif


#endif
