#!/bin/sh

for ((i=1;i<=5;i++)); do
	echo "sda$i"
	if [ -b /dev/sda$i ]; then
		echo "find /dev/sda$i"
		mount -t vfat /dev/sda$i /usr/usb
		if [ -f /usr/usb/tx300s-veex-arm.tar.gz ]; then
	
			echo "find image."
			break


		else
			echo "no image."
			umount /usr/usb
			continue
		fi
	else
		echo "No USB stick found, please make sure USB stick plug in."
		exit
	fi
done
