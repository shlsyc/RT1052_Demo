/**
  ******************************************************************************
  * @File       Touch_GT9xx
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      I2C����������

  * @Modified
  ******************************************************************************
  */
  
#ifndef TOUCH_GT9XX_H
#define TOUCH_GT9XX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "SysConf.h"

#define GTP_ADDR                0xBA
#define GTP_MAX_HEIGHT          480
#define GTP_MAX_WIDTH           800
#define GTP_INT_TRIGGER         0
#define GTP_MAX_TOUCH           5

#define GTP_ADDR_LENGTH         2
#define GTP_CONFIG_MIN_LENGTH   186
#define GTP_CONFIG_MAX_LENGTH   240

#define GTP_READ_COOR_ADDR      0x814E
#define GTP_REG_SLEEP           0x8040
#define GTP_REG_CONFIG_DATA     0x8047
#define GTP_REG_VERSION         0x8140

#define GTP_REG_TP1             0X8150      //��һ�����������ݵ�ַ
#define GTP_REG_TP2             0X8158      //�ڶ������������ݵ�ַ
#define GTP_REG_TP3             0X8160      //���������������ݵ�ַ
#define GTP_REG_TP4             0X8168      //���ĸ����������ݵ�ַ
#define GTP_REG_TP5             0X8170      //��������������ݵ�ַ

#define GTP_INFO(fmt,arg...)    printf("<<-GTP-INFO->> "fmt"\n",##arg)
#define GTP_ERROR(fmt,arg...)   printf("<<-GTP-ERROR->> "fmt"\n",##arg)




typedef enum 
{
    GT9157 = 0,
    GT911 = 1,
}TOUCH_IC;

/******************************************************************************
 *�������ƣ�GT9xx_Init
 *������������������ʼ��
 *����˵������
 *�� �� ֵ����
 *ע�����
 */
void GT9xx_Init(void);

/******************************************************************************
 *�������ƣ�GT9xx_ConfigPara
 *�������������ò���
 *����˵������
 *�� �� ֵ����
 *ע�����
 */
int32_t GT9xx_ConfigPara(void);

/******************************************************************************
 *�������ƣ�GTP_Get_Info
 *������������ȡ������Ϣ
 *����˵������
 *�� �� ֵ����
 *ע�����
 */
int32_t GT9xx_Get_Info(void);

/******************************************************************************
 *�������ƣ�GT9xx_ReadID
 *������������ȡоƬID
 *����˵������
 *�� �� ֵ����
 *ע�����
 */
int32_t GT9xx_ReadID(void);

/******************************************************************************
 *�������ƣ�GT9xx_Scan
 *������������ȡ�������ݡ���ȡȫ�������ݣ���Ҫ 720us����
 *����˵������
 *�� �� ֵ����
 *ע�����
 */
void GT9xx_Scan(void);

#ifdef __cplusplus
}
#endif

#endif
