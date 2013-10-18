CC = 	gcc -Wall -O2 -I.
RM = 	rm -rf

all: get_sndbuf_rcvbuf set_sndbuf_rcvbuf gettype \
	get_sndtimeo_rcvtimeo set_sndtimeo_rcvtimeo

get_sndbuf_rcvbuf: get_sndbuf_rcvbuf.o
get_sndbuf_rcvbuf.o: get_sndbuf_rcvbuf.c

set_sndbuf_rcvbuf: set_sndbuf_rcvbuf.o
set_sndbuf_rcvbuf.o: set_sndbuf_rcvbuf.c

gettype: gettype.o
gettype.o: gettype.c

get_sndtimeo_rcvtimeo: get_sndtimeo_rcvtimeo.o
get_sndtimeo_rcvtimeo.o: get_sndtimeo_rcvtimeo.c

set_sndtimeo_rcvtimeo: set_sndtimeo_rcvtimeo.o
set_sndtimeo_rcvtimeo.o: set_sndtimeo_rcvtimeo.c


clean:
	$(RM) *.o
