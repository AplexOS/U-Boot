/*
 * Copyright (C) 2012-2015 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the Freescale i.MX6Q SabreSD board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __MX6QSABRE_COMMON_CONFIG_H
#define __MX6QSABRE_COMMON_CONFIG_H

#define CONFIG_MX6

#ifdef CONFIG_MX6SOLO
#define CONFIG_MX6DL
#endif

#define CONFIG_EEPROM_GPIO_I2C4
/* uncomment for PLUGIN mode support */
/* #define CONFIG_USE_PLUGIN */

/* uncomment for SECURE mode support */
/* #define CONFIG_SECURE_BOOT */

#ifdef CONFIG_SECURE_BOOT
#ifndef CONFIG_CSF_SIZE
#define CONFIG_CSF_SIZE 0x4000
#endif
#endif

#include "mx6_common.h"
#include <linux/sizes.h>

#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

#include <asm/arch/imx-regs.h>
#include <asm/imx-common/gpio.h>

#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG
#define CONFIG_REVISION_TAG

#define CONFIG_SYS_GENERIC_BOARD

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(16 * SZ_1M)

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_LATE_INIT
#define CONFIG_MXC_GPIO

#define CONFIG_MXC_UART

#define CONFIG_CMD_FUSE
#ifdef CONFIG_CMD_FUSE
#define CONFIG_MXC_OCOTP
#endif

/* MMC Configs */
#define CONFIG_FSL_ESDHC
#define CONFIG_FSL_USDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR      0

#define CONFIG_MMC
#define CONFIG_CMD_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_BOUNCE_BUFFER
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_EXT4
#define CONFIG_CMD_EXT4_WRITE
#define CONFIG_CMD_FAT
#define CONFIG_DOS_PARTITION

#define CONFIG_SUPPORT_EMMC_BOOT /* eMMC specific */

#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_MII
#define CONFIG_CMD_NET
#define CONFIG_FEC_MXC
#define CONFIG_MII
#define IMX_FEC_BASE			ENET_BASE_ADDR
#define CONFIG_FEC_XCV_TYPE		RGMII
#define CONFIG_ETHPRIME			"FEC"
#define CONFIG_FEC_MXC_PHYADDR		1

#define CONFIG_PHYLIB
#define CONFIG_PHY_ATHEROS

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_CONS_INDEX              1
#define CONFIG_BAUDRATE                        115200

/* Command definition */
#include <config_cmd_default.h>

#define CONFIG_CMD_BMODE
#define CONFIG_CMD_BOOTZ
#define CONFIG_CMD_SETEXPR
#undef CONFIG_CMD_IMLS

#define CONFIG_BOOTDELAY               0

#define CONFIG_LOADADDR                        0x12000000
#define CONFIG_SYS_TEXT_BASE           0x17800000
#define CONFIG_SYS_MMC_IMG_LOAD_PART	1

#ifdef CONFIG_SYS_BOOT_NAND
#define CONFIG_MFG_NAND_PARTITION "mtdparts=gpmi-nand:64m(boot),16m(kernel),16m(dtb),-(rootfs) "
#else
#define CONFIG_MFG_NAND_PARTITION ""
#endif

#define CONFIG_MFG_ENV_SETTINGS \
	"mfgtool_args=setenv bootargs console=" CONFIG_CONSOLE_DEV ",115200 " \
		"rdinit=/linuxrc " \
		"g_mass_storage.stall=0 g_mass_storage.removable=1 " \
		"g_mass_storage.file=/fat g_mass_storage.ro=1 " \
		"g_mass_storage.idVendor=0x066F g_mass_storage.idProduct=0x37FF "\
		"g_mass_storage.iSerialNumber=\"\" "\
		"enable_wait_mode=off "\
		CONFIG_MFG_NAND_PARTITION \
		"\0" \
		"initrd_addr=0x12C00000\0" \
		"initrd_high=0xffffffff\0" \
		"bootcmd_mfg=run mfgtool_args;bootz ${loadaddr} ${initrd_addr} ${fdt_addr};\0" \

#ifdef CONFIG_SUPPORT_EMMC_BOOT
#define EMMC_ENV \
	"emmcdev=2\0" \
	"update_emmc_firmware=" \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"if ${get_cmd} ${update_sd_firmware_filename}; then " \
			"if mmc dev ${emmcdev} 1; then "	\
				"setexpr fw_sz ${filesize} / 0x200; " \
				"setexpr fw_sz ${fw_sz} + 1; "	\
				"mmc write ${loadaddr} 0x2 ${fw_sz}; " \
			"fi; "	\
		"fi\0"
#else
#define EMMC_ENV ""
#endif

#if defined(CONFIG_SYS_BOOT_NAND)
	/*
	 * The dts also enables the WEIN NOR which is mtd0.
	 * So the partions' layout for NAND is:
	 *     mtd1: 16M      (uboot)
	 *     mtd2: 16M      (kernel)
	 *     mtd3: 16M      (dtb)
	 *     mtd4: left     (rootfs)
	 */
#define CONFIG_EXTRA_ENV_SETTINGS \
	CONFIG_MFG_ENV_SETTINGS \
	"fdt_addr=0x18000000\0" \
	"fdt_high=0xffffffff\0"	  \
	"bootargs=console=" CONFIG_CONSOLE_DEV ",115200 ubi.mtd=4 "  \
		"root=ubi0:rootfs rootfstype=ubifs "		     \
		"mtdparts=gpmi-nand:64m(boot),16m(kernel),16m(dtb),-(rootfs)\0"\
	"bootcmd=nand read ${loadaddr} 0x4000000 0x800000;"\
		"nand read ${fdt_addr} 0x5000000 0x100000;"\
		"bootz ${loadaddr} - ${fdt_addr}\0"

#elif defined(CONFIG_SYS_BOOT_SATA)

#define CONFIG_EXTRA_ENV_SETTINGS \
		CONFIG_MFG_ENV_SETTINGS \
		"fdt_addr=0x18000000\0" \
		"fdt_high=0xffffffff\0"   \
		"bootargs=console=" CONFIG_CONSOLE_DEV ",115200 \0"\
		"bootargs_sata=setenv bootargs ${bootargs} " \
			"root=/dev/sda1 rootwait rw \0" \
		"bootcmd_sata=run bootargs_sata; sata init; " \
			"sata read ${loadaddr} 0x800  0x4000; " \
			"sata read ${fdt_addr} 0x8000 0x800; " \
			"bootz ${loadaddr} - ${fdt_addr} \0" \
		"bootcmd=run bootcmd_sata \0"

#else
#define CONFIG_EXTRA_ENV_SETTINGS \
	CONFIG_MFG_ENV_SETTINGS \
	"epdc_waveform=epdc_splash.bin\0" \
	"script=boot.scr\0" \
	"image=zImage\0" \
	"fdt_file=" CONFIG_DEFAULT_FDT_FILE "\0" \
	"fdt_addr=0x18000000\0" \
	"boot_fdt=try\0" \
	"ip_dyn=yes\0" \
	"console=" CONFIG_CONSOLE_DEV "\0" \
	"fdt_high=0xffffffff\0"	  \
	"initrd_high=0xffffffff\0" \
	"mmcdev=" __stringify(CONFIG_SYS_MMC_ENV_DEV) "\0" \
	"mmcpart=" __stringify(CONFIG_SYS_MMC_IMG_LOAD_PART) "\0" \
	"mmcroot=" CONFIG_MMCROOT " rootwait rw\0" \
	"mmcautodetect=yes\0" \
	"update_sd_firmware=" \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"if mmc dev ${mmcdev}; then "	\
			"if ${get_cmd} ${update_sd_firmware_filename}; then " \
				"setexpr fw_sz ${filesize} / 0x200; " \
				"setexpr fw_sz ${fw_sz} + 1; "	\
				"mmc write ${loadaddr} 0x2 ${fw_sz}; " \
			"fi; "	\
		"fi\0" \
	EMMC_ENV	  \
	"smp=" CONFIG_SYS_NOSMP "\0"\
	"mmcargs=setenv bootargs console=${console},${baudrate} ${smp} " \
		"root=${mmcroot}\0" \
	"loadbootscript=" \
		"fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${script};\0" \
	"bootscript=echo Running bootscript from mmc ...; " \
		"source\0" \
	"loadimage=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image}\0" \
	"loadfdt=fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${fdt_file}\0" \
	"mmcboot=echo Booting from mmc ...; " \
		"run mmcargs; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if run loadfdt; then " \
				"bootz ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0" \
	"netargs=setenv bootargs console=${console},${baudrate} ${smp} " \
		"root=/dev/nfs " \
		"ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp\0" \
	"netboot=echo Booting from net ...; " \
		"run netargs; " \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"${get_cmd} ${image}; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if ${get_cmd} ${fdt_addr} ${fdt_file}; then " \
				"bootz ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0"

#define CONFIG_BOOTCOMMAND \
	"mmc dev ${mmcdev};" \
	"if mmc rescan; then " \
		"if run loadbootscript; then " \
		"run bootscript; " \
		"else " \
			"if run loadimage; then " \
				"run mmcboot; " \
			"else run netboot; " \
			"fi; " \
		"fi; " \
	"else run netboot; fi"
#endif

#define CONFIG_ARP_TIMEOUT     200UL

/* Miscellaneous configurable options */
#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2     "> "
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_CBSIZE              1024

/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS             256
#define CONFIG_SYS_BARGSIZE CONFIG_SYS_CBSIZE

#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_MEMTEST_START       0x10000000
#define CONFIG_SYS_MEMTEST_END         0x10010000
#define CONFIG_SYS_MEMTEST_SCRATCH     0x10800000

#define CONFIG_SYS_LOAD_ADDR           CONFIG_LOADADDR

#define CONFIG_CMDLINE_EDITING
#define CONFIG_STACKSIZE               (128 * 1024)

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS           1
#define PHYS_SDRAM                     MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE          PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR       IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE       IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* FLASH and environment organization */
#define CONFIG_SYS_NO_FLASH

#define CONFIG_ENV_SIZE			(8 * 1024)

#ifndef CONFIG_SYS_NOSMP
#define CONFIG_SYS_NOSMP
#endif

#if defined CONFIG_SYS_BOOT_SPINOR
#define CONFIG_SYS_USE_SPINOR
#define CONFIG_ENV_IS_IN_SPI_FLASH
#elif defined CONFIG_SYS_BOOT_EIMNOR
#define CONFIG_SYS_USE_EIMNOR
#define CONFIG_ENV_IS_IN_FLASH
#elif defined CONFIG_SYS_BOOT_NAND
#define CONFIG_SYS_USE_NAND
#define CONFIG_ENV_IS_IN_NAND
#elif defined CONFIG_SYS_BOOT_SATA
#define CONFIG_ENV_IS_IN_SATA
#define CONFIG_CMD_SATA
#else
#define CONFIG_ENV_IS_IN_MMC
#endif

#ifdef CONFIG_CMD_SATA
#define CONFIG_DWC_AHSATA
#define CONFIG_SYS_SATA_MAX_DEVICE	1
#define CONFIG_DWC_AHSATA_PORT_ID	0
#define CONFIG_DWC_AHSATA_BASE_ADDR	SATA_ARB_BASE_ADDR
#define CONFIG_LBA48
#define CONFIG_LIBATA
#endif

#ifdef CONFIG_SYS_USE_SPINOR
#define CONFIG_CMD_SF
#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_STMICRO
#define CONFIG_MXC_SPI
#define CONFIG_SF_DEFAULT_BUS  0
#define CONFIG_SF_DEFAULT_SPEED 20000000
#define CONFIG_SF_DEFAULT_MODE (SPI_MODE_0)
#endif

#ifdef CONFIG_SYS_USE_EIMNOR
#undef CONFIG_SYS_NO_FLASH
#define CONFIG_SYS_FLASH_BASE           WEIM_ARB_BASE_ADDR
#define CONFIG_SYS_FLASH_SECT_SIZE     (128 * 1024)
#define CONFIG_SYS_MAX_FLASH_BANKS 1    /* max number of memory banks */
#define CONFIG_SYS_MAX_FLASH_SECT 256   /* max number of sectors on one chip */
#define CONFIG_SYS_FLASH_CFI            /* Flash memory is CFI compliant */
#define CONFIG_FLASH_CFI_DRIVER         /* Use drivers/cfi_flash.c */
#define CONFIG_SYS_FLASH_USE_BUFFER_WRITE /* Use buffered writes*/
#define CONFIG_SYS_FLASH_EMPTY_INFO
#endif

#ifdef CONFIG_SYS_USE_NAND
#define CONFIG_CMD_NAND
#define CONFIG_CMD_NAND_TRIMFFS

/* NAND stuff */
#define CONFIG_NAND_MXS
#define CONFIG_SYS_MAX_NAND_DEVICE     1
#define CONFIG_SYS_NAND_BASE           0x40000000
#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_NAND_ONFI_DETECTION

/* DMA stuff, needed for GPMI/MXS NAND support */
#define CONFIG_APBH_DMA
#define CONFIG_APBH_DMA_BURST
#define CONFIG_APBH_DMA_BURST8
#endif

#if defined(CONFIG_ENV_IS_IN_MMC)
#define CONFIG_ENV_OFFSET		(8 * 64 * 1024)
#elif defined(CONFIG_ENV_IS_IN_SPI_FLASH)
#define CONFIG_ENV_OFFSET              (768 * 1024)
#define CONFIG_ENV_SECT_SIZE           (64 * 1024)
#define CONFIG_ENV_SPI_BUS             CONFIG_SF_DEFAULT_BUS
#define CONFIG_ENV_SPI_CS              CONFIG_SF_DEFAULT_CS
#define CONFIG_ENV_SPI_MODE            CONFIG_SF_DEFAULT_MODE
#define CONFIG_ENV_SPI_MAX_HZ          CONFIG_SF_DEFAULT_SPEED
#elif defined(CONFIG_ENV_IS_IN_FLASH)
#undef CONFIG_ENV_SIZE
#define CONFIG_ENV_SIZE                        CONFIG_SYS_FLASH_SECT_SIZE
#define CONFIG_ENV_SECT_SIZE           CONFIG_SYS_FLASH_SECT_SIZE
#define CONFIG_ENV_OFFSET              (4 * CONFIG_SYS_FLASH_SECT_SIZE)
#elif defined(CONFIG_ENV_IS_IN_NAND)
#undef CONFIG_ENV_SIZE
#define CONFIG_ENV_OFFSET              (37 << 20)
#define CONFIG_ENV_SECT_SIZE           (128 << 10)
#define CONFIG_ENV_SIZE                        CONFIG_ENV_SECT_SIZE
#elif defined(CONFIG_ENV_IS_IN_SATA)
#define CONFIG_ENV_OFFSET		(768 * 1024)
#define CONFIG_SATA_ENV_DEV		0
#define CONFIG_SYS_DCACHE_OFF /* remove when sata driver support cache */
#endif

#define CONFIG_OF_LIBFDT

#ifndef CONFIG_SYS_DCACHE_OFF
#define CONFIG_CMD_CACHE
#endif

/* I2C Configs */
#define CONFIG_CMD_I2C
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_SPEED		  100000
#define CONFIG_SBC7112	1

/*#define CONFIG_UBOOT_LOGO_ENABLE_OVER_8BITS*/

#ifndef CONFIG_UBOOT_LOGO_ENABLE_OVER_8BITS
/* Framebuffer */
#define CONFIG_VIDEO
#define CONFIG_VIDEO_IPUV3
#define CONFIG_CFB_CONSOLE
#define CONFIG_VGA_AS_SINGLE_DEVICE
#define CONFIG_SYS_CONSOLE_IS_IN_ENV
#define CONFIG_SYS_CONSOLE_OVERWRITE_ROUTINE
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_BMP_16BPP
	#define CONFIG_FB_BASE	(CONFIG_SYS_TEXT_BASE + 0x1000000)
	#define UBOOT_LOGO_BMP_ADDR 0x00100000
/*#define CONFIG_VIDEO_LOGO*/
#define CONFIG_VIDEO_BMP_LOGO
#ifdef CONFIG_MX6DL                                                             
#define CONFIG_IPUV3_CLK 198000000
#else
#define CONFIG_IPUV3_CLK 264000000
#endif
#define CONFIG_IMX_HDMI
#define CONFIG_IMX_VIDEO_SKIP
#endif  /* CONFIG_UBOOT_LOGO_ENABLE_OVER_8BITS */

#ifdef CONFIG_UBOOT_LOGO_ENABLE_OVER_8BITS
	/* Select one of the output mode */
	/*#define IPU_OUTPUT_MODE_HDMI*/
	#define IPU_OUTPUT_MODE_LVDS
	/*#define IPU_OUTPUT_MODE_LCD*/

	#define CONFIG_FB_BASE	(CONFIG_SYS_TEXT_BASE + 0x1000000)
	#define UBOOT_LOGO_BMP_ADDR 0x00100000

	#define CONFIG_IMX_PWM
	#define IMX_PWM1_BASE	 PWM1_BASE_ADDR
	#define IMX_PWM2_BASE	 PWM2_BASE_ADDR

#ifdef CONFIG_MX6DL
	#define CONFIG_IPUV3_CLK 270000000
#else
	#define CONFIG_IPUV3_CLK 264000000
#endif

#ifdef IPU_OUTPUT_MODE_HDMI
#if 0
	/* For HDMI, 1280*720 resolution */
	#define DISPLAY_WIDTH	1280
	#define DISPLAY_HEIGHT	720
	#define DISPLAY_BPP	32
	#define DISPLAY_IF_BPP	24  /* RGB24 interface */

	#define DISPLAY_HSYNC_START	220
	#define DISPLAY_HSYNC_END	110
	#define DISPLAY_HSYNC_WIDTH	40

	#define DISPLAY_VSYNC_START	20
	#define DISPLAY_VSYNC_END	5
	#define DISPLAY_VSYNC_WIDTH	5

	#define DISPLAY_PIX_CLOCK	74250000  /*(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz) */
#endif
	/* For HDMI, 1920*1080 resolution */
	#define DISPLAY_WIDTH	1920
	#define DISPLAY_HEIGHT	1080
	#define DISPLAY_BPP	32
	#define DISPLAY_IF_BPP	24  /* RGB24 interface */

	#define DISPLAY_HSYNC_START	148
	#define DISPLAY_HSYNC_END	88
	#define DISPLAY_HSYNC_WIDTH	44

	#define DISPLAY_VSYNC_START	36
	#define DISPLAY_VSYNC_END	4
	#define DISPLAY_VSYNC_WIDTH	5

	#define DISPLAY_PIX_CLOCK	148500000  /*(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz) */

	#define DISPLAY_VSYNC_POLARITY			1
	#define DISPLAY_HSYNC_POLARITY			1
	#define DISPLAY_CLOCK_POLARITY			0
	#define DISPLAY_DATA_POLARITY			0
	#define DISPLAY_DATA_ENABLE_POLARITY		1

	#define IPU_NUM 		1  /* 1 for IPU1, 2 for IPU2. */
	#define DI_NUM			0  /* 0 for DI0, 1 for DI1. */
	#define DI_CLOCK_EXTERNAL_MODE  /* When clock external mode was defined, the DI clock root will be PLL5, without this macro, the DI root clock is IPU internal clock. */
	#define CONFIG_IMX_HDMI
#endif

#ifdef IPU_OUTPUT_MODE_LVDS
#if 1
	/* For LVDS, 800*480 resolution */
	#define DISPLAY_WIDTH					800
	#define DISPLAY_HEIGHT					480
	#define DISPLAY_BPP						32
	#define DISPLAY_IF_BPP					18  		/*RGB666 interface */

	#define DISPLAY_HSYNC_START				89
	#define DISPLAY_HSYNC_END				164
	#define DISPLAY_HSYNC_WIDTH				10

	#define DISPLAY_VSYNC_START				25
	#define DISPLAY_VSYNC_END				10
	#define DISPLAY_VSYNC_WIDTH				10
	
	#define DISPLAY_PIX_CLOCK					33484000 /*(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz)*/
#endif
#if 0
	/* For LVDS, 800*600 resolution */
	#define DISPLAY_WIDTH					800
	#define DISPLAY_HEIGHT					600
	#define DISPLAY_BPP						32
	#define DISPLAY_IF_BPP					18  		/*RGB666 interface */

	#define DISPLAY_HSYNC_START				78
	#define DISPLAY_HSYNC_END				78
	#define DISPLAY_HSYNC_WIDTH				100

	#define DISPLAY_VSYNC_START				5
	#define DISPLAY_VSYNC_END				5
	#define DISPLAY_VSYNC_WIDTH				18
	
	#define DISPLAY_PIX_CLOCK					40000000 /*(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz)*/
#endif
#if 0
	/* For LVDS, 1024*768 resolution */
	#define DISPLAY_WIDTH	1024
	#define DISPLAY_HEIGHT	768
	#define DISPLAY_BPP	32
	#define DISPLAY_IF_BPP	18  /* RGB666 interface */

	#define DISPLAY_HSYNC_START	220
	#define DISPLAY_HSYNC_END	40
	#define DISPLAY_HSYNC_WIDTH	60

	#define DISPLAY_VSYNC_START	21
	#define DISPLAY_VSYNC_END	7
	#define DISPLAY_VSYNC_WIDTH	10

	#define DISPLAY_PIX_CLOCK	64000000  /*(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz) */
#endif
#if 0
	/* For LVDS, 1920*1080 resolution, dual channel */
	#define DISPLAY_WIDTH	1920
	#define DISPLAY_HEIGHT	1080
	#define DISPLAY_BPP		32
	#define DISPLAY_IF_BPP	24	/* RGB24 interface */

	#define DISPLAY_HSYNC_START	100
	#define DISPLAY_HSYNC_END	40
	#define DISPLAY_HSYNC_WIDTH	10

	#define DISPLAY_VSYNC_START	20
	#define DISPLAY_VSYNC_END	3
	#define DISPLAY_VSYNC_WIDTH	2

	#define DISPLAY_PIX_CLOCK		135000000  /*(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz) */
	#define LVDS_SPLIT_MODE  /* For dual channel split mode. */
#endif
	#define DISPLAY_VSYNC_POLARITY			0
	#define DISPLAY_HSYNC_POLARITY			0
	#define DISPLAY_CLOCK_POLARITY			0
	#define DISPLAY_DATA_POLARITY			0
	#define DISPLAY_DATA_ENABLE_POLARITY		1

	#define IPU_NUM			1  /* 1 for IPU1, 2 for IPU2. */
	#define DI_NUM			1  /* 0 for DI0, 1 for DI1. */
	#define LVDS_PORT		1  /* 0 for LVDS0, 1 for LVDS1. */
	#define DI_CLOCK_EXTERNAL_MODE  /* When clock external mode was defined, the DI clock root will be PLL3 PFD1, without this macro, the DI root clock is IPU internal clock. */
	/* #define LVDS_CLOCK_SRC_PLL5 */
#endif

#ifdef IPU_OUTPUT_MODE_LCD
	/* For LCD, 800*480 resolution */
	#define DISPLAY_WIDTH	800
	#define DISPLAY_HEIGHT	480
	#define DISPLAY_BPP	32
	#define DISPLAY_IF_BPP	16  /* RGB565 interface */

	#define DISPLAY_HSYNC_START	40
	#define DISPLAY_HSYNC_END	60
	#define DISPLAY_HSYNC_WIDTH	20

	#define DISPLAY_VSYNC_START	10
	#define DISPLAY_VSYNC_END	10
	#define DISPLAY_VSYNC_WIDTH	10

	#define DISPLAY_PIX_CLOCK	27000000  /*(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz) */

	#define DISPLAY_VSYNC_POLARITY			0
	#define DISPLAY_HSYNC_POLARITY			0
	#define DISPLAY_CLOCK_POLARITY			1
	#define DISPLAY_DATA_POLARITY			0
	#define DISPLAY_DATA_ENABLE_POLARITY		1

	#define IPU_NUM			1  /* 1 for IPU1, 2 for IPU2. */
	#define DI_NUM			0  /* 0 for DI0, 1 for DI1. */
	#define DI_CLOCK_EXTERNAL_MODE  /* When clock external mode was defined, the DI clock root will be PLL5, without this macro, the DI root clock is IPU internal clock. */
#endif
#endif  /* CONFIG_UBOOT_LOGO_ENABLE_OVER_8BITS */

#if defined(CONFIG_ANDROID_SUPPORT)
#include "mx6sabreandroid_common.h"
#endif
#endif                         /* __MX6QSABRE_COMMON_CONFIG_H */
