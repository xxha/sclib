#!/bin/bash

#
#	version compare tool for veex net-updater systm (V300 platform)
#		by sword <szhang@vetronicsltd.com.cn>, 2012-02-24
#
#

version=0.1

checked=0
changed=0
newadded=0
removed=0
fsize=0
verold=""
vernew=""
eta=0
others=0

verusage()
{
	echo ""
	echo -e "version compare tool for veex net-updater systm(ux400), v$version"
	echo ""
	echo -e "Usage:"
	echo -e "\tvercmp old.tar.gz new.tar.gz"
	echo ""
}

err_check()
{
	if [ $? != 0 ]; then
		echo -e "$1"
		exit 1
	fi
}

#copy changed/new file to diff path.
# $1 is file name, $2 is diff path
do_cpy()
{
	local cursize

	pushd /tmp/vercmp/new/ >/dev/null

	#cp -a "/tmp/vercmp/new/$1" "/tmp/vercmp/diff/$2"
	tar czpf /tmp/vercmp/t.tar.gz "$1" > /dev/null 2>&1
	err_check "Error! Copy /tmp/vercmp/new/$1 to /tmp/vercmp/diff/$2 failed."

	tar xzpf /tmp/vercmp/t.tar.gz -C "/tmp/vercmp/diff/$2" > /dev/null 2>&1
	err_check "Error! Copy /tmp/vercmp/new/$1 to /tmp/vercmp/diff/$2 failed."

	rm /tmp/vercmp/t.tar.gz
	popd > /dev/null

	cursize=$(stat -c%s "/tmp/vercmp/new/$1")
	fsize=$(($fsize+$cursize))
}

#check md5sum. $1 is file name, $2 is diff path
md5_check()
{
	local oldfile
	local newfile
	local oldmd5
	local newmd5

	declare -a newarr
	declare -a oldarr

	oldfile="/tmp/vercmp/old/$1"
	newfile="/tmp/vercmp/new/$1"

	newmd5=$(md5sum "$newfile")
	oldmd5=$(md5sum "$oldfile")

	newarr=($newmd5)
	oldarr=($oldmd5)

	#echo "New: ${newarr[0]}"
	#echo "Old: ${oldarr[0]}"

	if [ "${newarr[0]}" != "${oldarr[0]}" ]; then
		echo "[>>>] $1"
		changed=$(($changed+1))
		#copy
		do_cpy "$1" "$2"
	#else
	#	echo "[EQU] $1, ${newarr[0]}"
	fi
		

	unset newarr
	unset oldarr
}

#check file. $1 is filename, $2 is diff path
file_check()
{
	local oldfile
	local newfile

	oldfile="/tmp/vercmp/old/$1"
	newfile="/tmp/vercmp/new/$1"

	#increment checked file counter.
	checked=$(($checked+1))

	if [ -f "$oldfile" ] ; then 
		#both exist, check md5sum
		md5_check "$1" "$2"
	else
		#old file not exist, new added.
		newadded=$(($newadded+1))
		echo "[+++] $1"

		#copy.
		do_cpy "$1" "$2"
	fi

}

#check new/changed files in new version.
# $1 is file name, $2 if diff path.
file_checknew()
{
	local newfile

	newfile="/tmp/vercmp/new/$1"

	if [ -f "$newfile" ]; then 
		#new OK, check old
		file_check "$1" "$2"
	elif [ -L "$newfile" ]; then
		#symbol link, copy directy
		do_cpy "$1" "$2"
		others=$(($others + 1))
	elif [ -b "$newfile" ]; then
		#block
		do_cpy "$1" "$2"
		others=$(($others + 1))
	elif [ -c "$newfile" ]; then
		#char
		do_cpy "$1" "$2"
		others=$(($others + 1))
	else
		#failed, new not exist.
		echo "Error! $newfile is not a file."
		exit 3
	fi

}

#check removed files from new
# $1 is path, $2 is name
file_checkrm_new()
{
	local newfile
	local cursize
	local rslt

	newfile="/tmp/vercmp/new/$1$2"

	stat "$newfile" >/dev/null 2>&1
	rslt=$?



	if [ "$rslt" == "0" ]; then 
		#OK, skip
		#echo "[   ] $1 OK"
		return
	else
		#failed, new not exist. record it
		echo $2 >> /tmp/vercmp/diff/rm.txt
		echo "[---] $1$2"
		removed=$(($removed+1))

		cursize=$(stat -c%s "/tmp/vercmp/old/$1$2")
		fsize=$(($fsize+$cursize))
	fi
}





#check removed files
# $1 is path, $2 is name
file_checkrm()
{
	local oldfile

	oldfile="/tmp/vercmp/old/$1$2"

	if [ -f "$oldfile" ]; then 
		#old OK, check if remove in new
		file_checkrm_new "$1" "$2"
	elif [ -L "$oldfile" ]; then
		#symbol link
		file_checkrm_new "$1" "$2"
	elif [ -b "$oldfile" ]; then
		#block 
		file_checkrm_new "$1" "$2"
	elif [ -c "$oldfile" ]; then
		#char 
		file_checkrm_new "$1" "$2"
	else
		#failed, old not exist.
		echo "Error! $oldfile is not a file."
		exit 3
	fi

}

#mount jffs2 image and copy to local directory
# $1 is image file name, $2 is local path, $3 is mount point
jffs_cpy()
{
	#echo "copy $1 to flash ram.."
	dd if="$1" of=/dev/mtdblock0 > /dev/null 2>&1
	err_check "copy $1 to /dev/mtdblock0 failed."

	umount "$3" 2> /dev/null

	mount -t jffs2 /dev/mtdblock0 "$3"
	err_check "Error! mount to $3 failed."

	sleep 1

	cp -r "$3" "$2"
	err_check "Error! copy to $2 failed."

	#umount
	umount "$3"
	err_check "Error! unmount $3 failed."
	
}

# $1 if file name
do_check()
{
	file "$1"
	checked=$(($checked+1))
}

#check parameter.
if [ "$#" != "2" ]; then
	verusage
	exit 1
fi

#check priviledge
if [ "$UID" != "0" ]; then
	echo "Failed, Please run with root priviledge."
	exit 5
fi


#remove old directory.
rm -rf /tmp/vercmp 2>/dev/null
rm -rf diff_md5 2>/dev/null

#$1 is file name, $2 is path to uncompress
do_uncompress()
{
	local rname
	local uname
	
	#untar all

	tar xf "$1" -C "$2"
	err_check "Failed. please check $1"

	pushd "$2" > /dev/null


	rname=$(ls | grep rootfs)
	#uname=$(ls | grep usrall)

	echo -e "rname = $rname"
	#echo -e "uname = $uname"

	#rootfs
	mkdir  rootfs
	err_check "create $1/rootfs failed"

	#tar xf "$rname" -C "$2/rootfs"
	#err_check "Failed. please check $1/$rname"

	#usr
	#mkdir usr
	#err_check "create $1/usr failed"

	#tar xf "$uname" -C "$2/usr"
	#err_check "Failed. please check $1/$uname"

	#rm $uname
	rm $rname

	#cd rootfs/p1
	#mv * ../
	#err_check "mv p1 to parent failed"

	#cd ..
	#rm -r p1


	popd  > /dev/null
}

#uncompress old
mkdir -p /tmp/vercmp/old
echo "Uncompress $1..."
do_uncompress "$1" "/tmp/vercmp/old"

#uncompress new
mkdir -p /tmp/vercmp/new
echo "Uncompress $2..."
do_uncompress "$2" "/tmp/vercmp/new"


#create diff direcotry
#mkdir -p /tmp/vercmp/diff/usr
mkdir -p /tmp/vercmp/diff/rootfs

#get version number
verold=$(cat /tmp/vercmp/old/etc/version)
vernew=$(cat /tmp/vercmp/new/etc/version)

echo ""
echo "Old: $verold"
echo "New: $vernew"


if [ "x$verold" == "x" ]; then
	echo "Error! get old version failed"
	exit 6
fi

if [ "x$vernew" == "x" ]; then
	echo "Error! get new version failed"
	exit 7
fi

if [ "$vernew" == "$verold" ]; then
	echo "Error! Versions are same"
	exit 8
fi




#remove .svn
pushd /tmp/vercmp/new > /dev/null
find -type d -iname "\.svn" -exec rm -rf {} \; 2>/dev/null
popd > /dev/null

pushd /tmp/vercmp/old > /dev/null
find -type d -iname "\.svn" -exec rm -rf {} \; 2>/dev/null
popd > /dev/null


# $1 subdir, such as "rootfs"
compare_dir()
{
	local fname

	echo ""
	echo "Begin checking $1.."

	pushd /tmp/vercmp/old/ > /dev/null
	find -type f -or -type l -or -type b -or -type c > /tmp/vercmp/oldfiles


	cd /tmp/vercmp/new/

	#get all file names
	find -type f -or -type l -or -type b -or -type c > /tmp/vercmp/files

	#compare files
	while read fname; do file_checknew "$fname" "$1" ; done < /tmp/vercmp/files


	#find deleted files
	while read fname; do file_checkrm "" "$fname" ; done < /tmp/vercmp/oldfiles

	mv /tmp/vercmp/diff/rm.txt /tmp/vercmp/diff/rm-$1.txt  2>/dev/null

	echo "Compress .."
	cd /tmp/vercmp/diff/$1
	err_check "Enter /tmp/vercmp/diff/$1 failed"

	tar cpf /tmp/vercmp/diff/$1.tar *
	err_check "compress to $1.tar.gz failed"

	#rm -rf /tmp/vercmp/diff/*

	popd > /dev/null

}

compare_dir "rootfs"

#compare_dir "usr"


echo ""
echo "Done. checked: $checked, changed: $changed, newadded: $newadded, Deled: $removed"
echo "others: $others"
echo "Total changed file size: $fsize"


fsize=0

#check usr.tar
#cursize=$(stat -c%s "/tmp/vercmp/diff/usr.tar")
#fsize=$(($fsize+$cursize))
#curtime=$(($cursize/217907))
#eta=$(($eta+$curtime))

#check rootfs.tar
cursize=$(stat -c%s "/tmp/vercmp/diff/rootfs.tar")
fsize=$(($fsize+$cursize))
curtime=$(($cursize/217907))
eta=$(($eta+$curtime))

#double eta
eta=$(($eta + $eta))

echo "flash write: $eta seconds"
echo "final size : $fsize"
echo "$fsize" > /tmp/vercmp/diff/size.txt

#compress all
echo "Compress all..."
pushd /tmp/vercmp > /dev/null
tar czf diff.tar.gz diff
err_check "Error! compress failed"
popd > /dev/null

mv /tmp/vercmp/diff.tar.gz "${verold}_to_${vernew}_eta_$eta.tar.gz"
ls -lh "${verold}_to_${vernew}_eta_$eta.tar.gz"

#add md5sum
mkdir diff_md5
md5sum "${verold}_to_${vernew}_eta_$eta.tar.gz" > "${verold}_to_${vernew}_eta_$eta.tar.gz.md5"
err_check "Error! md5sum failed"

#compress md5 diff
mv "${verold}_to_${vernew}_eta_$eta.tar.gz" diff_md5
mv "${verold}_to_${vernew}_eta_$eta.tar.gz.md5" diff_md5
tar czf "${verold}_to_${vernew}_eta_$eta.tar.gz" diff_md5
err_check "Error! compress md5 failed"

#rm -rf diff_md5

#rm -rf /tmp/vercmp



