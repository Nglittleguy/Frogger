#include <wiringPi.h>
#include <stdio.h>
#include "game.h"

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

const int lineColours[4][12] = {{GREEN, BLUE, RED, GREEN, RED, BLUE, BLUE, GREEN, CYAN, RED, BLUE, GREEN},
								{GREEN, BLUE, RED, GREEN, RED, BLUE, BLUE, GREEN, CYAN, RED, BLUE, GREEN},
								{GREEN, BLUE, RED, GREEN, RED, BLUE, BLUE, GREEN, CYAN, RED, BLUE, GREEN},
								{GREEN, BLUE, RED, GREEN, RED, BLUE, BLUE, GREEN, CYAN, RED, BLUE, GREEN}};



//void generateSprites(GameSprite* gs);

void generateLine(Line* li, int le, int i) {

	for(int k = 1; k<=9; k++) {
		GameSprite s;
		s.code = k;
		s.x = k*10;
		s.y = i*10;
		s.w = 10;
		s.h = 10;
		li->sprites[k]=s;
	}

	li->colour = lineColours[le][i];
	if(i==0 || i ==11 || (le%2==0 && i%3==0))
		li->direction = 0;
	else
		li->direction = ((i+le+1)*(i+le+2))%3-1;
}


void generateGame(Game* g) {

	for(int i = 0; i<4; i++) {
		Level le;
		for(int j = 0; j<12; j++) {
			Line li;
			generateLine(&li, i, j);
			le.lines[j] = li;
		}
		le.code = i;
		g->levels[i] = le;
	}

}



