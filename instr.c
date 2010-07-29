// roof
#include "instr.h"
#include "proc.h"

// instructions beginning with 8
void opcode8(cpu_t * cpu) {

    unsigned char n1, n2, n3, n4; // nibbles
    n1 = 0x08; // assumption..
    n2 = cpu->mem->rom[cpu->mem->pos] & 0x0F;
    n3 = cpu->mem->rom[cpu->mem->pos + 1] >> 1;
    n4 = cpu->mem->rom[cpu->mem->pos + 1] & 0x0F;

    switch(n4) { // with the 8xxx opcodes, the last four bits determine the behaviour
        case 0x00:              // 8XY0, VX = VY
            cpu->reg->v[n2] = cpu->reg->v[n3];
            break;
        case 0x01:              // 8XY1, VX = VX | VY
            cpu->reg->v[n2] = cpu->reg->v[n2] | cpu->reg->v[n3];
            break;
        case 0x02:              // 8XY2, VX = VX & VY
            cpu->reg->v[n2] = cpu->reg->v[n2] & cpu->reg->v[n3];
            break;
        case 0x03:              // 8XY3, VX = VX ^ VY
            cpu->reg->v[n2] = cpu->reg->v[n2] ^ cpu->reg->v[n3];
            break;
        case 0x04:              // 8XY4, VX = VX + VY, VF = carry
            if((cpu->reg->v[n2] + cpu->reg->v[n3]) > 0xFF) { // carry
                cpu->reg->v[n2] = (cpu->reg->v[n2] + cpu->reg->v[n3]) & 0xFF;
                cpu->reg->v[0x0F] = 0x01;
            } else { // no carry
                cpu->reg->v[n2] = cpu->reg->v[n2] + cpu->reg->v[n3];
                cpu->reg->v[0x0F] = 0x00;
            }
            break;
        case 0x05:              // 8XY5, VX = VX - VY, VF = not borrow
            if(cpu->reg->v[n2] >= cpu->reg->v[n3]) { // no borrow - set flag
                cpu->reg->v[n2] = cpu->reg->v[n2] - cpu->reg->v[n3];
                cpu->reg->v[0x0F] = 0x01;
            } else { // borrow - don't set flag
                cpu->reg->v[n2] = cpu->reg->v[n3] - cpu->reg->v[n2];
                cpu->reg->v[0x0F] = 0x00;
            }
        case 0x06:              // 8XY6, VX = VX >> 1, VF = carry
            cpu->reg->v[0x0F] = cpu->reg->v[n2] & 0x01;
            cpu->reg->v[n2]   = cpu->reg->v[n2] >> 1;
            break;
        case 0x07:              // 8XY7, VX = VY - VX, VF = not borrow
            if(cpu->reg->v[n3] >= cpu->reg->v[n2]) { // no borrow - set flag
                cpu->reg->v[n2] = cpu->reg->v[n3] - cpu->reg->v[n2];
                cpu->reg->v[0x0F] = 0x01;
            } else { // borrow - don't set flag
                cpu->reg->v[n2] = cpu->reg->v[n2] - cpu->reg->v[n3];
                cpu->reg->v[0x0F] = 0x00;
            }
            break;
        case 0x0E:              // 8XYE, VX = VX << 1, VF = carry
            cpu->reg->v[0x0F] = cpu->reg->v[n2] & 0x80;
            cpu->reg->v[n2]   = cpu->reg->v[n2] << 1;
            break;
        default:
            break;
    }

}

// instructions beginning with 9
void opcode9(cpu_t *cpu) {

    

}

