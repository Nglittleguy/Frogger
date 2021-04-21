#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "framebuffer.h"
#include "game.h"
#include "draw.h"
#include "var.h"
#include <string.h>

/*
Program to draw to screen - draw.c
- modified from DrawBlock
- Stephen Ng 30038689
*/

//Buffer of screen output (to prevent flicker)
int oldColour[1280][720];

//Draw the screen buffer to the screen
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

//Draw the background of the current level
void drawBackground(Game *g, int levelChosen) {

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) 
			//colour as defined by game array
			oldColour[x][y] = g->levels[levelChosen].lines[y/sizeBy24].colour;
	}

}

//Draw every sprite
void drawSprites(Game *g, int levelChosen) {
	int w;
	for(int i = 0; i<24; i++) {
		for(int j = 0; j<10; j++) {
			//for all sprites
			//only draw if code not null 
			if(g->levels[levelChosen].lines[i].sprites[j].code != 0) {
				//drawing the frog
				if(j==9) {
					w = 50; 	//widthBy24;
					for(int xOff = 0; xOff < w; xOff++) {				
						for(int yOff = 0; yOff < sizeBy24; yOff++) {
							//if within bounds and non-black pixel (transparency)
							if(g->levels[levelChosen].lines[i].sprites[j].x + xOff < width && 
								sizeBy24 * i + yOff < height && 
								s_frog[yOff][xOff]!=0) {

								//set buffer to be sprite RGB code (var.c)
								oldColour[g->levels[levelChosen].lines[i].sprites[j].x + xOff][sizeBy24 * i + yOff] 
									= s_frog[yOff][xOff];
							}
						}
					}
				}
				//drawing not the frog
				else {
					w = 100; 	//widthBy24*2;
					switch(levelChosen) {
						//level 0 (red car and yellow car)
						case 0:
							if(g->levels[levelChosen].lines[i].direction == -1) {
								//moving to the left (carLeft)
								for(int xOff = 0; xOff < w; xOff++) {			
									for(int yOff = 0; yOff < sizeBy24; yOff++) {
										//if within bounds and non-black pixel
										if(g->levels[levelChosen].lines[i].sprites[j].x + xOff < width && 
											sizeBy24 * i + yOff<height && 
											s_carLeft[yOff][xOff]!=0) {

											//set buffer to be sprite RGB code (var.c)
											oldColour[g->levels[levelChosen].lines[i].sprites[j].x + xOff][sizeBy24 * i + yOff] 
												= s_carLeft[yOff][xOff];
										}
									}
								}
							}
							else {
								//moving to the right (carRight)
								for(int xOff = 0; xOff < w; xOff++) {			
									for(int yOff = 0; yOff < sizeBy24; yOff++) {
										//if within bounds and non-black pixel
										if(g->levels[levelChosen].lines[i].sprites[j].x + xOff < width && 
											sizeBy24 * i + yOff<height && 
											s_carRight[yOff][xOff]!=0) {

											//set buffer to be sprite RGB code (var.c)
											oldColour[g->levels[levelChosen].lines[i].sprites[j].x + xOff][sizeBy24 * i + yOff] 
												= s_carRight[yOff][xOff];
										}
									}
								}
							}
							break;
						//level 1 (log and lilypad)
						case 1:
							if(g->levels[levelChosen].lines[i].direction == -1) {
								//moving to the left (log)
								for(int xOff = 0; xOff < w; xOff++) {			
									for(int yOff = 0; yOff < sizeBy24; yOff++) {
										//if within bounds and non_black pixel
										if(g->levels[levelChosen].lines[i].sprites[j].x + xOff < width && 
											sizeBy24 * i + yOff<height && 
											s_log[yOff][xOff]!=0) {

											//set buffer to be sprite RGB code (var.c)
											oldColour[g->levels[levelChosen].lines[i].sprites[j].x + xOff][sizeBy24 * i + yOff] 
												= s_log[yOff][xOff];
										}
									}
								}
							}
							else {
								//moving to the right (lilypad)
								for(int xOff = 0; xOff < w; xOff++) {				
									for(int yOff = 0; yOff < sizeBy24; yOff++) {
										//if within bounds and non-black pixel
										if(g->levels[levelChosen].lines[i].sprites[j].x + xOff < width && 
											sizeBy24 * i + yOff<height && 
											s_lilypad[yOff][xOff]!=0) {
											
											//set buffer to be sprite RGB code (var.c)
											oldColour[g->levels[levelChosen].lines[i].sprites[j].x + xOff][sizeBy24 * i + yOff] 
												= s_lilypad[yOff][xOff];
										}
									}
								}
							}
							break;
						//level 2 (rocket and satelite)
						case 2:
							if(g->levels[levelChosen].lines[i].direction == -1) {
								//moving to the left (rocket)
								for(int xOff = 0; xOff < w; xOff++) {				
									for(int yOff = 0; yOff < sizeBy24; yOff++) {
										//if within bounds and non-black pixel
										if(g->levels[levelChosen].lines[i].sprites[j].x + xOff < width && 
											sizeBy24 * i + yOff<height && 
											s_rocket[yOff][xOff]!=0) {
											
											//set buffer to be sprite RGB code (var.c)
											oldColour[g->levels[levelChosen].lines[i].sprites[j].x + xOff][sizeBy24 * i + yOff] 
												= s_rocket[yOff][xOff];
										}
									}
								}
							}
							else {
								//moving to the right (satelite)
								for(int xOff = 0; xOff < w; xOff++) {				
									for(int yOff = 0; yOff < sizeBy24; yOff++) {
										//if within bounds and non-black pixel
										if(g->levels[levelChosen].lines[i].sprites[j].x + xOff < width && 
											sizeBy24 * i + yOff<height && 
											s_satelite[yOff][xOff]!=0) {
											
											//set buffer to be sprite RGB code (var.c)
											oldColour[g->levels[levelChosen].lines[i].sprites[j].x + xOff][sizeBy24 * i + yOff] 
												= s_satelite[yOff][xOff];
										}
									}
								}
							}
							break;
						//level 3 (red lawnmower and green lawnmower)
						case 3:
							if(g->levels[levelChosen].lines[i].direction == -1) {
								//moving to the left (green lawnmower)
								for(int xOff = 0; xOff < w; xOff++) {										//draw sprite size
									for(int yOff = 0; yOff < sizeBy24; yOff++) {
										//if within bounds and non-black pixel
										if(g->levels[levelChosen].lines[i].sprites[j].x + xOff < width && 
											sizeBy24 * i + yOff<height && 
											s_leftLawnmower[yOff][xOff]!=0) {
											
											oldColour[g->levels[levelChosen].lines[i].sprites[j].x + xOff][sizeBy24 * i + yOff] 
												= s_leftLawnmower[yOff][xOff];
										}
									}
								}
							}
							else {
								//moving to the right (red lawnmower)
								for(int xOff = 0; xOff < w; xOff++) {										//draw sprite size
									for(int yOff = 0; yOff < sizeBy24; yOff++) {
										//if within bounds and non-black pixel
										if(g->levels[levelChosen].lines[i].sprites[j].x + xOff < width && 
											sizeBy24 * i + yOff<height && 
											s_rightLawnmower[yOff][xOff]!=0) {
											
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

//Draw sprite of the value pack
void drawPowerUp(Game* g) {
	for(int xOff = 0; xOff < 50; xOff++) {
		for(int yOff = 0; yOff < 30; yOff++) {
			//if within bounds
			if(g->powerUp.x + xOff < width && 
				sizeBy24/2 * g->powerUpLine + yOff < height) {

				//set buffer to be sprite RGB code (var.c)
				oldColour[g->powerUp.x + xOff][sizeBy24 * g->powerUpLine + yOff] 
					= s_valuePack[yOff][xOff];
			}
		}
	}
}

//Draws line based on length of limit - Deprecated Function
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

//Clears the frame buffer to all 0s
void drawBlank() {
	Pixel *pixel;
	pixel = malloc(sizeof(Pixel));
	//for every pixel within bounds
	for(int y = 0; y<height; y++) {
		for(int x = 0; x<width+80; x++) {
			pixel->x = x+bWidth;
			pixel->y = y+bHeight;
			//set to 0
			pixel->colour = 0;
			drawPixel(pixel);
		}
	}	
}

//Clear the buffer of screen output
void drawClearMem() {
	memset(oldColour, 0, sizeof(oldColour));
}

//Sets all pixels in buffer to a dark blue
void drawBackgroundBlue() {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width+80; x++) 
			oldColour[x][y] = 0x1905;
	}
	drawTotal();
}

//Draws the pause menu
void drawPaused(int cursor) {
	for(int y = 0; y<570; y++) {
		for(int x = 0; x<760; x++) 
			//set buffer to be sprite RGB code (var.c)
			oldColour[260+x][75+y] = s_paused[y][x];
	}
	
	//draw over the selected item with bolded box (selection)
	if(cursor==0) {
		for(int y = 0; y<100; y++) {
			for(int x = 0; x<300; x++) 
				//set buffer to be sprite RGB code (var.c)
				oldColour[490+x][320+y] = s_restart[y][x];
		}
	}
	else {
		for(int y = 0; y<100; y++) {
			for(int x = 0; x<300; x++) 
				//set buffer to be sprite RGB code (var.c)
				oldColour[490+x][440+y] = s_quit[y][x];
		}
	}
}


//Draws the main menu
void drawMainScreen(int cursor) {
	//Draw frog image
	for(int y = 0; y<530; y++) {
		for(int x = 0; x<580; x++) 
			//set buffer to be sprite RGB code (var.c)
			oldColour[x][200+y] = s_frogMain[y][x];
	}
	//Draw title with my name
	for(int y = 0; y<220; y++) {
		for(int x = 0; x<600; x++)
			//set buffer to be sprite RGB code (var.c) 
			oldColour[600+x][100+y] = s_titleMain[y][x];
		
	}
	//Draw selection (start, quit)
	for(int y = 0; y<210; y++) {
		for(int x = 0; x<300; x++)
			//set buffer to be sprite RGB code (var.c) 
			oldColour[700+x][370+y] = s_optionMain[y][x];
	}
	//draw over the selected item with bolded box (selection)
	if(cursor==0) {
		for(int y = 0; y<100; y++) {
			for(int x = 0; x<300; x++) 
				//set buffer to be sprite RGB code (var.c) 
				oldColour[700+x][365+y] = s_startGame[y][x];
		}
	}
	else {
		for(int y = 0; y<100; y++) {
			for(int x = 0; x<300; x++) 
				//set buffer to be sprite RGB code (var.c) 
				oldColour[700+x][480+y] = s_quit[y][x];
		}
	}
}

//Draws the win screen
void drawWinScreen() {
	//Draw the "Game Won" 
	for(int y = 0; y<100; y++) {
		for(int x = 0; x<700; x++) 
			//set buffer to be sprite RGB code (var.c) 
			oldColour[50+x][150+y] = s_won[y][x];
	}
	//Draws the explanation to press a button to continue
	for(int y = 0; y<25; y++) {
		for(int x = 0; x<350; x++) 
			//set buffer to be sprite RGB code (var.c) 
			oldColour[200+x][600+y] = s_buttonToMain[y][x];
	}
	//Draws the trophy image
	for(int y = 0; y<500; y++) {
		for(int x = 0; x<450; x++) 
			//set buffer to be sprite RGB code (var.c) 
			oldColour[740+x][150+y] = s_trophy[y][x];
	}
	//Draws the "Total Points"
	for(int y = 0; y<35; y++) {
		for(int x = 0; x<200; x++) 
			//set buffer to be sprite RGB code (var.c) 
			oldColour[200+x][400+y] = s_points[y][x];
	}
}

//Draws aht elost screen
void drawLostScreen() {
	//Draws the "Game Lost"
	for(int y = 0; y<100; y++) {
		for(int x = 0; x<700; x++) 
			//set buffer to be sprite RGB code (var.c) 
			oldColour[290+x][200+y] = s_lost[y][x];
	}
	//Draws the explanation to press a button to continue
	for(int y = 0; y<25; y++) {
		for(int x = 0; x<350; x++) 
			//set buffer to be sprite RGB code (var.c) 
			oldColour[465+x][600+y] = s_buttonToMain[y][x];
	}
	//Draws the ":("
	for(int y = 0; y<100; y++) {
		for(int x = 0; x<180; x++) 
			//set buffer to be sprite RGB code (var.c) 
			oldColour[550+x][400+y] = s_sad[y][x];
	}
}

//Draws the information on the side of the game
void drawInfo(int t, int movesLeft, int lives) {
	//Draws "Time left"
	for(int y = 0; y<50; y++) {
		for(int x = 0; x<80; x++) 
			oldColour[1200+x][0+y] = s_time[y][x];
	}
	//Draws the time/10 (seconds remaining)
	drawNumber(1250, 70, t/10);
	//Draws "Moves left"
	for(int y = 0; y<50; y++) {
		for(int x = 0; x<80; x++) 
			oldColour[1200+x][200+y] = s_moves[y][x];
	}
	//Draws the number of moves left
	drawNumber(1250, 270, movesLeft);
	//Draws "Lives"
	for(int y = 0; y<50; y++) {
		for(int x = 0; x<80; x++) 
			oldColour[1200+x][400+y] = s_lives[y][x];
	}
	//Draws the number of lives left
	drawNumber(1240, 470, lives);
}

//Draws a number given a x and y starting coordinate
void drawNumber(int x, int y, int n) {
	//current digit to draw is dig
	int dig = n%10;
	//draw the correct sprite - set buffer to be sprite RGB code (var.c) 
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
	//remove the last digit and draw the next number shifted over
	if(n/10>0) {
		drawNumber(x-20, y, n/10);
	}
}

/* Draw a pixel - from DrawBlock*/
void drawPixel(Pixel *pixel){
	long int location = (pixel->x +framebufferstruct.xOff) * (framebufferstruct.bits/8) +
      	(pixel->y+framebufferstruct.yOff) * framebufferstruct.lineLength;
    if(*((unsigned short int*)(framebufferstruct.fptr + location))!=pixel->colour)	{		
    	//only draw if different
		*((unsigned short int*)(framebufferstruct.fptr + location)) = pixel->colour;
	}
}

