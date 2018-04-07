/**
  ******************************************************************************
  * @File       Touch_GT9xx
  * @Author     shulai
  * @Date       2018-3-20
  * @Brief      I2C����������

  * @Modified
  ******************************************************************************
  */
  
#include "Touch_GT9xx.h"
#include "Hal_I2C.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  

#define TOUCH_RST_GPIO                  GPIO1
#define TOUCH_RST_GPIO_PIN              (2U)

#define TOUCH_INT_GPIO                  GPIO1
#define TOUCH_INT_GPIO_PIN              (11U)
#define TOUCH_INT_IRQ                   GPIO1_Combined_0_15_IRQn
#define TOUCH_IRQ_HANDLER               GPIO1_Combined_0_15_IRQHandler

/* 5����GT9157�������� */
const uint8_t CTP_CFG_GT9157[] ={ 
    0x00,0x20,0x03,0xE0,0x01,0x05,0x3C,0x00,0x01,0x08,
    0x28,0x0C,0x50,0x32,0x03,0x05,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x17,0x19,0x1E,0x14,0x8B,0x2B,0x0D,
    0x33,0x35,0x0C,0x08,0x00,0x00,0x00,0x9A,0x03,0x11,
    0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x32,0x00,0x00,
    0x00,0x20,0x58,0x94,0xC5,0x02,0x00,0x00,0x00,0x04,
    0xB0,0x23,0x00,0x93,0x2B,0x00,0x7B,0x35,0x00,0x69,
    0x41,0x00,0x5B,0x4F,0x00,0x5B,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,
    0x12,0x14,0x16,0x18,0x1A,0xFF,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0F,
    0x10,0x12,0x13,0x16,0x18,0x1C,0x1D,0x1E,0x1F,0x20,
    0x21,0x22,0x24,0x26,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
    0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0x48,0x01
};

/* 7����GT911�������� */
const uint8_t CTP_CFG_GT911[] =  {
  0x00,0x20,0x03,0xE0,0x01,0x05,0x0D,0x00,0x01,0x08,
  0x28,0x0F,0x50,0x32,0x03,0x05,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x8A,0x2A,0x0C,
  0x45,0x47,0x0C,0x08,0x00,0x00,0x00,0x02,0x02,0x2D,
  0x00,0x00,0x00,0x00,0x00,0x03,0x64,0x32,0x00,0x00,
  0x00,0x28,0x64,0x94,0xC5,0x02,0x07,0x00,0x00,0x04,
  0x9C,0x2C,0x00,0x8F,0x34,0x00,0x84,0x3F,0x00,0x7C,
  0x4C,0x00,0x77,0x5B,0x00,0x77,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x18,0x16,0x14,0x12,0x10,0x0E,0x0C,0x0A,
  0x08,0x06,0x04,0x02,0xFF,0xFF,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x16,0x18,0x1C,0x1D,0x1E,0x1F,0x20,0x21,
  0x22,0x24,0x13,0x12,0x10,0x0F,0x0A,0x08,0x06,0x04,
  0x02,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x24,0x01	
};

Touch_GT9xx_T Touch_GT9xx;
uint8_t config[GTP_CONFIG_MAX_LENGTH + GTP_ADDR_LENGTH] = {GTP_REG_CONFIG_DATA >> 8, GTP_REG_CONFIG_DATA & 0xff};

/******************************************************************************
 *�������ƣ�GTP_PinInit
 *���������������ʳ�ʼ��
 *����˵������
 *�� �� ֵ����
 *ע�����
 */
void GTP_PinInit()
{
    gpio_pin_config_t rst_int_config = 
    {
        .direction = kGPIO_DigitalOutput,
        .outputLogic = 0,
        .interruptMode = kGPIO_NoIntmode
    };
    
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_02_GPIO1_IO02,1U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_11_GPIO1_IO11,0U);    
    
    GPIO_PinInit(TOUCH_RST_GPIO, TOUCH_RST_GPIO_PIN, &rst_int_config);
    GPIO_PinInit(TOUCH_INT_GPIO, TOUCH_INT_GPIO_PIN, &rst_int_config);

    /* ��ʼ��GT9157,rstΪ�ߵ�ƽ��intΪ�͵�ƽ����gt9157���豸��ַ������Ϊ0xBA */    
    GPIO_PinWrite(TOUCH_INT_GPIO, TOUCH_INT_GPIO_PIN, 0U);
    GPIO_PinWrite(TOUCH_RST_GPIO, TOUCH_RST_GPIO_PIN, 0U);
    SConf_DelayUS(200*1000);
    /*����һ��ʱ�䣬���г�ʼ��*/
    GPIO_PinWrite(TOUCH_RST_GPIO, TOUCH_RST_GPIO_PIN, 1U);
    SConf_DelayUS(200*1000);
    
    /* int���ó��ж����� */
    rst_int_config.direction = kGPIO_DigitalInput;
    rst_int_config.outputLogic = 0;
    rst_int_config.interruptMode = kGPIO_IntRisingEdge;
    
    GPIO_PinInit(TOUCH_INT_GPIO, TOUCH_INT_GPIO_PIN, &rst_int_config);
    GPIO_PortEnableInterrupts(TOUCH_INT_GPIO, 1U << TOUCH_INT_GPIO_PIN);
}

/******************************************************************************
 *�������ƣ�GT9xx_Init
 *������������������ʼ��
 *����˵������
 *�� �� ֵ����
 *ע�����
 */
void GT9xx_Init()
{
    Hal_I2C_Master_Init(I2C_CH1,400000U);
    GTP_PinInit();
    
    GT9xx_ReadID();
    GT9xx_ConfigPara();
    SConf_DelayUS(10*1000);
//    EnableIRQ(TOUCH_INT_IRQ);
    GTP_Get_Info();
}

/******************************************************************************
 *�������ƣ�GT9xx_ReadID
 *������������ȡоƬID
 *����˵������
 *�� �� ֵ����
 *ע�����
 */
int32_t GT9xx_ReadID()
{
    int32_t ret = -1;
    uint8_t buf[8] = {GTP_REG_VERSION >> 8, GTP_REG_VERSION & 0xff};    //�Ĵ�����ַ
    ret = I2C_Transfer_Read(I2C_CH1, GTP_ADDR, buf, GTP_ADDR_LENGTH, sizeof(buf));
    
    if (ret < 0)
    {
        GTP_ERROR("GTP read version failed");
        return ret;
    }

    if (buf[5] == 0x00)
    {
        GTP_INFO("IC1 Version: %c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[7], buf[6]);
        if(buf[2] == '9' && buf[3] == '1' && buf[4] == '1')
            Touch_GT9xx.ChipID = GT911;
    }
    else
    {
        GTP_INFO("IC2 Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);
        if(buf[2] == '9' && buf[3] == '1' && buf[4] == '5' && buf[5] == '7')
            Touch_GT9xx.ChipID = GT9157; 
    }
    return ret;
}

/******************************************************************************
 *�������ƣ�GT9xx_ConfigPara
 *�������������ò���
 *����˵������
 *�� �� ֵ����
 *ע�����
 */
int32_t GT9xx_ConfigPara()
{
    uint8_t *cfg_info;
    uint8_t cfg_info_len;
    int32_t i = 0;
    int32_t ret = -1;
    int32_t retry = 0;
    uint8_t check_sum = 0;
    uint8_t cfg_num = 0x80FE - 0x8047 + 1;           //��Ҫ���õļĴ�������
    
    if(Touch_GT9xx.ChipID == GT9157)
    {
        cfg_info =  (uint8_t *)CTP_CFG_GT9157;  //ָ��Ĵ�������
        cfg_info_len = sizeof(CTP_CFG_GT9157) / sizeof(CTP_CFG_GT9157[0]);  //�������ñ��Ĵ�С
    }
    else
    {
        cfg_info =  (uint8_t *)CTP_CFG_GT911;  //ָ��Ĵ�������
        cfg_info_len = sizeof(CTP_CFG_GT911) / sizeof(CTP_CFG_GT911[0]);    //�������ñ��Ĵ�С
    }
    
    memset(&config[GTP_ADDR_LENGTH], 0, GTP_CONFIG_MAX_LENGTH);
    memcpy(&config[GTP_ADDR_LENGTH], cfg_info, cfg_info_len);
    
    //����Ҫд��checksum�Ĵ�����ֵ
    check_sum = 0;
    for (i = GTP_ADDR_LENGTH; i < cfg_num+GTP_ADDR_LENGTH; i++)
    {
        check_sum += config[i];
    }
    config[ cfg_num+GTP_ADDR_LENGTH] = (~check_sum) + 1;    //checksum
    config[ cfg_num+GTP_ADDR_LENGTH+1] =  1;                //refresh ���ø��±�־

    //д��������Ϣ
    for (retry = 0; retry < 5; retry++)
    {
        ret = I2C_Transfer_Write(I2C_CH1, GTP_ADDR, config , cfg_num + GTP_ADDR_LENGTH+2);
        if (ret > 0)
        {
            break;
        }
    }
    
    return 0;
}

/******************************************************************************
 *�������ƣ�GTP_Get_Info
 *������������ȡ������Ϣ
 *����˵������
 *�� �� ֵ����
 *ע�����
 */
int32_t GTP_Get_Info(void)
{
    uint8_t opr_buf[6] = {0};
    int32_t ret = 0;

    uint16_t abs_x_max = GTP_MAX_WIDTH;
    uint16_t abs_y_max = GTP_MAX_HEIGHT;
    uint8_t int_trigger_type = GTP_INT_TRIGGER;
        
    opr_buf[0] = (uint8_t)((GTP_REG_CONFIG_DATA+1) >> 8);
    opr_buf[1] = (uint8_t)((GTP_REG_CONFIG_DATA+1) & 0xFF);
    
    ret = I2C_Transfer_Read(I2C_CH1,GTP_ADDR, opr_buf, GTP_ADDR_LENGTH, 6);
    if (ret < 0)
    {
        return 1;
    }
    
    abs_x_max = (opr_buf[3] << 8) + opr_buf[2];
    abs_y_max = (opr_buf[5] << 8) + opr_buf[4];
    
    opr_buf[0] = (uint8_t)((GTP_REG_CONFIG_DATA+6) >> 8);
    opr_buf[1] = (uint8_t)((GTP_REG_CONFIG_DATA+6) & 0xFF);
    
    ret = I2C_Transfer_Read(I2C_CH1,GTP_ADDR, opr_buf, GTP_ADDR_LENGTH, 3);
    if (ret < 0)
    {
        return 1;
    }
    int_trigger_type = opr_buf[2] & 0x03;
    
    GTP_INFO("X_MAX = %d, Y_MAX = %d, TRIGGER = 0x%02x",
            abs_x_max,abs_y_max,int_trigger_type);
    
    return 0;    
}

/******************************************************************************
 *�������ƣ�vFT5X06_Scan
 *������������ȡ�������ݡ���ȡȫ�������ݣ���Ҫ 720us����
 *����˵������
 *�� �� ֵ����
 *ע�����
 */
void vFT5X06_Scan()
{
//    uint8_t buf[CFG_POINT_READ_BUF];
//    uint8_t i;
//    static uint8_t s_tp_down = 0;
//    uint16_t x, y;
//    static uint16_t x_save, y_save;

//    if (Touch_GT9xx.Enable == 0)
//    {
//        return;
//    }
//    
//    Touch_GT9xx.TimerCount++;
//    /* 10ms ִ��һ�� */
//    if (Touch_GT9xx.TimerCount < 10)
//    {
//        return;
//    }    

//    if (ucFT5X06_PenInt() == 0)
//    {
//        return;
//    }
//    
//    Touch_GT9xx.TimerCount = 0;

//    ucFT5X06_ReadReg(2, buf, 1);	
//    if ((buf[0] & 0x07) == 0)
//    {
//        if (s_tp_down == 1)
//        {
//            s_tp_down = 0;
//            vTouch_PutKey(TOUCH_RELEASE, x_save, y_save);	/* �ͷ� */
//        }
//        return;
//    }

//    /* �д�������ȡ���������� */
//    ucFT5X06_ReadReg(0, buf, CFG_POINT_READ_BUF);
//    
//    Touch_GT9xx.Count = buf[2] & 0x07; 
//    if (Touch_GT9xx.Count > FT5X06_TOUCH_POINTS)
//    {
//        Touch_GT9xx.Count = FT5X06_TOUCH_POINTS;
//    }
//    
//    Touch_GT9xx.Count = 0;
//    for (i = 0; i < FT5X06_TOUCH_POINTS; i++)
//    {
//        uint8_t pointid;
//        
//        pointid = (buf[5 + 6*i]) >> 4;
//        if (pointid >= 0x0f)
//        {
//            break;
//        }
//        else
//        {
//            Touch_GT9xx.Count++;
//            Touch_GT9xx.X[i] = (int16_t)(buf[3 + 6*i] & 0x0F)<<8 | (int16_t)buf[4 + 6*i];
//            Touch_GT9xx.Y[i] = (int16_t)(buf[5 + 6*i] & 0x0F)<<8 | (int16_t)buf[6 + 6*i];
//            Touch_GT9xx.Event[i] = buf[0x3 + 6*i] >> 6;
//            Touch_GT9xx.id[i] = (buf[5 + 6*i])>>4;
//        }
//    }
//    
//    /* ��ⰴ�� */
//    if (Touch_GT9xx.ChipID == 0x55)       /* 4.3�� 480 * 272 */
//    {
//        x = Touch_GT9xx.Y[0];
//        y = Touch_GT9xx.X[0];	
//        
//        /* �ж�ֵ�� */
//        if (x > 479)
//        {
//            x = 479;
//        }
//        
//        if (y > 271)
//        {
//            y = 271;
//        }			
//    }
//    
//    if (s_tp_down == 0)
//    {
//        s_tp_down = 1;
//        
//        vTouch_PutKey(TOUCH_DOWN, x, y);
//    }
//    else
//    {
//        vTouch_PutKey(TOUCH_MOVE, x, y);
//    }
//    x_save = x;	/* �������꣬�����ͷ��¼� */
//    y_save = y;
//    
//#if 0  /* ��ӡ5����������� */	
//	printf("(%5d,%5d,%3d,%3d) ",  xFT5X06.X[0], xFT5X06.Y[0], xFT5X06.Event[0],  xFT5X06.id[0]);
//	printf("(%5d,%5d,%3d,%3d) ",  xFT5X06.X[1], xFT5X06.Y[1], xFT5X06.Event[1],  xFT5X06.id[1]);
//	printf("(%5d,%5d,%3d,%3d) ",  xFT5X06.X[2], xFT5X06.Y[2], xFT5X06.Event[2],  xFT5X06.id[2]);
//	printf("(%5d,%5d,%3d,%3d) ",  xFT5X06.X[3], xFT5X06.Y[3], xFT5X06.Event[3],  xFT5X06.id[3]);
//	printf("(%5d,%5d,%3d,%3d) ",  xFT5X06.X[4], xFT5X06.Y[4], xFT5X06.Event[4],  xFT5X06.id[4]);
//	printf("\r\n");
//#endif
}

void TOUCH_IRQ_HANDLER(void)
{ 
    /* clear the interrupt status */
    GPIO_PortClearInterruptFlags(TOUCH_INT_GPIO, 1U << TOUCH_INT_GPIO_PIN);

    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}