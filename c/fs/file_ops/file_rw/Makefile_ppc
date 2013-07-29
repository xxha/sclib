#cross_compile stuff
CROSS_COMPILE = powerpc-860-linux-gnu-

#compile tools
AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
AR = $(CROSS_COMPILE)ar
STRIP = $(CROSS_COMPILE)strip

CFLAGS = -O2 -pipe -fsigned-char

all: file_test file_rw file_veex

file_test: file_test.o
	$(CC) -o file_test file_test.o

file_test.o: file_test.c
	$(CC) -c $(CFLAGS) file_test.c -o file_test.o

file_rw: file_rw.o
	$(CC) -o file_rw file_rw.o

file_rw.o: file_rw.c
	$(CC) -c $(CFLAGS) file_rw.c -o file_rw.o

file_veex: file_veex.o
	$(CC) -o file_veex file_veex.o

file_veex.o: file_veex.c
	$(CC) -c $(CFLAGS) file_veex.c -o file_veex.o

.PHONY : clean

clean:
	rm -rf *.o file_test file_rw
