#include <common.h>
#include <linux/mtd/mtd.h>
#include <command.h>
#include <console.h>
#include <watchdog.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <jffs2/jffs2.h>
#include <nand.h>
#include <asm/io.h>
#include <asm/errno.h>
#if defined(CONFIG_SOC_KEYSTONE)
#include <asm/ti-common/ti-gpmc.h>
#else
#include <asm/arch/mem.h>
#endif
#include <linux/mtd/omap_gpmc.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/bch.h>
#include <linux/compiler.h>
#include <nand.h>
#include <linux/mtd/omap_elm.h>
#include <dm.h>


#ifdef  CONFIG_CMD_NANDECC
extern void omap_nand_switch_ecc(nand_ecc_modes_t hardware, int32_t mode);
static int do_switch_ecc(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
    int type = 1;

    if (argc < 2)
        goto usage;

    if (strncmp(argv[1], "hw", 2) == 0) {
        if (argc == 3)
            type = simple_strtoul(argv[2], NULL, 10);
        omap_nand_switch_ecc(NAND_ECC_HW, type);
    }
    else if (strncmp(argv[1], "sw", 2) == 0)
        omap_nand_switch_ecc(NAND_ECC_NONE, 1);
    else
        goto usage;

    return 0;

usage:
    printf("Usage: nandecc %s\n", cmdtp->usage);
    return 1;
}


U_BOOT_CMD(
           nandecc, 3, 1,  do_switch_ecc,
           "Switch NAND ECC calculation algorithm b/w hardware and software",
           "[sw|hw <hw_type>] \n"
           "   [sw|hw]- Switch b/w hardware(hw) & software(sw) ecc algorithm\n"
           "   hw_type- 0 for Hamming code\n"
           "            4 for bch4\n"
           "            8 for bch8\n"
           "            16 for bch16\n"
          );
#endif
