#include "cvector.h"
#include "cvector_utils.h"
#include "funciones.h"

FILE *gameData;
int chunkSize = 0, startingYear = 1990, finishingYear = 2030;
int testGlobal = 0;
pthread_mutex_t mutex;
cvector_vector_type(GameData) yearlyData = NULL;
long int currentPos;

// Entradas: nombre de salida, año minimo y precio minimo
// Salidas: sin retorno
// Descripcion:Funcion que crea el archivo de salida

void createExitFile(char exportFile[MAX], int minimunYear, int minimunPrice) {
  FILE *toExport;
  toExport = fopen(exportFile, "w+");
  float division;
  int full = 0;
  for (int i = 0; i < cvector_size(yearlyData); i++) {
    // Si el año recibio datos
    if (yearlyData[i].total && yearlyData[i].minPrice >= minimunPrice &&
        yearlyData[i].year >= minimunYear) {
      fprintf(toExport, "--------------------Año %d---------------------\n",
              yearlyData[i].year);
      fprintf(toExport, "El juegos mas caro fue %s con %.1f\n",
              yearlyData[i].maxName, yearlyData[i].maxPrice);
      fprintf(toExport, "El juegos mas barato fue %s con %.1f\n",
              yearlyData[i].minName, yearlyData[i].minPrice);
      fprintf(toExport, "El precio promedio fue de $%.1f\n",
              yearlyData[i].sumPrice / yearlyData[i].totalpagados);
      fprintf(
          toExport, "El porcentaje de juegos para windows fue de %.1f\n",
          (division = (float)yearlyData[i].win * 100 / yearlyData[i].total));
      fprintf(
          toExport, "El porcentaje de juegos para Linux fue de %.1f\n",
          (division = (float)yearlyData[i].lin * 100 / yearlyData[i].total)),
          fprintf(toExport, "El porcentaje de juegos para Mac fue de %.1f\n",
                  (division =
                       (float)yearlyData[i].mac * 100 / yearlyData[i].total));
      fprintf(toExport,
              "--------------------Juegos Gratis---------------------/\n");
      fprintf(toExport, "%s\n", yearlyData[i].juegosGratis);
    }
  }
  fclose(toExport);
}
// Entradas: Sin entradas
// Salidas: sin retorno
// Descripcion:Funcion que lee los archivos y calcula.
void *readData() {
  char row[MAX];
  int readChunks = 0, currentYear = 0, currentIndex = 0;
  // Mientras el archivo no tenga EOF
  while (1) {
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < chunkSize; i++) {

      if (!fgets(row, MAX, gameData)) {
        pthread_mutex_unlock(&mutex);
        pthread_exit(NULL);
        return 0;
      }

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
      // Reviso si proxima
      currentPos = ftell(gameData);
      if (!fgets(row, MAX, gameData)) {
        pthread_mutex_unlock(&mutex);
        pthread_exit(NULL);
        return 0;
      }
      fseek(gameData, currentPos, SEEK_SET);
    }
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

  while ((flags = getopt(argc, argv, "i:o:d:p:n:c:b")) != -1)
    switch (flags) {
    case 'i':
      strncpy(file, optarg, sizeof(file));
      break;
    case 'o':
      strncpy(exitFile, optarg, sizeof(exitFile));
      break;
    case 'd':
      year = atoi(optarg);
      if (year < 1980 || year > 2023) {
        printf("ERROR: año minimo debe ser mayor o igual a 1980\n");
        exit(1);
      }
      break;
    case 'p':
      minPrice = atoi(optarg);
      if (minPrice < 0 || minPrice > 100) {
        printf("ERROR: Valor minimo debe ser mayor o igual a 0\n");
        exit(1);
      }
      break;
    case 'c':
      chunkSize = atoi(optarg);
      if (!chunkSize || chunkSize > 500) {
        printf("ERROR: Tamaño de chunk debe ser mayor a 0\n");
        exit(1);
      }
      break;
    case 'n':
      threadsAmount = atoi(optarg);
      if (!threadsAmount || threadsAmount > 100) {
        printf("ERROR: Cantidad de hilos debe ser mayor a 0 y menor a 100\n");
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
    pthread_create(&tid[i], NULL, (void *)readData, NULL);
  }
  // Espero al termino de los hilos
  for (int i = 0; i < threadsAmount; i++) {
    pthread_join(tid[i], NULL);
  }

  // Creo el archivo de salida y lo muestro por consola
  createExitFile(exitFile, year, minPrice);
  if (showData == 1)
    showResults(exitFile);

  fclose(gameData);
  pthread_mutex_destroy(&mutex);
  return 1;
}
