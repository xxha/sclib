#!/bin/sh

/usr/app/startup > /usr/config.log
grep "SIOCSIFNETMASK" /usr/config.log
if [ $? -ne 0 ]; then
	echo "ipconfig succeed"
	insmod /mnt/syslib/modules/fecdev.ko
	/usr/app/slm &
	/usr/app/softupdate slm &
else
	echo "sleep 70s"
	sleep 70
	/usr/app/startup &
	insmod /mnt/syslib/modules/fecdev.ko
        /usr/app/slm &
        /usr/app/softupdate slm &
fi	
