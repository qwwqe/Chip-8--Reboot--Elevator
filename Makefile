CC	= /usr/bin/gcc
CFLAGS = -c -Wall -g

.PHONY: clean all

all: clean chip8

chip8: proc.o main.o
	$(CC) proc.o main.o -o $@

main.o: main.c
	$(CC) $(CFLAGS) $<

proc.o: proc.c
	$(CC) $(CFLAGS) $<

clean:
	rm -f chip8 *.o
