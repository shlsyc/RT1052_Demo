/**
  ******************************************************************************
  * @File       SysConf
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      ϵͳ���ýӿ������ļ�

  * @Modified
  ******************************************************************************
  */

#include "SysConf.h"

volatile uint32_t g_SysTimeTicks = 0;

/******************************************************************************
 *�������ƣ�SConf_DelayUS
 *����������΢�뼶��ʱ
 *����˵����us:΢����
 *�� �� ֵ����
 *ע�������ʹ��ucOS
 */
void SConf_DelayUS(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told,tnow,tcnt=0;
    uint16_t fac_us = SystemCoreClock/1000000;
    uint32_t reload = SysTick->LOAD;        //LOAD��ֵ
    ticks = nus*fac_us;                     //��Ҫ�Ľ�����
    tcnt = 0;
    told = SysTick->VAL;                    //�ս���ʱ�ļ�����ֵ
    while(1)
    {
        tnow=SysTick->VAL;  
        if(tnow!=told)
        {       
            if(tnow<told)tcnt+=told-tnow;   //����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
            else tcnt+=reload-tnow+told;
            told=tnow;
            if(tcnt>=ticks)break;           //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
        }  
    }
}
