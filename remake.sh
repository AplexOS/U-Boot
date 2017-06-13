

export ARCH=arm
export CROSS_COMPILE=~/myandroid/prebuilts/gcc/linux-x86/arm/arm-eabi-4.6/bin/arm-eabi-
make distclean
rm -f ~/myandroid/out/target/product/sabresd_6dq/u-boot*.imx
make mx6dlsabresdandroid_config
make

cp u-boot.imx ~/myandroid/out/target/product/sabresd_6dq/u-boot-imx6dl.imx
cp u-boot.imx /media/sf_7112/IMX6_L5.1_2.1.0_MFG_TOOL/Profiles/Linux/OS\ Firmware/files/android/sabresd/u-boot-imx6dl.imx
sync
sleep 1
echo "copy image (:"
