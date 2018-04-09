/**
  ******************************************************************************
  * @File       SysConf
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      系统配置接口驱动文件

  * @Modified
  ******************************************************************************
  */
  
#ifndef SYSCONF_H
#define SYSCONF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>
#include "fsl_common.h"

#define FALSE                       0
#define TRUE                        1

extern volatile uint32_t g_SysTimeTicks;

/******************************************************************************
 *函数名称：SConf_DelayUS
 *功能描述：微秒级延时
 *参数说明：us:微秒数
 *返 回 值：无
 *注意事项：不使用ucOS
 */
void SConf_DelayUS(uint32_t nus);


#ifdef __cplusplus
}
#endif

#endif
