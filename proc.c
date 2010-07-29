// bz
#include "proc.h"

#include <stdio.h>
#include <stdlib.h>

int read_rom(const char *filename, unsigned char **rom) {

 int rom_size;
 FILE *fd = NULL;

 if((fd = fopen(filename, "r")) != NULL) {
  fseek(fd, 0, SEEK_END);
  rom_size = ftell(fd);
  rewind(fd);
  (*rom) = malloc(rom_size);
  fread((*rom), 1, rom_size, fd);
  fclose(fd);

  return rom_size;
 }

 return -1;
}

