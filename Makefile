CC	= /usr/bin/gcc
CFLAGS	= -c -Wall -g `sdl-config --cflags`
LIBS	= `sdl-config --libs`

.PHONY: clean all

all: clean chip8

chip8: proc.o instr.o main.o
	$(CC) proc.o main.o instr.o -o $@

main.o: main.c
	$(CC) $(LIBS) $(CFLAGS) $<

proc.o: proc.c
	$(CC) $(LIBS) $(CFLAGS) $<

instr.o: instr.c
	$(CC) $(LIBS) $(CFLAGS) $<

clean:
	rm -f chip8 *.o
