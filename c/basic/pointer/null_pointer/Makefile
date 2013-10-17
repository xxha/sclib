CC =	gcc -Wall -O2 -I.
RM =	rm -rf

all: pointer_assign null_pointer

pointer_assign: pointer_assign.o
pointer_assign.o: pointer_assign.c

null_pointer: null_pointer.o
null_pointer.o: null_pointer.c

clean:
	$(RM)  *.o
