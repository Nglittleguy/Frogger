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
int contGame = 0;
const int timeLimit = 2000;
int t = timeLimit; 								//READ: main, WRITE: timingClock
int timeWait = 0;								//READ: main, controller, WRITE: main
int timeTill = 0;								//READ: main, controller, WRITE: main
int paused = 0;									//READ: main, controller, WRITE: main
int mainScreen = 1;								//READ: main, controller, WRITE: main
int lives = 5;
int movesLeft = 500;
int resetTime = 0;
int powerUpOnScreen = 0;
int poweredUp = 0;
int points = 0; 								//READ: main, WRITE: controller (readPress), main (!readPress)
int won = 0;						//winning flag is 1, losing is 2, skip is 0

const int height = 720;
const int sizeBy24 = height/24;
const int width = 1280;
const int widthBy24 = width/24;
const int bWidth = 250;
const int bHeight = 100;

void *timingClock(void *param) {
	printf("Time %d\n", t);
	while(cont) {

		if((t==0 || movesLeft==0 || lives == 0) && contGame) {
			printf("YOU LOSE - %d, %d, %d \n", t, movesLeft, lives);
			contGame = 0;						//end the game
			won = 2;
		}
		while(paused);
		if(resetTime) {
			printf("Resetting time\n");
			t = timeLimit;	
			movesLeft = 500;
			lives = 5;
			resetTime = 0;
		}
		if(t<2000 && t%100==0 && powerUpOnScreen == 0) {
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
				// if(i==3) {						//end execution if "Start"
				// 	cont = 0;
				// 	printf("Program is terminating...\n");
				// 	break;
				// }
				if(hold[i]==0 && readPress)	{	//if held spinlock is released and controller turn to write, 
					printMessage(i);			//		print the message
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

	
	int levelChosen;	
	int currentLine;
	int updateLevel;
	int noPowerTimeYet;
	int oldLine;
	int goBackToLoop;
	int cursor = 0;
	int restart = 0;

	while(cont) {
		while(mainScreen) {
			//drawMainScreen(cursor);

			if(!readPress) {
				printf("Main menu, waiting for...\n");
				printMessage(press);
				if(press==8) {			//press A
					if(cursor) {		//Quit
						drawClearMem();
						drawBlank();
						contGame = 0;
						cont = 0;
						mainScreen = 0;
						printf("Leave game:\n");
					}
					else {				//Start Game
						contGame = 1;
						mainScreen = 0;
						printf("start game:\n");
					}
					restart = 0;
				}
				else if(press==4){	//press Up 
					printf("Start game\n");
					if(cursor==1)
						cursor=0;
				}
				else if(press==5) {	//press Down
					printf("Quit game\n");
					if(cursor==0)
						cursor=1;
				}
				readPress = 1;
			}	
		}

		//only called if !mainScreen
		if(restart) {
			restart = 0;
			contGame = 1;
			printf("RESTARTING\n");
		}

		paused = 0;
		resetTime = 1;
		delayMicroseconds(100000);
		levelChosen = 0;
		currentLine = 23;
		updateLevel = 0;
		noPowerTimeYet = 1;
		oldLine = 23;
		goBackToLoop = 0;
		generateGame(&g, width, height, widthBy24);
		printf("I am winner! %d\n", won);
		while(contGame) {
			//In between Levels
			if(updateLevel != 0) {	
				drawClearMem();
				if(levelChosen<3)
					levelChosen+=updateLevel;
				else {
					//Congratulation, Winner!
					printf("WINNER!!!! %d - %d - %d\n", t, movesLeft, lives);
					if(contGame) {						//only 1 can change won flag (mutex)
						won = 1;
						contGame = 0;
					}
				}

				if(powerUpOnScreen) {
					removePowerUp(&g);
					powerUpOnScreen = 0;
				}

				updateLevel = 0;
				poweredUp = 0;
				noPowerTimeYet = 1;
				goBackToLoop = 0;

			}

			//Reading an input
			if(!readPress) {
				if(press == 3) {
					if(paused == 1)
						paused = 0;
					else {
						paused = 1;						//enter pause menu here
						cursor = 0;
					}
				}

				//not paused
				if(!paused) {
					if(press!=3) {
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
				}
				//is paused
				else {
					printf("Paused: \n");
					if(press==8) {		//press A
						if(cursor) {		//Quit
							printf("Pressed quit:\n");
							contGame = 0;
							mainScreen = 1;
							cursor = 0;
						}
						else {				//Restart
							printf("Pressed restart:\n");
							contGame = 0;
							restart = 1;
							mainScreen = 0;
							cursor = 0;
						}
					}
					else if(press==4){	//press Up
						if(cursor==1) {
							printf("Restart\n");
							cursor=0;
						}
					}
					else if(press==5) {	//press Down
						if(cursor==0) {
							printf("Quit\n");
							cursor=1;
						}
					}
				}
				readPress = 1;
				if(goBackToLoop)
					continue;
			}

			//Responding to updates
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

				//Draw the screen
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
			}
			else {
				//paused menu screen draw
			}

			

			//Wait for next 1/24 
			delayMicroseconds(42);
		}

		if(won!=0) {
			if(won==1) {		//won the game
				mainScreen = 1;
				points += t+movesLeft+100*lives;
				printf("Winner - won down there\n");
				//DRAW STATS OF GAME WIN
				
			}
			else if(won==2) {	//lost the game
				//DRAW LOSER SCREEN
				printf("Loser!! Haha!\n");
				mainScreen = 1;
			}
			resetTime = 1;
			while(readPress);	//stay here until button press
			readPress = 1;
		}
		
		won = 0;

	}

	munmap(framebufferstruct.fptr, framebufferstruct.screenSize);
	

   	pthread_join(cThread, NULL);
   	pthread_join(clThread, NULL);

	


    return 0;
}
