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
		for(int x = 0; x<width+80; x++) {		
			pixel->x = x+bWidth;
			pixel->y = y+bHeight;
			pixel->colour = oldColour[x][y];
			drawPixel(pixel);	
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
				if(j==9) {
					w = 50; //widthBy24;
					for(int xOff = 0; xOff < w; xOff++) {										//draw sprite size
						for(int yOff = 0; yOff < sizeBy24; yOff++) {
							if(g->levels[levelChosen].lines[i].sprites[j].x + xOff < width && sizeBy24 * i + yOff<height && s_frog[yOff][xOff]!=0) {
								oldColour[g->levels[levelChosen].lines[i].sprites[j].x + xOff][sizeBy24 * i + yOff] 
								= s_frog[yOff][xOff];
							}
						}
					}
				}
				else {
					w = 100; //widthBy24*2;
					switch(levelChosen) {
						case 0:
							if(g->levels[levelChosen].lines[i].direction == -1) {
								for(int xOff = 0; xOff < w; xOff++) {										//draw sprite size
									for(int yOff = 0; yOff < sizeBy24; yOff++) {
										if(g->levels[levelChosen].lines[i].sprites[j].x + xOff < width && sizeBy24 * i + yOff<height && s_carLeft[yOff][xOff]!=0) {
											oldColour[g->levels[levelChosen].lines[i].sprites[j].x + xOff][sizeBy24 * i + yOff] 
											= s_carLeft[yOff][xOff];
										}
									}
								}
							}

							else {
								for(int xOff = 0; xOff < w; xOff++) {										//draw sprite size
									for(int yOff = 0; yOff < sizeBy24; yOff++) {
										if(g->levels[levelChosen].lines[i].sprites[j].x + xOff < width && sizeBy24 * i + yOff<height && s_carRight[yOff][xOff]!=0) {
											oldColour[g->levels[levelChosen].lines[i].sprites[j].x + xOff][sizeBy24 * i + yOff] 
											= s_carRight[yOff][xOff];
										}
									}
								}
							}
							break;
						case 1:
							if(g->levels[levelChosen].lines[i].direction == -1) {
								for(int xOff = 0; xOff < w; xOff++) {										//draw sprite size
									for(int yOff = 0; yOff < sizeBy24; yOff++) {
										if(g->levels[levelChosen].lines[i].sprites[j].x + xOff < width && sizeBy24 * i + yOff<height && s_log[yOff][xOff]!=0) {
											oldColour[g->levels[levelChosen].lines[i].sprites[j].x + xOff][sizeBy24 * i + yOff] 
											= s_log[yOff][xOff];
										}
									}
								}
							}

							else {
								for(int xOff = 0; xOff < w; xOff++) {										//draw sprite size
									for(int yOff = 0; yOff < sizeBy24; yOff++) {
										if(g->levels[levelChosen].lines[i].sprites[j].x + xOff < width && sizeBy24 * i + yOff<height && s_lilypad[yOff][xOff]!=0) {
											oldColour[g->levels[levelChosen].lines[i].sprites[j].x + xOff][sizeBy24 * i + yOff] 
											= s_lilypad[yOff][xOff];
										}
									}
								}
							}
							break;
						case 2:
							if(g->levels[levelChosen].lines[i].direction == -1) {
								for(int xOff = 0; xOff < w; xOff++) {										//draw sprite size
									for(int yOff = 0; yOff < sizeBy24; yOff++) {
										if(g->levels[levelChosen].lines[i].sprites[j].x + xOff < width && sizeBy24 * i + yOff<height && s_rocket[yOff][xOff]!=0) {
											oldColour[g->levels[levelChosen].lines[i].sprites[j].x + xOff][sizeBy24 * i + yOff] 
											= s_rocket[yOff][xOff];
										}
									}
								}
							}

							else {
								for(int xOff = 0; xOff < w; xOff++) {										//draw sprite size
									for(int yOff = 0; yOff < sizeBy24; yOff++) {
										if(g->levels[levelChosen].lines[i].sprites[j].x + xOff < width && sizeBy24 * i + yOff<height && s_satelite[yOff][xOff]!=0) {
											oldColour[g->levels[levelChosen].lines[i].sprites[j].x + xOff][sizeBy24 * i + yOff] 
											= s_satelite[yOff][xOff];
										}
									}
								}
							}
							break;
						case 3:
							if(g->levels[levelChosen].lines[i].direction == -1) {
								for(int xOff = 0; xOff < w; xOff++) {										//draw sprite size
									for(int yOff = 0; yOff < sizeBy24; yOff++) {
										if(g->levels[levelChosen].lines[i].sprites[j].x + xOff < width && sizeBy24 * i + yOff<height && s_leftLawnmower[yOff][xOff]!=0) {
											oldColour[g->levels[levelChosen].lines[i].sprites[j].x + xOff][sizeBy24 * i + yOff] 
											= s_leftLawnmower[yOff][xOff];
										}
									}
								}
							}

							else {
								for(int xOff = 0; xOff < w; xOff++) {										//draw sprite size
									for(int yOff = 0; yOff < sizeBy24; yOff++) {
										if(g->levels[levelChosen].lines[i].sprites[j].x + xOff < width && sizeBy24 * i + yOff<height && s_rightLawnmower[yOff][xOff]!=0) {
											oldColour[g->levels[levelChosen].lines[i].sprites[j].x + xOff][sizeBy24 * i + yOff] 
											= s_rightLawnmower[yOff][xOff];
										}
									}
								}
							}
							break;
					}					
				}
			}
		}
	}
} 

void drawPowerUp(Game* g) {
	for(int xOff = 0; xOff < 100; xOff++) {
		for(int yOff = 0; yOff < 30; yOff++) {
			if(g->powerUp.x + xOff < width && sizeBy24/2 * g->powerUpLine + yOff<height) {
				oldColour[g->powerUp.x + xOff][sizeBy24 * g->powerUpLine + yOff] 
				= s_valuePack[yOff][xOff];
			}
		}
	}
}

//deprecated function
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
		for(int x = 0; x<width+80; x++) {
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
		for (int x = 0; x < width+80; x++) 
			oldColour[x][y] = 0x1905;
	}
	drawTotal();
}

void drawPaused(int cursor) {
	for(int y = 0; y<570; y++) {
		for(int x = 0; x<760; x++) 
			oldColour[260+x][75+y] = s_paused[y][x];
		
	}
	//
	if(cursor==0) {
		for(int y = 0; y<100; y++) {
			for(int x = 0; x<300; x++) 
				oldColour[490+x][320+y] = s_restart[y][x];
		}
	}
	else {
		for(int y = 0; y<100; y++) {
			for(int x = 0; x<300; x++) 
				oldColour[490+x][440+y] = s_quit[y][x];
		}
	}
	//change these offsets to get it right
}


void drawMainScreen(int cursor) {
	for(int y = 0; y<530; y++) {
		for(int x = 0; x<580; x++) 
			oldColour[200+x][100+y] = s_frogMain[y][x];
		
	}
	for(int y = 0; y<220; y++) {
		for(int x = 0; x<600; x++) 
			oldColour[600+x][100+y] = s_titleMain[y][x];
		
	}
	for(int y = 0; y<210; y++) {
		for(int x = 0; x<300; x++) 
			oldColour[600+x][320+y] = s_frogMain[y][x];
		
	}
	if(cursor==0) {
		for(int y = 0; y<100; y++) {
			for(int x = 0; x<300; x++) 
				oldColour[600+x][320+y] = s_startGame[y][x];
		}
	}
	else {
		for(int y = 0; y<100; y++) {
			for(int x = 0; x<300; x++) 
				oldColour[600+x][440+y] = s_quit[y][x];
		}
	}
	//change these offsets to get it right
}

void drawWinScreen() {
	for(int y = 0; y<100; y++) {
		for(int x = 0; x<700; x++) 
			oldColour[200+x][200+y] = s_won[y][x];
		
	}
	for(int y = 0; y<350; y++) {
		for(int x = 0; x<25; x++) 
			oldColour[200+x][500+y] = s_buttonToMain[y][x];
		
	}
	for(int y = 0; y<500; y++) {
		for(int x = 0; x<450; x++) 
			oldColour[600+x][200+y] = s_trophy[y][x];
		
	}
	for(int y = 0; y<35; y++) {
		for(int x = 0; x<200; x++) 
			oldColour[400+x][400+y] = s_points[y][x];
		
	}
}

void drawLostScreen() {
	for(int y = 0; y<100; y++) {
		for(int x = 0; x<700; x++) 
			oldColour[200+x][200+y] = s_lost[y][x];
		
	}
	for(int y = 0; y<350; y++) {
		for(int x = 0; x<25; x++) 
			oldColour[200+x][500+y] = s_buttonToMain[y][x];
		
	}
	for(int y = 0; y<180; y++) {
		for(int x = 0; x<100; x++) 
			oldColour[400+x][400+y] = s_sad[y][x];
		
	}

}

void drawInfo(int t, int movesLeft, int lives) {
	for(int y = 0; y<50; y++) {
		for(int x = 0; x<80; x++) 
			oldColour[1200+x][0+y] = s_time[y][x];
	}
	drawNumber(1260, 70, t);
	for(int y = 0; y<50; y++) {
		for(int x = 0; x<80; x++) 
			oldColour[1200+x][300+y] = s_moves[y][x];
	}
	drawNumber(1260, 370, movesLeft);
	for(int y = 0; y<50; y++) {
		for(int x = 0; x<80; x++) 
			oldColour[1200+x][800+y] = s_lives[y][x];
	}
	drawNumber(1260, 870, lives);
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

	if(n/10>0) {
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

