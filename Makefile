CC	= /usr/bin/gcc
DEBUG	= -Wall -g

.PHONY: clean all

all: clean chip8

chip8:
	$(CC) $(DEBUG) $< -o $@ main.c

clean:
	rm -f chip8 *.o
