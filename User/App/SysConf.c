/**
  ******************************************************************************
  * @File       SysConf
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      系统配置接口驱动文件

  * @Modified
  ******************************************************************************
  */

#include "SysConf.h"

volatile uint32_t g_SysTimeTicks = 0;

/******************************************************************************
 *函数名称：SConf_DelayUS
 *功能描述：微秒级延时
 *参数说明：us:微秒数
 *返 回 值：无
 *注意事项：不使用ucOS
 */
void SConf_DelayUS(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told,tnow,tcnt=0;
    uint16_t fac_us = SystemCoreClock/1000000;
    uint32_t reload = SysTick->LOAD;        //LOAD的值
    ticks = nus*fac_us;                     //需要的节拍数
    tcnt = 0;
    told = SysTick->VAL;                    //刚进入时的计数器值
    while(1)
    {
        tnow=SysTick->VAL;  
        if(tnow!=told)
        {       
            if(tnow<told)tcnt+=told-tnow;   //这里注意一下SYSTICK是一个递减的计数器就可以了.
            else tcnt+=reload-tnow+told;
            told=tnow;
            if(tcnt>=ticks)break;           //时间超过/等于要延迟的时间,则退出.
        }  
    }
}
