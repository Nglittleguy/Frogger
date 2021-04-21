#ifndef game
#define game

/*
Program to handle game logic and functionality - game.h
- Stephen Ng 30038689
*/

typedef struct GameSprite{
	int code;
	int x;
	int w;
	int h;
} GameSprite;

typedef struct Line {
	int colour;
	int direction;
	GameSprite sprites[10];
} Line;

typedef struct Level {
	int code;
	Line lines[24];
} Level;

typedef struct Game {
	Level levels[4];
	GameSprite powerUp;
	int powerUpLine;
} Game;


int levelSelect;
void generateLine(Line* li, int le, int i, int w, int h, int sw);
void generateGame(Game* g, int w, int h, int sw);
void moveToStart(Game *g, int le, int w, int sw, int frog);
int collision(Game* g, int le, int sw, int currLine, int frog);
int movePlayer(Game* g, int le, int w, int press, int sw, int bw);
int updateTime(Game* g, int le, int w, int bw, int sw, int currentLine);
void setUpPowerUp(Game* g, int steps, int currentLine);
void removePowerUp(Game* g);
int collectPowerUp(Game* g, int currentLine, int le, int sw);
void changeLevel(Game* g, int le, int up);


#endif
