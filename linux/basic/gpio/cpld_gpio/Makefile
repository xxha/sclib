CROSS_COMPILE=arm-none-linux-gnueabi-
cc = $(CROSS_COMPILE)gcc

all: test_gpio

test_gpio: test_gpio.o cpld_gpio.o
	$(cc) -o test_gpio test_gpio.o cpld_gpio.o
test_gpio.o: test_gpio.c
	$(cc) -c test_gpio.c

cpld_gpio.o: cpld_gpio.c cpld_gpio.h
	$(cc) -c cpld_gpio.c cpld_gpio.h

clean:
	rm -rf *.o
	rm -rf testgpio
