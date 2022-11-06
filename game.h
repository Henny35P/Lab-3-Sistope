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
