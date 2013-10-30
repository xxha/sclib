CC =	gcc -Wall -O2 -I.
RM = 	rm -rf

all: pcap_test simplesniffer arpsniffer tcpsyndos

pcap_test: pcap_test.o
	$(CC) -o pcap_test pcap_test.o -lpcap
pcap_test.o: pcap_test.c

simplesniffer: simplesniffer.o
	$(CC) -o simplesniffer simplesniffer.o -lpcap
simplesniffer.o: simplesniffer.c

arpsniffer: arpsniffer.o
	$(CC) -o arpsniffer arpsniffer.o -lpcap
arpsniffer.o: arpsniffer.c

tcpsyndos: tcpsyndos.o
	$(CC) -o tcpsyndos tcpsyndos.o -lpcap
tcpsyndos.o: tcpsyndos.c


clean:
	$(RM) *.o 
