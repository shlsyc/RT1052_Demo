/**
  ******************************************************************************
  * @File       Hal_Led
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      Led指示灯接口驱动

  * @Modified
  ******************************************************************************
  */

#ifndef HAL_LED_H
#define HAL_LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "SysConf.h"

#define HAL_NUM_LEDS                            8

//每一位代表一LED
#define HAL_LED_1                               0x01
#define HAL_LED_2                               0x02
#define HAL_LED_3                               0x04
#define HAL_LED_4                               0x08
#define HAL_LED_5                               0x10
#define HAL_LED_6                               0x20
#define HAL_LED_7                               0x40
#define HAL_LED_8                               0x80
#define HAL_LED_ALL                             (HAL_LED_1 | HAL_LED_2 | HAL_LED_3 | HAL_LED_4 | HAL_LED_5 | HAL_LED_6 | HAL_LED_7 | HAL_LED_8)

//模式
#define	HAL_LED_MODE_OFF                        0x00
#define	HAL_LED_MODE_ON                         0x01
#define	HAL_LED_MODE_BLINK                      0x02
#define	HAL_LED_MODE_FLASH                      0x04
#define	HAL_LED_MODE_TOGGLE                     0x08

#define HAL_LED_DEFAULT_MAX_NUM                 8
#define HAL_LED_DEFAULT_DUTY_CYCLE              5
#define HAL_LED_DEFAULT_FLASH_COUNT             50
#define HAL_LED_DEFAULT_FLASH_TIME              1000

#define ACTIVE_LOW                              !
#define ACTIVE_HIGH                             !!

#define LED_POLARITY                            ACTIVE_LOW


#define LED_1_GPIO                              GPIO1
#define LED_1_GPIO_PIN                          (9U)
#define LED_1_IOMUXC                            IOMUXC_GPIO_AD_B0_09_GPIO1_IO09

#define LED_1_ON()                              GPIO_PinWrite(LED_1_GPIO, LED_1_GPIO_PIN, 0U)
#define LED_1_OFF()                             GPIO_PinWrite(LED_1_GPIO, LED_1_GPIO_PIN, 1U)
#define LED_1_STATE()                           (LED_1_GPIO->DR)


#define HAL_LED1_ON()                           LED_1_ON()
//#define HAL_LED2_ON()                           LED_2_ON()
//#define HAL_LED3_ON()                           LED_3_ON()
//#define HAL_LED4_ON()                           LED_4_ON()
//#define HAL_LED5_ON()                           LED_5_ON()
//#define HAL_LED6_ON()                           LED_6_ON()
//#define HAL_LED7_ON()                           LED_7_ON()
//#define HAL_LED8_ON()                           LED_8_ON()

#define HAL_LED1_OFF()                          LED_1_OFF()
//#define HAL_LED2_OFF()                          LED_2_OFF()
//#define HAL_LED3_OFF()                          LED_3_OFF()
//#define HAL_LED4_OFF()                          LED_4_OFF()
//#define HAL_LED5_OFF()                          LED_5_OFF()
//#define HAL_LED6_OFF()                          LED_6_OFF()
//#define HAL_LED7_OFF()                          LED_7_OFF()
//#define HAL_LED8_OFF()                          LED_8_OFF()

#define HAL_LED1_STATE()                        LED_POLARITY(LED_1_STATE())
//#define HAL_LED2_STATE()                        LED_POLARITY(LED_2_STATE())
//#define HAL_LED3_STATE()                        LED_POLARITY(LED_3_STATE())
//#define HAL_LED4_STATE()                        LED_POLARITY(LED_4_STATE())
//#define HAL_LED5_STATE()                        LED_POLARITY(LED_5_STATE())
//#define HAL_LED6_STATE()                        LED_POLARITY(LED_6_STATE())
//#define HAL_LED7_STATE()                        LED_POLARITY(LED_7_STATE())
//#define HAL_LED8_STATE()                        LED_POLARITY(LED_8_STATE())

#define HAL_LED1_TOGGLE()                       st(if(HAL_LED1_STATE) {HAL_LED1_ON()} else {HAL_LED1_OFF()})
//#define HAL_LED2_TOGGLE()                       st(if(HAL_LED2_STATE) {HAL_LED2_ON()} else {HAL_LED2_OFF()})
//#define HAL_LED3_TOGGLE()                       st(if(HAL_LED3_STATE) {HAL_LED3_ON()} else {HAL_LED3_OFF()})
//#define HAL_LED4_TOGGLE()                       st(if(HAL_LED4_STATE) {HAL_LED4_ON()} else {HAL_LED4_OFF()})
//#define HAL_LED5_TOGGLE()                       st(if(HAL_LED5_STATE) {HAL_LED5_ON()} else {HAL_LED5_OFF()})
//#define HAL_LED6_TOGGLE()                       st(if(HAL_LED6_STATE) {HAL_LED6_ON()} else {HAL_LED6_OFF()})
//#define HAL_LED7_TOGGLE()                       st(if(HAL_LED7_STATE) {HAL_LED7_ON()} else {HAL_LED7_OFF()})
//#define HAL_LED8_TOGGLE()                       st(if(HAL_LED8_STATE) {HAL_LED8_ON()} else {HAL_LED8_OFF()})

/******************************************************************************
 *函数名称：HalLedInit
 *功能描述：LED指示灯初始化
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
void HalLedInit(void);

/******************************************************************************
 *函数名称：HalLedSet
 *功能描述：设置指定的LED灯
 *参数说明：leds-LED屏蔽位  mode-LED操作模式
 *返 回 值：无
 *注意事项：无
 */
uint8_t HalLedSet(uint8_t leds, uint8_t mode);

/******************************************************************************
 *函数名称：HalLedBlink
 *功能描述：设置指定的LED灯闪烁
 *参数说明：
 *返 回 值：无
 *注意事项：无
 */
void HalLedBlink(uint8_t leds, uint8_t blinkNums, uint8_t percent, uint16_t period);

/******************************************************************************
 *函数名称：HalLedUpdate
 *功能描述：更新LED工作状态
 *参数说明：
 *返 回 值：无
 *注意事项：必须放在定时任务里
 */
void HalLedUpdate(void);

/******************************************************************************
 *函数名称：HalLedOnOff
 *功能描述：开关指定的LED灯闪烁
 *参数说明：
 *返 回 值：无
 *注意事项：无
 */
void HalLedOnOff(uint8_t leds, uint8_t mode);

/******************************************************************************
 *函数名称：HalLedEnterSleep
 *功能描述：LED进入睡眠模式,保存当前LED状态
 *参数说明：
 *返 回 值：无
 *注意事项：无
 */
void HalLedEnterSleep(void);

/******************************************************************************
 *函数名称：HalLedExitSleep
 *功能描述：LED退出睡眠模式,恢复之前LED状态
 *参数说明：
 *返 回 值：无
 *注意事项：无
 */
void HalLedExitSleep(void);




#ifdef __cplusplus
}
#endif

#endif
