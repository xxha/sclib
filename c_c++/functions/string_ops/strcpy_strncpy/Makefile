CC =	gcc -Wall -O2 -I.
RM =	rm -rf

all: copy1 copy2 copy3 strcpy_unsigned_char

copy1: copy1.o
copy1.o: copy1.c

copy2: copy2.o
copy2.o: copy2.c


copy3: copy3.o
copy3.o: copy3.c

strcpy_unsigned_char: strcpy_unsigned_char.o
strcpy_unsigned_char.o: strcpy_unsigned_char.c

clean:
	$(RM)  *.o
