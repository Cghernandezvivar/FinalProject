CC=gcc
CFLAGS=-Wall -Wextra -O2 -g
LDLIBS=

OBJS=

all: sender recv

sender: sender.o $(OBJS)
	$(CC) -o $@ $^ $(LDLIBS)

recv: recv.o $(OBJS)
	$(CC) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f sender recv *.o
