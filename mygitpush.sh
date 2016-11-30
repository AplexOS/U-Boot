#!/bin/sh


TARGET=$1

if [ $# != 1 ]; then
    echo "Please input push target , bitbucket or gitlab or all "
    exit 0
fi

if [ "$TARGET" = "all" ]; then
    git push git@bitbucket.org:chenfulin5/u-boot.git master
    git push git@192.168.1.4:chenfulin5/u-boot_2016.05.git master
fi


if [ "$TARGET" = "bitbucket" ]; then
    git push git@bitbucket.org:chenfulin5/u-boot.git master
fi

if [ "$TARGET" = "gitlab" ]; then
    git push git@192.168.1.4:chenfulin5/u-boot_2016.05.git master
fi
