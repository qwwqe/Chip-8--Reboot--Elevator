#include <stdio.h>
#include <stdlib.h>


void opcode0(int a) { printf("opcode0: %d\n", a); }
void opcode1(int a) { printf("opcode1: %d\n", a + 1); }
void opcode2(int a) { printf("opcode2: %d\n", a + 2); }
void opcode3(int a) { printf("opcode3: %d\n", a + 4); }

void (*fn[16])(int a) = {
 opcode0,
 opcode1,
 opcode2,
 opcode3
};
int main(void) {

 (*fn[0])(10);
 (*fn[1])(10);
 (*fn[2])(10);
 (*fn[3])(10);

 exit(EXIT_SUCCESS);
}
