// roof
#include "instr.h"
#include "proc.h"

#include <stdlib.h>

void opcode0(cpu_t *cpu) {
 unsigned char n1, n2, n3, n4;
 n1 = cpu->mem->rom[cpu->mem->pos] >> 1;
 n2 = cpu->mem->rom[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->rom[cpu->mem->pos + 1] >> 1;
 n4 = cpu->mem->rom[cpu->mem->pos + 1] & 0x0F;

 switch(n3) {
  case 0x0C:    // opcode: 00CN
                // scroll down: N (n2) lines
  break;
  case 0x0E:
   switch(n4) {
    case 0x00:  // opcode: 00E0
                // erase screen
    break;
    case 0x0E:  // opcode: 00EE
                // return from chip8 sub
    break;
   }
  break;
  case 0x0F:
   switch(n4) {
    case 0x0B:  // opcode: 00FB
                // scroll right: 4 pixels
    break;
    case 0x0C:  // opcode: 00FC
                // scroll left:  4 pixels
    break;
    case 0x0D:  // opcode: 00FD
                // quit the emulator
     exit(EXIT_SUCCESS);
    break;
    case 0x0E:  // opcode: 00FE
                // set chip8 graphics
    break;
    case 0x0F:  // opcode: 00FF
                // set schip graphics
    break;
   }
  break;
  default:      // 0NNN
                // call 1802 machine code program at NNN (n2n3n4)
  break;
 }

  cpu->mem->pos +=2;

}

void opcode1(cpu_t *cpu) {
 unsigned char n1, n2, n3, n4;
 n1 = cpu->mem->rom[cpu->mem->pos] >> 1;
 n2 = cpu->mem->rom[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->rom[cpu->mem->pos + 1] >> 1;
 n4 = cpu->mem->rom[cpu->mem->pos + 1] & 0x0F;

 // 1NNN
 // jump to NNN (n2n3n4)
}

void opcode2(cpu_t *cpu) {
 unsigned char n1, n2, n3, n4;
 n1 = cpu->mem->rom[cpu->mem->pos] >> 1;
 n2 = cpu->mem->rom[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->rom[cpu->mem->pos + 1] >> 1;
 n4 = cpu->mem->rom[cpu->mem->pos + 1] & 0x0F;

 // 2NNN
 // call chip8 sub at NNN (n2n3n4); 16 successive calls max
}

void opcode3(cpu_t *cpu) {
 unsigned char n1, n2, n3, n4;
 n1 = cpu->mem->rom[cpu->mem->pos] >> 1;
 n2 = cpu->mem->rom[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->rom[cpu->mem->pos + 1] >> 1;
 n4 = cpu->mem->rom[cpu->mem->pos + 1] & 0x0F;

 // 3XKK
 if(cpu->reg->v[n2] == cpu->mem->rom[cpu->mem->pos + 1]) {
  cpu->mem->pos += 2;
  cpu->advpc = 1;
 }
}

void opcode4(cpu_t *cpu) {
 unsigned char n1, n2, n3, n4;
 n1 = cpu->mem->rom[cpu->mem->pos] >> 1;
 n2 = cpu->mem->rom[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->rom[cpu->mem->pos + 1] >> 1;
 n4 = cpu->mem->rom[cpu->mem->pos + 1] & 0x0F;

 // 4XKK
 if(cpu->reg->v[n2] != cpu->mem->rom[cpu->mem->pos + 1]) {
  cpu->mem->pos += 2;
  cpu->advpc = 1;
 }
}

void opcode5(cpu_t *cpu) {
 unsigned char n1, n2, n3, n4;
 n1 = cpu->mem->rom[cpu->mem->pos] >> 1;
 n2 = cpu->mem->rom[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->rom[cpu->mem->pos + 1] >> 1;
 n4 = cpu->mem->rom[cpu->mem->pos + 1] & 0x0F;

 // 5XY0
 if(n4 == 0x00 && cpu->reg->v[n2] == cpu->reg->v[n3]) {
  cpu->mem->pos += 2;
  cpu->advpc = 1;
 }
}

void opcode6(cpu_t *cpu) {
 unsigned char n1, n2, n3, n4;
 n1 = cpu->mem->rom[cpu->mem->pos] >> 1;
 n2 = cpu->mem->rom[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->rom[cpu->mem->pos + 1] >> 1;
 n4 = cpu->mem->rom[cpu->mem->pos + 1] & 0x0F;

 // 6XKK
 cpu->reg->v[n2] = cpu->mem->rom[cpu->mem->pos + 1];
}

void opcode7(cpu_t *cpu) {
 unsigned char n1, n2, n3, n4;
 n1 = cpu->mem->rom[cpu->mem->pos] >> 1;
 n2 = cpu->mem->rom[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->rom[cpu->mem->pos + 1] >> 1;
 n4 = cpu->mem->rom[cpu->mem->pos + 1] & 0x0F;

 // 6XKK
 cpu->reg->v[n2] += cpu->mem->rom[cpu->mem->pos + 1];
}

// instructions beginning with 8
void opcode8(cpu_t *cpu) {

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

    unsigned char n1, n2, n3, n4; // nibbles
    n1 = 0x08; // assumption..
    n2 = cpu->mem->rom[cpu->mem->pos] & 0x0F;
    n3 = cpu->mem->rom[cpu->mem->pos + 1] >> 1;
    n4 = cpu->mem->rom[cpu->mem->pos + 1] & 0x0F;

    // 9XY0 - skip next instruction if VX != VY
    if(cpu->mem->rom[n2] != cpu->mem->rom[n3]) {
        cpu->mem->pos += 2;
        cpu->advpc = 1;
    }

}

 // instructions beginning with a
void opcodea(cpu_t *cpu) {}

// instructions beginning with b
void opcodeb(cpu_t *cpu) {}

// instructions beginning with c
void opcodec(cpu_t *cpu) {}

// instructions beginning with d
void opcoded(cpu_t *cpu) {} 

// instructions beginning with e
void opcodee(cpu_t *cpu) {} 

// instructions beginning with f
void opcodef(cpu_t *cpu) {} 

