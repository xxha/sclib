lsusb: lsusb.o
	#gcc -o lsusb lsusb.o -L/usr/lib -lusb
	gcc -o lsusb lsusb.o -lusb

lsusb.o: lsusb.c
	#gcc -c lsusb.c -L/usr/lib -lusb
	gcc -c lsusb.c -lusb
#default:
#	gcc lsusb.c -o lsusb -L/usr/lib -lusb
clean:
	rm *.o
