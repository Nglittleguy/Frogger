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
const int n = 24;
const int distTravelled = 10;

const int lineColours[4][24] = {{GREEN, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GREEN, 
								GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GREEN},
								
								{GREEN, BLUE, BLUE, GREEN, BLUE, BLUE, BLUE, GREEN, BLUE, BLUE, BLUE, GREEN,
								GREEN, BLUE, BLUE, GREEN, BLUE, BLUE, BLUE, GREEN, BLUE, BLUE, BLUE, GREEN},
								
								{GRAY, SKYBLUE, SKYBLUE, SKYBLUE, SKYBLUE, SKYBLUE, SKYBLUE, SKYBLUE, SKYBLUE, SKYBLUE, SKYBLUE, GRAY,
								GRAY, SKYBLUE, SKYBLUE, SKYBLUE, SKYBLUE, SKYBLUE, SKYBLUE, SKYBLUE, SKYBLUE, SKYBLUE, SKYBLUE, GRAY},
								
								{GRAY, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GRAY, GREEN, GREEN,
								GRAY, GRAY, RED, GREEN, RED, GRAY, GRAY, GREEN, CYAN, RED, GRAY, GRAY}};

const int spriteStart[24][10] = 
{
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 200, 600, 900, 1100, 1400, 1500, 1700, 2000, 2200},
	{100, 300, 500, 600, 900, 1200, 1300, 1600, 1900, 2300},
	{50, 450, 500, 750, 850, 1050, 1400, 1600, 1800, 2100},
	{0, 150, 300, 500, 800, 1000, 1300, 1500, 2000, 2400},
	{100, 400, 600, 900, 1000, 1300, 1400, 1800, 2100, 2300},
	{50, 250, 500, 800, 1100, 1300, 1700, 1800, 2100, 2300}, 
	{0, 200, 400, 800, 1000, 1200, 1400, 1700, 1900, 2200},
	{100, 200, 500, 700, 1100, 1400, 1600, 1900, 2100, 2400},
	{50, 300, 400, 700, 900, 1100, 1400, 1800, 2000, 2300},
	{0, 300, 400, 700, 900, 1000, 1300, 1500, 1900, 2200},
	{50, 450, 500, 750, 850, 1050, 1400, 1600, 1800, 2100},
	{0, 150, 300, 500, 800, 1000, 1300, 1500, 2000, 2400},
	{0, 200, 400, 800, 1000, 1200, 1400, 1700, 1900, 2200},
	{100, 200, 500, 700, 1100, 1400, 1600, 1900, 2100, 2400},
	{100, 400, 600, 900, 1000, 1300, 1400, 1800, 2100, 2300},
	{50, 300, 400, 700, 900, 1100, 1400, 1800, 2000, 2300},
	{0, 150, 300, 500, 800, 1000, 1300, 1500, 2000, 2400},
	{0, 200, 600, 900, 1100, 1400, 1500, 1700, 2000, 2200},
	{50, 250, 500, 800, 1100, 1300, 1700, 1800, 2100, 2300}, 
	{100, 200, 500, 700, 1100, 1400, 1600, 1900, 2100, 2400},
	{0, 150, 300, 500, 800, 1000, 1300, 1500, 2000, 2400},
	{50, 450, 500, 750, 850, 1050, 1400, 1600, 1800, 2100},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
}


const int spriteColours[4][24][10] = 
{
	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
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
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

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
		{0, 0, 0, 0, 0, 0, 0, 0, 0, FROG} 
	},

	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0}, 
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0}, 
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0}, 
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0}, 
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0}, 
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0}, 
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0},
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0}, 
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0},
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0}, 
		{MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, FROG} 
	},

	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
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
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 0}, 
		{YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 0}, 
		{YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 0}, 
		{YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 0}, 
		{YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 0}, 
		{YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 0}, 
		{YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 0}, 
		{YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 0}, 
		{YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 0},  
		{0, 0, 0, 0, 0, 0, 0, 0, 0, FROG} 
	},
	

	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
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
		{0, 0, 0, 0, 0, 0, 0, 0, 0, FROG} 
	},
};

GameSprite powerUp;
int powerUpLine;

void generateLine(Line* li, int le, int i, int w, int h, int sw) {

	for(int k = 0; k<10; k++) {
		GameSprite s;
		s.code = spriteColours[le][i][k];
		s.x = spriteStart[i][k];
		if(s.code==FROG)
			s.w = sw;
		else
			s.w = 2*sw;
		s.h = h;
		li->sprites[k]=s;
	}

	if(li->sprites[9].code==FROG) {
		li->sprites[9].x = w/2-sw;							//center the player
	}

	li->colour = lineColours[le][i];
	if(li->sprites[0].code==0)
		li->direction = 0;									//stationary sections
	else
		li->direction = ((i+le)%2)*2-1;						//either 1, -1
}


void generateGame(Game* g, int w, int h, int sw) {

	for(int i = 0; i<4; i++) {
		Level le;
		for(int j = 0; j<n; j++) {
			Line li;
			generateLine(&li, i, j, w, h, sw);
			le.lines[j] = li;
		}
		le.code = i;
		g->levels[i] = le;
	}

	g->powerUp.code = 0;
	g->powerUp.x = 2000;
	g->powerUp.w = sw;
	g->powerUp.h = h;
	g->powerUpLine = 0;

}


void moveToStart(Game *g, int le, int w, int sw, int frog) {
	g->levels[le].lines[frog].sprites[9].code = 0;
	g->levels[le].lines[23].sprites[9].code = FROG;
	g->levels[le].lines[23].sprites[9].x = w/2-sw;
}


int collision(Game* g, int le, int sw, int currLine, int frog) {
	int alive;
	int bounds;
	switch(le) {
		
		//hits objects
		case 0:
		case 3:
			alive = 1;
			if(g->levels[le].lines[currLine].direction==0)
				return 1;
			for(int i = 0; i<9; i++) {
				bounds = g->levels[le].lines[currLine].sprites[i].x;  	//only need to check line with frog
 				if(bounds < frog+sw && bounds+2*sw > frog)	{		
					alive = 0;
				}
			}
			break;
		
		//ride on objects
		case 1:
		case 2:
			alive = 0;
			if(g->levels[le].lines[currLine].direction==0)
				return 1;
			for(int i = 0; i<9; i++) {
				bounds = g->levels[le].lines[currLine].sprites[i].x;  	//only need to check line with frog
 				if(bounds < frog+sw && bounds+2*sw > frog)	{		
					alive = 1;
				}
			}
			break;
	}

	return alive;
}


int movePlayer(Game* g, int le, int w, int press, int sw, int bw) {
	int currentLine = 0;
	for(int i = 0; i<n; i++) {
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
				currentLine = currentLine-1;
			}
			break;
		case 5:	//DOWN
			printf("DOWN");
			if(currentLine!=23) {
				g->levels[le].lines[currentLine+1].sprites[9].code = FROG;
				g->levels[le].lines[currentLine+1].sprites[9].x = g->levels[le].lines[currentLine].sprites[9].x;
				g->levels[le].lines[currentLine].sprites[9].code = 0;
				currentLine = currentLine+1;
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
	return currentLine;
	
}

int updateTime(Game* g, int le, int w, int bw, int sw, int currentLine) {
	for(int i = 0; i<n; i++) {
		for(int j = 0; j<10; j++) {
			if(g->levels[le].lines[i].sprites[j].code == FROG && (le==0||le==3)) {	//don't move for levels 0 and 3
				continue;
			}
			g->levels[le].lines[i].sprites[j].x += g->levels[le].lines[i].direction * distTravelled;
			if(g->levels[le].lines[i].sprites[j].x <= 0) 
				g->levels[le].lines[i].sprites[j].x = 2500;				//loop up
			
			else if(g->levels[le].lines[i].sprites[j].x >= 2500)
				g->levels[le].lines[i].sprites[j].x = 0;				//loop down
			
		}
	}
	return collision(g, le, sw, currentLine, g->levels[le].lines[currentLine].sprites[9].x);
}

void setUpPowerUp(Game* g, int steps, int currentLine) {
	g->powerUpLine = (steps%22)+1;				//from 1 to 22
	g->powerUp.x = (currentLine * steps) % 1200;
}

void removePowerUp(Game* g) {
	g->powerUpLine = 0;
	g->powerUp.x = 2000;
}

int collectPowerUp(Game* g, int currentLine, int le) {
	if(g->powerUpLine == currentLine) {
		int frog = g->levels[le].lines[currentLine].sprites[9].x;
		if(g->powerUp.x < frog+sw && g->powerUp.x+2*sw > frog)	
			return 1;
	}
	return 0;
}




