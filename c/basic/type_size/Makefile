CC =	gcc -Wall -O2 -I.
RM =	rm -rf

all: type_size pointer_size

type_size: type_size.o
type_size.o: type_size.c

pointer_size: pointer_size.o
pointer_size.o: pointer_size.c

clean:
	$(RM)  *.o
