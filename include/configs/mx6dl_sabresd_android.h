/*
 * Copyright (C) 2010-2012 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the MX6DL SabreSD Freescale board.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef MX6Q_SABRESD_ANDROID_H
#define MX6Q_SABRESD_ANDROID_H

#include <asm/arch/mx6.h>
#include "mx6dl_sabresd.h"

#define CONFIG_USB_DEVICE
#define CONFIG_IMX_UDC		       1
#define CONFIG_FASTBOOT		       1
#define CONFIG_FASTBOOT_STORAGE_EMMC_SATA
#define CONFIG_FASTBOOT_VENDOR_ID      0x18d1
#define CONFIG_FASTBOOT_PRODUCT_ID     0x0d02
#define CONFIG_FASTBOOT_BCD_DEVICE     0x311
#define CONFIG_FASTBOOT_MANUFACTURER_STR  "Freescale"
#define CONFIG_FASTBOOT_PRODUCT_NAME_STR "i.mx6dl Sabre SmartDevice"
#define CONFIG_FASTBOOT_INTERFACE_STR	 "Android fastboot"
#define CONFIG_FASTBOOT_CONFIGURATION_STR  "Android fastboot"
#define CONFIG_FASTBOOT_SERIAL_NUM	"12345"
#define CONFIG_FASTBOOT_SATA_NO		 0
#define CONFIG_BOOTDELAY 0

/*  For system.img growing up more than 256MB, more buffer needs
*   to receive the system.img*/
#define CONFIG_FASTBOOT_TRANSFER_BUF	0x2c000000
#define CONFIG_FASTBOOT_TRANSFER_BUF_SIZE 0x14000000 /* 320M byte */


#define CONFIG_CMD_BOOTI
#define CONFIG_ANDROID_RECOVERY
/* which mmc bus is your main storage ? */
#define CONFIG_ANDROID_MAIN_MMC_BUS 3

#define CONFIG_ANDROID_BOOT_PARTITION_MMC 1
#define CONFIG_ANDROID_SYSTEM_PARTITION_MMC 5
#define CONFIG_ANDROID_RECOVERY_PARTITION_MMC 2
#define CONFIG_ANDROID_CACHE_PARTITION_MMC 6


#define CONFIG_ANDROID_RECOVERY_BOOTARGS_MMC NULL
#define CONFIG_ANDROID_RECOVERY_BOOTCMD_MMC  \
	"booti mmc2 recovery"
#define CONFIG_ANDROID_RECOVERY_CMD_FILE "/recovery/command"
#define CONFIG_INITRD_TAG

#undef CONFIG_LOADADDR
#undef CONFIG_RD_LOADADDR
#undef CONFIG_EXTRA_ENV_SETTINGS


#define CONFIG_LOADADDR		0x10800000	/* loadaddr env var */
#define CONFIG_RD_LOADADDR      0x11000000

#define CONFIG_INITRD_TAG
#define	CONFIG_EXTRA_ENV_SETTINGS					\
                "netdev=eth0\0"                                         \
                "ethprime=FEC0\0"                                       \
                "uboot=u-boot.bin\0"                    \
                "kernel=uImage\0"                               \
                "nfsroot=/opt/eldk/arm\0"                               \
		        "bootargs_base=setenv bootargs console=ttymxc0,115200 video=mxcfb0:dev=ldb,LDB-XGA,bpp=32\0" \
                "bootargs_nfs=setenv bootargs ${bootargs} root=/dev/nfs "\
                        "ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp\0"\
                "bootcmd_net=run bootargs_base bootargs_nfs; "          \
                        "tftpboot ${loadaddr} ${kernel}; bootm\0"       \
                "bootargs_mmc=setenv bootargs ${bootargs} ip=none "     \
                        "root=/dev/mmcblk0p1 rootwait\0"                \
                "bootcmd_mmc=run bootargs_base bootargs_mmc; "   \
                "mmc dev 2; "   \
                "mmc read ${loadaddr} 0x800 0x3000; bootm\0"    \
                "bootcmd=run bootcmd_mmc\0 "   \
		"splashimage=0x30000000\0"				\
		"splashpos=m,m\0"					\
		"lvds_num=1\0"             \

                // "bootargs_base=setenv bootargs console=ttymxc0,115200 video=mxcfb0:dev=ldb,800x480@60,if=RGB666,bpp=32 fbcon=rotate:1\0"\
                //"bootargs_base=setenv bootargs console=ttymxc0,115200 video=mxcfb0:dev=lcd,SEIKO-WVGA,if=RGB24,bpp=32 fbcon=rotate:3\0" \
                //"bootargs_base=setenv bootargs console=tty0 console=ttymxc0,115200 video=mxcfb0:dev=lcd,SEIKO-WVGA,if=RGB24,bpp=32 fbcon=rotate:1\0" \
                //"bootargs_base=setenv bootargs console=ttymxc0,115200 video=mxcfb0:dev=ldb,800x480@60,if=RGB666,bpp=32 fbcon=rotate:1\0"\
                //"bootargs_base=setenv bootargs console=ttymxc0,115200 video=mxcfb0:dev=lcd,SEIKO-WVGA,if=RGB24,bpp=32\0" \
                //"bootargs_base=setenv bootargs console=ttymxc0,115200 video=mxcfb0:dev=ldb,if=RGB666,bpp=32\0"\
                //"bootargs_base=setenv bootargs console=ttymxc0,115200 video=mxcfb0:dev=lcd,SEIKO-WVGA,if=RGB666,bpp=32\0"\
                //"bootargs_base=setenv bootargs console=ttymxc0,115200 video=mxcfb0:dev=ldb,800x480M@60,if=RGB666,bpp=32 fbmem=10M,30M vmalloc=400M\0"LDB-XGA,\
		//"bootargs=console=ttymxc0,115200 init=/init video=mxcfb0:dev=ldb,800x480M@60,if=RGB666,bpp=32 video=mxcfb1:dev=ldb,800x480M@60,if=RGB666,bpp=32 video=mxcfb2:off ldb=dul1 fbmem=10M,30M vmalloc=400M androidboot.console=ttymxc0 androidboot.hardware=freescale\0" 
                //"bootargs_base=setenv bootargs console=ttymxc0,115200\0"\
                //"bootcmd=run bootcmd_mmc\0"                             \
//		"netdev=eth0\0"						\
//		"ethprime=FEC0\0"					\
//		"fastboot_dev=mmc2\0"					\
//		"bootcmd=booti mmc2\0"					\
//		"splashimage=0x30000000\0"				\
//		"splashpos=m,m\0"					\
//		"android_test=keyvalue\0"					\
//		"bootargs=console=ttymxc0,115200 init=/init video=mxcfb0:dev=ldb,bpp=32 video=mxcfb1:off video=mxcfb2:off fbmem=30M fb0base=0x27b00000 vmalloc=400M androidboot.console=ttymxc0 androidboot.hardware=freescale\0" \
//		"lvds_num=1\0"             
#endif
     //"bootargs_base=setenv bootargs console=ttymxc0,115200 video=mxcfb0:off video=mxcfb1:dev=hdmi,1920x1080M@60,if=RGB24,bpp=32 video=mxcfb2:off ldb=dul1 fbmem=30M vmalloc=400M\0"\
     // "bootargs_base=setenv bootargs console=ttymxc0,115200 video=mxcfb0:dev=ldb,640x480M@40,if=RGB666,bpp=32 video=mxcfb1:dev=hdmi,1920x1080M@60,if=RGB24,bpp=32 video=mxcfb2:off ldb=dul1 fbmem=10M,30M vmalloc=400M\0"\
		//"bootargs=console=ttymxc0,115200 init=/init video=mxcfb0:dev=ldb,bpp=32 video=mxcfb1:dev=hdmi,1920x1080M@60,if=RGB24,bpp=32 video=mxcfb2:off ldb=dul1 fbmem=30M,30M vmalloc=400M androidboot.console=ttymxc0 androidboot.hardware=freescale\0" \
		//"bootargs=console=ttymxc0,115200 init=/init video=mxcfb0:dev=ldb,800x480M@60,if=RGB666,bpp=32 video=mxcfb1:dev=hdmi,1920x1080M@60,if=RGB24,bpp=32 video=mxcfb2:off ldb=dul1 fbmem=10M,30M vmalloc=400M androidboot.console=ttymxc0 androidboot.hardware=freescale\0" \
		//"bootargs=console=ttymxc0,115200 init=/init video=mxcfb0:dev=ldb,LDB-XGA,bpp=32 video=mxcfb1:dev=hdmi,640x480M@60,if=RGB24,bpp=32 video=mxcfb2:off ldb=dul1 fbmem=10M,30M vmalloc=400M androidboot.console=ttymxc0 androidboot.hardware=freescale\0" \
		//"bootargs=console=ttymxc0,115200 init=/init video=mxcfb0:dev=ldb,bpp=32 video=mxcfb1:off video=mxcfb2:off fbmem=10M fb0base=0x27b00000 vmalloc=400M androidboot.console=ttymxc0 androidboot.hardware=freescale\0" \
		//"bootargs=console=ttymxc0,115200 init=/init video=mxcfb0:dev=ldb,800x480M@60,if=RGB666,bpp=32 video=mxcfb1:dev=hdmi,800x480M@60,if=RGB24,bpp=32 video=mxcfb2:off ldb=dul1 fbmem=10M,10M vmalloc=400M androidboot.console=ttymxc0 androidboot.hardware=freescale\0" \
		//"bootargs=console=ttymxc0,115200 init=/init video=mxcfb0:dev=ldb,LDB-XGA,if=RGB24,fbpix=RGB32 video=mxcfb1:off video=mxcfb2:off fbmem=40M fb0base=0x27b00000 vmalloc=400M androidboot.console=ttymxc0 androidboot.hardware=freescale\0" \

//default for mx6dl
//console=ttymxc0,115200 init=/init video=mxcfb0:dev=ldb,bpp=32 video=mxcfb1:off video=mxcfb2:off fbmem=10M fb0base=0x27b00000 vmalloc=400M androidboot.console=ttymxc0 androidboot.hardware=freescale

//use for lvds with hdmi 800*480
//"bootargs=console=ttymxc0,115200 init=/init video=mxcfb0:dev=ldb,800x600M@60,if=RGB666,bpp=32 video=mxcfb1:dev=hdmi,800x600M@60,if=RGB24,bpp=32 video=mxcfb2:off ldb=dul1 fbmem=10M,10M vmalloc=400M androidboot.console=ttymxc0 androidboot.hardware=freescale\0" \

//use for lvds with hdmi 800*600
//"bootargs=console=ttymxc0,115200 init=/init video=mxcfb0:dev=ldb,800x600M@60,if=RGB666,bpp=32 video=mxcfb1:dev=hdmi,800x600M@60,if=RGB24,bpp=32 video=mxcfb2:off ldb=dul1 fbmem=10M,10M vmalloc=400M androidboot.console=ttymxc0 androidboot.hardware=freescale\0" \

//use for lvds with hdmi 1920*1080
//"bootargs=console=ttymxc0,115200 init=/init video=mxcfb0:dev=ldb,800x480M@60,if=RGB666,bpp=32 video=mxcfb1:dev=hdmi,1920x1080M@60,if=RGB24,bpp=32 video=mxcfb2:off ldb=dul1 fbmem=10M,30M vmalloc=400M androidboot.console=ttymxc0 androidboot.hardware=freescale\0" \

//use for two lvds display
//"bootargs=console=ttymxc0,115200 init=/init video=mxcfb0:dev=ldb,bpp=32,LDB-XGA ldb=dul0 video=mxcfb1:dev=ldb,bpp=32,LDB-XGA video=mxcfb2:off fbmem=10M fb0base=0x27b00000 vmalloc=400M androidboot.console=ttymxc0 androidboot.hardware=freescale\0" \

//use for 24bit lvds display 
//"bootargs=console=ttymxc0,115200 init=/init video=mxcfb0:dev=ldb,LDB-XGA,if=RGB24,fbpix=RGB32 video=mxcfb1:off video=mxcfb2:off fbmem=40M fb0base=0x27b00000 vmalloc=400M androidboot.console=ttymxc0 androidboot.hardware=freescale\0" \
