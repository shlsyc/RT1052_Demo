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

typedef struct
{
    uint8_t Event[TOUCH_FIFO_SIZE];         /* �����¼� */
    int16_t XBuf[TOUCH_FIFO_SIZE];          /* �������껺���� */
    int16_t YBuf[TOUCH_FIFO_SIZE];          /* �������껺���� */
    uint8_t Read;                           /* ��������ָ�� */
    uint8_t Write;                          /* ������дָ�� */
}TOUCH_t;

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

#ifdef __cplusplus
}
#endif


#endif
