/**
  ******************************************************************************
  * @File       Hal_LCD_Port
  * @Author     shulai
  * @Date       2018-3-11
  * @Brief      LCD接口驱动

  * @Modified
  ******************************************************************************
  */
  
#ifndef HAL_LCD_PORT_H
#define HAL_LCD_PORT_H
  
#ifdef __cplusplus
extern "C" {
#endif
    
#include "SysConf.h"
    

/* 背景光控制 */
#define BRIGHT_MAM                  255
#define BRIGHT_MIN                  0
#define BRIGHT_DEFAULT              200
#define BRIGHT_STEP                 5

#define CL_DEPTH                    24      /*Color depth: 1/8/16/24*/

typedef union
{
    uint8_t blue  :1;
    uint8_t green :1;
    uint8_t red   :1;
    uint8_t full  :1;
}CL1_t;

typedef union
{
    struct
    {
        uint8_t blue  :2;
        uint8_t green :3;
        uint8_t red   :3;
    };
    uint8_t full;
}CL8_t;

typedef union
{
    struct
    {
        uint16_t blue  :5;
        uint16_t green :6;
        uint16_t red   :5;
    };
    uint16_t full;
}CL16_t;

typedef union
{
    struct
    {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t alpha;
    };
    uint32_t full;
}CL24_t;

#if CL_DEPTH == 1
typedef CL1_t CL_t;
#elif CL_DEPTH == 8
typedef CL8_t CL_t;
#elif CL_DEPTH == 16
typedef CL16_t CL_t;
#elif CL_DEPTH == 24
typedef CL24_t CL_t;
#else
#error "Invalid LV_COLOR_DEPTH in misc_conf.h! Set it to 1, 8, 16 or 24!"
#endif

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#if CL_DEPTH == 1
#define COLOR_MAKE(r8, g8, b8) ((CL_t){(b8 >> 7 | g8 >> 7 | r8 >> 7)})
#elif CL_DEPTH == 8
#define COLOR_MAKE(r8, g8, b8) ((CL_t){{b8 >> 6, g8 >> 5, r8 >> 5}})
#elif CL_DEPTH == 16
#define COLOR_MAKE(r8, g8, b8) ((CL_t){{b8 >> 3, g8 >> 2, r8 >> 3}})
#elif CL_DEPTH == 24
#define COLOR_MAKE(r8, g8, b8) ((CL_t){{b8, g8, r8, 0xff}})            /*Fix 0xff alpha*/
#endif
#else
#if CL_DEPTH == 1
#define COLOR_MAKE(r8, g8, b8) ((CL_t){(r8 >> 7 | g8 >> 7 | b8 >> 7)})
#elif CL_DEPTH == 8
#define COLOR_MAKE(r8, g8, b8) ((CL_t){{r8 >> 6, g8 >> 5, b8 >> 5}})
#elif CL_DEPTH == 16
#define COLOR_MAKE(r8, g8, b8) ((CL_t){{r8 >> 3, g8 >> 2, b8 >> 3}})
#elif CL_DEPTH == 24
#define COLOR_MAKE(r8, g8, b8) ((CL_t){{0xff, r8, g8, b8}})            /*Fix 0xff alpha*/
#endif
#endif

/* 解码出 R=8bit G=8bit B=8bit */
#define RGB565_R(x)                 ((x >> 8) & 0xF8)
#define RGB565_G(x)                 ((x >> 3) & 0xFC)
#define RGB565_B(x)                 ((x << 3) & 0xF8)

/* 解码出 R=5bit G=6bit B=5bit */
#define RGB565_R2(x)                ((x >> 11) & 0x1F)
#define RGB565_G2(x)                ((x >> 5) & 0x3F)
#define RGB565_B2(x)                ((x >> 0) & 0x1F)

#define COLOR_BLACK                 COLOR_MAKE(0x00,0x00,0x00)
#define COLOR_WHITE                 COLOR_MAKE(0xFF,0xFF,0xFF)
#define COLOR_RED                   COLOR_MAKE(0xFF,0x00,0x00)
#define COLOR_LIME                  COLOR_MAKE(0x00,0xFF,0x00)
#define COLOR_BLUE                  COLOR_MAKE(0x00,0x00,0xFF)
#define COLOR_YELLOW                COLOR_MAKE(0xFF,0xFF,0x00)
#define COLOR_CYAN                  COLOR_MAKE(0x00,0xFF,0xFF)
#define COLOR_AQUA                  COLOR_CYAN
#define COLOR_MAGENTA               COLOR_MAKE(0xFF,0x00,0xFF)
#define COLOR_SILVER                COLOR_MAKE(0xC0,0xC0,0xC0)
#define COLOR_GRAY                  COLOR_MAKE(0x80,0x80,0x80)
#define COLOR_MARRON                COLOR_MAKE(0x80,0x00,0x00)
#define COLOR_OLIVE                 COLOR_MAKE(0x80,0x80,0x00)
#define COLOR_GREEN                 COLOR_MAKE(0x00,0x80,0x00)
#define COLOR_PURPLE                COLOR_MAKE(0x80,0x00,0x80)
#define COLOR_TEAL                  COLOR_MAKE(0x00,0x80,0x80)
#define COLOR_NAVY                  COLOR_MAKE(0x00,0x00,0x80)
#define COLOR_ORANGE                COLOR_MAKE(0xFF,0xA5,0x00)

typedef struct
{
    uint16_t usWidth;                       /* LCD 宽度 */
    uint16_t usHeight;                      /* LCD 高度 */
    uint8_t ucDirection;                    /* LCD屏幕方向 */
                                            /* 0 横屏 */
                                            /* 1 横屏180° */
                                            /* 2 竖屏 */
                                            /* 3 竖屏180° */
    void (*vLCD_HardInit)();                /* LCD硬件初始化 */
    void (*vLCD_SetDirection)(uint8_t ucDir);       /* 设置显示方向 */
    uint32_t (*usReadPoint)(uint16_t usX,uint16_t usY);                 /* 读点函数 */
    void (*vDrawPoint)(uint16_t usX,uint16_t usY,uint32_t usColor);     /* 画点函数 */
    void (*vFillRect)(uint16_t usX,uint16_t usY,uint16_t usHeight, uint16_t usWidth,uint32_t usColor);      /* 填充函数 */
}LCD_Dev_t;
extern LCD_Dev_t xLCD_Dev;
    
    
    
    
#ifdef __cplusplus
}
#endif
    
#endif

