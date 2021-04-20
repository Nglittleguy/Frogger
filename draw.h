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

int oldColour[1280][720];
struct fbs framebufferstruct;
void drawPixel(Pixel *pixel);
void drawBackground(Game *g, int levelChosen);
void drawSprites(Game *g, int levelChosen);
void drawPowerUp(Game *g);
void drawTime(int t, int tl);
void drawTotal();
void drawBlank();
void drawClearMem();
void drawBackgroundBlue();
void drawNumber(int x, int y, int n);
void drawPaused(int cursor);
void drawMainScreen(int cursor);
void drawWinScreen();
void drawLostScreen();
void drawInfo(int t, int movesLeft, int lives);
