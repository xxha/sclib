#!/bin/bash

version=0.1

verusage()
{
        echo ""
        echo -e "Version upgrade tool for veex net-updater systm(ux400), v$version"
        echo ""
        echo -e "Usage:"
        echo -e "\tupgrade v1.0.20_to_v1.0.21_eta_14.tar.gz"
        echo ""
}

err_check()
{
        if [ $? != 0 ]; then
                echo -e "$1"
                exit 1
        fi
}

#check priviledge
if [ "$UID" != "0" ]; then
        echo "Failed, Please run with root priviledge."
        exit 5
fi

#check parameter.
if [ "$#" != "1" ]; then
        verusage
        exit 1
fi

#remount rootfs
mount / -o remount,rw
err_check "Error! remount fs failed."

#remove old directory.
rm -rf /tmp/diff_md5 2>/dev/null
rm -rf /rm-rootfs.txt 2>/dev/null
rm -rf /tmp/diff 2>/dev/null

md5_check()
{
	local newmd5
	local oldmd5

	declare -a newarr
	declare -a oldarr

	pushd /tmp/diff_md5 > /dev/null

	newmd5=$(md5sum "/tmp/diff_md5/$1")
	oldmd5=$(cat "/tmp/diff_md5/$1.md5")

	newarr=($newmd5)
	oldarr=($oldmd5)

	if [ "${newarr[0]}" == "${oldarr[0]}" ]; then
		echo -e "md5sum is right."
	else
		echo -e "Error! package is broken."
		exit 1
	fi

	popd > /dev/null

	unset newarr
	unset oldarr
}

#uncompress upgrade package
tar xf $1 -C /tmp
err_check "Error! umcompress upgrade package failed."

pushd /tmp/diff_md5 >/dev/null

md5_check "$1"

tar xf $1 -C /tmp
err_check "Error! umcompress upgrade package failed."
popd > /dev/null

#upgrade files
echo ""
echo "Upgrading files ..."
cp -rf /tmp/diff/rootfs/* /
err_check "Error! upgrade files failed."

#remove files in rm-rootfs.txt 
rm_files()
{
	local fname

	echo ""
	echo "Removing files ..."

	while read fname; do rm -rf $fname ; done < /rm-rootfs.txt
}

cp /tmp/diff/rm-rootfs.txt /
err_check "Error! get rm-rootfs.txt failed."

cd /
rm_files

#rm redundants
#rm -rf /rm-rootfs.txt
#rm -rf /tmp/diff
#rm -rf /tmp/diff_md5

echo ""
echo "Upgrade ux400 system succeed"
