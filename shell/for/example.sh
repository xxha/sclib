# !/bin/sh

i=1
function test_while(){
	i=1
	while [ $i ]
	do
		echo $i
		i=`expr $i + 1`
		if [ $i -ge 10 ]; then
			break
		fi
	done
}

function test_for(){
	i=1
	for ((i=1; i<=100;i++)); do
		echo $i
		if [ $i -ge 10 ]; then
			break
		fi
done
}

function test_continue(){
	i=1
	for i in $(seq 100); do
		if (( i%10==0 )); then
			echo $i
			continue
		fi
	done
}

echo "test_while..."
test_while

echo "test_for..."
test_for

echo "test_continue..."
test_continue
