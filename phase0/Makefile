CC := gcc
CFLAGS := -Wall -Wextra -pedantic

TARGETS := assignip hostnr showip socket bind connect accept

all: $(TARGETS)

bind: bind.c 
	$(CC) $(CFLAGS) bind.c -o bind

accept: accept.c 
	$(CC) $(CFLAGS) accept.c -o accept

connect: connect.c 
	$(CC) $(CFLAGS) connect.c -o connect

assignip: assignip.c
	$(CC) $(CFLAGS) assignip.c -o assignip

hostnr: hostnr.c
	$(CC) $(CFLAGS) hostnr.c -o hostnr

showip: showip.c
	$(CC) $(CFLAGS) showip.c -o showip

socket: socket.c
	$(CC) $(CFLAGS) socket.c -o socket

clean:
	rm -f $(TARGETS) a.out

rebuild: clean all

.PHONY: all clean rebuild
