CC=gcc
CROSS_COMPILE = /home/xxha/cross_compile/gcc-4.1.2-glibc-2.5-nptl-3/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-
all: version chipid

version: version.o
	$(CROSS_COMPILE)$(CC) version.o -o version
version.o: version.c
	$(CROSS_COMPILE)$(CC) -c version.c

chipid: chipid.o
	$(CROSS_COMPILE)$(CC) chipid.o -o chipid
chipid.o: chipid.c
	$(CROSS_COMPILE)$(CC) -c chipid.c
clean:
	rm *.o version chipid
	
