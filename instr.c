// roof
#include "instr.h"
#include "proc.h"

// instructions beginning with 8
void opcode8(cpu_t cpu*) {

    switch(cpu->mem->rom[cpu->mem->pos + 1] & 0x0F) { // with the 8xxx opcodes, the last four bits determine the behaviour
        case 0:             // 8XY0, VX = VY
            cpu->reg->v
    }

}

