#!/bin/sh



make -j2 O=out CROSS_COMPILE=arm-linux-gnueabihf- am335x_baltos_defconfig all
