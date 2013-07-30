#!/bin/sh
for i in `ls /dev/mydev*`; do
    cat $i > /tmp/`basename $i` & 
done
exit 0

