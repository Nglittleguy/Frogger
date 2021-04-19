#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "framebuffer.h"
#include "game.h"
#include "draw.h"
#include "var.h"
#include <string.h>

int oldColour[1280][720];

void drawTotal() {
	Pixel *pixel;
	pixel = malloc(sizeof(Pixel));
	for(int y = 0; y<height; y++) {
		for(int x = 0; x<width; x++) {
			// long int location = (x +framebufferstruct.xOff) * (framebufferstruct.bits/8) +
   //                     (y+framebufferstruct.yOff) * framebufferstruct.lineLength;
   //          if(*((unsigned short int*)(framebufferstruct.fptr + location))!=oldColour[x][y]) {		//only redraw background
				
				pixel->x = x+bWidth;
				pixel->y = y+bHeight;
				pixel->colour = oldColour[x][y];
				drawPixel(pixel);	
			//}
		}
	}
	free(pixel);
	pixel = NULL;
}

void drawBackground(Game *g, int levelChosen) {

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) 
			oldColour[x][y] = g->levels[levelChosen].lines[y/sizeBy24].colour;
	}

}

void drawSprites(Game *g, int levelChosen) {
	int w;
	for(int i = 0; i<24; i++) {
		for(int j = 0; j<10; j++) {
			if(g->levels[levelChosen].lines[i].sprites[j].code != 0) {						//null spot, don't draw
				if(j==9)
					w = 50; //widthBy24;
					for(int xOff = 0; xOff < w; xOff++) {										//draw sprite size
						for(int yOff = 0; yOff < sizeBy24; yOff++) {
							if(g->levels[levelChosen].lines[i].sprites[j].x + xOff < 1280 && sizeBy24 * i + yOff<720) {
								oldColour[g->levels[levelChosen].lines[i].sprites[j].x + xOff][sizeBy24 * i + yOff] 
								= s_frog[yOff][xOff];
							}
							
						}
					}
				else
					w = 100; //widthBy24*2;
				const int *spriteLoc[30][100];
				switch(levelChosen) {
					case 0:
						if(g->levels[levelChosen].lines[i].direction == -1)
							spriteLoc = &s_carLeft;
						else
							spriteLoc = &s_carRight;
						break;
					case 1:
						if(g->levels[levelChosen].lines[i].direction == -1)
							spriteLoc = &s_log;
						else
							spriteLoc = &s_lilypad;
						break;
					case 2:
						if(g->levels[levelChosen].lines[i].direction == -1)
							spriteLoc = &s_rocket;
						else
							spriteLoc = &s_satelite;
						break;
					case 3:
						if(g->levels[levelChosen].lines[i].direction == -1)
							spriteLoc = &s_leftLawnmower;
						else
							spriteLoc = &s_rightLawnmower;
						break;
				}
				for(int xOff = 0; xOff < w; xOff++) {										//draw sprite size
					for(int yOff = 0; yOff < sizeBy24; yOff++) {
						if(g->levels[levelChosen].lines[i].sprites[j].x + xOff < 1280 && sizeBy24 * i + yOff<720) {
							oldColour[g->levels[levelChosen].lines[i].sprites[j].x + xOff][sizeBy24 * i + yOff] 
							= (*spriteLoc)[yOff][xOff];
						}
						
					}
				}
			}
		}
	}
} 

void drawPowerUp(Game* g) {
	for(int xOff = 0; xOff < 100; xOff++) {
		for(int yOff = 0; yOff < 30; yOff++) {
			if(g->powerUp.x + xOff < 1280 && sizeBy24 * g->powerUpLine + yOff<720) {
				oldColour[g->powerUp.x + xOff][sizeBy24 * g->powerUpLine + yOff] 
				= s_valuePack[yOff][xOff];
			}
		}
	}
}


void drawTime(int t, int tl) {
	Pixel *pixel;
	pixel = malloc(sizeof(Pixel));
	for(int j = 0; j<tl; j++) {
		pixel->x = bWidth+width-tl+j;
		if(tl-j>t) {
			pixel->colour = 0x0000;
			for(int i = 0; i<20; i++) {
				pixel->y = bHeight-i;
				drawPixel(pixel);
			}
		}
		else {
			pixel->colour = 0xFFFF;
			for(int i = 0; i<20; i++) {
				pixel->y = bHeight-i;
				drawPixel(pixel);
			}
		}
	}
	
	/* free pixel's allocated memory */
	free(pixel);
	pixel = NULL;
}


void drawBlank() {
	Pixel *pixel;
	pixel = malloc(sizeof(Pixel));
	for(int y = 0; y<height; y++) {
		for(int x = 0; x<width; x++) {
			pixel->x = x+bWidth;
			pixel->y = y+bHeight;
			pixel->colour = 0;
			drawPixel(pixel);
		}
	}	
}

void drawClearMem() {
	memset(oldColour, 0, sizeof(oldColour));
}

void drawBackgroundBlue() {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) 
			oldColour[x][y] = 0x1905;
	}
}

void drawPaused(int cursor) {
	for(int y = 0; y<760; y++) {
		for(int x = 0; x<570; x++) 
			oldColour[260+x][75+y] = s_paused[y][x];
		
	}
	if(cursor==0) {
		for(int y = 0; y<100; y++) {
			for(int x = 0; x<300; x++) 
				oldColour[490+x][330+y] = s_restart[y][x];
		}
	}
	else {
		for(int y = 0; y<100; y++) {
			for(int x = 0; x<300; x++) 
				oldColour[490+x][450+y] = s_quit[y][x];
		}
	}
	//change these offsets to get it right
}

void drawNumber(int x, int y, int n) {
	int dig = n%10;
	switch(dig) {
		case 0:	
			for(int yOff = 0; yOff<35; yOff++) {
				for(int xOff = 0; xOff<20; xOff++) {
					oldColour[x+xOff][y+yOff] = s_0[yOff][xOff];
				}
			}
			break;
		case 1:
			for(int yOff = 0; yOff<35; yOff++) {
				for(int xOff = 0; xOff<20; xOff++) {
					oldColour[x+xOff][y+yOff] = s_1[yOff][xOff];
				}
			}
			break;
		case 2:
			for(int yOff = 0; yOff<35; yOff++) {
				for(int xOff = 0; xOff<20; xOff++) {
					oldColour[x+xOff][y+yOff] = s_2[yOff][xOff];
				}
			}
			break;
		case 3:
			for(int yOff = 0; yOff<35; yOff++) {
				for(int xOff = 0; xOff<20; xOff++) {
					oldColour[x+xOff][y+yOff] = s_3[yOff][xOff];
				}
			}
			break;
		case 4:
			for(int yOff = 0; yOff<35; yOff++) {
				for(int xOff = 0; xOff<20; xOff++) {
					oldColour[x+xOff][y+yOff] = s_4[yOff][xOff];
				}
			}
			break;
		case 5:
			for(int yOff = 0; yOff<35; yOff++) {
				for(int xOff = 0; xOff<20; xOff++) {
					oldColour[x+xOff][y+yOff] = s_5[yOff][xOff];
				}
			}
			break;
		case 6:
			for(int yOff = 0; yOff<35; yOff++) {
				for(int xOff = 0; xOff<20; xOff++) {
					oldColour[x+xOff][y+yOff] = s_6[yOff][xOff];
				}
			}
			break;
		case 7:
			for(int yOff = 0; yOff<35; yOff++) {
				for(int xOff = 0; xOff<20; xOff++) {
					oldColour[x+xOff][y+yOff] = s_7[yOff][xOff];
				}
			}
			break;
		case 8:
			for(int yOff = 0; yOff<35; yOff++) {
				for(int xOff = 0; xOff<20; xOff++) {
					oldColour[x+xOff][y+yOff] = s_8[yOff][xOff];
				}
			}
			break;
		case 9:
			for(int yOff = 0; yOff<35; yOff++) {
				for(int xOff = 0; xOff<20; xOff++) {
					oldColour[x+xOff][y+yOff] = s_9[yOff][xOff];
				}
			}
			break;
	}

	if(dig/10!=0) {
		drawNumber(x-20, y, n/10);
	}
}

/* Draw a pixel */
void drawPixel(Pixel *pixel){
	//comment out to see beyond boundaries
	//if(pixel->x < bWidth + width && pixel->x > bWidth && pixel->y > bHeight && pixel->y < bHeight + height) {	
		long int location = (pixel->x +framebufferstruct.xOff) * (framebufferstruct.bits/8) +
                       (pixel->y+framebufferstruct.yOff) * framebufferstruct.lineLength;
    	if(*((unsigned short int*)(framebufferstruct.fptr + location))!=pixel->colour)		{		//only draw if different
			*((unsigned short int*)(framebufferstruct.fptr + location)) = pixel->colour;
		}

	//}
	
}

