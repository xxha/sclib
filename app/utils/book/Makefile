CC = 	gcc -Wall -O2 -I.
RM = 	rm -rf

all: book_manage char_replace test2

book_manage: book_manage.o
book_manage.o: book_manage.c

char_replace: char_replace.o
char_replace.o: char_replace.c

test2: test2.o
test2.o: test2.c

test: test.o
	g++ test.o -o test
test.o: test.cpp
	g++ test.cpp

clean:
	$(RM) *.o
