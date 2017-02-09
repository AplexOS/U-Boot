make clean
make mx6dl_sabresd_android_config
make

SHARE_PATH_PATH="SBC7112_A4.2_TOOL/Profiles/MX6DL Linux Update/OS Firmware/files/android"
cp u-boot.bin "$win/$SHARE_PATH_PATH/u-boot.bin"
echo "work over.    :)"
