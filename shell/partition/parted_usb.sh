#!/bin/sh
#the partitions size different according to the sector size.
#normal usb sector size is 512, some mmcblk* device sector size is 64*512 = 32768.

dd if=/dev/zero of=$1 bs=1024 count=1

echo -e "fdisk $1 now..."
fdisk $1 <<eof
n
p
1

1024000

n
p
2

3072000

n
p
3


w
eof

mke2fs /dev/sdb1
sync
mke2fs /dev/sdb2
sync
mkfs.ext4 /dev/sdb3

