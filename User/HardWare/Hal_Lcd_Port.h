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
    
#define LCD_WIDTH                   800
#define LCD_HEIGHT                  480

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

#if CL_DEPTH == 1
#define RGB(r8, g8, b8) ((lv_color_t){(r8 >> 7 | g8 >> 7 | b8 >> 7)})
#elif CL_DEPTH == 8
#define RGB(r8, g8, b8) ((lv_color_t){{r8 >> 6, g8 >> 5, b8 >> 5}})
#elif CL_DEPTH == 16
#define RGB(r8, g8, b8) ((lv_color_t){{r8 >> 3, g8 >> 2, b8 >> 3}})
#elif CL_DEPTH == 24
#define RGB(r8, g8, b8) ((CL_t){{0xff, r8, g8, b8}})            /*Fix 0xff alpha*/
#endif

/* 解码出 R=8bit G=8bit B=8bit */
#define RGB565_R(x)                 ((x >> 8) & 0xF8)
#define RGB565_G(x)                 ((x >> 3) & 0xFC)
#define RGB565_B(x)                 ((x << 3) & 0xF8)

/* 解码出 R=5bit G=6bit B=5bit */
#define RGB565_R2(x)                ((x >> 11) & 0x1F)
#define RGB565_G2(x)                ((x >> 5) & 0x3F)
#define RGB565_B2(x)                ((x >> 0) & 0x1F)

enum
{
    CL_WHITE            =           RGB(255,255,255),           /* 白色 */
    CL_BLACK            =           RGB(  0,  0,  0),           /* 黑色 */
    CL_RED              =           RGB(255,  0,  0),           /* 红色 */
    CL_GREEN            =           RGB(  0,255,  0),           /* 绿色 */
    CL_BLUE             =           RGB(  0,  0,255),           /* 蓝色 */
    CL_YELLOW           =           RGB(255,255,  0),           /* 黄色 */

    CL_GREY             =           RGB( 98, 98, 98),           /* 深灰色 */
    CL_GREY1            =           RGB( 150, 150, 150),        /* 浅灰色 */
    CL_GREY2            =           RGB( 180, 180, 180),        /* 浅灰色 */
    CL_GREY3            =           RGB( 200, 200, 200),        /* 最浅灰色 */
    CL_GREY4            =           RGB( 230, 230, 230),        /* 最浅灰色 */

    CL_BUTTON_GREY      =           RGB( 220, 220, 220),        /* WINDOWS 按钮表面灰色 */

    CL_BLUE1            =           RGB(  0,  0, 240),          /* 深蓝色 */
    CL_BLUE2            =           RGB(  0,  0, 128),          /* 深蓝色 */
    CL_BLUE3            =           RGB(  68, 68, 255),         /* 浅蓝色1 */
    CL_BLUE4            =           RGB(  0, 64, 128),          /* 浅蓝色1 */

    /* UI 界面 Windows控件常用色 */
    CL_BTN_FACE         =           RGB(236, 233, 216),         /* 按钮表面颜色(灰) */

    CL_BTN_FONT         =           CL_BLACK,                   /* 按钮字体颜色（黑） */

    CL_BOX_BORDER1      =           RGB(172, 168,153),          /* 分组框主线颜色 */
    CL_BOX_BORDER2      =           RGB(255, 255,255),          /* 分组框阴影线颜色 */

    CL_MASK             =           0x9999                      /* 颜色掩码，用于文字背景透明 */
};

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

