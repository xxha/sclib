CC =	gcc -Wall -O2 -I.
RM =	rm -rf

all: file_open1 file_open2

file_open1: file_open1.o
file_open1.o: file_open1.c

file_open2: file_open2.o
file_open2.o: file_open2.c

hello:hello.o
hello.o:hello.c

test_system_fprintf_fopen:test_system_fprintf_fopen.o
test_system_fprintf_fopen.o:test_system_fprintf_fopen.c

write_lseek_read:write_lseek_read.o
write_lseek_read.o:write_lseek_read.c

clean:
	$(RM)  *.o
