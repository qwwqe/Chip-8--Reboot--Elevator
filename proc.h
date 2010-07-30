// etc
#ifndef PROC_H
#define PROC_H

#define MEM_SIZE 0x1000
#define ROM_LOC	 0x0200

typedef struct _stack_t {

 unsigned int adr;
 struct _stack_t *next;

} stack_t;

typedef struct {

 unsigned char *mem;
 unsigned int pos;
 unsigned int rom_size;

} mem_t;

typedef struct {

 unsigned char v[0x0F];
 unsigned short int i;

} reg_t;

typedef struct _cpu_t {

 void (*fn[0x10])(struct _cpu_t *cpu);
 mem_t	*mem;
 reg_t	*reg;
 stack_t *stack;	// root node
 int advpc;		// advance program counter flag
 int delay_timer;
 int snd_timer;
} cpu_t;

int read_rom(const char *filename, unsigned char **mem); // load rom file into memory

#endif
