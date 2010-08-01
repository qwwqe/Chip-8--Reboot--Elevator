// "sip 8"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "SDL.h"

#include "instr.h"
#include "proc.h"
#include "video.h"

int main(int argc, char **argv) {

 mem_t		*mem;
 reg_t		*reg;
 SDL_Surface	*screen;

 Uint32 ticks;
    int frames;

 // temporarily unused
 // Uint32		sticks, cticks;

 if(argc < 2) {
  fprintf(stderr, "Error: Missing arguments\n");
  exit(EXIT_FAILURE);
 }

 mem = (mem_t *)malloc(sizeof(mem_t));
 reg = (reg_t *)malloc(sizeof(reg_t));
 if(mem == NULL || reg == NULL) {
  fprintf(stderr, "Error: Failed to allocate memory for MMU\n");
  exit(EXIT_FAILURE);
 }

 cpu_t cpu = {
  {
   opcode0, opcode1, opcode2, opcode3,
   opcode4, opcode5, opcode6, opcode7,
   opcode8, opcode9, opcodea, opcodeb,
   opcodec, opcoded, opcodee, opcodef
  },
  mem, reg, NULL, 0, 0, 0               // memory, registers, stack, program counter flag, delay timer, sound timer
 };

 printf("Initialized CPU: %p\n", (void *)&cpu);

 if((cpu.mem->rom_size = read_rom(argv[1], &cpu.mem->mem)) == -1) {
  fprintf(stderr, "Failed to load rom: %s\n", argv[1]);
  exit(EXIT_FAILURE);
 }

 printf("Loaded rom:      %d bytes\n", cpu.mem->rom_size);


 // init SDL
 if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0) {
  fprintf(stderr, "Error: Failed to initialize SDL: %s\n", SDL_GetError());
  exit(EXIT_FAILURE);
 }

 screen = SDL_SetVideoMode(SWIDTH, SHEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);

 if(!screen) {
  fprintf(stderr, "Error: Failed to initialize SDL screen surface: %s\n", SDL_GetError());
  exit(EXIT_FAILURE);
 }

 printf("Initialized SDL: %p\n", screen);

    printf("clocks per second: %ld\n", CLOCKS_PER_SEC);
    ticks = clock();
    frames = 0;

    cpu.mem->pos = ROM_LOC; // set memory pointer to rom location
    ticks = SDL_GetTicks();
    while(cpu.mem->pos >= ROM_LOC && cpu.mem->pos < (cpu.mem->rom_size + ROM_LOC)) {
        printf("OPCODE: %02X%02X\n", cpu.mem->mem[cpu.mem->pos], cpu.mem->mem[cpu.mem->pos + 1]);
        printf("\tPC: %x\n", cpu.mem->pos);
        cpu.fn[cpu.mem->mem[cpu.mem->pos] >> 4](&cpu);

        if(!cpu.advpc)
           cpu.mem->pos += 2;
          cpu.advpc = 0;

        // one timer decrement per instruction, so cpu must run at 60hz  
        if(cpu.delay_timer > 0)
            --cpu.delay_timer;
        if(cpu.sound_timer > 0)
            --cpu.sound_timer;

        // sleepy time
        SDL_Delay(1000 / CPU_SPEED);

        //if((clock() - ticks) / 
        //usleep((cpu.delay_timer * (1000 / 60)) * 1000); // obviously this isn't correct
        /*usleep(CPU_SPEED / 60);
        frames++;
        if(frames > 1000)
            break;*/
    } 

    printf("average framerate over %f seconds: %f\n", (float)clock() / (float)CLOCKS_PER_SEC, (float)frames / ((float)clock() / (float)CLOCKS_PER_SEC));
    printf("clock: %ld, clocks per second: %ld\n", clock(), CLOCKS_PER_SEC); // !?LKAPDIOFJSIODUFJ

 exit(EXIT_SUCCESS);
}
