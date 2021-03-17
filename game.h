#ifndef game
#define game

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
	Line lines[12];
} Level;

typedef struct Game {
	Level levels[4];
} Game;


int levelSelect;
//void generateSprites(GameSprite* gs);
void generateLine(Line* li, int le, int i);
void generateGame(Game* g);
void movePlayer(Game* g, int le, int w, int press, int sw, int bw);


#endif
