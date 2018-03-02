export PATH=$HOME/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/bin:$PATH
export CROSS_COMPILE=arm-linux-gnueabihf-
export ARCH=arm

#make distclean
make am335x_sbc_7109_455_defconfig
make 
cp MLO /media/sf_share/7109/.
cp u-boot.img /media/sf_share/7109/.

sync
sleep 1
date
