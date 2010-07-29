// etc
#ifndef PROC_H
#define PROC_H

typedef struct _stack_t {

    unsigned int adr;
    struct _stack_t *next;

} stack_t;

typedef struct {

 unsigned char *rom;
 unsigned int 	pos;
 unsigned int	size;

} mem_t;

typedef struct {

 unsigned char v[0x0F];
 unsigned short int i;

} reg_t;

typedef struct _cpu_t {

 void (*fn[0x10])(struct _cpu_t *cpu);
 mem_t	*mem;
 reg_t	*reg;
 stack_t *stack; // root node
 int advpc; // advance program counter flag

} cpu_t;

int read_rom(const char *filename, unsigned char **rom); // load rom file into blah blah blah

#endif
