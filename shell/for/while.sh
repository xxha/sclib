#!/bin/bash
clear

i=1
while (($i<100))
do
	if (($i%3==0))
	then
		echo $i
	fi
	i=$(($i+1))
done
