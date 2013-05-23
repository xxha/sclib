#cross_compile
CROSS_COMPILE = arm-none-linux-gnueabi-

#compile tools
AS	=     $(CROSS_COMPILE)as
CC	=     $(CROSS_COMPILE)gcc
LD	=     $(CROSS_COMPILE)ld
AR	=     $(CROSS_COMPILE)ar
STRIP = $(CROSS_COMPILE)strip

all: readeeprom testeeprom

testeeprom: testeeprom.o libeeprom.o
	$(CC) -o testeeprom libeeprom.o testeeprom.o
testeeprom.o : testeeprom.c
	$(CC) -c testeeprom.c -o testeeprom.o

readeeprom : readeeprom.o libeeprom.o
	$(CC) -o readeeprom readeeprom.o libeeprom.o
readeeprom.o : readeeprom.c
	$(CC) -c readeeprom.c -o readeeprom.o


libeeprom.o : libeeprom.c
	$(CC) -c libeeprom.c -o libeeprom.o

.PHONY : clean

clean:
	rm -rf *.o
	rm -rf readeeprom testeeprom

