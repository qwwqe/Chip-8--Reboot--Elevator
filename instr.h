// roof
#ifndef INSTR_H
#define INSTR_H

#include "proc.h"

void opcode0(cpu_t *cpu); // instructions beginning with 0
void opcode1(cpu_t *cpu); // instructions beginning with 1
void opcode2(cpu_t *cpu); // instructions beginning with 2
void opcode3(cpu_t *cpu); // instructions beginning with 3
void opcode4(cpu_t *cpu); // instructions beginning with 4
void opcode5(cpu_t *cpu); // instructions beginning with 5
void opcode6(cpu_t *cpu); // instructions beginning with 6
void opcode7(cpu_t *cpu); // instructions beginning with 7
void opcode8(cpu_t *cpu); // instructions beginning with 8
void opcode9(cpu_t *cpu); // instructions beginning with 9
void opcodea(cpu_t *cpu); // instructions beginning with a
void opcodeb(cpu_t *cpu); // instructions beginning with b
void opcodec(cpu_t *cpu); // instructions beginning with c
void opcoded(cpu_t *cpu); // instructions beginning with d
void opcodee(cpu_t *cpu); // instructions beginning with e
void opcodef(cpu_t *cpu); // instructions beginning with f

#endif
