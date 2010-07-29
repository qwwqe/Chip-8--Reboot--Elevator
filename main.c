// "sip 8"

#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"

#include "instr.h"
#include "proc.h"
#include "video.h"

int main(int argc, char **argv) {


    mem_t *mem;
    reg_t *reg;
    SDL_Surface *screen;
    Uint32 sticks, cticks;

 if(argc < 2) {
  fprintf(stderr, "error: Missing arguments\n");
  exit(EXIT_FAILURE);
 }

  mem = (mem_t *)malloc(sizeof(mem_t));
  reg = (reg_t *)malloc(sizeof(reg_t));
 if(mem == NULL || reg == NULL) {
  fprintf(stderr, "error: Failed to allocate memory for MMU\n");
  exit(EXIT_FAILURE);
 }

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

 printf("Loaded rom:      %d bytes\n", cpu.mem->rom_size);

    // init SDL
    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Failed to init sdl crap.. %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    screen = SDL_SetVideoMode(SWIDTH, SHEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if(!screen) {
        fprintf(stderr, "Failed to init sdl crap.. %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    cpu.mem->pos = 0x0200;
  while(cpu.mem->pos >= 0x0200 && cpu.mem->pos < (cpu.mem->rom_size + 0x0200)) {
        printf("OPCODE: %02X%02X\n", cpu.mem->mem[cpu.mem->pos], cpu.mem->mem[cpu.mem->pos + 1]);
        printf("\tPC: %x\n", cpu.mem->pos);
        cpu.fn[cpu.mem->mem[cpu.mem->pos] >> 4](&cpu);

        if(!cpu.advpc)
            cpu.mem->pos += 2;
        cpu.advpc = 0;
 } 

 exit(EXIT_SUCCESS);
}
