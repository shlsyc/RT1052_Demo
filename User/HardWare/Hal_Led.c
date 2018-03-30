/**
  ******************************************************************************
  * @File       Hal_Led
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      Ledָʾ�ƽӿ�����

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

static uint8_t sLedState;                           //���һ�ε�LED״̬
static uint8_t sLedSleepState;           
static uint8_t sPreBlinkState;                      //����Blinkģʽǰ�ĳ�ʼ״̬

static xLedStatus_t  xLedStatusControl;         

/******************************************************************************
 *�������ƣ�HalLedInit
 *����������LEDָʾ�Ƴ�ʼ��
 *����˵������
 *�� �� ֵ����
 *ע�������
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
    
    HalLedSet(HAL_LED_ALL,HAL_LED_MODE_OFF);        //�ر�����LED��
    xLedStatusControl.sleepActive = FALSE;          //ʧ��˯��ģʽ
}

/******************************************************************************
 *�������ƣ�HalLedSet
 *��������������ָ����LED��
 *����˵����leds-LED����λ  mode-LED����ģʽ
 *�� �� ֵ����
 *ע�������
 */
uint8_t HalLedSet(uint8_t leds, uint8_t mode)
{
    uint8_t led;
    xLedControl_t    *pxLedControl;
    
    switch(mode)
    {
        case HAL_LED_MODE_BLINK:
            HalLedBlink(leds, 1, HAL_LED_DEFAULT_DUTY_CYCLE, HAL_LED_DEFAULT_FLASH_TIME);       //Ĭ�ϵ�LED��˸��ʽ
            break;
        
        case HAL_LED_MODE_FLASH:
            HalLedBlink(leds, HAL_LED_DEFAULT_FLASH_COUNT, HAL_LED_DEFAULT_DUTY_CYCLE, HAL_LED_DEFAULT_FLASH_TIME);     //Ĭ�ϵ�LED���ⷽʽ
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
 *�������ƣ�HalLedBlink
 *��������������ָ����LED����˸
 *����˵����
 *�� �� ֵ����
 *ע�������
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
                    //�ڽ���BLINKģʽǰ���ȱ��浱ǰLED״̬
                    if(pxLedControl->mode < HAL_LED_MODE_BLINK)
                        sPreBlinkState |= (led & sLedState);
                    pxLedControl->mode = HAL_LED_MODE_OFF;        //�ݶ�֮ǰ��BLINK״̬
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
            HalLedSet(leds, HAL_LED_MODE_ON);           // >=100%, ����ȫ��
        }
    }
    else
        HalLedSet(leds, HAL_LED_MODE_OFF);
    
}

/******************************************************************************
 *�������ƣ�HalLedUpdate
 *��������������LED����״̬
 *����˵����
 *�� �� ֵ����
 *ע�����������ڶ�ʱ������
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
                    
                    if(wait >= pxLedControl->time)            //��ֹ��g_SysTimeTicks��������pxLedControl->nextԶԶ����time,                                                           
                    {                                           //�Ӷ�����LED��������
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
 *�������ƣ�HalLedOnOff
 *��������������ָ����LED����˸
 *����˵����
 *�� �� ֵ����
 *ע�������
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
    
    //���浱ǰLed״̬
    if(mode)    
        sLedState |= leds;
    else
        sLedState &= (leds ^ 0xFF);
}

/******************************************************************************
 *�������ƣ�HalLedEnterSleep
 *����������LED����˯��ģʽ,���浱ǰLED״̬
 *����˵����
 *�� �� ֵ����
 *ע�������
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
 *�������ƣ�HalLedExitSleep
 *����������LED�˳�˯��ģʽ,�ָ�֮ǰLED״̬
 *����˵����
 *�� �� ֵ����
 *ע�������
 */
void HalLedExitSleep(void)
{
    HalLedOnOff(sLedSleepState,HAL_LED_MODE_ON);
    
    HalLedUpdate();

    xLedStatusControl.sleepActive = FALSE;
}



