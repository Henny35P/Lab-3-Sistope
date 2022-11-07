#include "cvector.h"
#include "cvector_utils.h"
#include "game.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

FILE *gameData;
int chunkSize, startingYear = 1990, finishingYear = 2030;
int testGlobal = 0;
pthread_mutex_t mutex;
cvector_vector_type(GameData) yearlyData = NULL;

void *readData(void *arg) {
  char row[MAX];
  int readChunks = 0, currentYear = 0, currentIndex = 0;
  // Mientras el archivo no tenga EOF o el thread no haya leido su cantidad
  // de lineas especificadas
  while (fgets(row, MAX, gameData) || chunkSize >= readChunks) {
    testGlobal++;
    pthread_mutex_lock(&mutex);
    Game currentGame = *getGame(row);
    // Mover estos calculos a una funcion independiente?
    // Primero, encuentro de que año es el juego
    currentYear = currentGame.year;
    // Con una resta encuentro el index correspondiente
    if (!currentYear) {
      break;
    }
    currentIndex = currentYear - startingYear;
    // Hago los calculos correspondientes
    if (currentGame.free) {
      strcat(yearlyData[currentIndex].juegosGratis, currentGame.name);
      strcat(yearlyData[currentIndex].juegosGratis, "\n");
    } else {
      yearlyData[currentIndex].totalpagados++;
    }

    if (currentGame.price > yearlyData[currentIndex].maxPrice) {
      strcpy(yearlyData[currentIndex].maxName, currentGame.name);
      yearlyData[currentIndex].maxPrice = currentGame.price;
    }

    if (currentGame.price < yearlyData[currentIndex].minPrice &&
        currentGame.price != 0) {
      strcpy(yearlyData[currentIndex].minName, currentGame.name);
      yearlyData[currentIndex].minPrice = currentGame.price;
    }

    if (currentGame.win) {
      yearlyData[currentIndex].win++;
    }
    if (currentGame.lin) {
      yearlyData[currentIndex].lin++;
    }
    if (currentGame.mac) {
      yearlyData[currentIndex].mac++;
    }
    yearlyData[currentIndex].sumPrice += currentGame.price;
    yearlyData[currentIndex].total++;
    readChunks++;
    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  // --------------------------------------------------------------
  // ------------- Captura parametros de entrada -------------
  // --------------------------------------------------------------
  // Se inicializan las variables para capturar la informacion desde argc.
  int flags;
  char file[256];
  char exitFile[256];
  int year = 0;
  int minPrice = 0;
  int threadsAmount = 1;
  int showData = 0;
  FILE *og;
  int x = 0;
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
  // --------------------------------------------------------------
  // ------------- Finalizacion parametros de entrada -------------
  // --------------------------------------------------------------
  //
  // Leo el archivo global, error en caso de que no se pueda leer
  if (!(gameData = fopen(file, "r"))) {
    perror("Error: Archivo inexistente o corrupto");
    return 0;
  }
  // Testing variables!!
  chunkSize = 6;
  threadsAmount = 6;
  pthread_t tid[threadsAmount];

  // Creo un vector principal con los datos para cada año presente
  // Se les asigna datos genericos para llenar el vector
  for (int i = startingYear; i < finishingYear; i++) {
    GameData X = *crearData(i, "lorem", 0, "impsum", 100, 0, 0, 0, 0, 0, 0, "");
    cvector_push_back(yearlyData, X);
  }
  // Inicializo la exclusion mutua
  pthread_mutex_init(&mutex, NULL);

  // Creo los hilos segun la cantidad ingresada
  for (int i = 0; i < threadsAmount; i++) {
    pthread_create(&tid[i], NULL, readData, NULL);
  }
  // Espero al termino de los hilos
  for (int i = 0; i < threadsAmount; i++) {
    pthread_join(tid[i], NULL);
  }
  pthread_mutex_destroy(&mutex);
  for (int i = 0; i < cvector_size(yearlyData); i++) {
    printf("%s\n", yearlyData[i].maxName);
  }

  return 1;
}
