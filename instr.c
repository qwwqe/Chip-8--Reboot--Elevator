// roof
#include "instr.h"
#include "proc.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void opcode0(cpu_t *cpu) {

 unsigned char n1, n2, n3, n4;	// nibbles
 n1 = 0x00;			// assumption
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

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

 unsigned char n1, n2, n3, n4;	// nibbles
 n1 = 0x01;			// assumption
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

 // 1NNN
 // jump to NNN (n2n3n4)

 cpu->mem->pos = (n2 << 8) | (n3 << 4) | n4;
 cpu->advpc = 1;

}
 
// 2NNN
// call chip8 sub at NNN (n2n3n4); 16 successive calls max (OR ARE THERE??????)
void opcode2(cpu_t *cpu) {

 unsigned char n1, n2, n3, n4;	// nibbles
 stack_t *crstk;

 n1 = 0x02;			// assumption
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

 
 if(cpu->stack == NULL) {	// we are in top level
    cpu->stack = (stack_t *)malloc(sizeof(stack_t));
    if(cpu->stack == NULL) {
     fprintf(stderr, "Failed to memory for stack\n");
     exit(EXIT_FAILURE);
    }
    cpu->stack->next = NULL;

    cpu->stack->adr = cpu->mem->pos;
    cpu->mem->pos = (n2 << 8) | (n3 << 4) | n4;
    printf("%x\n", cpu->mem->pos);
 } else {
    crstk = cpu->stack;
    while(crstk->next)
        crstk = crstk->next;

    crstk->next = (stack_t *)malloc(sizeof(stack_t));
    if(crstk->next == NULL) {
     fprintf(stderr, "Failed to memory for stack item\n");
     exit(EXIT_FAILURE);
    }
    crstk = crstk->next;
    crstk->next = NULL;

    crstk->adr = cpu->mem->pos;
    cpu->mem->pos = (n2 << 8) | (n3 << 4) | n4;
 }

 cpu->advpc = 1;

}

void opcode3(cpu_t *cpu) {

 unsigned char n1, n2, n3, n4;	// nibbles
 n1 = 0x03;			// assumption
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

 // 3XKK
 if(cpu->reg->v[n2] == cpu->mem->mem[cpu->mem->pos + 1]) {
  cpu->mem->pos += 2;
  cpu->advpc = 1;
 }
}

void opcode4(cpu_t *cpu) {

 unsigned char n1, n2, n3, n4;	// nibbles
 n1 = 0x04;			// assumption
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

 // 4XKK
 if(cpu->reg->v[n2] != cpu->mem->mem[cpu->mem->pos + 1]) {
  cpu->mem->pos += 2;
  cpu->advpc = 1;
 }
}

void opcode5(cpu_t *cpu) {

 unsigned char n1, n2, n3, n4;	// nibbles
 n1 = 0x05;			// assumption
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

 // 5XY0
 if(n4 == 0x00 && cpu->reg->v[n2] == cpu->reg->v[n3]) {
  cpu->mem->pos += 2;
  cpu->advpc = 1;
 }
}

void opcode6(cpu_t *cpu) {

 unsigned char n1, n2, n3, n4;	// nibbles
 n1 = 0x06;			// assumption
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

 // 6XKK
 cpu->reg->v[n2] = cpu->mem->mem[cpu->mem->pos + 1];
}

void opcode7(cpu_t *cpu) {

 unsigned char n1, n2, n3, n4;	// nibbles
 n1 = 0x07;			// assumption
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

 // 6XKK
 cpu->reg->v[n2] += cpu->mem->mem[cpu->mem->pos + 1];
}

// instructions beginning with 8
// 8XY0; 8XY1; 8XY2; 8XY3; 8XY4; 8XY5; 8XY6; 8XY7; 8XYE
void opcode8(cpu_t *cpu) {

    unsigned char n1, n2, n3, n4; // nibbles
    n1 = 0x08; // assumption..
    n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
    n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
    n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

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
// 9XY0, skip next intstruction if VX != VY
void opcode9(cpu_t *cpu) {

    unsigned char n1, n2, n3, n4; // nibbles
    n1 = 0x09; // assumption..
    n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
    n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
    n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

    if(cpu->mem->mem[n2] != cpu->mem->mem[n3]) {
        cpu->mem->pos += 2;
        cpu->advpc = 1;
    }

}

// instructions beginning with a
// ANNN, i = NNN
void opcodea(cpu_t *cpu) {

    unsigned char n1, n2, n3, n4; // nibbles
    n1 = 0x0A; // assumption..
    n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
    n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
    n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

    cpu->reg->i = (n2 << 8) | (n3 << 4) | n4;

}

// instructions beginning with b
// BNNN, jump to NNN + V0
void opcodeb(cpu_t *cpu) {

    unsigned char n1, n2, n3, n4; // nibbles
    n1 = 0x0B; // assumption..
    n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
    n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
    n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

    cpu->mem->pos = ((n2 << 8) | (n3 << 4) | n4) + cpu->reg->v[0x00];

}

// instructions beginning with c
// CXKK, VX = random number & KK
void opcodec(cpu_t *cpu) {

    unsigned char n1, n2, n3, n4; // nibbles
    n1 = 0x0C; // assumption..
    n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
    n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
    n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

    srand(time(NULL));
    cpu->reg->v[n2] = rand() ^ ((n3 << 4) | n4);

}

// instructions beginning with d
// DXYN, draw sprite at (VX, VY) of width 8 and height N starting from address stored in i.
//       If N is 0, height and width will both be 16
//       VF is set to 1 if screen pixels are changed from set to unset, 0 if they aren't
void opcoded(cpu_t *cpu) {

} 

// instructions beginning with e
// EX9E; EXA1
void opcodee(cpu_t *cpu) {

    unsigned char n1, n2, n3, n4; // nibbles
    n1 = 0x0E; // assumption..
    n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
    n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
    n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

    // EX9E, skip next instruction if key stored in VX is pressed
    
    // EXA1, skip next instruction if key stored in VX isn't pressed

} 

// instructions beginning with f
// FX07; FX0A; FX15; FX18; FX1E; FX29; FX33; FX55; FX65
void opcodef(cpu_t *cpu) {

    int t;
    unsigned char n1, n2, n3, n4; // nibbles
    n1 = 0x0F; // assumption..
    n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
    n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
    n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;    

    switch((n3 << 4) | n4) {
        case 0x07:    // FX07, VX = delay timer
            break;
        case 0x0A:    // FX0A, VX = awaited key press
            break;
        case 0x15:    // FX15, delay timer = VX
            break;
        case 0x18:    // FX18, sound timer = VX
            break;
        case 0x1E:    // FX1E, I = I + VX
            cpu->reg->i = cpu->reg->i + cpu->reg->v[n2];
            break;
        case 0x29:    // FX29, I = location for sprite character or something (5 * VX.....)
            cpu->reg->i = cpu->reg->v[n2] * 5;
            break;
        case 0x33:    // FX33, I, I + 1, and I + 2 = BCD representation of VX
            printf("PONG BCD PONG BCD\n");
            break;
        case 0x55:    // FX55, store V0->VX in memory starting at address in I
            for(t = 0; t <= n2; t++)
                cpu->mem->mem[cpu->reg->i + t] = cpu->reg->v[t];
            break;
        case 0x65:    // FX65, store memory starting at address in I in V0->VX
            for(t = 0; t <= n2; t++)
                cpu->reg->v[t] = cpu->mem->mem[cpu->reg->i + t];
            break;
    }

} 

