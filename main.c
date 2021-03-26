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
const int timeLimit = 500;
int t = timeLimit;
int paused = 0;				

const int height = 500;
const int sizeBy12 = height/12;
const int width = 1000;
const int widthBy24 = width/24;
const int bWidth = 250;
const int bHeight = 250;

void *timingClock(void *param) {
	while(cont) {
		if(t==0) 
			cont = 0;							//end the game
		while(paused);
		t--;
		delayMicroseconds(10000);
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
				}
			}
			if(hold[i]>0)						//decrease spinlock value
				hold[i]--;
			
		}

		if(pressed) {
			printf("Please press a button...\n");
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

	while(cont) {
		if(!readPress) {
			if(press == 3) {
				paused = 1;						//enter pause menu here

			}
			if(!paused) {
				movePlayer(&g, levelChosen, width, press, widthBy24, bWidth); //ISSUE - constants not working
			}
			readPress = 1;
		}
		if(!paused) {
			updateTime(&g, levelChosen, width, bWidth);
		}
		drawBackground(&g, levelChosen);
		drawSprites(&g, levelChosen);
		drawTime(t, timeLimit);
		delayMicroseconds(42);
	}

	munmap(framebufferstruct.fptr, framebufferstruct.screenSize);
	

   	pthread_join(cThread, NULL);
   	pthread_join(clThread, NULL);

	


    return 0;
}
