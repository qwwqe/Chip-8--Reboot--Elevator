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
 SDL_Event      event;

 Uint32 ticks;
    int frames;
    int running;
    int i, j;
    Uint8 colour;
    Uint8 *keys;
    int sdlkeys[0x10] = {
        SDLK_0, SDLK_1, SDLK_2, SDLK_3,
        SDLK_4, SDLK_5, SDLK_6, SDLK_7,
        SDLK_8, SDLK_9, SDLK_a, SDLK_b,
        SDLK_c, SDLK_d, SDLK_e, SDLK_f
    };

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

    // something about memset, blah blah blah
    for(i = 0; i < SHEIGHT; i++)
        for(j = 0; j < SWIDTH; j++)
            mem->vmem[i][j] = 0;

 cpu_t cpu = {      // memory, registers, stack, keys, program counter flag, delay timer, sound timer
  {
   opcode0, opcode1, opcode2, opcode3,
   opcode4, opcode5, opcode6, opcode7,
   opcode8, opcode9, opcodea, opcodeb,
   opcodec, opcoded, opcodee, opcodef
  },
  mem, reg, NULL, {}, 0, 0, 0               
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
    running = 1;
    while((cpu.mem->pos >= ROM_LOC && cpu.mem->pos < (cpu.mem->rom_size + ROM_LOC)) &&
          running) {
        printf("OPCODE: %02X%02X\n", cpu.mem->mem[cpu.mem->pos], cpu.mem->mem[cpu.mem->pos + 1]);
        printf("\tPC: %x\n", cpu.mem->pos);

        // event handling
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                    if(event.key.keysym.sym == SDLK_ESCAPE)
                        running = 0;
                    break;
                default:
                    break;
            }
        }
    
        // something about key states and there is something that could make this shorter
        keys = SDL_GetKeyState(NULL);
        for(i = 0; i < 0x10; i++)
            if(keys[sdlkeys[i]])
                cpu.keys[i] = 1;
            else
                cpu.keys[i] = 0;

        // cpu
        cpu.fn[cpu.mem->mem[cpu.mem->pos] >> 4](&cpu);

        // gfxz lul -- could probably wrap this in an if(opcode == 0xDXYN)....
        if(SDL_MUSTLOCK(screen)) {
            if(SDL_LockSurface(screen)) {
                fprintf(stderr, "shit, i don't know...: %s\n", SDL_GetError());
                exit(EXIT_FAILURE);
            }
        }

        for(i = 0; i < SHEIGHT; i++) {
            for(j = 0; j < SWIDTH; j++) {
                if(cpu.mem->vmem[i][j])
                    colour = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
                else
                    colour = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
              
                switch(screen->format->BytesPerPixel) {
                    case 1:     // 8 bit
                        *(Uint8 *)(screen->pixels + (i * screen->pitch) + (j * screen->format->BytesPerPixel)) = colour;
                        //printf("Drawing 8 bit\n");
                        break;
                    case 2:     // 16 bit
                        *(Uint16 *)(screen->pixels + (i * screen->pitch) + (j * screen->format->BytesPerPixel)) = colour;
                        //printf("Drawing 16 bit\n");
                        break;
                    case 3:     // 24 bit
                        // later...
                        //printf("Drawing 24 bit\b\n");
                        break;
                    case 4:
                        *(Uint32 *)(screen->pixels + i * screen->pitch + j * screen->format->BytesPerPixel) = colour;
                        //printf("x: %d, y: %d\n", j, i);
                        break;
                    default:
                        //printf("None?!\n");
                        break;
                }
            }
        }

        if(SDL_MUSTLOCK(screen))
            SDL_UnlockSurface(screen);

        SDL_Flip(screen);

        // one timer decrement per instruction, so cpu must run at 60hz  
        if(cpu.delay_timer > 0)
            --cpu.delay_timer;
        if(cpu.sound_timer > 0)
            --cpu.sound_timer;

        // advance program counter
        if(!cpu.advpc)
           cpu.mem->pos += 2;
          cpu.advpc = 0;

        // sleepy time
        SDL_Delay(1000 / CPU_SPEED);
        //SDL_Delay(250);

        //if((clock() - ticks) / 
        //usleep((cpu.delay_timer * (1000 / 60)) * 1000); // obviously this isn't correct
        /*usleep(CPU_SPEED / 60);
        frames++;
        if(frames > 1000)
            break;*/
    } 

    SDL_Quit();

    printf("average framerate over %f seconds: %f\n", (float)clock() / (float)CLOCKS_PER_SEC, (float)frames / ((float)clock() / (float)CLOCKS_PER_SEC));
    printf("clock: %ld, clocks per second: %ld\n", clock(), CLOCKS_PER_SEC); // !?LKAPDIOFJSIODUFJ

 exit(EXIT_SUCCESS);
}
