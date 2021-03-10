#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int main(){
	Game g;
	generateGame(&g);

	for(int i = 0; i<4; i++) {
	    printf("------------------");
	    printf("Level %d:\n", g.levels[i].code);
	    for(int j = 0; j<12; j++) {
	        printf("Line %x: point in direction: %d\n", g.levels[i].lines[j].color, g.levels[i].lines[j].direction);
	        // for(int k = 0; k<10; k++) {
	        //     printf("(S %d: %d, %d, %d, %d), ", 
	        //     g.levels[i].lines[j].sprites[k].code, 
	        //     g.levels[i].lines[j].sprites[k].x, 
	        //     g.levels[i].lines[j].sprites[k].y, 
	        //     g.levels[i].lines[j].sprites[k].h, 
	        //     g.levels[i].lines[j].sprites[k].w);
	        // }
	        printf("\n");
	    }
	}
}