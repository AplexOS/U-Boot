#!/bin/sh

TARGET=$1
TARGET_BITBUCKET=git@bitbucket.org:chenfulin5/u-boot2016.05.git
TARGET_GITLAB=git@192.168.1.4:chenfulin5/u-boot_2016.05.git



if [ $# != 1 ]; then
    echo "Please input push target , bitbucket or gitlab or all "
    exit 0
fi

if [ "$TARGET" = "all" ]; then
    git push $TARGET_BITBUCKET master
    git push $TARGET_GITLAB master
elif [ "$TARGET" = "bitbucket" ]; then
    git push $TARGET_BITBUCKET master
elif [ "$TARGET" = "gitlab" ]; then
    git push $TARGET_GITLAB master
else
    echo "Please input push target , bitbucket or gitlab or all "
    exit 0
fi

