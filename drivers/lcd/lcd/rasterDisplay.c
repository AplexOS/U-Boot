/**
 * \file  rasterDisplay.c
 * 
 * \brief Sample application for raster
 *
*/

#include "soc_AM335x.h"
#include "evmAM335x.h"
#include "raster.h"
//#include "image.h"
#include "rasterDisplay.h"

/******************************************************************************
**                      INTERNAL FUNCTION PROTOTYPES
*******************************************************************************/
static void SetUpLCD(void);
/******************************************************************************
**              FUNCTION DEFINITIONS
******************************************************************************/

void Lcd_Init(unsigned int image, unsigned int image_size)
{

    SetUpLCD();
    // Configuring the base ceiling 
    if(image >0 || image_size>20000)
	{
		RasterDMAFBConfig(SOC_LCDC_0_REGS,
				  (unsigned int)image,
				  (unsigned int)image + image_size - 2,
				  0);
		RasterDMAFBConfig(SOC_LCDC_0_REGS,
				  (unsigned int)image,
				  (unsigned int)image + image_size - 2,
				  1);
	}
    // Enable raster 
    RasterEnable(SOC_LCDC_0_REGS);

}


void Lcd_reset(void)
{
    // disable raster 
    RasterDisable(SOC_LCDC_0_REGS);  
    // disable LCDC clock and DMA clock 
    RasterSoftWareClkDisable(SOC_LCDC_0_REGS);
    RasterDMASoftWareClkDisable(SOC_LCDC_0_REGS);
    // software reset LCDC
    RasterSoftWareResetControlEnable(SOC_LCDC_0_REGS, RASTER_LCD_MODULE_RESET);
}

void Lcd_off(void)
{
    // disable raster -- disable lcd controller
    RasterDisable(SOC_LCDC_0_REGS);

}

void Lcd_on(void)
{
    // Enable raster enable lcd controller
    RasterEnable(SOC_LCDC_0_REGS);

}

/*
** Configures raster to display image 
*/
static void SetUpLCD(void )
{
	int edid=eeprom_i2c_get_EDID();
	int screen_width=eeprom_i2c_get_width();
	int screen_height=eeprom_i2c_get_height();
    /* Enable clock for LCD Module */ 

    LCDModuleClkConfig();
    LCDPinMuxSetup();
	
    /* 
    **Clock for DMA,LIDD and for Core(which encompasses
    ** Raster Active Matrix and Passive Matrix logic) 
    ** enabled.
    */
    RasterClocksEnable(SOC_LCDC_0_REGS);

    /* Disable raster */
    RasterDisable(SOC_LCDC_0_REGS);
    
    /* Configure the pclk and enabel the raster mode */
	switch(edid)
	{
		case 1:
    		RasterClkConfig(SOC_LCDC_0_REGS, 25,	720); //25M
    		break;
		default:
		case 2:
			RasterClkConfig(SOC_LCDC_0_REGS, 30,	720); //33M
    		break;
		case 3:
			RasterClkConfig(SOC_LCDC_0_REGS, 36,	720); //38M
			break;
		case 4:
			RasterClkConfig(SOC_LCDC_0_REGS, 40,	720); //40M
			break;
		case 5:
			RasterClkConfig(SOC_LCDC_0_REGS, 63,	720); //40M
			break;
		case 6:
			RasterClkConfig(SOC_LCDC_0_REGS, 71,	720); //40M
			break;
		case 7:
			RasterClkConfig(SOC_LCDC_0_REGS, 75,	720); //40M
			break;
		//case 8:
			//RasterClkConfig(SOC_LCDC_0_REGS, 40000000,	1200); //40M
			//break;
	}
    /* Configuring DMA of LCD controller */ 
    RasterDMAConfig(SOC_LCDC_0_REGS,
    				RASTER_DOUBLE_FRAME_BUFFER,
                    RASTER_BURST_SIZE_16,
					RASTER_FIFO_THRESHOLD_8,
                    RASTER_BIG_ENDIAN_DISABLE);

    /* Configuring modes(ex:tft or stn,color or monochrome etc) for raster controller */
    RasterModeConfig(SOC_LCDC_0_REGS,
    				 RASTER_DISPLAY_MODE_TFT_UNPACKED, //确定是UNPACKED, unpacked, 会没有颜色
                 	 RASTER_PALETTE_DATA,
					 RASTER_COLOR,
					 RASTER_RIGHT_ALIGNED);

     /* Configuring the polarity of timing parameters of raster controller */

     RasterTiming2Configure(SOC_LCDC_0_REGS,
    		 	 	 	 	 	 	RASTER_FRAME_CLOCK_HIGH |
    		 	 	 	 	 	 	RASTER_LINE_CLOCK_HIGH  |
									RASTER_PIXEL_CLOCK_HIGH | //rising 22
									RASTER_SYNC_EDGE_RISING | //rising 24 确定是上升沿
									RASTER_SYNC_CTRL_ACTIVE | //25 确定是Active
									RASTER_AC_BIAS_HIGH,
											 0,
											 255);

	switch(edid)
	{
		default:
		case 2:
     		RasterHparamConfig(SOC_LCDC_0_REGS, screen_width, 40, 210, 46);
     		RasterVparamConfig(SOC_LCDC_0_REGS,	screen_height, 20, 22, 23);
    		break;
		case 4:
     		RasterHparamConfig(SOC_LCDC_0_REGS, screen_width, 40, 210, 46);
     		RasterVparamConfig(SOC_LCDC_0_REGS,	screen_height, 20, 22, 23);
			break;
		case 5:
     		RasterHparamConfig(SOC_LCDC_0_REGS, screen_width, 40, 210, 46);
     		RasterVparamConfig(SOC_LCDC_0_REGS,	screen_height, 20, 22, 23);
			break;
		case 6:
     		RasterHparamConfig(SOC_LCDC_0_REGS, screen_width,	40, 80, 40);
     		RasterVparamConfig(SOC_LCDC_0_REGS,	screen_height, 	8, 8, 7);
    		break;
			break;
		case 7:
			RasterClkConfig(SOC_LCDC_0_REGS, 75,	600); //40M
			break;
		//case 8:
			//RasterClkConfig(SOC_LCDC_0_REGS, 40000000,	1200); //40M
			//break;
	}

    RasterFIFODMADelayConfig(SOC_LCDC_0_REGS, 128);

}

/***************************** End Of File ************************************/
