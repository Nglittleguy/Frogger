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
#define FROG 0xFFFF
#define ORANGE 0XFC40
#define GRAY 0x9492
#define SKYBLUE 0x56BF
// http://www.barth-dev.de/online/rgb565-color-picker/

const int lineColours[4][12] = {{GREEN, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GREEN},
								{GREEN, BLUE, BLUE, GREEN, BLUE, BLUE, BLUE, GREEN, BLUE, BLUE, BLUE, GREEN},
								{GRAY, SKYBLUE, SKYBLUE, SKYBLUE, SKYBLUE, SKYBLUE, SKYBLUE, SKYBLUE, SKYBLUE, SKYBLUE, SKYBLUE, GRAY},
								{GRAY, GRAY, RED, GREEN, RED, GRAY, GRAY, GREEN, CYAN, RED, GRAY, GRAY}};


const int spriteColours[4][12][10] = 
{
	{
		{RED, RED, RED, RED, RED, RED, RED, RED, RED, 0}, 
		{RED, RED, RED, RED, RED, RED, RED, RED, RED, 0}, 
		{RED, RED, RED, RED, RED, RED, RED, RED, RED, 0}, 
		{RED, RED, RED, RED, RED, RED, RED, RED, RED, 0},  
		{RED, RED, RED, RED, RED, RED, RED, RED, RED, 0}, 
		{RED, RED, RED, RED, RED, RED, RED, RED, RED, 0}, 
		{RED, RED, RED, RED, RED, RED, RED, RED, RED, 0}, 
		{RED, RED, RED, RED, RED, RED, RED, RED, RED, 0},
		{RED, RED, RED, RED, RED, RED, RED, RED, RED, 0}, 
		{RED, RED, RED, RED, RED, RED, RED, RED, RED, 0}, 
		{RED, RED, RED, RED, RED, RED, RED, RED, RED, 0},  
		{RED, RED, RED, RED, RED, RED, RED, RED, RED, FROG}, 
	},

	{
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0}, 
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0}, 
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0}, 
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0}, 
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0}, 
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0},
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0}, 
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0}, 
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0},
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0}, 
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0}, 
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, FROG} 
	},

	{
		{YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 0}, 
		{YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 0}, 
		{YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 0}, 
		{YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 0}, 
		{YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 0}, 
		{YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 0}, 
		{YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 0}, 
		{YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 0}, 
		{YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 0}, 
		{YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 0}, 
		{YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 0},  
		{YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, FROG}, 
	},
	

	{
		{ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, 0}, 
		{ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, 0}, 
		{ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, 0}, 
		{ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, 0}, 
		{ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, 0}, 
		{ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, 0}, 
		{ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, 0}, 
		{ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, 0}, 
		{ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, 0}, 
		{ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, 0}, 
		{ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, 0}, 
		{ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, FROG},
	},
};

void generateLine(Line* li, int le, int i, int w, int h, int sw) {

	for(int k = 0; k<10; k++) {
		GameSprite s;
		s.code = spriteColours[le][i][k];
		s.x = (k+1)*100;
		s.w = w;
		s.h = h;
		li->sprites[k]=s;
	}

	if(li->sprites[9].code==FROG) {
		li->sprites[9].x = w/2-sw;							//center the player
	}

	li->colour = lineColours[le][i];
	if(i==0 || i ==11 || (le%2==0 && i%3==0))
		li->direction = 0;
	else
		li->direction = ((i+le+1)*(i+le+2))%3-1;
}


void generateGame(Game* g, int w, int h, int sw) {

	for(int i = 0; i<4; i++) {
		Level le;
		for(int j = 0; j<12; j++) {
			Line li;
			generateLine(&li, i, j, w, h, sw);
			le.lines[j] = li;
		}
		le.code = i;
		g->levels[i] = le;
	}

}

void movePlayer(Game* g, int le, int w, int press, int sw, int bw) {
	int currentLine;
	for(int i = 0; i<12; i++) {
		if(g->levels[le].lines[i].sprites[9].code==FROG)
			currentLine = i;
	}
	switch(press) {
		case 4:	//UP
			printf("UP");
			if(currentLine!=0) {
				g->levels[le].lines[currentLine-1].sprites[9].code = FROG;
				g->levels[le].lines[currentLine-1].sprites[9].x = g->levels[le].lines[currentLine].sprites[9].x;
				g->levels[le].lines[currentLine].sprites[9].code = 0;
			}
			break;
		case 5:	//DOWN
			printf("DOWN");
			if(currentLine!=11) {
				g->levels[le].lines[currentLine+1].sprites[9].code = FROG;
				g->levels[le].lines[currentLine+1].sprites[9].x = g->levels[le].lines[currentLine].sprites[9].x;
				g->levels[le].lines[currentLine].sprites[9].code = 0;
			}
			break;
		case 6: //LEFT
			printf("LEFT");
			if(g->levels[le].lines[currentLine].sprites[9].x - sw >= bw) {
				g->levels[le].lines[currentLine].sprites[9].x -= sw;
			}
			break;
		case 7: //RIGHT
			printf("RIGHT");
			if(g->levels[le].lines[currentLine].sprites[9].x + sw <= bw + w) {
				g->levels[le].lines[currentLine].sprites[9].x += sw;
			}
			break;
	}
}




