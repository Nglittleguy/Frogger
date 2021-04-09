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
			if(g->levels[levelChosen].lines[i].sprites[j].code != 0) {								//null spot, don't draw
				if(j==9)
					w = widthBy24;
				else
					w = widthBy24*2;
				for(int xOff = 0; xOff < w; xOff++) {										//draw sprite size
					for(int yOff = 0; yOff < sizeBy24; yOff++) {
						if(g->levels[levelChosen].lines[i].sprites[j].x + xOff < 1280 && sizeBy24 * i + yOff<720) {
							oldColour[g->levels[levelChosen].lines[i].sprites[j].x + xOff][sizeBy24 * i + yOff] 
							= g->levels[levelChosen].lines[i].sprites[j].code;
						}
						
					}
				}
			}
		}
	}
} 

void drawPowerUp(Game* g) {
	for(int xOff = 0; xOff < widthBy24; xOff++) {
		for(int yOff = 0; yOff < sizeBy24; yOff++) {
			if(g->powerUp.x + xOff < 1280 && sizeBy24 * g->powerUpLine + yOff<720) {
				oldColour[g->powerUp.x + xOff][sizeBy24 * g->powerUpLine + yOff] 
				= g->powerUp.code;
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

