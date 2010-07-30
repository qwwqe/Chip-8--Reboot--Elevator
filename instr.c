// roof
#include "instr.h"
#include "proc.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void opcode0(cpu_t *cpu) {		// opcodes starting with 0: 0NNN, 00CN, 00E0, 00EE, 00FB, 00FC, 00FD, 00FF

 unsigned char n1, n2, n3, n4;	// nibbles
 n1 = 0x00;						// assumption
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

 stack_t *crstk, *pvstk;

 switch((n3 << 4) | n4) {	// determine behavior via last two nibbles

  case 0xE0:		// opcode: 00E0, erase the screen
   // stub
   break;

  case 0xEE:		// opcode: 00EE, return from chip8 sub
   if(cpu->stack == NULL) {
    fprintf(stderr, "error: Stack not found!\n");
    exit(EXIT_FAILURE);
   }
   crstk = pvstk = cpu->stack;
   
   while(crstk->next) {		// crstk will be the last node, pvstk will be the second last
    crstk = crstk->next;
    if(crstk->next)
     pvstk = crstk;
   }
        
   cpu->mem->pos = crstk->adr;
   free(crstk);
   pvstk->next = NULL;
   break;

  case 0xFB:		// opcode: 00FB, scroll right: 4 pixel
   // stub
   break;

  case 0xFC:		// opcode: 00FC, scroll left: 4 pixel
   // stub
   break;

  case 0xFD:		// opcode: 00FD, quit the emulator
   exit(EXIT_SUCCESS);
   break;

  case 0xFE:		// opcode: 00FE, set chip8 graphics
   // stub
   break;

  case 0xFF:		// opcode: 00FF, set schip graphics
   // stub
   break;

  default:
   if(n3 == 0x0C) {	// opcode: 00CN, scroll down N (n4) lines
   // stub
   } else {			// opcode: 0NNN, call 1802 machine code program at NNN (n2n3n4)
   // stub
   }
   break;

 }

 cpu->mem->pos +=2;
 cpu->advpc = 1;

}

void opcode1(cpu_t *cpu) {		// opcode: 1NNN, jump to NNN (n2n3n4)

 unsigned char n1, n2, n3, n4;	// nibbles
 n1 = 0x01;						// assumption
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;


 cpu->mem->pos = (n2 << 8) | (n3 << 4) | n4;
 cpu->advpc = 1;

}
 
void opcode2(cpu_t *cpu) {		// opcode: 2NNN, call chip8 at NNN (n2n3n4); maximum of 16 successive calls (OR ARE THERE????)

 unsigned char n1, n2, n3, n4;	// nibbles
 n1 = 0x02;						// assumption
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

 stack_t *crstk;
 
 if(cpu->stack == NULL) {		// we are now in top level

  cpu->stack = (stack_t *)malloc(sizeof(stack_t));

  if(cpu->stack == NULL) {
   fprintf(stderr, "Failed to allocate memory for stack\n");
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
   fprintf(stderr, "Failed to allocate memory for stack item\n");
   exit(EXIT_FAILURE);
  }

  crstk = crstk->next;
  crstk->next = NULL;

  crstk->adr = cpu->mem->pos;
  cpu->mem->pos = (n2 << 8) | (n3 << 4) | n4;

 }

 cpu->advpc = 1;

}

void opcode3(cpu_t *cpu) {		// opcode: 3XKK, skip next instruction if VX == KK

 unsigned char n1, n2, n3, n4;	// nibbles
 n1 = 0x03;						// assumption
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

 if(cpu->reg->v[n2] == ((n3 << 4) | n4)) {
  cpu->mem->pos += 2;
  cpu->advpc = 1;
 }
}

void opcode4(cpu_t *cpu) {		// opcode: 4XKK, skip next instruction if VX != KK

 unsigned char n1, n2, n3, n4;	// nibbles
 n1 = 0x04;						// assumption
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

 if(cpu->reg->v[n2] != cpu->mem->mem[cpu->mem->pos + 1]) {
  cpu->mem->pos += 2;
  cpu->advpc = 1;
 }
}

void opcode5(cpu_t *cpu) {		// opcode: 5XY0, skip next instruction if VX == VY

 unsigned char n1, n2, n3, n4;	// nibbles
 n1 = 0x05;						// assumption
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

 if(n4 == 0x00 && cpu->reg->v[n2] == cpu->reg->v[n3]) {
  cpu->mem->pos += 2;
  cpu->advpc = 1;
 }
}

void opcode6(cpu_t *cpu) {		// opcode: 6XKK, VX = KK

 unsigned char n1, n2, n3, n4;	// nibbles
 n1 = 0x06;						// assumption
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

 cpu->reg->v[n2] = (n3 << 4) | n4;
}

void opcode7(cpu_t *cpu) {		// opcode: 7XKK, VX = VX + KK

 unsigned char n1, n2, n3, n4;	// nibbles
 n1 = 0x07;						// assumption
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

 cpu->reg->v[n2] += (n3 << 4) | n4;
}

void opcode8(cpu_t *cpu) {		// opcode starting with 8: 8XY0, 8XY1, 8XY2, 8XY3, 8XY4, 8XY5, 8XY6, 8XY7, 8XYE

 unsigned char n1, n2, n3, n4;	// nibbles
 n1 = 0x08;						// assumption
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

    switch(n4) {	// with the 8xxx opcodes, the last four bits determine the behaviour

        case 0x00:	// opcode: 8XY0, VX = VY
            cpu->reg->v[n2] = cpu->reg->v[n3];
            break;

        case 0x01:	// opcode: 8XY1, VX = VX | VY
            cpu->reg->v[n2] = cpu->reg->v[n2] | cpu->reg->v[n3];
            break;

        case 0x02:	// opcode: 8XY2, VX = VX & VY
            cpu->reg->v[n2] = cpu->reg->v[n2] & cpu->reg->v[n3];
            break;

        case 0x03:	// opcode: 8XY3, VX = VX ^ VY
            cpu->reg->v[n2] = cpu->reg->v[n2] ^ cpu->reg->v[n3];
            break;

        case 0x04:	// opcode: 8XY4, VX = VX + VY, VF = carry
            if((cpu->reg->v[n2] + cpu->reg->v[n3]) > 0xFF) {
				// carry
                cpu->reg->v[n2] = (cpu->reg->v[n2] + cpu->reg->v[n3]) & 0xFF;
                cpu->reg->v[0x0F] = 0x01;
            } else {
				// no carry
                cpu->reg->v[n2] = cpu->reg->v[n2] + cpu->reg->v[n3];
                cpu->reg->v[0x0F] = 0x00;
            }
            break;

        case 0x05:	// opcode: 8XY5, VX = VX - VY, VF = not borrow
            if(cpu->reg->v[n2] >= cpu->reg->v[n3]) {
				// no borrow, set flag
                cpu->reg->v[n2] = cpu->reg->v[n2] - cpu->reg->v[n3];
                cpu->reg->v[0x0F] = 0x01;
            } else {
				// borrow, don't set flag
                cpu->reg->v[n2] = cpu->reg->v[n3] - cpu->reg->v[n2];
                cpu->reg->v[0x0F] = 0x00;
            }

        case 0x06:	// opcode: 8XY6, VX = VX >> 1, VF = carry
            cpu->reg->v[0x0F] = cpu->reg->v[n2] & 0x01;
            cpu->reg->v[n2]   = cpu->reg->v[n2] >> 1;
            break;

        case 0x07:	// opcode: 8XY7, VX = VY - VX, VF = not borrow
            if(cpu->reg->v[n3] >= cpu->reg->v[n2]) {
				// no borrow, set flag
                cpu->reg->v[n2] = cpu->reg->v[n3] - cpu->reg->v[n2];
                cpu->reg->v[0x0F] = 0x01;
            } else {
				// borrow, don't set flag
                cpu->reg->v[n2] = cpu->reg->v[n2] - cpu->reg->v[n3];
                cpu->reg->v[0x0F] = 0x00;
            }
            break;

        case 0x0E:	// opcode: 8XYE, VX = VX << 1, VF = carry
            cpu->reg->v[0x0F] = cpu->reg->v[n2] & 0x80;
            cpu->reg->v[n2]   = cpu->reg->v[n2] << 1;
            break;

        default:	// unnecssary?
            break;
    }

}

void opcode9(cpu_t *cpu) {		// opcode: 9XY0, skip next instruction if VX != VY

 unsigned char n1, n2, n3, n4;	// nibbles
 n1 = 0x09;						// assumption
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

 if(n4 == 0x00 && cpu->mem->mem[n2] != cpu->mem->mem[n3]) {
  cpu->mem->pos += 2;
  cpu->advpc = 1;
 }

}

void opcodea(cpu_t *cpu) {		// opcode: ANNN, i = NNN (n2n3n4)

 unsigned char n1, n2, n3, n4;	// nibbles
 n1 = 0x0A;						// assumption
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

 cpu->reg->i = (n2 << 8) | (n3 << 4) | n4;

}

void opcodeb(cpu_t *cpu) {		// opcode: BNNN, jump to NNN (n2n3n4) + V0

 unsigned char n1, n2, n3, n4;	// nibbles
 n1 = 0x0B;						// assumption
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

 cpu->mem->pos = ((n2 << 8) | (n3 << 4) | n4) + cpu->reg->v[0x00];
 printf("Jumping to byte %d in memory: %p \n", cpu->mem->pos, &cpu->mem->mem[cpu->mem->pos]);

}

void opcodec(cpu_t *cpu) {		// opcode: CXKK, VX = random number & KK

 unsigned char n1, n2, n3, n4;	// nibbles
 n1 = 0x0C;						// assumption..
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

 srand(time(NULL));
 cpu->reg->v[n2] = rand() ^ ((n3 << 4) | n4);

}

void opcoded(cpu_t *cpu) {		// opcode: DXYN, draw sprite at (VX, VY) starting from address i in memory
								// if N = 0: 16x16 sprite
								// else: height = N, width = 8
								// VF = 1 if screen pixels are changed from set to unset, 0 if they aren't
 // stub
} 

void opcodee(cpu_t *cpu) {		// opcodes starting with E: EX9E, EXA1

 unsigned char n1, n2, n3, n4;	// nibbles
 n1 = 0x0E;						// assumption
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;

 switch((n3 << 4) | n4) {

  case 0x9E:	// opcode: EX9E, skip next instruction if key stored in VX is pressed
   // stub
   break;

  case 0xA1:	// opcode: EXA1, skip next instruction if key stored in VX isn't pressed
   // stub
   break;

 }

} 

void opcodef(cpu_t *cpu) {		// opcodes starting with F: FX07, FX0A, FX15, FX18, FX1E, FX29, FX33, FX55, FX65

 int t;
 unsigned char n1, n2, n3, n4;	// nibbles
 n1 = 0x0F;						// assumption
 n2 = cpu->mem->mem[cpu->mem->pos] & 0x0F;
 n3 = cpu->mem->mem[cpu->mem->pos + 1] >> 4;
 n4 = cpu->mem->mem[cpu->mem->pos + 1] & 0x0F;    

    switch((n3 << 4) | n4) {

        case 0x07:    // opcode: FX07, VX = delay timer
            // stub
            break;

        case 0x0A:    // opcode: FX0A, VX = awaited key press
            // stub
            break;

        case 0x15:    // opcode: FX15, delay timer = VX
            // stub
            break;

        case 0x18:    // opcode: FX18, sound timer = VX
            // stub
            break;

        case 0x1E:    // opcode: FX1E, I = I + VX
            cpu->reg->i = cpu->reg->i + cpu->reg->v[n2];
            break;

        case 0x29:    // opcode: FX29, I = location for sprite character or something (5 * VX.....)
            cpu->reg->i = cpu->reg->v[n2] * 5;
            break;

        case 0x33:    // opcode: FX33, I, I + 1, and I + 2 = BCD representation of VX
            printf("PONG BCD PONG BCD\n");
            // stub?
            break;

        case 0x55:    // opcode: FX55, store V0->VX in memory starting at address in I
            for(t = 0; t <= n2; t++)
                cpu->mem->mem[cpu->reg->i + t] = cpu->reg->v[t];
            break;

        case 0x65:    // opcode: FX65, store memory starting at address in I in V0->VX
            for(t = 0; t <= n2; t++)
                cpu->reg->v[t] = cpu->mem->mem[cpu->reg->i + t];
            break;
    }

}
