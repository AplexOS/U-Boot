#!/bin/bash

rm -rf ./ok335x
make O=ok335x ok335x


cp ok335x/MLO $APLEX_PATH  -rf
cp ok335x/u-boot.img $APLEX_PATH -rf

