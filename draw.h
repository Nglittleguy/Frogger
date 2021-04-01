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

int oldColour[1000][720];
struct fbs framebufferstruct;
void drawPixel(Pixel *pixel);
void drawBackground(Game *g, int levelChosen);
void drawSprites(Game *g, int levelChosen);
void drawTime(int t, int tl);
void drawTotal();

