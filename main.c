// "sip 8"

#include <stdio.h>
#include <stdlib.h>

#include "proc.h"

int main(int argc, char **argv) {

 mem_t *mem = (mem_t *)malloc(sizeof(mem_t));
 reg_t *reg = (reg_t *)malloc(sizeof(reg_t));

 cpu_t cpu;
 cpu.mem = mem;
 cpu.reg = reg;

 if((cpu.mem->size = read_rom(argv[1], &cpu.mem->rom)) < 0) {
  fprintf(stderr, "Failed to load rom: %s\n", argv[1]);
  exit(EXIT_FAILURE);
 }

 printf("Loaded rom: %d bytes\n", cpu.mem->size);

 int x;
 for(x = 0; x < cpu.mem->size; x += 2) {
  printf("byte #%03d: %02X%02X\n", x, cpu.mem->rom[x], cpu.mem->rom[x + 1]);
 }

 exit(EXIT_SUCCESS);
}
