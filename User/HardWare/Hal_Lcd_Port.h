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

/*In color conversations:
 * - When converting to bigger color type the LSB weight of 1 LSB is calculated 
 *   E.g. 16 bit Red has 5 bits
 *         8 bit Red has 2 bits
 *        ----------------------
 *        8 bit red LSB = (2^5 - 1) / (2^2 - 1) = 31 / 3 = 10
 * 
 * - When calculating to smaller color type simply shift out the LSBs
 *   E.g.  8 bit Red has 2 bits 
 *        16 bit Red has 5 bits
 *        ----------------------
 *         Shift right with 5 - 3 = 2
 */

static inline uint8_t color_to1(CL_t color)
{
#if CL_DEPTH == 1
    return color.full;
#elif CL_DEPTH == 8
    if((color.red   & 0x4) ||
       (color.green & 0x4) ||
        (color.blue  & 0x2)) {
        return 1;
    } else {
        return 0;
    }
#elif CL_DEPTH == 16
    if((color.red   & 0x10) ||
       (color.green & 0x20) ||
	   (color.blue  & 0x10)) {
    	return 1;
    } else {
    	return 0;
    }
#elif CL_DEPTH == 24
    if((color.red   & 0x80) ||
       (color.green & 0x80) ||
        (color.blue  & 0x80)) {
        return 1;
    } else {
        return 0;
    }
#endif
}

static inline uint8_t color_to8(CL_t color)
{
#if CL_DEPTH == 1
    if(color.full == 0) return 0;
    else return 0xFF;
#elif CL_DEPTH == 8
    return color.full;
#elif CL_DEPTH == 16
    CL8_t ret;
    ret.red = color.red >> 2;       /* 5 - 3  = 2*/
    ret.green = color.green >> 3;   /* 6 - 3  = 3*/
    ret.blue = color.blue >> 3;     /* 5 - 2  = 3*/
    return ret.full;
#elif CL_DEPTH == 24
    CL8_t ret;
    ret.red = color.red >> 5;       /* 8 - 3  = 5*/
    ret.green = color.green >> 5;   /* 8 - 3  = 5*/
    ret.blue = color.blue >> 6;     /* 8 - 2  = 6*/
    return ret.full;
#endif
}

static inline uint16_t color_to16(CL_t color)
{
#if CL_DEPTH == 1
    if(color.full == 0) return 0;
    else return 0xFFFF;
#elif CL_DEPTH == 8
    CL16_t ret;
    ret.red = color.red * 4;       /*(2^5 - 1)/(2^3 - 1) = 31/7 = 4*/
    ret.green = color.green * 9;   /*(2^6 - 1)/(2^3 - 1) = 63/7 = 9*/
    ret.blue = color.blue * 10;    /*(2^5 - 1)/(2^2 - 1) = 31/3 = 10*/
    return ret.full;
#elif CL_DEPTH == 16
    return color.full;
#elif CL_DEPTH == 24
    CL16_t ret;
    ret.red = color.red >> 3;       /* 8 - 5  = 3*/
    ret.green = color.green >> 2;   /* 8 - 6  = 2*/
    ret.blue = color.blue >> 3;     /* 8 - 5  = 3*/
    return ret.full;
#endif
}

static inline uint32_t color_to24(CL_t color)
{
#if CL_DEPTH == 1
    if(color.full == 0) return 0;
    else return 0xFFFFFFFF;
#elif CL_DEPTH == 8
    CL24_t ret;
    ret.red = color.red * 36;        /*(2^8 - 1)/(2^3 - 1) = 255/7 = 36*/
    ret.green = color.green * 36;    /*(2^8 - 1)/(2^3 - 1) = 255/7 = 36*/
    ret.blue = color.blue * 85;      /*(2^8 - 1)/(2^2 - 1) = 255/3 = 85*/
    ret.alpha = 0xFF;
    return ret.full;
#elif CL_DEPTH == 16
    CL24_t ret;
    ret.red = color.red * 8;       /*(2^8 - 1)/(2^5 - 1) = 255/31 = 8*/
    ret.green = color.green * 4;   /*(2^8 - 1)/(2^6 - 1) = 255/63 = 4*/
    ret.blue = color.blue * 8;     /*(2^8 - 1)/(2^5 - 1) = 255/31 = 8*/
    ret.alpha = 0xFF;
    return ret.full;
#elif CL_DEPTH == 24
    return color.full;
#endif
}
    
    
#ifdef __cplusplus
}
#endif
    
#endif

