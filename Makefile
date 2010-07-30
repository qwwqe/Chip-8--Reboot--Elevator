CC	= /usr/bin/gcc
CFLAGS	= -c -Wall -g `sdl-config --cflags`
LIBS	= `sdl-config --libs`

.PHONY: clean all

all: clean chip8

chip8: proc.o instr.o main.o
	$(CC) $(LIBS) proc.o main.o instr.o -o $@

main.o: main.c
	$(CC) $(CFLAGS) $(LIBS) $<

proc.o: proc.c
	$(CC) $(CFLAGS) $(LIBS) $<

instr.o: instr.c
	$(CC) $(CFLAGS) $(LIBS) $<

clean:
	rm -f chip8 *.o
