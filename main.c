// "sip 8"

#include <stdio.h>
#include <stdlib.h>

#include "proc.h"

typedef int (*opcode_function)(unsigned char, unsigned char, unsigned char);

int opcode6X(unsigned char second, unsigned char third, unsigned char fourth) {
 printf("%02X %02X %02X\n", second, third, fourth);
 return 1;
}

int main(int argc, char **argv) {

 unsigned char rom_size	= 0;
 unsigned char *rom	= NULL;

 opcode_function opcode_functions[16] = { NULL };
 opcode_functions[6] = &opcode6X;

 if((rom_size = read_rom(argv[1], &rom)) < 0) {
  fprintf(stderr, "Failed to load rom: %s\n", argv[1]);
  exit(EXIT_FAILURE);
 }

 printf("Loaded rom: %d bytes\n", rom_size);

 int x;
 for(x = 0; x < rom_size; x++) {
  printf("byte #%03d: %02X\n", x, rom[x] & 0xF0);
  (*opcode_functions[rom[x] >> 1])(rom[x] & 0x0F, rom[x+1] >> 1, rom[x+1] & 0x0F);
  x++;
 }

 exit(EXIT_SUCCESS);
}
