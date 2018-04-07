/*
** ###################################################################
**     Processors:          MIMXRT1052CVL5A
**                          MIMXRT1052DVL6A
**
**     Compilers:           Keil ARM C/C++ Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**                          MCUXpresso Compiler
**
**     Reference manual:    IMXRT1050RM Rev.C, 08/2017
**     Version:             rev. 0.1, 2017-01-10
**     Build:               b170927
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2017 NXP
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**
**     1. Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**
**     2. Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**
**     3. Neither the name of the copyright holder nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**     - rev. 0.1 (2017-01-10)
**         Initial version.
**
** ###################################################################
*/

/*!
 * @file MIMXRT1052
 * @version 0.1
 * @date 2017-01-10
 * @brief Device specific configuration file for MIMXRT1052 (implementation file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#include <stdint.h>
#include "fsl_device_registers.h"

static uint32_t _RDWORD(uint32_t address)
{    
    return *((uint32_t *)address);
}

static void _WDWORD(uint32_t address, uint32_t value)
{
    *((uint32_t *)address) = value;
}

void SDRAM_WaitIpCmdDone(void)
{
    unsigned long reg;
    do
    {
        reg = _RDWORD(0x402F003C);
    }while((reg & 0x3) == 0);
}

void _clock_init(void)
{
    // Enable all clocks
    _WDWORD(0x400FC068,0xffffffff);
    _WDWORD(0x400FC06C,0xffffffff);
    _WDWORD(0x400FC070,0xffffffff);
    _WDWORD(0x400FC074,0xffffffff);
    _WDWORD(0x400FC078,0xffffffff);
    _WDWORD(0x400FC07C,0xffffffff);
    _WDWORD(0x400FC080,0xffffffff);

    _WDWORD(0x400D8030,0x00002001); 
    _WDWORD(0x400D8100,0x001D0000);   
    _WDWORD(0x400FC014,0x00010D40);   
}

void _sdr_Init(void)
{
    // Config IOMUX
    _WDWORD(0x401F8014, 0x00000000);
    _WDWORD(0x401F8018, 0x00000000);
    _WDWORD(0x401F801C, 0x00000000);
    _WDWORD(0x401F8020, 0x00000000);
    _WDWORD(0x401F8024, 0x00000000);
    _WDWORD(0x401F8028, 0x00000000);
    _WDWORD(0x401F802C, 0x00000000);
    _WDWORD(0x401F8030, 0x00000000);
    _WDWORD(0x401F8034, 0x00000000);
    _WDWORD(0x401F8038, 0x00000000);
    _WDWORD(0x401F803C, 0x00000000);
    _WDWORD(0x401F8040, 0x00000000);
    _WDWORD(0x401F8044, 0x00000000);
    _WDWORD(0x401F8048, 0x00000000);
    _WDWORD(0x401F804C, 0x00000000);
    _WDWORD(0x401F8050, 0x00000000);
    _WDWORD(0x401F8054, 0x00000000);
    _WDWORD(0x401F8058, 0x00000000);
    _WDWORD(0x401F805C, 0x00000000);
    _WDWORD(0x401F8060, 0x00000000);
    _WDWORD(0x401F8064, 0x00000000);
    _WDWORD(0x401F8068, 0x00000000);
    _WDWORD(0x401F806C, 0x00000000);
    _WDWORD(0x401F8070, 0x00000000);
    _WDWORD(0x401F8074, 0x00000000);
    _WDWORD(0x401F8078, 0x00000000);
    _WDWORD(0x401F807C, 0x00000000);
    _WDWORD(0x401F8080, 0x00000000);
    _WDWORD(0x401F8084, 0x00000000);
    _WDWORD(0x401F8088, 0x00000000);
    _WDWORD(0x401F808C, 0x00000000);
    _WDWORD(0x401F8090, 0x00000000);
    _WDWORD(0x401F8094, 0x00000000);
    _WDWORD(0x401F8098, 0x00000000);
    _WDWORD(0x401F809C, 0x00000000);
    _WDWORD(0x401F80A0, 0x00000000);
    _WDWORD(0x401F80A4, 0x00000000);
    _WDWORD(0x401F80A8, 0x00000000);
    _WDWORD(0x401F80AC, 0x00000000);
    _WDWORD(0x401F80B0, 0x00000010); // EMC_39, DQS PIN, enable SION
    _WDWORD(0x401F80B4, 0x00000000);
    _WDWORD(0x401F80B8, 0x00000000);

    // PAD ctrl
    // drive strength = 0x7 to increase drive strength
    // otherwise the data7 bit may fail.
    _WDWORD(0x401F8204, 0x000110F9);
    _WDWORD(0x401F8208, 0x000110F9);
    _WDWORD(0x401F820C, 0x000110F9);
    _WDWORD(0x401F8210, 0x000110F9);
    _WDWORD(0x401F8214, 0x000110F9);
    _WDWORD(0x401F8218, 0x000110F9);
    _WDWORD(0x401F821C, 0x000110F9);
    _WDWORD(0x401F8220, 0x000110F9);
    _WDWORD(0x401F8224, 0x000110F9);
    _WDWORD(0x401F8228, 0x000110F9);
    _WDWORD(0x401F822C, 0x000110F9);
    _WDWORD(0x401F8230, 0x000110F9);
    _WDWORD(0x401F8234, 0x000110F9);
    _WDWORD(0x401F8238, 0x000110F9);
    _WDWORD(0x401F823C, 0x000110F9);
    _WDWORD(0x401F8240, 0x000110F9);
    _WDWORD(0x401F8244, 0x000110F9);
    _WDWORD(0x401F8248, 0x000110F9);
    _WDWORD(0x401F824C, 0x000110F9);
    _WDWORD(0x401F8250, 0x000110F9);
    _WDWORD(0x401F8254, 0x000110F9);
    _WDWORD(0x401F8258, 0x000110F9);
    _WDWORD(0x401F825C, 0x000110F9);
    _WDWORD(0x401F8260, 0x000110F9);
    _WDWORD(0x401F8264, 0x000110F9);
    _WDWORD(0x401F8268, 0x000110F9);
    _WDWORD(0x401F826C, 0x000110F9);
    _WDWORD(0x401F8270, 0x000110F9);
    _WDWORD(0x401F8274, 0x000110F9);
    _WDWORD(0x401F8278, 0x000110F9);
    _WDWORD(0x401F827C, 0x000110F9);
    _WDWORD(0x401F8280, 0x000110F9);
    _WDWORD(0x401F8284, 0x000110F9);
    _WDWORD(0x401F8288, 0x000110F9);
    _WDWORD(0x401F828C, 0x000110F9);
    _WDWORD(0x401F8290, 0x000110F9);
    _WDWORD(0x401F8294, 0x000110F9);
    _WDWORD(0x401F8298, 0x000110F9);
    _WDWORD(0x401F829C, 0x000110F9);
    _WDWORD(0x401F82A0, 0x000110F9);
    _WDWORD(0x401F82A4, 0x000110F9);
    _WDWORD(0x401F82A8, 0x000110F9);

    // Config SDR Controller Registers/
    _WDWORD(0x402F0000,0x10000004); // MCR
    _WDWORD(0x402F0008,0x00030524); // BMCR0
    _WDWORD(0x402F000C,0x06030524); // BMCR1
    _WDWORD(0x402F0010,0x8000001B); // BR0, 32MB
    _WDWORD(0x402F0014,0x8200001B); // BR1, 32MB
    _WDWORD(0x402F0018,0x8400001B); // BR2, 32MB
    _WDWORD(0x402F001C,0x8600001B); // BR3, 32MB
    _WDWORD(0x402F0020,0x90000021); // BR4,
    _WDWORD(0x402F0024,0xA0000019); // BR5,
    _WDWORD(0x402F0028,0xA8000017); // BR6,
    _WDWORD(0x402F002C,0xA900001B); // BR7,
    _WDWORD(0x402F0030,0x00000021); // BR8,
    _WDWORD(0x402F0004,0x000079A8);  //IOCR,SEMC_CCSX0 as NOR CE, SEMC_CSX1 as PSRAM CE, SEMC_CSX2 as NAND CE, SEMC_CSX3 as DBI CE.

    // _WDWORD(0x402F0004,0x00000008); // IOCR, SEMC_CCSX0 as SDRAM_CS1
    _WDWORD(0x402F0040,0x00000F31); // SDRAMCR0
    _WDWORD(0x402F0044,0x00652922); // SDRAMCR1
    _WDWORD(0x402F0048,0x00010920); // SDRAMCR2
    _WDWORD(0x402F004C,0x50210A08); // SDRAMCR3

    _WDWORD(0x402F0080,0x00000021); // DBICR0
    _WDWORD(0x402F0084,0x00888888); // DBICR1
    _WDWORD(0x402F0094,0x00000002); // IPCR1
    _WDWORD(0x402F0098,0x00000000); // IPCR2

    _WDWORD(0x402F0090,0x80000000); // IPCR0
    _WDWORD(0x402F009C,0xA55A000F); // IPCMD, SD_CC_IPREA
    SDRAM_WaitIpCmdDone();
    _WDWORD(0x402F0090,0x80000000); // IPCR0
    _WDWORD(0x402F009C,0xA55A000C); // SD_CC_IAF
    SDRAM_WaitIpCmdDone();
    _WDWORD(0x402F0090,0x80000000); // IPCR0
    _WDWORD(0x402F009C,0xA55A000C); // SD_CC_IAF
    SDRAM_WaitIpCmdDone();
    _WDWORD(0x402F00A0,0x00000033); // IPTXDAT
    _WDWORD(0x402F0090,0x80000000); // IPCR0
    _WDWORD(0x402F009C,0xA55A000A); // SD_CC_IMS
    SDRAM_WaitIpCmdDone();
    _WDWORD(0x402F004C,0x50210A09 ); // enable sdram self refresh again after initialization done.
}

void SDRAM_Init (void) 
{
    _clock_init();
    _sdr_Init();
}

/* ----------------------------------------------------------------------------
   -- Core clock
   ---------------------------------------------------------------------------- */

uint32_t SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

/* ----------------------------------------------------------------------------
   -- SystemInit()
   ---------------------------------------------------------------------------- */

void SystemInit (void) {
#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
  SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));    /* set CP10, CP11 Full Access */
#endif /* ((__FPU_PRESENT == 1) && (__FPU_USED == 1)) */

/* Watchdog disable */

#if (DISABLE_WDOG)
    if (WDOG1->WCR & WDOG_WCR_WDE_MASK)
    {
        WDOG1->WCR &= ~WDOG_WCR_WDE_MASK;
    }
    if (WDOG2->WCR & WDOG_WCR_WDE_MASK)
    {
        WDOG2->WCR &= ~WDOG_WCR_WDE_MASK;
    }
    RTWDOG->CNT = 0xD928C520U; /* 0xD928C520U is the update key */
    RTWDOG->TOVAL = 0xFFFF;
    RTWDOG->CS = (uint32_t) ((RTWDOG->CS) & ~RTWDOG_CS_EN_MASK) | RTWDOG_CS_UPDATE_MASK;
#endif /* (DISABLE_WDOG) */

    /* Disable Systick which might be enabled by bootrom */
    if (SysTick->CTRL & SysTick_CTRL_ENABLE_Msk)
    {
        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    }

/* Enable instruction and data caches */
#if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT
   SCB_EnableICache();
#endif
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
   SCB_EnableDCache();
#endif
    SDRAM_Init();
}

/* ----------------------------------------------------------------------------
   -- SystemCoreClockUpdate()
   ---------------------------------------------------------------------------- */

void SystemCoreClockUpdate (void) {

    uint32_t freq;
    uint32_t PLL1MainClock;
    uint32_t PLL2MainClock;

    /* Periph_clk2_clk ---> Periph_clk */
    if (CCM->CBCDR & CCM_CBCDR_PERIPH_CLK_SEL_MASK)
    {
        switch (CCM->CBCMR & CCM_CBCMR_PERIPH_CLK2_SEL_MASK)
        {
            /* Pll3_sw_clk ---> Periph_clk2_clk ---> Periph_clk */
            case CCM_CBCMR_PERIPH_CLK2_SEL(0U):
                freq = (24000000UL * ((CCM_ANALOG->PLL_USB1 & CCM_ANALOG_PLL_USB1_DIV_SELECT_MASK) ? 22U : 20U));
                break;

            /* Osc_clk ---> Periph_clk2_clk ---> Periph_clk */
            case CCM_CBCMR_PERIPH_CLK2_SEL(1U):
                freq = 24000000UL;
                break;

            case CCM_CBCMR_PERIPH_CLK2_SEL(2U):
            case CCM_CBCMR_PERIPH_CLK2_SEL(3U):
            default:
                freq = 0U;
                break;
        }

        freq /= (((CCM->CBCDR & CCM_CBCDR_PERIPH_CLK2_PODF_MASK) >> CCM_CBCDR_PERIPH_CLK2_PODF_SHIFT) + 1U);
    }
    /* Pre_Periph_clk ---> Periph_clk */
    else
    {
        PLL1MainClock = ((24000000UL * ((CCM_ANALOG->PLL_ARM & CCM_ANALOG_PLL_ARM_DIV_SELECT_MASK) >>
                                         CCM_ANALOG_PLL_ARM_DIV_SELECT_SHIFT)) >> 1U);

        PLL2MainClock = (24000000UL * ((CCM_ANALOG->PLL_SYS & CCM_ANALOG_PLL_SYS_DIV_SELECT_MASK) ? 22U : 20U));
        PLL2MainClock += ((uint64_t)24000000UL * ((uint64_t)(CCM_ANALOG->PLL_SYS_NUM))) / ((uint64_t)(CCM_ANALOG->PLL_SYS_DENOM));


        switch (CCM->CBCMR & CCM_CBCMR_PRE_PERIPH_CLK_SEL_MASK)
        {
            /* PLL2 ---> Pre_Periph_clk ---> Periph_clk */
            case CCM_CBCMR_PRE_PERIPH_CLK_SEL(0U):
                freq = PLL2MainClock;
                break;

            /* PLL2 PFD2 ---> Pre_Periph_clk ---> Periph_clk */
            case CCM_CBCMR_PRE_PERIPH_CLK_SEL(1U):
                freq = PLL2MainClock / ((CCM_ANALOG->PFD_528 & CCM_ANALOG_PFD_528_PFD2_FRAC_MASK) >> CCM_ANALOG_PFD_528_PFD2_FRAC_SHIFT) * 18U;
                break;

            /* PLL2 PFD0 ---> Pre_Periph_clk ---> Periph_clk */
            case CCM_CBCMR_PRE_PERIPH_CLK_SEL(2U):
                freq = PLL2MainClock / ((CCM_ANALOG->PFD_528 & CCM_ANALOG_PFD_528_PFD0_FRAC_MASK) >> CCM_ANALOG_PFD_528_PFD0_FRAC_SHIFT) * 18U;
                break;

            /* PLL1 divided(/2) ---> Pre_Periph_clk ---> Periph_clk */
            case CCM_CBCMR_PRE_PERIPH_CLK_SEL(3U):
                freq = PLL1MainClock / (((CCM->CACRR & CCM_CACRR_ARM_PODF_MASK) >> CCM_CACRR_ARM_PODF_SHIFT) + 1U);
                break;

            default:
                freq = 0U;
                break;
        }
    }

    SystemCoreClock = (freq / (((CCM->CBCDR & CCM_CBCDR_AHB_PODF_MASK) >> CCM_CBCDR_AHB_PODF_SHIFT) + 1U));

}
