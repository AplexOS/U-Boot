/**
 * \file   lcd.c
 *
 * \brief  This file contains functions which configure the lcd
 */

/*
* Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
*/
/*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#include "hw_control_AM335x.h"
#include "soc_AM335x.h"
#include "evmAM335x.h"
#include "hw_cm_per.h"
#include "hw_types.h"
#include "hw_cm_dpll.h"
#include "hw_cm_wkup.h"

unsigned int LCDPinMuxSetup(void)
{
    {
         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(0)) =
                   (0 << CONTROL_CONF_LCD_DATA0_CONF_LCD_DATA0_MMODE_SHIFT)    |
                   (1 << CONTROL_CONF_LCD_DATA0_CONF_LCD_DATA0_PUDEN_SHIFT)    |
                   (0 << CONTROL_CONF_LCD_DATA0_CONF_LCD_DATA0_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_LCD_DATA0_CONF_LCD_DATA0_RXACTIVE_SHIFT) |
                   (0 << CONTROL_CONF_LCD_DATA0_CONF_LCD_DATA0_SLEWCTRL_SHIFT); 

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(1)) =
                   (0 << CONTROL_CONF_LCD_DATA1_CONF_LCD_DATA1_MMODE_SHIFT)    |
                   (1 << CONTROL_CONF_LCD_DATA1_CONF_LCD_DATA1_PUDEN_SHIFT)    |
                   (0 << CONTROL_CONF_LCD_DATA1_CONF_LCD_DATA1_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_LCD_DATA1_CONF_LCD_DATA1_RXACTIVE_SHIFT) |
                   (0 << CONTROL_CONF_LCD_DATA1_CONF_LCD_DATA1_SLEWCTRL_SHIFT);  

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(2)) =
                   (0 << CONTROL_CONF_LCD_DATA2_CONF_LCD_DATA2_MMODE_SHIFT)    |
                   (1 << CONTROL_CONF_LCD_DATA2_CONF_LCD_DATA2_PUDEN_SHIFT)    |
                   (0 << CONTROL_CONF_LCD_DATA2_CONF_LCD_DATA2_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_LCD_DATA2_CONF_LCD_DATA2_RXACTIVE_SHIFT) |
                   (0 << CONTROL_CONF_LCD_DATA2_CONF_LCD_DATA2_SLEWCTRL_SHIFT);  

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(3)) =
                   (0 << CONTROL_CONF_LCD_DATA3_CONF_LCD_DATA3_MMODE_SHIFT)    |
                   (1 << CONTROL_CONF_LCD_DATA3_CONF_LCD_DATA3_PUDEN_SHIFT)    |
                   (0 << CONTROL_CONF_LCD_DATA3_CONF_LCD_DATA3_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_LCD_DATA3_CONF_LCD_DATA3_RXACTIVE_SHIFT) |
                   (0 << CONTROL_CONF_LCD_DATA3_CONF_LCD_DATA3_SLEWCTRL_SHIFT); 

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(4)) =
                   (0 << CONTROL_CONF_LCD_DATA4_CONF_LCD_DATA4_MMODE_SHIFT)    |
                   (1 << CONTROL_CONF_LCD_DATA4_CONF_LCD_DATA4_PUDEN_SHIFT)    |
                   (0 << CONTROL_CONF_LCD_DATA4_CONF_LCD_DATA4_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_LCD_DATA4_CONF_LCD_DATA4_RXACTIVE_SHIFT) |
                   (0 << CONTROL_CONF_LCD_DATA4_CONF_LCD_DATA4_SLEWCTRL_SHIFT); 
 
         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(5)) =
                   (0 << CONTROL_CONF_LCD_DATA5_CONF_LCD_DATA5_MMODE_SHIFT)    |
                   (1 << CONTROL_CONF_LCD_DATA5_CONF_LCD_DATA5_PUDEN_SHIFT)    |
                   (0 << CONTROL_CONF_LCD_DATA5_CONF_LCD_DATA5_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_LCD_DATA5_CONF_LCD_DATA5_RXACTIVE_SHIFT) |
                   (0 << CONTROL_CONF_LCD_DATA5_CONF_LCD_DATA5_SLEWCTRL_SHIFT); 

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(6)) =
                   (0 << CONTROL_CONF_LCD_DATA6_CONF_LCD_DATA6_MMODE_SHIFT)    |
                   (1 << CONTROL_CONF_LCD_DATA6_CONF_LCD_DATA6_PUDEN_SHIFT)    |
                   (0 << CONTROL_CONF_LCD_DATA6_CONF_LCD_DATA6_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_LCD_DATA6_CONF_LCD_DATA6_RXACTIVE_SHIFT) |
                   (0 << CONTROL_CONF_LCD_DATA6_CONF_LCD_DATA6_SLEWCTRL_SHIFT);

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(7)) =
                   (0 << CONTROL_CONF_LCD_DATA7_CONF_LCD_DATA7_MMODE_SHIFT)    |
                   (1 << CONTROL_CONF_LCD_DATA7_CONF_LCD_DATA7_PUDEN_SHIFT)    |
                   (0 << CONTROL_CONF_LCD_DATA7_CONF_LCD_DATA7_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_LCD_DATA7_CONF_LCD_DATA7_RXACTIVE_SHIFT) |
                   (0 << CONTROL_CONF_LCD_DATA7_CONF_LCD_DATA7_SLEWCTRL_SHIFT);

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(8)) =
                   (0 << CONTROL_CONF_LCD_DATA8_CONF_LCD_DATA8_MMODE_SHIFT)    |
                   (1 << CONTROL_CONF_LCD_DATA8_CONF_LCD_DATA8_PUDEN_SHIFT)    |
                   (0 << CONTROL_CONF_LCD_DATA8_CONF_LCD_DATA8_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_LCD_DATA8_CONF_LCD_DATA8_RXACTIVE_SHIFT) |
                   (0 << CONTROL_CONF_LCD_DATA8_CONF_LCD_DATA8_SLEWCTRL_SHIFT); 

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(9)) =
                   (0 << CONTROL_CONF_LCD_DATA9_CONF_LCD_DATA9_MMODE_SHIFT)    |
                   (1 << CONTROL_CONF_LCD_DATA9_CONF_LCD_DATA9_PUDEN_SHIFT)    |
                   (0 << CONTROL_CONF_LCD_DATA9_CONF_LCD_DATA9_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_LCD_DATA9_CONF_LCD_DATA9_RXACTIVE_SHIFT) |
                   (0 << CONTROL_CONF_LCD_DATA9_CONF_LCD_DATA9_SLEWCTRL_SHIFT); 

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(10)) =
                 (0 << CONTROL_CONF_LCD_DATA10_CONF_LCD_DATA10_MMODE_SHIFT)    |
                 (1 << CONTROL_CONF_LCD_DATA10_CONF_LCD_DATA10_PUDEN_SHIFT)    |
                 (0 << CONTROL_CONF_LCD_DATA10_CONF_LCD_DATA10_PUTYPESEL_SHIFT)|
                 (1 << CONTROL_CONF_LCD_DATA10_CONF_LCD_DATA10_RXACTIVE_SHIFT) |
                 (0 << CONTROL_CONF_LCD_DATA10_CONF_LCD_DATA10_SLEWCTRL_SHIFT); 

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(11)) =
                 (0 << CONTROL_CONF_LCD_DATA11_CONF_LCD_DATA11_MMODE_SHIFT)     |
                 (1 << CONTROL_CONF_LCD_DATA11_CONF_LCD_DATA11_PUDEN_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_DATA11_CONF_LCD_DATA11_PUTYPESEL_SHIFT) |
                 (1 << CONTROL_CONF_LCD_DATA11_CONF_LCD_DATA11_RXACTIVE_SHIFT)  |
                   (0 << CONTROL_CONF_LCD_DATA11_CONF_LCD_DATA11_SLEWCTRL_SHIFT); 

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(12)) =
                 (0 << CONTROL_CONF_LCD_DATA12_CONF_LCD_DATA12_MMODE_SHIFT)     |
                 (1 << CONTROL_CONF_LCD_DATA12_CONF_LCD_DATA12_PUDEN_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_DATA12_CONF_LCD_DATA12_PUTYPESEL_SHIFT) |
                 (1 << CONTROL_CONF_LCD_DATA12_CONF_LCD_DATA12_RXACTIVE_SHIFT)  |
                 (0 << CONTROL_CONF_LCD_DATA12_CONF_LCD_DATA12_SLEWCTRL_SHIFT);

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(13)) =
                 (0 << CONTROL_CONF_LCD_DATA13_CONF_LCD_DATA13_MMODE_SHIFT)     |
                 (1 << CONTROL_CONF_LCD_DATA13_CONF_LCD_DATA13_PUDEN_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_DATA13_CONF_LCD_DATA13_PUTYPESEL_SHIFT) |
                 (1 << CONTROL_CONF_LCD_DATA13_CONF_LCD_DATA13_RXACTIVE_SHIFT)  |
                 (0 << CONTROL_CONF_LCD_DATA13_CONF_LCD_DATA13_SLEWCTRL_SHIFT);

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(14)) =
                 (0 << CONTROL_CONF_LCD_DATA14_CONF_LCD_DATA14_MMODE_SHIFT)     |
                 (1 << CONTROL_CONF_LCD_DATA14_CONF_LCD_DATA14_PUDEN_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_DATA14_CONF_LCD_DATA14_PUTYPESEL_SHIFT) |
                 (1 << CONTROL_CONF_LCD_DATA14_CONF_LCD_DATA14_RXACTIVE_SHIFT)  |
                 (0 << CONTROL_CONF_LCD_DATA14_CONF_LCD_DATA14_SLEWCTRL_SHIFT);

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(15)) =
                 (0 << CONTROL_CONF_LCD_DATA15_CONF_LCD_DATA15_MMODE_SHIFT)     |
                 (1 << CONTROL_CONF_LCD_DATA15_CONF_LCD_DATA15_PUDEN_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_DATA15_CONF_LCD_DATA15_PUTYPESEL_SHIFT) |
                 (1 << CONTROL_CONF_LCD_DATA15_CONF_LCD_DATA15_RXACTIVE_SHIFT)  |
                 (0 << CONTROL_CONF_LCD_DATA15_CONF_LCD_DATA15_SLEWCTRL_SHIFT);
#if 0
         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(15) ) =
                 (1 << CONTROL_CONF_LCD_DATA16_CONF_LCD_DATA16_MMODE_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_DATA16_CONF_LCD_DATA16_PUDEN_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_DATA16_CONF_LCD_DATA16_PUTYPESEL_SHIFT) |
                 (1 << CONTROL_CONF_LCD_DATA16_CONF_LCD_DATA16_RXACTIVE_SHIFT)  |
                 (0 << CONTROL_CONF_LCD_DATA16_CONF_LCD_DATA16_SLEWCTRL_SHIFT);

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(14) ) =
                 (1 << CONTROL_CONF_LCD_DATA17_CONF_LCD_DATA17_MMODE_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_DATA17_CONF_LCD_DATA17_PUDEN_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_DATA17_CONF_LCD_DATA17_PUTYPESEL_SHIFT) |
                 (1<< CONTROL_CONF_LCD_DATA17_CONF_LCD_DATA17_RXACTIVE_SHIFT)  |
                 (0 << CONTROL_CONF_LCD_DATA17_CONF_LCD_DATA17_SLEWCTRL_SHIFT);

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(13) ) =
                 (1 << CONTROL_CONF_LCD_DATA18_CONF_LCD_DATA18_MMODE_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_DATA18_CONF_LCD_DATA18_PUDEN_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_DATA18_CONF_LCD_DATA18_PUTYPESEL_SHIFT) |
                 (1 << CONTROL_CONF_LCD_DATA18_CONF_LCD_DATA18_RXACTIVE_SHIFT)  |
                   (0 << CONTROL_CONF_LCD_DATA18_CONF_LCD_DATA18_SLEWCTRL_SHIFT);

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(12) ) =
                 (1 << CONTROL_CONF_LCD_DATA19_CONF_LCD_DATA19_MMODE_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_DATA19_CONF_LCD_DATA19_PUDEN_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_DATA19_CONF_LCD_DATA19_PUTYPESEL_SHIFT) |
                 (1<< CONTROL_CONF_LCD_DATA19_CONF_LCD_DATA19_RXACTIVE_SHIFT)  |
                 (0 << CONTROL_CONF_LCD_DATA19_CONF_LCD_DATA19_SLEWCTRL_SHIFT);

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(11) ) =
                 (1 << CONTROL_CONF_LCD_DATA20_CONF_LCD_DATA20_MMODE_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_DATA20_CONF_LCD_DATA20_PUDEN_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_DATA20_CONF_LCD_DATA20_PUTYPESEL_SHIFT) |
                 (1<< CONTROL_CONF_LCD_DATA20_CONF_LCD_DATA20_RXACTIVE_SHIFT)  |
                 (0 << CONTROL_CONF_LCD_DATA20_CONF_LCD_DATA20_SLEWCTRL_SHIFT);

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(10) ) =
                 (1 << CONTROL_CONF_LCD_DATA21_CONF_LCD_DATA21_MMODE_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_DATA21_CONF_LCD_DATA21_PUDEN_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_DATA21_CONF_LCD_DATA21_PUTYPESEL_SHIFT) |
                 (1 << CONTROL_CONF_LCD_DATA21_CONF_LCD_DATA21_RXACTIVE_SHIFT)  |
                 (0 << CONTROL_CONF_LCD_DATA21_CONF_LCD_DATA21_SLEWCTRL_SHIFT);

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(9) ) =
                 (1 << CONTROL_CONF_LCD_DATA22_CONF_LCD_DATA22_MMODE_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_DATA22_CONF_LCD_DATA22_PUDEN_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_DATA22_CONF_LCD_DATA22_PUTYPESEL_SHIFT) |
                 (1 << CONTROL_CONF_LCD_DATA22_CONF_LCD_DATA22_RXACTIVE_SHIFT)  |
                 (0 << CONTROL_CONF_LCD_DATA22_CONF_LCD_DATA22_SLEWCTRL_SHIFT);

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(8) ) =
                 (1 << CONTROL_CONF_LCD_DATA23_CONF_LCD_DATA23_MMODE_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_DATA23_CONF_LCD_DATA23_PUDEN_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_DATA23_CONF_LCD_DATA23_PUTYPESEL_SHIFT) |
                 (1 << CONTROL_CONF_LCD_DATA23_CONF_LCD_DATA23_RXACTIVE_SHIFT)  |
                 (0 << CONTROL_CONF_LCD_DATA23_CONF_LCD_DATA23_SLEWCTRL_SHIFT);
#endif
         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_VSYNC) =
                 (0 << CONTROL_CONF_LCD_VSYNC_CONF_LCD_VSYNC_MMODE_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_VSYNC_CONF_LCD_VSYNC_PUDEN_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_VSYNC_CONF_LCD_VSYNC_PUTYPESEL_SHIFT) |
                 (1 << CONTROL_CONF_LCD_VSYNC_CONF_LCD_VSYNC_RXACTIVE_SHIFT)  |
                 (0 << CONTROL_CONF_LCD_VSYNC_CONF_LCD_VSYNC_SLEWCTRL_SHIFT);

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_HSYNC) =
                 (0 << CONTROL_CONF_LCD_HSYNC_CONF_LCD_HSYNC_MMODE_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_HSYNC_CONF_LCD_HSYNC_PUDEN_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_HSYNC_CONF_LCD_HSYNC_PUTYPESEL_SHIFT) |
                 (1 << CONTROL_CONF_LCD_HSYNC_CONF_LCD_HSYNC_RXACTIVE_SHIFT)  |
                 (0 << CONTROL_CONF_LCD_HSYNC_CONF_LCD_HSYNC_SLEWCTRL_SHIFT);

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_PCLK) =
                 (0 << CONTROL_CONF_LCD_PCLK_CONF_LCD_PCLK_MMODE_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_PCLK_CONF_LCD_PCLK_PUDEN_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_PCLK_CONF_LCD_PCLK_PUTYPESEL_SHIFT) |
                 (1 << CONTROL_CONF_LCD_PCLK_CONF_LCD_PCLK_RXACTIVE_SHIFT)  |
                 (0 << CONTROL_CONF_LCD_PCLK_CONF_LCD_PCLK_SLEWCTRL_SHIFT);

         HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_AC_BIAS_EN) =
                 (0 << CONTROL_CONF_LCD_AC_BIAS_EN_CONF_LCD_AC_BIAS_EN_MMODE_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_AC_BIAS_EN_CONF_LCD_AC_BIAS_EN_PUDEN_SHIFT)     |
                 (0 << CONTROL_CONF_LCD_AC_BIAS_EN_CONF_LCD_AC_BIAS_EN_PUTYPESEL_SHIFT) |
                 (1 << CONTROL_CONF_LCD_AC_BIAS_EN_CONF_LCD_AC_BIAS_EN_RXACTIVE_SHIFT)  |
                 (0 << CONTROL_CONF_LCD_DATA23_CONF_LCD_DATA23_SLEWCTRL_SHIFT);
         return TRUE;
     }
     return FALSE;
}


/**  
 * \brief  This API returns a unique number which identifies itself  
 *         with the LCDC IP in AM335x SoC.  
 * \param  None  
 * \return This returns a number '2' which is unique to LCDC IP in AM335x.
 */
unsigned int LCDVersionGet(void)
{
    return 2;
}

/**
 * \brief   This function will configure the required clocks for LCDC instance.
 *
 * \return  None.
 *
 */
void LCDModuleClkConfig(void)
{
	int i =0;

    //L3
    HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKSTCTRL) |=
                             CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKSTCTRL) &
     CM_PER_L3_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP){
    	i += 1 ;
    	if (i %100 == 0 ){
    		printf("L3 CLKSTC is still waiting sw wakeup.\n");
    	}
    }

    i = 0;
	//L3S
    HWREG(SOC_PRCM_REGS + CM_PER_L3S_CLKSTCTRL) |= 
                             CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L3S_CLKSTCTRL) & 
     CM_PER_L3S_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP){
    	i += 1 ;
    	if (i %100 == 0 ){
    		printf("L3S CLKSTC is still waiting sw wakeup.\n");
    	}
    }


    //L3 INSTR
    i = 0;
    HWREG(SOC_PRCM_REGS + CM_PER_L3_INSTR_CLKCTRL) |= 
                             CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L3_INSTR_CLKCTRL) & 
                               CM_PER_L3_INSTR_CLKCTRL_MODULEMODE) != 
                                   CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE){
    	i += 1 ;
    	if (i %100 == 0 ){
    		printf("L3S CLKSTC is still module enable.\n");
    	}
    }

    // L3 CLC CTRL
    HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKCTRL) |= 
                             CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE;

    i = 0;
    while((HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKCTRL) & 
        CM_PER_L3_CLKCTRL_MODULEMODE) != CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE){
    	i += 1 ;
    	if (i %100 == 0 ){
    		printf("L3S CLKCTRL is still waiting for module enable.\n");
    	}
    }

    // L3 OCPWP

    HWREG(SOC_PRCM_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) |= 
                             CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;
    i = 0;
    while((HWREG(SOC_PRCM_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) & 
                               CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL) !=
                                CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP){
    	i += 1 ;
    	if (i %100 == 0 ){
    		printf("L3S OCPWP is still waiting for wake up.\n");
    	}
    }


    //L4LS CLKCTRL
    HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKCTRL) |= 
                             CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE;
    i =0;
    while((HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKCTRL) & 
      CM_PER_L4LS_CLKCTRL_MODULEMODE) != CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE){
    	i += 1 ;
    	if (i %100 == 0 ){
    		printf("L4LS CLKCTRL  is still waiting for enable.\n");
    	}
    }


    //L4LS state control
    HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKSTCTRL) |=
                             CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKSTCTRL) &
                             CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL) !=
                               CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP){
    	i += 1;
    	if (i %100 == 0 ){
    		printf("L4LS CLKSTCTRL  is still waiting for wakeup.\n");
    	}
    }

    // page 1188, 1341 0x0 = SEL1 : Select DISP PLL CLKOUTM2
    /* lcd pixel clock is derived from peripheral pll */  
    HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_LCDC_PIXEL_CLK) = 
                             CM_DPLL_CLKSEL_LCDC_PIXEL_CLK_CLKSEL_SEL1;		//DEFAULT:100M	SEE 0x44e00454	0x00006417
                             //CM_DPLL_CLKSEL_LCDC_PIXEL_CLK_CLKSEL_SEL2;	//250M
                             //CM_DPLL_CLKSEL_LCDC_PIXEL_CLK_CLKSEL_SEL3;	//192M

	// default :0x6417	(24*100/(23+1))=2400/24=100M 
	// 0x7803 (24*120/3+1))=720M
	HWREG(SOC_CM_WKUP_REGS+ CM_WKUP_CM_CLKSEL_DPLL_DISP ) = 0x7803;//leelin set base clock 720M
    //start the control
    //configuring the Display PLL, page 1313.
    //switch PLL to bypass mode by setting CM_CLKMODE_DPLL_DISP.DPLL_EN = 0x4
//    HWREG(SOC_CM_WKUP_REGS+ CM_WKUP_CM_CLKMODE_DPLL_DISP ) |=
//    			CM_WKUP_CM_CLKMODE_DPLL_MPU_DPLL_EN_DPLL_MN_BYP_MODE;
//
//    i = 0;
//    while((HWREG(SOC_CM_WKUP_REGS+ CM_WKUP_CM_IDLEST_DPLL_DISP) &
//    		CM_WKUP_CM_IDLEST_DPLL_MPU_ST_MN_BYPASS) !=
//    				CM_WKUP_CM_IDLEST_DPLL_MPU_ST_MN_BYPASS_MN_BYPASS << CM_WKUP_CM_IDLEST_DPLL_MPU_ST_MN_BYPASS_SHIFT){
//    	i += 1 ;
//    	if (i %100 == 0 ){
//    		printf("DPLL DISP MN BYPASS  is still waiting.\n");
//    	}
//    }
//    //page 1289
//    if (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_IDLEST_DPLL_DISP) & CM_WKUP_CM_IDLEST_DPLL_MPU_ST_DPLL_CLK
//    		!= CM_WKUP_CM_IDLEST_DPLL_MPU_ST_DPLL_CLK_DPLL_UNLOCKED << CM_WKUP_CM_IDLEST_DPLL_MPU_ST_DPLL_CLK_SHIFT) {
//    	printf("the DPLL DISP is not in the locked status\n");
//    }

    //configure multiply and divide values by setting CM_CLKSEL_DPLL_DISP.DPLL_MULT and DPLL_DIV to the desired value.
    // M = DPLL_MULT
    // DIV =DPLL_DIV
    //M/(N+1)*CLKINP * 1/M2 ==> REGM4XEN = 0
    //4M/(N+1)*CLKINP * 1/M2 ==> REGM4XEN = 1


   //end control

    HWREG(SOC_PRCM_REGS + CM_PER_LCDC_CLKCTRL) |= 
                             CM_PER_LCDC_CLKCTRL_MODULEMODE_ENABLE;
    i = 0;
    while((HWREG(SOC_PRCM_REGS + CM_PER_LCDC_CLKCTRL) & 
      CM_PER_LCDC_CLKCTRL_MODULEMODE) != CM_PER_LCDC_CLKCTRL_MODULEMODE_ENABLE){
    	i += 1 ;
    	if (i %100 == 0 ){
    		printf("LCDC CLKCTRL is still waiting for enable.\n");
    	}
    }

    i = 0;
    while(!(HWREG(SOC_PRCM_REGS + CM_PER_L3S_CLKSTCTRL) & 
            CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK)){
    	i += 1 ;
    	if (i %100 == 0 ){
    		printf("L3S CLKSTC is still waiting live.\n");
    	}
    }

    i = 0;
    while(!(HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKSTCTRL) & 
            CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK)){
    	i += 1 ;
    	if (i %100 == 0 ){
    		printf("L3 CLKSTC is still waiting live.\n");
    	}
    }

    i = 0;
    while(!(HWREG(SOC_PRCM_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) & 
           (CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK | 
            CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L4_GCLK))){
    	i += 1 ;
    	if (i %100 == 0 ){
    		printf("OCPWP_L3 CLK is still waiting live.\n");
    	}
    }

    i = 0;
    //L4LS CLK State Control, make sure the L4LS and LCDC clock is active
    while(!(HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKSTCTRL) & 
           (CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_L4LS_GCLK | 
            CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_LCDC_GCLK))){
    		i += 1;
    		if (i%100 == 0) {
    			printf("L4LS CLK is still waiting live.\n");
    		}
    }
    
}

