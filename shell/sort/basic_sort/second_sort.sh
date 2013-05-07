#!/bin/bash
x=(12 4 16 8 12 24 3 17);
for((i=0;i<${#x[@]};i++))
do
        echo -n " old x = ${x[$i]} "
done
echo
for((i=0;i<${#x[@]};i++))
do
        n=i
        min=${x[$i]}
        for((j=i+1;j<${#x[@]};j++))
        do
                if [[ ${x[$j]} -lt $min ]]
                then
                        n=$j
                        min=${x[$j]}
                fi
        done
        t=${x[$n]}
        x[$n]=${x[$i]}
        x[$i]=$t
done
for((i=0;i<${#x[@]};i++))
do
        echo -n "new x = ${x[$i]} "
done
echo
