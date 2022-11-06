#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // Se inicializan las variables para capturar la informacion desde argc.
  int flags;
  char file[256];
  char exitFile[256];
  char year[5];
  char minPrice[10];
  int showData = 0;
  FILE *og;
  int x = 0;
  // Se obtienen la informacion de las flags correspondientes
  if (x) {
    printf("Hello World");
  }
  while ((flags = getopt(argc, argv, "i:o:d:p:b")) != -1)
    switch (flags) {
    case 'i':
      strncpy(file, optarg, sizeof(file));
      break;
    case 'o':
      strncpy(exitFile, optarg, sizeof(exitFile));
      break;
    case 'd':
      strncpy(year, optarg, sizeof(year));
      break;
    case 'p':
      strncpy(minPrice, optarg, sizeof(minPrice));
      break;
    case 'b':
      showData = 1;
      break;
    }
  if (!(og = fopen(file, "r"))) {
    perror("ERROR");
    return 0;
  }
  printf("Hola mundo\n");
  printf("Usando hebras!\n");
  return 0;
}
