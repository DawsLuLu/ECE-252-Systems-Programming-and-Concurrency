all = curl curlComplete \
	  perror getopt \
	  catfile catfileComplete \
	  fileio \
	  fork forkClean \
	  funcPtr \
	  signal msgq \
	  sockserver sockclient \
	  bigsend bigrecv \
	  pipe pipeComplete \
	  shmem \
	  memerrors mmap pc rendezvous threrrors

CC=gcc
CFLAGS=-g -std=c99 -D_GNU_SOURCE
LIBS=

default: $(all)

fileio: fileio.c input.dat
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
input.dat:
	@echo "1 2\n3 4 5 6" > $@

mmap: mmap.c example.txt
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

example.txt:
	@echo "This is sample text." > $@

curl curlComplete: LIBS+=-lcurl

pc rendezvous threrrors: LIBS+=-pthread

%: %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	rm -f $(all) example.txt input.dat
