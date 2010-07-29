CC	= /usr/bin/gcc
DEBUG	= -Wall -g

.PHONY: clean all

all: clean chip8

chip8: proc.o
	$(CC) $(DEBUG) $< -o $@ main.c

proc.o: proc.c proc.h
	$(CC) $(DEBUG) $< -o $@

clean:
	rm -f chip8 *.o
