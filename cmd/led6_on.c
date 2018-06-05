/*
 * Control GPIO pins on the fly
 *
 * Copyright (c) 2008-2011 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <command.h>
#include <errno.h>
#include <dm.h>
#include <asm/gpio.h>

static int do_gpio(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{	

   // printf("   Warning: no access to GPIO output value\n");
   //writel((readl(gpio_con) |(0x7), gpf3con);
   //         writel((readl(gpio_oe) & (~(0x1<<22)), gpio_oe);
    //                 writel(readl(gpf3dat)|(0x1<<22), gpf3dat);
    unsigned int * con_module = (unsigned int *)0x44E10944; 
    unsigned int * gpio_oe    = (unsigned int *)0x44E07134; 
    unsigned int * gpio_date  = (unsigned int *)0x44E0713C;
    *con_module               = (*con_module) | (0x7);
    *gpio_oe                  = (*gpio_oe) & (~(0x1<<29));
    *gpio_date                = (*gpio_date) |(0x1<<29);
    printf("gpio_0_29_out_on\n");
}
U_BOOT_CMD(led6, 4, 0, do_gpio,
	   "query and control gpio pins",
	   "System burn success sign");
