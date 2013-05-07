#!/bin/bash

# output the odd number between 0-20.

i=0
j=0

while [ $i -lt 20 ]; do

	let j=$i%2;
	#echo "j = $j\n"
	#if [ $j == 1 ]; then
	if [ $j -eq 1 ]; then  # the same as above 
		echo -n "$i "
	fi
	let i+=1
done
echo ""
