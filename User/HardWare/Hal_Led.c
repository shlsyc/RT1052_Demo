/**
  ******************************************************************************
  * @File       Hal_Led
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      Led指示灯接口驱动

  * @Modified
  ******************************************************************************
  */
  
#include "Hal_Led.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  

typedef struct
{
    uint8_t mode;        //Operation mode
    uint8_t todo;        //Blink cycles left
    uint8_t onPct;       //On cycle percentage
    uint16_t time;       //On/off cycle time (msec)
    uint32_t next;       //Time for next change
}xLedControl_t;

typedef struct
{
    xLedControl_t        LedControlTable[HAL_LED_DEFAULT_MAX_NUM];
    uint8_t      sleepActive;
}xLedStatus_t;

static uint8_t sLedState;                           //最近一次的LED状态
static uint8_t sLedSleepState;           
static uint8_t sPreBlinkState;                      //进入Blink模式前的初始状态

static xLedStatus_t  xLedStatusControl;         

/******************************************************************************
 *函数名称：HalLedInit
 *功能描述：LED指示灯初始化
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
void HalLedInit(void)
{
    gpio_pin_config_t led_config = 
    {
        .direction = kGPIO_DigitalOutput,
        .outputLogic = 0,
        .interruptMode = kGPIO_NoIntmode
    };
        
    IOMUXC_SetPinMux(LED_1_IOMUXC,0);
    IOMUXC_SetPinConfig(LED_1_IOMUXC,0x10B0u);
    /* Slew Rate Field: Slow Slew Rate
       Drive Strength Field: R0/6
       Speed Field: medium(100MHz)
       Open Drain Enable Field: Open Drain Disabled
       Pull / Keep Enable Field: Pull/Keeper Enabled
       Pull / Keep Select Field: Keeper
       Pull Up / Down Config. Field: 100K Ohm Pull Down
       Hyst. Enable Field: Hysteresis Disabled  */
    
    GPIO_PinInit(LED_1_GPIO, LED_1_GPIO_PIN, &led_config);
    
    HalLedSet(HAL_LED_ALL,HAL_LED_MODE_OFF);        //关闭所有LED灯
    xLedStatusControl.sleepActive = FALSE;          //失能睡眠模式
}

/******************************************************************************
 *函数名称：HalLedSet
 *功能描述：设置指定的LED灯
 *参数说明：leds-LED屏蔽位  mode-LED操作模式
 *返 回 值：无
 *注意事项：无
 */
uint8_t HalLedSet(uint8_t leds, uint8_t mode)
{
    uint8_t led;
    xLedControl_t    *pxLedControl;
    
    switch(mode)
    {
        case HAL_LED_MODE_BLINK:
            HalLedBlink(leds, 1, HAL_LED_DEFAULT_DUTY_CYCLE, HAL_LED_DEFAULT_FLASH_TIME);       //默认的LED闪烁方式
            break;
        
        case HAL_LED_MODE_FLASH:
            HalLedBlink(leds, HAL_LED_DEFAULT_FLASH_COUNT, HAL_LED_DEFAULT_DUTY_CYCLE, HAL_LED_DEFAULT_FLASH_TIME);     //默认的LED闪光方式
            break;
        
        case HAL_LED_MODE_OFF:
        case HAL_LED_MODE_ON:
        case HAL_LED_MODE_TOGGLE:
            
            led = HAL_LED_1;
            leds &= HAL_LED_ALL;
            pxLedControl = xLedStatusControl.LedControlTable;
            
            while(leds)
            {
                if(leds & led)
                {
                    if(mode != HAL_LED_MODE_TOGGLE)
                    {
                        pxLedControl->mode = mode;
                    }
                    else
                    {
                        pxLedControl->mode ^= HAL_LED_MODE_ON;
                    }
                    HalLedOnOff(led,pxLedControl->mode);
                    leds ^= led;
                }
                led <<= 1;
                pxLedControl++;
            }
            break;
            
        default:
            break;
        
    }
    return sLedState;
}

/******************************************************************************
 *函数名称：HalLedBlink
 *功能描述：设置指定的LED灯闪烁
 *参数说明：
 *返 回 值：无
 *注意事项：无
 */
void HalLedBlink(uint8_t leds, uint8_t blinkNums, uint8_t percent, uint16_t period)
{
    uint8_t led;
    xLedControl_t *pxLedControl;
    
    if(leds && percent && period)
    {
        if(percent < 100)
        {
            led = HAL_LED_1;
            leds &= HAL_LED_ALL;
            pxLedControl = xLedStatusControl.LedControlTable;
            
            while(leds)
            {
                if(leds & led)
                {
                    //在进入BLINK模式前，先保存当前LED状态
                    if(pxLedControl->mode < HAL_LED_MODE_BLINK)
                        sPreBlinkState |= (led & sLedState);
                    pxLedControl->mode = HAL_LED_MODE_OFF;        //暂定之前的BLINK状态
                    pxLedControl->time = period;
                    pxLedControl->onPct = percent;
                    pxLedControl->todo = blinkNums;
                    if(!blinkNums)
                        pxLedControl->mode |=  HAL_LED_MODE_FLASH;
                    pxLedControl->next = g_SysTimeTicks;
                    pxLedControl->mode |= HAL_LED_MODE_BLINK;
                    leds ^= led;
                }
                led <<= 1;
                pxLedControl++;
            }
        }
        else
        {
            HalLedSet(leds, HAL_LED_MODE_ON);           // >=100%, 设置全亮
        }
    }
    else
        HalLedSet(leds, HAL_LED_MODE_OFF);
    
}

/******************************************************************************
 *函数名称：HalLedUpdate
 *功能描述：更新LED工作状态
 *参数说明：
 *返 回 值：无
 *注意事项：必须放在定时任务里
 */
void HalLedUpdate(void)
{
    uint8_t led;
    uint8_t leds;
    uint8_t pct;
    uint32_t wait;
    uint32_t time;
    xLedControl_t *pxLedControl;
    
    
    led = HAL_LED_1;
    leds = HAL_LED_ALL;
    pxLedControl = xLedStatusControl.LedControlTable;
    
    if(!xLedStatusControl.sleepActive)
    {
        while(leds)
        {
            if(leds & led)
            {
                if(pxLedControl->mode & HAL_LED_MODE_BLINK)
                {
                    time = g_SysTimeTicks;
                    if(time >= pxLedControl->next)
                    {
                        if(pxLedControl->mode & HAL_LED_MODE_ON)
                        {
                            pct = 100 - pxLedControl->onPct;
                            pxLedControl->mode &= ~HAL_LED_MODE_ON;
                            HalLedOnOff(led, HAL_LED_MODE_OFF);
                            
                            if(!(pxLedControl->mode & HAL_LED_MODE_FLASH))
                                pxLedControl->todo--;
                        }
                        else if((!pxLedControl->todo) && (!(pxLedControl->mode & HAL_LED_MODE_FLASH)))
                        {
                            pxLedControl->mode ^= HAL_LED_MODE_BLINK;
                        }
                        else
                        {
                            pct = pxLedControl->onPct;
                            pxLedControl->mode |= HAL_LED_MODE_ON;
                            HalLedOnOff(led, HAL_LED_MODE_ON);
                        }
                        if(pxLedControl->mode & HAL_LED_MODE_BLINK)
                        {
                            wait = (((uint32_t)pct * (uint32_t)pxLedControl->time)/100);
                            pxLedControl->next = time + wait;
                        }
                        else
                        {
                            wait = 0;
                            HalLedSet(led,((sPreBlinkState & led)!=0)?HAL_LED_MODE_ON:HAL_LED_MODE_OFF);
                            sPreBlinkState &= (led ^ 0xFF);
                        }
                    }
                    else
                    {
                        wait = pxLedControl->next - time;
                    }
                    
                    if(wait >= pxLedControl->time)            //防止因g_SysTimeTicks溢出，造成pxLedControl->next远远大于time,                                                           
                    {                                           //从而导致LED常亮或常灭
                        pxLedControl->next = g_SysTimeTicks;
                    }
                }
                leds ^= led;
            }
            led <<= 1;
            pxLedControl++;
        }
    }
}

/******************************************************************************
 *函数名称：HalLedOnOff
 *功能描述：开关指定的LED灯闪烁
 *参数说明：
 *返 回 值：无
 *注意事项：无
 */
void HalLedOnOff(uint8_t leds, uint8_t mode)
{
    if(leds & HAL_LED_1)
    {
        if(mode == HAL_LED_MODE_ON)
            HAL_LED1_ON();
        else
            HAL_LED1_OFF();
    }
    
//    if(leds & HAL_LED_2)
//    {
//        if(mode == HAL_LED_MODE_ON)
//            HAL_LED2_ON();
//        else
//            HAL_LED2_OFF();
//    }
//  
//    if(leds & HAL_LED_3)
//    {
//        if(mode == HAL_LED_MODE_ON)
//            HAL_LED3_ON();
//        else
//            HAL_LED3_OFF();
//    }
//  
//    if(leds & HAL_LED_4)
//    {
//        if(mode == HAL_LED_MODE_ON)
//            HAL_LED4_ON();
//        else
//            HAL_LED4_OFF();
//    }
//  
//    if(leds & HAL_LED_5)
//    {
//        if(mode == HAL_LED_MODE_ON)
//            HAL_LED5_ON();
//        else
//            HAL_LED5_OFF();
//    }
//  
//    if(leds & HAL_LED_6)
//    {
//        if(mode == HAL_LED_MODE_ON)
//            HAL_LED6_ON();
//        else
//            HAL_LED6_OFF();
//    }
//  
//    if(leds & HAL_LED_7)
//    {
//        if(mode == HAL_LED_MODE_ON)
//            HAL_LED7_ON();
//        else
//            HAL_LED7_OFF();
//    }
//  
//    if(leds & HAL_LED_8)
//    {
//        if(mode == HAL_LED_MODE_ON)
//            HAL_LED8_ON();
//        else
//            HAL_LED8_OFF();
//    }
    
    //保存当前Led状态
    if(mode)    
        sLedState |= leds;
    else
        sLedState &= (leds ^ 0xFF);
}

/******************************************************************************
 *函数名称：HalLedEnterSleep
 *功能描述：LED进入睡眠模式,保存当前LED状态
 *参数说明：
 *返 回 值：无
 *注意事项：无
 */
void HalLedEnterSleep(void)
{
    xLedStatusControl.sleepActive = TRUE;
    sLedSleepState = 0;
    
    sLedSleepState |= HAL_LED1_STATE();
//    sLedSleepState |= HAL_LED2_STATE() << 1;
//    sLedSleepState |= HAL_LED3_STATE() << 2;
//    sLedSleepState |= HAL_LED4_STATE() << 3;
//    sLedSleepState |= HAL_LED5_STATE() << 4;
//    sLedSleepState |= HAL_LED6_STATE() << 5;
//    sLedSleepState |= HAL_LED7_STATE() << 6;
//    sLedSleepState |= HAL_LED8_STATE() << 7;
    
    HalLedOnOff(HAL_LED_ALL,HAL_LED_MODE_OFF);
}

/******************************************************************************
 *函数名称：HalLedExitSleep
 *功能描述：LED退出睡眠模式,恢复之前LED状态
 *参数说明：
 *返 回 值：无
 *注意事项：无
 */
void HalLedExitSleep(void)
{
    HalLedOnOff(sLedSleepState,HAL_LED_MODE_ON);
    
    HalLedUpdate();

    xLedStatusControl.sleepActive = FALSE;
}



