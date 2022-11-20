#include "funciones.h"
// Entradas: nombre de salida
// Salidas: sin retorno
// Descripcion:Funcion que recibe el nombre de salida del archivo generado, lo
// lee y lo muestra por consola.
void showResults(char *nombreSalida) {
  FILE *fich;
  if ((fich = fopen(nombreSalida, "r")) == NULL) {
    printf(" Error en la apertura. Es posible que el fichero no exista \n ");
  }
  char *contents = NULL;
  size_t len = 32;

  while (getline(&contents, &len, fich) != -1) {
    printf("%s", contents);
  }
  fclose(fich);
  free(contents);
}
