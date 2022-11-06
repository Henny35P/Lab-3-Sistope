#include "game.h"
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
