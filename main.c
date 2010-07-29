// "sip 8"

#include <stdio.h>
#include <stdlib.h>

#include "instr.h"
#include "proc.h"

int main(int argc, char **argv) {

 if(argc < 2) {
  fprintf(stderr, "Missing arguments\n");
  exit(EXIT_FAILURE);
 }

 mem_t *mem = (mem_t *)malloc(sizeof(mem_t));
 reg_t *reg = (reg_t *)malloc(sizeof(reg_t));

 cpu_t cpu = {
  {
   opcode0, opcode1, opcode2, opcode3,
   opcode4, opcode5, opcode6, opcode7,
   opcode8, opcode9, opcodea, opcodeb,
   opcodec, opcoded, opcodee, opcodef
  },
  mem, reg, NULL, 0
 };

 printf("Initialized CPU: %p\n", &cpu);

 if((cpu.mem->rom_size = read_rom(argv[1], &cpu.mem->mem)) == -1) {
  fprintf(stderr, "Failed to load rom: %s\n", argv[1]);
  exit(EXIT_FAILURE);
 }

 printf("Loaded rom: %d bytes\n", cpu.mem->rom_size);

 cpu.mem->pos = 512;
 while(cpu.mem->pos < (cpu.mem->rom_size - 2)) {
  printf("OPCODE: %.2x%.2x\n", cpu.mem->mem[cpu.mem->pos], cpu.mem->mem[cpu.mem->pos + 1]);
  cpu.fn[cpu.mem->mem[cpu.mem->pos] >> 4](&cpu);

  if(!cpu.advpc)
   cpu.mem->pos += 2;
  cpu.advpc = 0;
 } 

 exit(EXIT_SUCCESS);
}
