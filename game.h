#define MAX 8192

struct game {
  float price;
  long int fd;
  char name[MAX];
  int id, age_restriction, year, release, win, lin, mac, free;
};
typedef struct game Game;

Game *crearGame(int id, char name[], int age_restriction, float price,
                int release, int year, int free, int win, int lin, int mac);
Game *getGame(char string[MAX]);
char *gameToString(Game X);
Game *stringToGame(char stringGame[MAX]);
int stringToBool(char string[20]);

struct gameData {
  int year, totalpagados, win, lin, mac, total;
  char maxName[MAX], minName[MAX], juegosGratis[MAX];
  float maxPrice, minPrice, sumPrice;
};
typedef struct gameData GameData;

GameData *crearData(int year, char maxName[], float maxPrice, char minName[],
                    float minPrice, float sumprice, int totalpagados, int win,
                    int lin, int mac, int total, char juegosGratis[]);
