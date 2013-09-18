all: demo log va_list test

demo: demo.c
	gcc demo.c -o demo
log: log.c
	gcc log.c -o log
va_list: va_list.cpp
	g++ va_list.cpp -o va_list

test: test.c
	gcc test.c -o test

clean:
	rm -rf *.o demo log va_list test
