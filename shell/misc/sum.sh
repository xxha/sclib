#echo sum = 1+2+...+49+50

i=0
sum=0
while [ $i -lt 50 ]; do
	let i+=1;
	let sum+=i;
done
echo "sum = $sum"
