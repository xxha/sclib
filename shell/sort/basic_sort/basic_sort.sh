#!/bin/bash

exchange()
{
	local tmp=${array[$1]}
	array[$1]=${array[$2]}
	array[$2]=$tmp

}

declare -a array

array=(10 8 9 7 5 6 4 3 1 2 0) 

echo "array length = ${#array[@]}"
echo "old array = ${array[*]}"

num=${#array[@]}

for (( last = $num - 1 ; last > 0 ; last-- ))
do
	for (( i = 0 ; i < last ; i++ ))
	do
		if [[ ${array[$i]} -ge ${array[$((i+1))]} ]] ;
		#if [[ ${array[$i]} -lt ${array[$((i+1))]} ]] ;
		then
			exchange $i $((i+1))
		fi
	done
done

echo ""
echo "new array = ${array[*]}"

