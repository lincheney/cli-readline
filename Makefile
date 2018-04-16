CFLAGS=-O3 -Wall
LDFLAGS=-lreadline
PROG=cli-readline
OBJS=cli-readline.o

$(PROG): $(OBJS)
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f *.o *~ $(PROG)
