/**
  ******************************************************************************
  * @File       Touch_GT9xx
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      I2C电容屏驱动

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

#define GTP_REG_TP1             0X8150      //第一个触摸点数据地址
#define GTP_REG_TP2             0X8158      //第二个触摸点数据地址
#define GTP_REG_TP3             0X8160      //第三个触摸点数据地址
#define GTP_REG_TP4             0X8168      //第四个触摸点数据地址
#define GTP_REG_TP5             0X8170      //第五个触摸点数据地址

#define GTP_INFO(fmt,arg...)    printf("<<-GTP-INFO->> "fmt"\n",##arg)
#define GTP_ERROR(fmt,arg...)   printf("<<-GTP-ERROR->> "fmt"\n",##arg)




typedef enum 
{
    GT9157 = 0,
    GT911 = 1,
}TOUCH_IC;

typedef struct
{
    uint16_t ChipID;
    uint8_t Enable;
    uint8_t TimerCount;
    
    uint8_t Count;              /* 几个点按下 */
    
    uint16_t X[GTP_MAX_TOUCH];
    uint16_t Y[GTP_MAX_TOUCH];
    uint8_t id[GTP_MAX_TOUCH];
    uint8_t Event[GTP_MAX_TOUCH];
}Touch_GT9xx_T;
extern Touch_GT9xx_T Touch_GT9xx;

/******************************************************************************
 *函数名称：GT9xx_Init
 *功能描述：触摸面板初始化
 *参数说明：无
 *返 回 值：无
 *注意事项：
 */
void GT9xx_Init(void);

/******************************************************************************
 *函数名称：GT9xx_ConfigPara
 *功能描述：配置参数
 *参数说明：无
 *返 回 值：无
 *注意事项：
 */
int32_t GT9xx_ConfigPara(void);

/******************************************************************************
 *函数名称：GTP_Get_Info
 *功能描述：获取参数信息
 *参数说明：无
 *返 回 值：无
 *注意事项：
 */
int32_t GTP_Get_Info(void);

/******************************************************************************
 *函数名称：GT9xx_ReadID
 *功能描述：读取芯片ID
 *参数说明：无
 *返 回 值：无
 *注意事项：
 */
int32_t GT9xx_ReadID(void);

#ifdef __cplusplus
}
#endif

#endif
