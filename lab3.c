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
  int year = 0;
  int minPrice = 0;
  int chunkSize = 1;
  int threadsAmount = 1;
  int showData = 0;
  FILE *og;
  int x = 0;
  // Se obtienen la informacion de las flags correspondientes
  while ((flags = getopt(argc, argv, "i:o:d:p:c:t:b")) != -1)
    switch (flags) {
    case 'i':
      strncpy(file, optarg, sizeof(file));
      break;
    case 'o':
      strncpy(exitFile, optarg, sizeof(exitFile));
      break;
    case 'd':
      year = atoi(optarg);
      if (year < 1980) {
        printf("ERROR: año minimo debe ser mayor o igual a 1980");
        exit(1);
      }
      break;
    case 'p':
      minPrice = atoi(optarg);
      if (!minPrice || minPrice > 100) {
        printf("ERROR: Valor minimo debe ser mayor o igual a 0");
        exit(1);
      }
      break;
    case 'c':
      chunkSize = atoi(optarg);
      if (!chunkSize || threadsAmount > 100) {
        printf("ERROR: Tamaño de chunk debe ser mayor a 0");
        exit(1);
      }
      break;
    case 't':
      threadsAmount = atoi(optarg);
      if (!threadsAmount || threadsAmount > 100) {
        printf("ERROR: Cantidad de hilos debe ser mayor a 0 y menor a 100");
        exit(1);
      }
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