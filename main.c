#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "framebuffer.h"
#include "game.h"

/* Definitions */
typedef struct {
	int colour;
	int x, y;
} Pixel;

struct fbs framebufferstruct;
void drawPixel(Pixel *pixel);

const int height = 500;
int sizeBy12 = height/12;
const int width = 1000;
const int bWidth = 250;
const int bHeight = 250;

void drawBackground(Game *g, int levelChosen) {
	/* initialize a pixel */
	Pixel *pixel;
	pixel = malloc(sizeof(Pixel));

	
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++) 
		{	
			pixel->x = x+bWidth;
			pixel->y = y+bHeight ;
			pixel->colour = g->levels[levelChosen].lines[y/sizeBy12].colour;
			drawPixel(pixel);
		}
		//printf("Colour at %d of %x\n", y, g.levels[levelChosen].lines[y/sizeBy12].colour);
	}
	/* free pixel's allocated memory */
	free(pixel);
	pixel = NULL;
}

void drawSprites(Game *g, int levelChosen) {
	/* initialize a pixel */
	Pixel *pixel;
	pixel = malloc(sizeof(Pixel));
	for(int i = 0; i<12; i++) {
		for(int j = 0; j<10; j++) {
			if(g->levels[levelChosen].lines[i].sprites[j].code != 0) {	//null spot, don't draw
				


				for(int xOff = 0; xOff<30; xOff++) {					//draw sprite
					for(int yOff = 0; yOff<30; yOff++) {
						pixel->x = g->levels[levelChosen].lines[i].sprites[j].x + xOff + bWidth;
						pixel->y = sizeBy12 * i + yOff + bHeight;
						pixel->colour = g->levels[levelChosen].lines[i].sprites[j].code;
						drawPixel(pixel);
					}
				}



			}
		}
	}

	/* free pixel's allocated memory */
	free(pixel);
	pixel = NULL;
} 


/* main function */
int main(){

	/* initialize + get FBS */
	framebufferstruct = initFbInfo();
	Game g;
	generateGame(&g, sizeBy12);
	int levelChosen = 0;	

	drawBackground(&g);
	drawSprites();
	munmap(framebufferstruct.fptr, framebufferstruct.screenSize);
	
	return 0;
}



/* Draw a pixel */
void drawPixel(Pixel *pixel){
	long int location = (pixel->x +framebufferstruct.xOff) * (framebufferstruct.bits/8) +
                       (pixel->y+framebufferstruct.yOff) * framebufferstruct.lineLength;
	*((unsigned short int*)(framebufferstruct.fptr + location)) = pixel->colour;
}

