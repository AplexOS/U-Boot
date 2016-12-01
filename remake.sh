#!/bin/bash

set -x
#make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm distclean
rm -rf ./ok335x
#make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm O=am335x am335x_evm
#make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm am335x_evm
#make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm O=ok335x ok335x
make O=ok335x ok335x

[ -e ok335x/MLO ] && mv ok335x/MLO ok335x/nand_MLO
[ -e ok335x/u-boot.img ] && mv ok335x/u-boot.img ok335x/nand_u-boot.img

cp ok335x/nand_MLO $DST_PATH  -rf
cp ok335x/nand_u-boot.img $DST_PATH -rf

#cp ok335x/nand_MLO /mnt/hgfs/Destkop/SBC7109S_8inch\(800,600\)P_Q4.8.5V001_Standard20160328_All/SBC7109S_8inch\(800,600\)P_Q4.8.5V001_Standard20160328/Open_tty_SBC7109S_8inch\(800,600\)P_Q4.8.5V001_Standard
