#include <wiringPi.h>
#include <stdio.h>
#include "game.h"

/*
Program to handle game logic and functionality - game.c
- Stephen Ng 30038689
*/

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define DARKGREEN 0x4BC4
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define FROG 0xFFFF
#define ORANGE 0XFC40
#define GRAY 0x9492
#define DARKGRAY 0x2104
// http://www.barth-dev.de/online/rgb565-color-picker/

//Default values:
const int n = 24;					//number of lines per level
const int distTravelled = 10;		//distance of pixels travelled left/right per tick

//line colours per level
const int lineColours[4][24] = {{GREEN, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GREEN, 
								GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY, GREEN},
								
								{GREEN, BLUE, BLUE, GREEN, BLUE, BLUE, BLUE, GREEN, BLUE, BLUE, BLUE, GREEN,
								GREEN, BLUE, BLUE, GREEN, BLUE, BLUE, BLUE, GREEN, BLUE, BLUE, BLUE, GREEN},
								
								{GRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY,
								GRAY, GRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, GRAY},
								
								{GRAY, DARKGREEN, DARKGREEN, DARKGREEN, DARKGREEN, DARKGREEN, DARKGREEN, DARKGREEN, DARKGREEN, DARKGREEN, DARKGREEN, DARKGREEN,
								DARKGREEN, DARKGREEN, DARKGREEN, DARKGREEN, DARKGREEN, DARKGREEN, DARKGREEN, DARKGREEN, DARKGREEN, DARKGREEN, DARKGREEN, GRAY}};

//sprite start locations (pseudo random looking, while still being playable)
const int spriteStart[24][10] = 
{
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 300, 800, 1200, 1500, 1900, 2100, 2400, 2800, 3100},
	{100, 400, 700, 900, 1300, 1700, 1900, 2300, 2700, 3200},
	{50, 550, 700, 1050, 1250, 1550, 2000, 2300, 2600, 3000},
	{0, 250, 500, 800, 1200, 1500, 1900, 2200, 2800, 3300},
	{100, 500, 800, 1200, 1400, 1800, 2000, 2500, 2900, 3200},
	{50, 350, 700, 1100, 1500, 1800, 2300, 2500, 2900, 3200}, 
	{0, 300, 600, 1100, 1400, 1700, 2000, 2400, 2700, 3100},
	{100, 300, 700, 1000, 1500, 1900, 2200, 2600, 2900, 3300},
	{50, 400, 600, 1000, 1300, 1600, 2000, 2500, 2800, 3200},
	{0, 400, 600, 1000, 1300, 1500, 1900, 2200, 2700, 3100},
	{50, 550, 700, 1050, 1250, 1550, 2000, 2300, 2600, 3000},
	{0, 250, 500, 800, 1200, 1500, 1900, 2200, 2800, 3300},
	{0, 300, 600, 1100, 1400, 1700, 2000, 2400, 2700, 3100},
	{100, 300, 700, 1000, 1500, 1900, 2200, 2600, 2900, 3300},
	{100, 500, 800, 1200, 1400, 1800, 2000, 2500, 2900, 3200},
	{50, 400, 600, 1000, 1300, 1600, 2000, 2500, 2800, 3200},
	{0, 250, 500, 800, 1200, 1500, 1900, 2200, 2800, 3300},
	{0, 300, 800, 1200, 1500, 1900, 2100, 2400, 2800, 3100},
	{50, 350, 700, 1100, 1500, 1800, 2300, 2500, 2900, 3200}, 
	{100, 300, 700, 1000, 1500, 1900, 2200, 2600, 2900, 3300},
	{0, 250, 500, 800, 1200, 1500, 1900, 2200, 2800, 3300},
	{50, 550, 700, 1050, 1250, 1550, 2000, 2300, 2600, 3000},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

//Colours no longer used - replaced with sprites in var.c
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

//Populates a line based on parameters
void generateLine(Line* li, int le, int i, int w, int h, int sw) {

	for(int k = 0; k<10; k++) {								//10 sprites
		GameSprite s;
		s.code = spriteColours[le][i][k];					//set the code to know if not frog
		s.x = spriteStart[i][k];							//set the sprite's starting location
		//width of the sprite
		if(s.code==FROG)
			s.w = sw;
		else
			s.w = 2*sw;
		//height of sprite
		s.h = h;
		li->sprites[k]=s;									//log the sprite into the line
	}

	if(li->sprites[9].code==FROG) {
		li->sprites[9].x = w/2-sw;							//center the player
	}

	li->colour = lineColours[le][i];						//set the colour of the line
	if(li->sprites[0].code==0)
		li->direction = 0;									//stationary sections (no sprites)
	else
		li->direction = ((i+le)%2)*2-1;						//either 1, -1
}

//Populates the game (all levels)
void generateGame(Game* g, int w, int h, int sw) {

	for(int i = 0; i<4; i++) {								//4 levels
		Level le;
		for(int j = 0; j<n; j++) { 							//n number of lines
			Line li;
			generateLine(&li, i, j, w, h, sw);
			le.lines[j] = li;								//log the line to the level
		}
		le.code = i;
		g->levels[i] = le;									//log the level to the game
	}

	//setting default valuePack values
	g->powerUp.code = 0;
	g->powerUp.x = 2000;
	g->powerUp.w = sw;
	g->powerUp.h = h;
	g->powerUpLine = 0;

}

//Moves the player to the center of the first row of the level
void moveToStart(Game *g, int le, int w, int sw, int frog) {
	g->levels[le].lines[frog].sprites[9].code = 0;			//remove sprite from line
	g->levels[le].lines[23].sprites[9].code = FROG;			//put in the starting line
	g->levels[le].lines[23].sprites[9].x = w/2-sw; 			//put in the middle
}

//Detects collisions of the player with the terrain
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
 				//if contact, then dead
 				if(bounds < frog+sw && bounds+2*sw > frog)	
					alive = 0;
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
 				//if contact, then survives
 				if(bounds < frog+sw && bounds+2*sw > frog)	{		
					alive = 1;
				}
			}
			break;
	}

	return alive;
}

//Moves the player if possible
int movePlayer(Game* g, int le, int w, int press, int sw, int bw) {
	int currentLine = 0;
	//find the current line of the player
	for(int i = 0; i<n; i++) {
		if(g->levels[le].lines[i].sprites[9].code==FROG)
			currentLine = i;
	}
	switch(press) {
		case 4:	//UP;
			if(currentLine!=0) {	
				//if not at the top line, then move the frog up a line
				g->levels[le].lines[currentLine-1].sprites[9].code = FROG;
				g->levels[le].lines[currentLine-1].sprites[9].x = g->levels[le].lines[currentLine].sprites[9].x;
				g->levels[le].lines[currentLine].sprites[9].code = 0;
				currentLine = currentLine-1;
			}
			else if(le!=3) {
				//if not the last level, then move the frog to the next level
				changeLevel(g, le, 1);
				return 23;
			}
			else if(le==3) {
				//if the last level, then don't change the level
				return 23;
			}
			break;
		case 5:	//DOWN
			if(currentLine!=23) {
				//if not the bottom line, then move the frog down a line
				g->levels[le].lines[currentLine+1].sprites[9].code = FROG;
				g->levels[le].lines[currentLine+1].sprites[9].x = g->levels[le].lines[currentLine].sprites[9].x;
				g->levels[le].lines[currentLine].sprites[9].code = 0;
				currentLine = currentLine+1;
			}
			else if(le!=0) {
				//if not the first level, then change the level
				changeLevel(g, le, 0);
				return 0;
			}
			break;
		case 6: //LEFT
			if(g->levels[le].lines[currentLine].sprites[9].x - sw >= 0) 
				g->levels[le].lines[currentLine].sprites[9].x -= sw;
				//if not move out of bounds, then move it left
			break;
		case 7: //RIGHT
			if(g->levels[le].lines[currentLine].sprites[9].x + sw <= w)
				g->levels[le].lines[currentLine].sprites[9].x += sw;
				//if not move out of bounds, then move it right
			break;
	}
	return currentLine;
}

//Move from 1 level to another
void changeLevel(Game* g, int le, int up) {
	//set the position of the frog in the next level in the same position as the last level's frog
	if(up) 
		g->levels[le+1].lines[23].sprites[9].x = g->levels[le].lines[0].sprites[9].x;
	else 
		g->levels[le-1].lines[0].sprites[9].x = g->levels[le].lines[23].sprites[9].x;
}

//Move level items as time passes
int updateTime(Game* g, int le, int w, int bw, int sw, int currentLine) {
	//for every line and every sprite
	for(int i = 0; i<n; i++) {
		for(int j = 0; j<10; j++) {
			//don't move for levels 0 and 3
			if(g->levels[le].lines[i].sprites[j].code == FROG && (le==0||le==3)) 
				continue;

			//move the sprite in the direction of the line
			g->levels[le].lines[i].sprites[j].x += g->levels[le].lines[i].direction * distTravelled;
			
			if(g->levels[le].lines[i].sprites[j].x <= 0) 
				g->levels[le].lines[i].sprites[j].x = 3500;				//loop back to right
			
			else if(g->levels[le].lines[i].sprites[j].x >= 3500)
				g->levels[le].lines[i].sprites[j].x = 0;				//loop back to left

			//maintain the frog stays within bounds even if carried off
			if(g->levels[le].lines[currentLine].sprites[9].x < 0) 
				g->levels[le].lines[currentLine].sprites[9].x += distTravelled;
			if(g->levels[le].lines[currentLine].sprites[9].x + sw > w) 
				g->levels[le].lines[currentLine].sprites[9].x -= distTravelled;
		}
	}
	//check if the frog is still alive after the this update
	return collision(g, le, sw, currentLine, g->levels[le].lines[currentLine].sprites[9].x);
}

//Initialize the position of the valuePack given state of game (pseudo-random)
void setUpPowerUp(Game* g, int steps, int currentLine) {
	g->powerUpLine = (steps%22)+1;										//from 1 to 22
	g->powerUp.x = (currentLine * steps) % 1100;						//within bounds
}

//Move the valuePack values off screen
void removePowerUp(Game* g) {
	g->powerUpLine = 0;
	g->powerUp.x = 3000;
}

//Check if the player collides with the valuePack
int collectPowerUp(Game* g, int currentLine, int le, int sw) {
	if(g->powerUpLine == currentLine) {
		int frog = g->levels[le].lines[currentLine].sprites[9].x;
		if(g->powerUp.x < frog+sw && g->powerUp.x+2*sw > frog)	
			return 1;
	}
	return 0;
}