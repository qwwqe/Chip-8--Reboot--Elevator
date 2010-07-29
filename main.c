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
  mem,
  reg,
  0    
 };

 printf("Initialized CPU: %p\n", &cpu);

 if((cpu.mem->size = read_rom(argv[1], &cpu.mem->rom)) == -1) {
  fprintf(stderr, "Failed to load rom: %s\n", argv[1]);
  exit(EXIT_FAILURE);
 }

 printf("Loaded rom: %d bytes\n", cpu.mem->size);

 cpu.mem->pos = 0;
 while(1) {
  //cpu.fn[cpu.mem->rom[cpu.mem->pos] >> 1](&cpu);

  if(!cpu.advpc)
   cpu.mem->pos += 2;
  cpu.advpc = 0;
 } 
 
/*
 * int x;
 * for(x = 0; x < cpu.mem->size; x += 2) {
 *  printf("byte #%03d: %02X%02X\n", x, cpu.mem->rom[x], cpu.mem->rom[x + 1]);
 * }
 *
 */

 exit(EXIT_SUCCESS);
}
