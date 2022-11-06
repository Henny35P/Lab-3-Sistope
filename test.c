
#include "cvector.h"
#include "cvector_utils.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {

  FILE *csv;
  char row[MAX];
  csv = fopen("./datos_juegos_1000.csv", "r");
  cvector_vector_type(Game) testArray = NULL;
  while (fgets(row, MAX, csv)) {
    Game X = *getGame(row);
    cvector_push_back(testArray, X);
  }
  fclose(csv);
  for (int i = 0; i < cvector_size(testArray); ++i) {
    printf("%s\n", testArray[i].name);
  }
  return 0;
}
