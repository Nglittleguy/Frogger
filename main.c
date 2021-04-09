#include <wiringPi.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "initGPIO.h"
#include "pinIO.h"
#include "controller.h"
#include "framebuffer.h"
#include "game.h"
#include "draw.h"
#include "var.h"

/*
This is the main function for the 359 Project. For now, it is just used to read SNES controller inputs.
Later functionality will be based around the Frogger game - requiring a separate thread to handle the
controller inputs. For now though, this should suffice.
*/

int press;										//READ: main, WRITE: controller (up down left right start select a)
int readPress;									//READ: controller, main, WRITE: controller (1), main (0),
Game g;
int cont = 1;									//READ: controller, main, WRITE: main
const int timeLimit = 2000;
int t = timeLimit; 								//READ: main, WRITE: timingClock
int timeWait = 0;								//READ: main, controller, WRITE: main
int timeTill = 0;								//READ: main, controller, WRITE: main
int paused = 0;									//READ: main, controller, WRITE: main
int lives = 5;
int movesLeft = 500;
int resetTime = 0;
int powerUpOnScreen = 0;
int poweredUp = 0;
int points = 0; 								//READ: main, WRITE: controller (readPress), main (!readPress)

const int height = 720;
const int sizeBy24 = height/24;
const int width = 1280;
const int widthBy24 = width/24;
const int bWidth = 250;
const int bHeight = 100;

void *timingClock(void *param) {
	printf("Time %d\n", t);
	while(cont) {
		if(t==0 || movesLeft==0 || lives == 0) 
			cont = 0;							//end the game
		while(paused);
		if(resetTime) {
			//t = 500;
			resetTime = 0;
		}
		if(t%100==0 && powerUpOnScreen == 0) {
			powerUpOnScreen = 1;
		}
		t--;
		
		delayMicroseconds(100000);
	}
	pthread_exit(0);
}

void *controls(void *param) {
	//unsigned int *gpioPtr = (unsigned int *)param;

	//get gpio pointer
    unsigned int *gpioPtr = getGPIOPtr();  		//init GPIO Pointer
	inputGPIO(gpioPtr, 9);
	inputGPIO(gpioPtr, 10);
	inputGPIO(gpioPtr, 11);
	outputGPIO(gpioPtr, 9);
	outputGPIO(gpioPtr, 11);
	//initilaizing the GPIO pins

	int buttons[16] = {1};						//all button states stored in array						
	int hold[16] = {0};							//prevents multiple presses while holding (50 cycles gap)
	int pressed = 1;
	while(cont){
		readSNES(gpioPtr, buttons);				//controller functionality
		
		for(int i = 0; i<16; i++) {				//read through all button states
			//for(int j = 0; j<16; j++)
			//	printf("%d ", buttons[j]);
			
			if(buttons[i] != 1) {
				if(i==3) {						//end execution if "Start"
					cont = 0;
					printf("Program is terminating...\n");
					break;
				}
				if(hold[i]==0 && readPress)	{	//if held spinlock is released and controller turn to write, 
					printMessage(i);					//		print the message
					press = i;
					hold[i] = 1000;				//reset spinlock value
					pressed = 1;				//if pressed, rewrite the "Press a button"
					readPress = 0;				//have not read this press yet
					movesLeft--;
					if(poweredUp) 
						points+=20;
					else
						points+=10;
				}
			}
			if(hold[i]>0)						//decrease spinlock value
				hold[i]--;
			
		}

		if(pressed) {
			//printf("Please press a button...\n");
			pressed = 0;
		}
		
	}
	pthread_exit(0);
}


int main()
{
	printf("Created by: Stephen Ng 30038689\n");
	//printf("Please press a button...\n");
	

	pthread_t cThread;
	pthread_attr_t cAttr;
	pthread_attr_init(&cAttr);
	long check = pthread_create(&cThread, &cAttr, controls, NULL);
	if (check != 0) {
     	printf("Oops, pthread_create returned error code %ld\n", check);
      	exit(-1);
    }

    pthread_t clThread;
    pthread_attr_t clAttr;
    pthread_attr_init(&clAttr);
    check = pthread_create(&clThread, &clAttr, timingClock, NULL);
    if (check != 0) {
     	printf("Oops, pthread_create returned error code %ld\n", check);
      	exit(-1);
    }

	
	framebufferstruct = initFbInfo();

	generateGame(&g, width, height, widthBy24);		//ISSUE - constants not working
	int levelChosen = 0;	
	int currentLine = 23;
	int updateLevel = 0;
	int noPowerTimeYet = 1;
	int oldLine = 23;
	int goBackToLoop = 0;
	while(cont) {
		if(updateLevel != 0) {					//inbetween levels
			drawClearMem();
			if(levelChosen<3)
				levelChosen+=updateLevel;
			else {
				printf("WINNER!!!! %d - %d - %d", t, movesLeft, lives);
				cont = 0;
			}

			if(powerUpOnScreen) {
				removePowerUp(&g);
				powerUpOnScreen = 0;
			}

			updateLevel = 0;
			poweredUp = 0;
			noPowerTimeYet = 1;
			resetTime = 1;
			goBackToLoop = 0;

		}
		if(!readPress) {
			if(press == 3) {
				paused = 1;						//enter pause menu here

			}
			if(!paused) {
				oldLine = currentLine;
				currentLine = movePlayer(&g, levelChosen, width, press, widthBy24, bWidth); 
				if(oldLine == 0 && currentLine == 23){
					updateLevel = 1;
					goBackToLoop = 1;
				}
				else if(oldLine == 23 && currentLine == 0) {
					updateLevel = -1;
					goBackToLoop = 1;
				}
			}
			readPress = 1;
			if(goBackToLoop)
				continue;
		}
		if(!paused) {
			if(updateTime(&g, levelChosen, width, bWidth, widthBy24, currentLine)==0) {
				// dead
				lives--;
				moveToStart(&g, levelChosen, width, widthBy24, currentLine);
				currentLine = 23;
			}
			if(powerUpOnScreen) {
				if(collectPowerUp(&g, currentLine, levelChosen, widthBy24)) {
					removePowerUp(&g);
					powerUpOnScreen = 0;
					poweredUp = 1;
				}
			}

		}
		drawBackground(&g, levelChosen);
		drawSprites(&g, levelChosen);
		if(powerUpOnScreen) {
			if(noPowerTimeYet) {
				setUpPowerUp(&g, movesLeft, currentLine);
				noPowerTimeYet = 0;
			}
			drawPowerUp(&g);
		}
		drawTotal();
		drawTime(t, timeLimit);
		delayMicroseconds(42);

		
	}

	munmap(framebufferstruct.fptr, framebufferstruct.screenSize);
	

   	pthread_join(cThread, NULL);
   	pthread_join(clThread, NULL);

	


    return 0;
}
