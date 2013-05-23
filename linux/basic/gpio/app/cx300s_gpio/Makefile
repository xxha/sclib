CC=gcc
CROSS_COMPILE = /home/xxha/cross_compile/gcc-4.1.2-glibc-2.5-nptl-3/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-

test_gpio: testgpio.o gpio.o
	$(CROSS_COMPILE)$(CC) testgpio.o gpio.o -o test_gpio

testgpio.o: testgpio.c
	$(CROSS_COMPILE)$(CC) -c testgpio.c

gpio.o: gpio.c gpio.h
	$(CROSS_COMPILE)$(CC) -c gpio.c gpio.h

clean:
	rm *.o test_gpio
	
