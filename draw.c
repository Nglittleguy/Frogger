#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "framebuffer.h"
#include "game.h"
#include "draw.h"
#include "var.h"


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



/* Draw a pixel */
void drawPixel(Pixel *pixel){
	//comment out to see beyond boundaries
	//if(pixel->x < bWidth + width && pixel->x > bWidth && pixel->y > bHeight && pixel->y < bHeight + height) {	
		long int location = (pixel->x +framebufferstruct.xOff) * (framebufferstruct.bits/8) +
                       (pixel->y+framebufferstruct.yOff) * framebufferstruct.lineLength;
    	if(*((unsigned short int*)(framebufferstruct.fptr + location))!=pixel->colour)				//only draw if different
			*((unsigned short int*)(framebufferstruct.fptr + location)) = pixel->colour;

	//}
	
}

