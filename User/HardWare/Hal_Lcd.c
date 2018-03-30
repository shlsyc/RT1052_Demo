/**
  ******************************************************************************
  * @File       Hal_Lcd
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      LCD接口驱动

  * @Modified
  ******************************************************************************
  */
  
#include "Hal_Lcd.h"
#include "Hal_Lcd_Port.h"

#include "fsl_iomuxc.h"
#include "fsl_gpio.h" 
#include "fsl_pxp.h"
#include "fsl_elcdif.h"
#include "fsl_pxp.h"

#define IMG_HEIGHT 480
#define IMG_WIDTH 800
#define IMG_HSW 41
#define IMG_HFP 4
#define IMG_HBP 8
#define IMG_VSW 10
#define IMG_VFP 4
#define IMG_VBP 2
#define IMG_POL_FLAGS \
    (kELCDIF_DataEnableActiveHigh | kELCDIF_VsyncActiveLow | kELCDIF_HsyncActiveLow | kELCDIF_DriveDataOnRisingClkEdge)

/*
 * Frame buffer data alignment.
 * The PXP input buffer, output buffer, and LCDIF frame buffer address 64B align.
 */
#define FRAME_BUFFER_ALIGN 64

#define PS_WIDTH        (IMG_WIDTH / 2U)
#define PS_HEIGHT       (IMG_HEIGHT / 2U)

#define PS_ULC_X 0U
#define PS_ULC_Y 0U

#define PS_BPP  4U   /* Use 24-bit RGB888 format. */
    
AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_psBufferLcd[2][IMG_HEIGHT][IMG_WIDTH], FRAME_BUFFER_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_psBufferPxp[PS_HEIGHT][PS_WIDTH], FRAME_BUFFER_ALIGN);

pxp_output_buffer_config_t outputBufferConfig;

uint32_t ulCurrentFrameBuffer;
uint8_t ucCurrentLayer;

/******************************************************************************
 *函数名称：Hal_Lcd_Init
 *功能描述：LCD 初始化
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
void Hal_Lcd_Init()
{
    Hal_Lcd_PinInit();
    Hal_Lcd_PixelClockInit();
    Hal_Lcd_InitRGB();
    Hal_Lcd_InitPXP();
    
    Hal_Lcd_SetLayer(LCD_LAYER_1);
}

/******************************************************************************
 *函数名称：Hal_Lcd_PinInit
 *功能描述：LCD 引脚初始化
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
void Hal_Lcd_PinInit()
{
    IOMUXC_SetPinMux(LCD_RST_IOMUXC,0);                             /* LCD RST */
    IOMUXC_SetPinMux(LCD_BL_IOMUXC,0);                              /* LCD BL */
    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_00_LCD_CLK,0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_01_LCD_ENABLE,0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_02_LCD_HSYNC,0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_03_LCD_VSYNC,0);

    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_04_LCD_DATA00,0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_05_LCD_DATA01,0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_06_LCD_DATA02,0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_07_LCD_DATA03,0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_08_LCD_DATA04,0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_09_LCD_DATA05,0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_10_LCD_DATA06,0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_11_LCD_DATA07,0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_12_LCD_DATA08,0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_13_LCD_DATA09,0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_14_LCD_DATA10,0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_15_LCD_DATA11,0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_00_LCD_DATA12,0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_01_LCD_DATA13,0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_02_LCD_DATA14,0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_03_LCD_DATA15,0);
    
    IOMUXC_SetPinConfig(LCD_RST_IOMUXC,0x10B0u);                    /* LCD RST */
    IOMUXC_SetPinConfig(LCD_BL_IOMUXC,0x10B0u);                     /* LCD BL */
    /* 0x10B0u
       Slew Rate Field: Slow Slew Rate
       Drive Strength Field: R0/6
       Speed Field: medium(100MHz)
       Open Drain Enable Field: Open Drain Disabled
       Pull / Keep Enable Field: Pull/Keeper Enabled
       Pull / Keep Select Field: Keeper
       Pull Up / Down Config. Field: 100K Ohm Pull Down
       Hyst. Enable Field: Hysteresis Disabled  */
    
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_00_LCD_CLK,0x01B0B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_01_LCD_ENABLE,0x01B0B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_02_LCD_HSYNC,0x01B0B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_03_LCD_VSYNC,0x01B0B0u);
    
    /* 0x01B0B0u
       Slew Rate Field: Slow Slew Rate
       Drive Strength Field: R0/6
       Speed Field: medium(100MHz)
       Open Drain Enable Field: Open Drain Disabled
       Pull / Keep Enable Field: Pull/Keeper Enabled
       Pull / Keep Select Field: Pull
       Pull Up / Down Config. Field: 100K Ohm Pull Up
       Hyst. Enable Field: Hysteresis Enabled */

    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_04_LCD_DATA00,0x01B0B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_05_LCD_DATA01,0x01B0B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_06_LCD_DATA02,0x01B0B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_07_LCD_DATA03,0x01B0B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_08_LCD_DATA04,0x01B0B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_09_LCD_DATA05,0x01B0B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_10_LCD_DATA06,0x01B0B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_11_LCD_DATA07,0x01B0B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_12_LCD_DATA08,0x01B0B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_13_LCD_DATA09,0x01B0B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_14_LCD_DATA10,0x01B0B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_15_LCD_DATA11,0x01B0B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_00_LCD_DATA12,0x01B0B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_01_LCD_DATA13,0x01B0B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_02_LCD_DATA14,0x01B0B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_03_LCD_DATA15,0x01B0B0u);
    
    gpio_pin_config_t config = 
    {
        .direction = kGPIO_DigitalOutput,
        .outputLogic = 0,
        .interruptMode = kGPIO_NoIntmode
    };
    /* 复位 */
    GPIO_PinInit(LCD_RST_GPIO, LCD_RST_GPIO_PIN, &config);
    GPIO_WritePinOutput(LCD_RST_GPIO, LCD_RST_GPIO_PIN, 0);
    SConf_DelayUS(5000);
    GPIO_WritePinOutput(LCD_RST_GPIO, LCD_RST_GPIO_PIN, 1);
    /* 开启背光 */
    config.outputLogic = 1;
    GPIO_PinInit(LCD_BL_GPIO, LCD_BL_GPIO_PIN, &config);
}

/******************************************************************************
 *函数名称：Hal_Lcd_PixelClockInit
 *功能描述：LCD 时钟初始化
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
void Hal_Lcd_PixelClockInit()
{
    /*
     * The desired output frame rate is 60Hz. So the pixel clock frequency is:
     * (480 + 41 + 4 + 18) * (272 + 10 + 4 + 2) * 60 = 9.2M.
     * Here set the LCDIF pixel clock to 9.3M.
     */

    /*
     * Initialize the Video PLL.
     * Video PLL output clock is OSC24M * (loopDivider + (denominator / numerator)) / postDivider = 93MHz.
     */
    clock_video_pll_config_t config = {
        .loopDivider = 31, .postDivider = 8, .numerator = 0, .denominator = 0,
    };

    CLOCK_InitVideoPll(&config);

    /*
     * 000 derive clock from PLL2
     * 001 derive clock from PLL3 PFD3
     * 010 derive clock from PLL5
     * 011 derive clock from PLL2 PFD0
     * 100 derive clock from PLL2 PFD1
     * 101 derive clock from PLL3 PFD1
     */
    CLOCK_SetMux(kCLOCK_Lcdif1PreMux, 2);

    CLOCK_SetDiv(kCLOCK_Lcdif1PreDiv, 4);

    CLOCK_SetDiv(kCLOCK_Lcdif1Div, 1);

    /*
     * 000 derive clock from divided pre-muxed lcdif1 clock
     * 001 derive clock from ipp_di0_clk
     * 010 derive clock from ipp_di1_clk
     * 011 derive clock from ldb_di0_clk
     * 100 derive clock from ldb_di1_clk
     */
    CLOCK_SetMux(kCLOCK_Lcdif1Mux, 0);
}

/******************************************************************************
 *函数名称：Hal_Lcd_InitRGB
 *功能描述：LCD RGB初始化
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
void Hal_Lcd_InitRGB()
{
    const elcdif_rgb_mode_config_t config = {
        .panelWidth = IMG_WIDTH,
        .panelHeight = IMG_HEIGHT,
        .hsw = IMG_HSW,
        .hfp = IMG_HFP,
        .hbp = IMG_HBP,
        .vsw = IMG_VSW,
        .vfp = IMG_VFP,
        .vbp = IMG_VBP,
        .polarityFlags = IMG_POL_FLAGS,
        .bufferAddr = (uint32_t)s_psBufferLcd[0],
        .pixelFormat = kELCDIF_PixelFormatXRGB8888,
        .dataBus = kELCDIF_DataBus16Bit,
    };

    ELCDIF_RgbModeInit(LCDIF, &config);

    ELCDIF_RgbModeStart(LCDIF);    
}

/******************************************************************************
 *函数名称：Hal_Lcd_InitPXP
 *功能描述：LCD PXP初始化
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
void Hal_Lcd_InitPXP()
{
    PXP_Init(PXP);

    /* PS configure. */
    const pxp_ps_buffer_config_t psBufferConfig = {
        .pixelFormat = kPXP_PsPixelFormatRGB888,
        .swapByte = false,
        .bufferAddr = (uint32_t)s_psBufferPxp,
        .bufferAddrU = 0U,
        .bufferAddrV = 0U,
        .pitchBytes = PS_WIDTH * PS_BPP,
    };

    PXP_SetProcessSurfaceBackGroundColor(PXP, 0U);

    PXP_SetProcessSurfaceBufferConfig(PXP, &psBufferConfig);

    /* Disable AS. */
    PXP_SetAlphaSurfacePosition(PXP, 0xFFFFU, 0xFFFFU, 0U, 0U);

    /* Output config. */
    outputBufferConfig.pixelFormat = kPXP_OutputPixelFormatRGB888;
    outputBufferConfig.interlacedMode = kPXP_OutputProgressive;
    outputBufferConfig.buffer0Addr = (uint32_t)s_psBufferLcd[0];
    outputBufferConfig.buffer1Addr = 0U;
    outputBufferConfig.pitchBytes = IMG_WIDTH * PS_BPP;
    outputBufferConfig.width = IMG_WIDTH;
    outputBufferConfig.height = IMG_HEIGHT;

    PXP_SetOutputBufferConfig(PXP, &outputBufferConfig);

    /* Disable CSC1, it is enabled by default. */
    PXP_EnableCsc1(PXP, false);
}

/******************************************************************************
 *函数名称：Hal_Lcd_InitBuffer
 *功能描述：LCD 初始化缓存
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
void Hal_Lcd_InitBuffer(void)
{
    uint32_t i, j;

    /*
     * The PS input buffer is:
     *
     *  -----------------------------
     *  |             |             |
     *  |             |             |
     *  |   BLUE      |   GREEN     |
     *  |             |             |
     *  |             |             |
     *  |-------------+-------------|
     *  |             |             |
     *  |             |             |
     *  |   WHITE     |    RED      |
     *  |             |             |
     *  |             |             |
     *  -----------------------------
     */
//    for (i = 0; i < (PS_HEIGHT / 2); i++)
//    {
//        for (j = 0; j < (PS_WIDTH / 2); j++)
//        {
//            s_psBufferPxp[i][j] = 0xFFU;
//        }

//        for (; j < PS_WIDTH; j++)
//        {
//            s_psBufferPxp[i][j] = 0xFF00U;
//        }
//    }

//    for (; i < PS_HEIGHT; i++)
//    {
//        for (j = 0; j < (PS_WIDTH / 2); j++)
//        {
//            s_psBufferPxp[i][j] = 0xFFFFFFU;
//        }

//        for (; j < PS_WIDTH; j++)
//        {
//            s_psBufferPxp[i][j] = 0xFF0000U;
//        }
//    }

//    for (i = 0; i < (PS_HEIGHT / 2); i++)
//    {
//        for (j = 0; j < (PS_WIDTH / 2); j++)
//        {
//            s_psBufferLcd[0][i][j] = 0xFFU;
//        }

//        for (; j < PS_WIDTH; j++)
//        {
//            s_psBufferLcd[0][i][j] = 0xFF00U;
//        }
//    }

//    for (; i < PS_HEIGHT; i++)
//    {
//        for (j = 0; j < (PS_WIDTH / 2); j++)
//        {
//            s_psBufferLcd[0][i][j] = 0xFFFFFFU;
//        }

//        for (; j < PS_WIDTH; j++)
//        {
//            s_psBufferLcd[0][i][j] = 0xFF0000U;
//        }
//    }

    memset(s_psBufferLcd, 0xFFFF, sizeof(s_psBufferLcd));
}

/******************************************************************************
 *函数名称：Hal_Lcd_SetLayer
 *功能描述：切换层。只是更改程序变量，以便于后面的代码更改相关寄存器。硬件支持2层。
 *参数说明：无
 *返 回 值：无
 *注意事项：
 */
void Hal_Lcd_SetLayer(uint8_t _ucLayer)
{
    if (_ucLayer == LCD_LAYER_1)
    {
        ulCurrentFrameBuffer = (uint32_t)s_psBufferLcd[0];
        ucCurrentLayer = LCD_LAYER_1;
    }
    else if (_ucLayer == LCD_LAYER_2)
    {
        ulCurrentFrameBuffer = (uint32_t)s_psBufferLcd[1];
        ucCurrentLayer = LCD_LAYER_2;
    }
}

/******************************************************************************
 *函数名称：Hal_Lcd_SetDirection
 *功能描述：设置显示屏显示方向（横屏 竖屏）
 *参数说明：显示方向代码 0 横屏正常, 1=横屏180度翻转, 2=竖屏, 3=竖屏180度翻转
 *返 回 值：无
 *注意事项：
 */
void Hal_Lcd_SetDirection(uint8_t _dir)
{
    uint16_t temp;

    if (_dir == 0 || _dir == 1)         /* 横屏， 横屏180度 */
    {
        if (xLCD_Dev.usWidth < xLCD_Dev.usHeight)
        {
            temp = xLCD_Dev.usWidth;
            xLCD_Dev.usWidth = xLCD_Dev.usHeight;
            xLCD_Dev.usHeight = temp;
        }
    }
    else if (_dir == 2 || _dir == 3)    /* 竖屏, 竖屏180°*/
    {
        if (xLCD_Dev.usWidth > xLCD_Dev.usHeight)
        {
            temp = xLCD_Dev.usWidth;
            xLCD_Dev.usWidth = xLCD_Dev.usHeight;
            xLCD_Dev.usHeight = temp;
        }
    }
}

/******************************************************************************
 *函数名称：Hal_Lcd_GetPixel
 *功能描述：读1个像素
 *参数说明：无
 *返 回 值：无
 *注意事项：
 */
uint32_t Hal_Lcd_GetPixel(uint16_t _usX, uint16_t _usY)
{
    uint32_t ulRGB;
    uint32_t *p;
    uint32_t index = 0;

    p = (uint32_t *)ulCurrentFrameBuffer;

    if (xLCD_Dev.ucDirection == 0)      /* 横屏 */
    {
        index = (uint32_t)_usY * xLCD_Dev.usWidth + _usX;
    }
    else if (xLCD_Dev.ucDirection == 1) /* 横屏180°*/
    {
        index = (uint32_t)(xLCD_Dev.usHeight - _usY - 1) * xLCD_Dev.usWidth + (xLCD_Dev.usWidth - _usX - 1);
    }
    else if (xLCD_Dev.ucDirection == 2) /* 竖屏 */
    {
        index = (uint32_t)(xLCD_Dev.usWidth - _usX - 1) * xLCD_Dev.usHeight + _usY;
    }
    else if (xLCD_Dev.ucDirection == 3) /* 竖屏180° */
    {
        index = (uint32_t)_usX * xLCD_Dev.usHeight + xLCD_Dev.usHeight - _usY - 1;
    }

    ulRGB = p[index];

    return ulRGB;
}

/******************************************************************************
 *函数名称：Hal_Lcd_PutPixel
 *功能描述：画1个像素
 *参数说明：无
 *返 回 值：无
 *注意事项：
 */
void Hal_Lcd_PutPixel(uint16_t _usX, uint16_t _usY, uint32_t _usColor)
{
    uint32_t *p;
    uint32_t index = 0;

    p = (uint32_t *)ulCurrentFrameBuffer;

    if (xLCD_Dev.ucDirection == 0)      /* 横屏 */
    {
        index = (uint32_t)_usY * xLCD_Dev.usWidth + _usX;
    }
    else if (xLCD_Dev.ucDirection == 1) /* 横屏180°*/
    {
        index = (uint32_t)(xLCD_Dev.usHeight - _usY - 1) * xLCD_Dev.usWidth + (xLCD_Dev.usWidth - _usX - 1);
    }
    else if (xLCD_Dev.ucDirection == 2) /* 竖屏 */
    {
        index = (uint32_t)(xLCD_Dev.usWidth - _usX - 1) * xLCD_Dev.usHeight + _usY;
    }
    else if (xLCD_Dev.ucDirection == 3) /* 竖屏180° */
    {
        index = (uint32_t)_usX * xLCD_Dev.usHeight + xLCD_Dev.usHeight - _usY - 1;
    }

    if (index < xLCD_Dev.usHeight * xLCD_Dev.usWidth)
    {
        p[index] = _usColor;
    }
}

/******************************************************************************
 *函数名称：Hal_Lcd_FillRect
 *功能描述：用一个颜色值填充一个矩形
 *参数说明：无
 *返 回 值：无
 *注意事项：
 */
void Hal_Lcd_FillRect(uint16_t usX, uint16_t usY, uint16_t usHeight, uint16_t usWidth, uint32_t usColor)
{
    
}

/******************************************************************************
 *函数名称：Hal_Lcd_DrawCircle
 *功能描述：绘制一个圆，笔宽为1个像素
 *参数说明：圆心的坐标-圆的半径
 *返 回 值：无
 *注意事项：
 */
void Hal_Lcd_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint32_t _usColor)
{
    int32_t  D;         /* Decision Variable */
    uint32_t  CurX;     /* 当前 X 值 */
    uint32_t  CurY;     /* 当前 Y 值 */

    D = 3 - (_usRadius << 1);
    CurX = 0;
    CurY = _usRadius;

    while (CurX <= CurY)
    {
        Hal_Lcd_PutPixel(_usX + CurX, _usY + CurY, _usColor);
        Hal_Lcd_PutPixel(_usX + CurX, _usY - CurY, _usColor);
        Hal_Lcd_PutPixel(_usX - CurX, _usY + CurY, _usColor);
        Hal_Lcd_PutPixel(_usX - CurX, _usY - CurY, _usColor);
        Hal_Lcd_PutPixel(_usX + CurY, _usY + CurX, _usColor);
        Hal_Lcd_PutPixel(_usX + CurY, _usY - CurX, _usColor);
        Hal_Lcd_PutPixel(_usX - CurY, _usY + CurX, _usColor);
        Hal_Lcd_PutPixel(_usX - CurY, _usY - CurX, _usColor);

        if (D < 0)
        {
            D += (CurX << 2) + 6;
        }
        else
        {
            D += ((CurX - CurY) << 2) + 10;
            CurY--;
        }
        CurX++;
    }
}

/******************************************************************************
 *函数名称：Hal_Lcd_ScaleTest
 *功能描述：LCD 缩放测试
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
static void Hal_Lcd_ScaleTest(void)
{
    uint8_t curLcdBufferIdx = 1U;

    float downScaleStart = 0.5f;
    float downScaleEnd = 4.0f;
    float downScaleStep = 1.01f;
    float downScaleFact;
    uint16_t outputWidth, outputHeight;

    for (;;)
    {
        for (downScaleFact = downScaleStart; downScaleFact <= downScaleEnd; downScaleFact *= downScaleStep)
        {
            outputWidth = (uint16_t)((float)PS_WIDTH / downScaleFact);
            outputHeight = (uint16_t)((float)PS_HEIGHT / downScaleFact);
            
            /* Prepare next buffer for LCD. */
            PXP_SetProcessSurfaceScaler(PXP, PS_WIDTH, PS_HEIGHT, outputWidth, outputHeight);
            PXP_SetProcessSurfacePosition(PXP, PS_ULC_X, PS_ULC_Y, PS_ULC_X + outputWidth - 1U,
                                          PS_ULC_Y + outputHeight - 1U);

            outputBufferConfig.buffer0Addr = (uint32_t)s_psBufferLcd[curLcdBufferIdx];

            PXP_SetOutputBufferConfig(PXP, &outputBufferConfig);

            PXP_Start(PXP);

            /* Wait for process complete. */
            while (!(kPXP_CompleteFlag & PXP_GetStatusFlags(PXP)))
            {
            }

            PXP_ClearStatusFlags(PXP, kPXP_CompleteFlag);

            /* Now new frame is ready, pass it to LCDIF. */
            ELCDIF_SetNextBufferAddr(LCDIF, (uint32_t)s_psBufferLcd[curLcdBufferIdx]);

            /*
             * The new frame is not loaded untill current frame display finished. So
             * wait until current frame finished.
             */
            ELCDIF_ClearInterruptStatus(LCDIF, kELCDIF_CurFrameDone);
            while (!(kELCDIF_CurFrameDone & ELCDIF_GetInterruptStatus(LCDIF)))
            {
            }

            /* Switch to the other LCD buffer. */
            curLcdBufferIdx ^= 1U;
        }
    }
}

/******************************************************************************
 *函数名称：Hal_Lcd_Test
 *功能描述：LCD Test
 *参数说明：无
 *返 回 值：无
 *注意事项：无
 */
void Hal_Lcd_Test()
{
    Hal_Lcd_InitBuffer();
    Hal_Lcd_DrawCircle(320,320,80,0xFF0000);
//    Hal_Lcd_ScaleTest();
}

