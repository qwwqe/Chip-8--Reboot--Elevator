// etc
#ifndef PROC_H
#define PROC_H

#define MEM_SIZE    0x1000
#define ROM_LOC	    0x0200
#define TIMER_SPEED 60      // in hertz
#define CPU_SPEED   TIMER_SPEED     // in hertz, or... "fps"
#define SWIDTH 64
#define SHEIGHT 32

typedef struct _stack_t {

 unsigned int adr;
 struct _stack_t *next;

} stack_t;

typedef struct {

 unsigned char *mem;
 unsigned int vmem[SHEIGHT][SWIDTH]; // just ones and zeros
 unsigned int pos;
 unsigned int rom_size;

} mem_t;

typedef struct {

 unsigned char v[0x10];
 unsigned short int i;

} reg_t;

typedef struct _cpu_t {

 void (*fn[0x10])(struct _cpu_t *cpu);
 mem_t	*mem;
 reg_t	*reg;
 stack_t *stack;	// root node
 int keys[0x10];    // key states
 int advpc;		// advance program counter flag
 int delay_timer;
 int sound_timer;
} cpu_t;

int read_rom(const char *filename, unsigned char **mem); // load rom file into memory

#endif
