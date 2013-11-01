CC =	gcc -Wall -O2 -I.
RM =	rm -rf

all: unsigned_char int_to_char mac_char

unsigned_char: unsigned_char.o
unsigned_char.o: unsigned_char.c

int_to_char: int_to_char.o
int_to_char.o: int_to_char.c

mac_char: mac_char.o
mac_char.o: mac_char.c

clean:
	$(RM)  *.o
