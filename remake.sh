#!/bin/sh



make -j2 O=out CROSS_COMPILE=arm-linux-gnueabihf- am335x_baltos_defconfig all

cp ~/am335x/u-boot_2016.05/out/MLO  $CHEN_PATH -rf
cp ~/am335x/u-boot_2016.05/out/u-boot.img  $CHEN_PATH  -rf
echo  Copy u-boot image over ....
