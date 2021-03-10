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


/* main function */
int main(){

	/* initialize + get FBS */
	framebufferstruct = initFbInfo();
	
	/* initialize a pixel */
	Pixel *pixel;
	pixel = malloc(sizeof(Pixel));
	

	Game g;
	generateGame(&g, sizeBy12);

	int levelChosen = 0;	
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++) 
		{	

			pixel->x = x+bWidth;
			pixel->y = y+bHeight ;
			pixel->colour = g.levels[levelChosen].lines[y/sizeBy12].colour;

			// pixel->colour = 0xF800; // red pixel
			// pixel->x = x;
			// pixel->y = y;
	
			drawPixel(pixel);
		}
		//printf("Colour at %d of %x\n", y, g.levels[levelChosen].lines[y/sizeBy12].colour);

	}
	/* free pixel's allocated memory */
	free(pixel);
	pixel = NULL;
	munmap(framebufferstruct.fptr, framebufferstruct.screenSize);
	
	return 0;
}



/* Draw a pixel */
void drawPixel(Pixel *pixel){
	long int location = (pixel->x +framebufferstruct.xOff) * (framebufferstruct.bits/8) +
                       (pixel->y+framebufferstruct.yOff) * framebufferstruct.lineLength;
	*((unsigned short int*)(framebufferstruct.fptr + location)) = pixel->colour;
}

