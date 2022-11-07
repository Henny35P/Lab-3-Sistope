#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Entradas: Ingresa los datos correspondientes para crear un nuevo Game segun
// la api mencionada en el pdf. Salidas: retorna un nuevo Game. Descripcion:
// Constructor de un game.

Game *crearGame(int id, char name[], int age_restriction, float price,
                int release, int year, int free, int win, int lin, int mac) {
  Game *new_game;
  new_game = (Game *)malloc(sizeof(Game));
  new_game->id = id;
  new_game->price = price;
  new_game->age_restriction = age_restriction;
  strcpy(new_game->name, name);
  new_game->release = release;
  new_game->year = year;
  new_game->free = free;
  new_game->win = win;
  new_game->lin = lin;
  new_game->mac = mac;
  return new_game;
}

// Entradas: agrc y argv para la captura de flags por terminal
// Salidas: retorna struct game
// Descripcion:Funcion que captura el formato de los juegos.
Game *getGame(char string[MAX]) {
  // Inicio de variables
  int id, age_restriction, year;
  float price;
  char name[MAX], check[MAX], buffer[MAX];
  char release[20], free[20], win[20], lin[20], mac[20];

  // Copio el string ingreaso a un string temporal
  strcpy(buffer, string);
  // Escaneo el nombre del juego hasta la coma
  sscanf(buffer, "%*d,%[^,] ", check);

  // Si el nombre del juego no comienza con ",
  // Se lee hasta la coma
  if (strchr(check, '\"') == NULL) {
    sscanf(string, "%d,%[^,],%d,%f,%[^,],%d,%[^,],%[^,],%[^,],%[^,]", &id, name,
           &age_restriction, &price, release, &year, free, win, lin, mac);
    // Si el nombre si lo contiene
    // Se lee solamente el nombre entre los " "
  } else {
    sscanf(string, "%d,\"%[^\"],%d,%f,%[^,],%d,%[^,],%[^,],%[^,],%[^,]", &id,
           name, &age_restriction, &price, release, &year, free, win, lin, mac);
  }
  // Se toman los datos importantes
  // Se transforman a int (bool) para hacer mas facil el uso despues
  // Se crea una variable con el struct Game
  Game *build_game =
      crearGame(id, name, age_restriction, price, stringToBool(release), year,
                stringToBool(free), stringToBool(win), stringToBool(lin),
                stringToBool(mac));
  return build_game;
}
// Entradas: String valido
// Salidas: Struct de tipo Game
// Descripcion: Funcion que permite parsear de un string
// valido a un struct de tipo Game.

Game *stringToGame(char stringGame[MAX]) {
  int id, age_restriction, year, release, free, win, lin, mac;
  float price;
  char name[MAX];
  sscanf(stringGame, "%d,%[^,],%d,%f,%d,%d,%d,%d,%d,%d", &id, name,
         &age_restriction, &price, &release, &year, &free, &win, &lin, &mac);
  Game *build_game = crearGame(id, name, age_restriction, price, release, year,
                               free, win, lin, mac);
  return build_game;
}

// Entradas: string a consultar
// Salidas: 0 o 1 segun corresponda
// Descripcion:
//  Se usa ya que API continene True y YES
//  y permite facilitar el calculo prosiguiente
int stringToBool(char string[20]) {
  // Si en el string de entrada se encuentran estos strings
  // regresara 0
  if (strstr(string, "False") || strstr(string, "No")) {
    return 0;
  }
  // Si no, regresa 1
  return 1;
}

char *gameToString(Game X) {
  static char stringGame[MAX];
  // Inicializo el buffer de el string
  // Guardo los datos de struct en string formateado
  snprintf(stringGame, MAX, "%d,%s,%d,%f,%d,%d,%d,%d,%d,%d", X.id, X.name,
           X.age_restriction, X.price, X.release, X.year, X.free, X.win, X.lin,
           X.mac);
  return stringGame;
}
GameData *crearData(int year, char maxName[], float maxPrice, char minName[],
                    float minPrice, float sumprice, int totalpagados, int win,
                    int lin, int mac, int total, char juegosGratis[]) {

  GameData *newData;
  newData = (GameData *)malloc(sizeof(GameData));
  newData->year = year;
  strcpy(newData->maxName, maxName);
  newData->maxPrice = maxPrice;
  strcpy(newData->minName, minName);
  newData->minPrice = minPrice;
  newData->sumPrice = sumprice;
  newData->totalpagados = totalpagados;
  newData->win = win;
  newData->lin = lin;
  newData->mac = mac;
  newData->total = total;
  strcpy(newData->juegosGratis, juegosGratis);
  return newData;
}
