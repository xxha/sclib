
timer: timer_poll.o main.o
	g++ timer_poll.o main.o -g -o timer -lpthread

timer_poll.o: timer_poll.cpp timer_poll.h
	g++ timer_poll.cpp -c -o timer_poll.o -lpthread

#-lpthread for pthread_create()
main.o: main.cpp
	g++ main.cpp -c -o main.o -lpthread

clean:
	rm -rf *.o
