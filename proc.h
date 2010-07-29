// etc
#ifndef PROC_H
#define PROC_H

typedef struct {

 unsigned char *rom;
 unsigned int 	pos;
 unsigned int	size;

} mem_t;

typedef struct {

 unsigned char v[0x0F];
 unsigned short int i;

} reg_t;

typedef struct {

// instr	fn[0x0F];
 mem_t	*mem;
 reg_t	*reg;

} cpu_t;

// typedef int (*instr)(cpu_t *cpu);

int read_rom(const char *filename, unsigned char **rom); // load rom file into blah blah blah

#endif
