// bz
#include "proc.h"

#include <stdio.h>
#include <stdlib.h>

reg_t reg = { };

int read_rom(const char *filename, unsigned char **mem) {

 int rom_size;
 FILE *fd = NULL;

 if((fd = fopen(filename, "r")) != NULL) {
  fseek(fd, 0, SEEK_END);
  rom_size = ftell(fd);
  rewind(fd);
  (*mem) = malloc(MEM_SIZE); // size of memory
  fread((*mem + ROM_LOC), 1, rom_size, fd);
  fclose(fd);

  return rom_size;
 }

 return -1;
}

