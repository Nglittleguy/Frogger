#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "framebuffer.h"
#include "game.h"
#include "draw.h"

static const int height = 500;
static int sizeBy12 = height/12;
static const int width = 1000;
static int widthBy24 = width/24;
static const int bWidth = 250;
static const int bHeight = 250;

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
			if(g->levels[levelChosen].lines[i].sprites[j].code != 0) {								//null spot, don't draw
				


				for(int xOff = 0; xOff < widthBy24; xOff++) {										//draw sprite size
					for(int yOff = 0; yOff < sizeBy12; yOff++) {
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



/* Draw a pixel */
void drawPixel(Pixel *pixel){
	long int location = (pixel->x +framebufferstruct.xOff) * (framebufferstruct.bits/8) +
                       (pixel->y+framebufferstruct.yOff) * framebufferstruct.lineLength;
	*((unsigned short int*)(framebufferstruct.fptr + location)) = pixel->colour;
}

