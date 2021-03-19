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
void drawBackground(Game *g, int levelChosen);
void drawSprites(Game *g, int levelChosen);

static const int height;
static int sizeBy12;
static const int width;
static int widthBy24;
static const int bWidth;
static const int bHeight;
