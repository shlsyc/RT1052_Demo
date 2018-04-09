/**
  ******************************************************************************
  * @File       SysConf
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      ϵͳ���ýӿ������ļ�

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
 *�������ƣ�SConf_DelayUS
 *����������΢�뼶��ʱ
 *����˵����us:΢����
 *�� �� ֵ����
 *ע�������ʹ��ucOS
 */
void SConf_DelayUS(uint32_t nus);


#ifdef __cplusplus
}
#endif

#endif
