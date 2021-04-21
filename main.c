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
This is the main function for the 359 Project - the Frogger game 
- separate threads to keep track of time, controller inputs, and game output
- Stephen Ng 30038689
*/

//////////////////////////////////////////////////////////////////////
//Global variables to share between threads
Game g;
int won = 0;									//winning flag is 1, losing is 2, skip is 0

int press;										//READ: main, WRITE: controller (up down left right start select a)
int readPress;									//flag to coordinate thread (mutex) controller (1), main (0),
int cont = 1;									//READ: controller, main, WRITE: main
int contGame = 0;
const int timeLimit = 2000;
int tEnter = 2000;
int t = timeLimit; 								//READ: main, WRITE: timingClock
int lives = 5;									
int movesLeft = 400;

int paused = 0;									//READ: main, controller, WRITE: main
int mainScreen = 1;								//READ: main, controller, WRITE: main
int resetTime = 0;								//flag to coordinate thread (mutex)

int powerUpOnScreen = 0;
int poweredUp = 0;
int points = 0; 								//READ: main, WRITE: controller (readPress), main (!readPress)
//////////////////////////////////////////////////////////////////////

/*
Thread to update the time every second (checks every 10th of a second)
*/
void *timingClock(void *param) {
	while(cont) {
		//check if end the game
		if((t==0 || movesLeft==0 || lives == 0) && contGame) {
			contGame = 0;	
			won = 2;
		}
		//if paused, wait and don't update
		while(paused);
		//if required to reset values to initial values
		if(resetTime) {
			t = timeLimit;	
			movesLeft = 400;
			lives = 5;
			resetTime = 0;
		}
		//if the value pack should be introduced
		if(t < tEnter && powerUpOnScreen == 0) {
			powerUpOnScreen = 1;
		}

		//decrement the time remaining
		t--;
		
		//wait for 0.1s
		delayMicroseconds(100000);
	}
	pthread_exit(0);
}

/*
Thread to read the controller inputs when a button is pressed
*/
void *controls(void *param) {
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
			if(buttons[i] != 1) {
				if(hold[i]==0 && readPress)	{	//if held spinlock is released and controller turn to write, 
					press = i;
					hold[i] = 1000;				//reset spinlock value
					pressed = 1;				//if pressed, rewrite the "Press a button"
					readPress = 0;				//have not read this press yet
					movesLeft--;
				}
			}
			if(hold[i]>0)						//decrease spinlock value
				hold[i]--;
		}

		if(pressed) 
			pressed = 0;
	}
	pthread_exit(0);
}

/*
Main execution thread for the game
*/
int main()
{
	printf("Created by: Stephen Ng 30038689\n");

	//Create the controller thread
	pthread_t cThread;
	pthread_attr_t cAttr;
	pthread_attr_init(&cAttr);
	long check = pthread_create(&cThread, &cAttr, controls, NULL);
	if (check != 0) {
     	printf("Oops, pthread_create returned error code %ld\n", check);
      	exit(-1);
    }

    //Create the clock thread
    pthread_t clThread;
    pthread_attr_t clAttr;
    pthread_attr_init(&clAttr);
    check = pthread_create(&clThread, &clAttr, timingClock, NULL);
    if (check != 0) {
     	printf("Oops, pthread_create returned error code %ld\n", check);
      	exit(-1);
    }

	//initialize the frame buffer
	framebufferstruct = initFbInfo();

	//local variables to keep track of game state
	int levelChosen;	
	int currentLine;
	int updateLevel;
	int noPowerTimeYet;
	int oldLine;
	int goBackToLoop;
	int cursor = 0;
	int restart = 0;

	//loop until quit program
	while(cont) {
		drawBackgroundBlue();								//draw screen blue - active
		//while on th main screen
		while(mainScreen) {
			drawMainScreen(cursor);							//draw the main screen
			drawTotal();

			//if a button is read by the controller thread
			if(!readPress) {		
				if(press==8) {								//press A
					if(cursor) {							//"Quit"
						drawClearMem();						//clear buffer
						drawBlank();						//clear screen
						contGame = 0;						//end game
						cont = 0;							//end program
						mainScreen = 0;						//end main screen
					}
					else {									//"Start Game"
						contGame = 1;						//start game
						mainScreen = 0;						//end main screen
					}
					restart = 0;						
				}
				else if(press==4){							//press Up ("Start Game")
					if(cursor==1)
						cursor=0;
				}
				else if(press==5) {							//press Down ("Quit")
					if(cursor==0)
						cursor=1;
				}
				readPress = 1;								//input read, back to controller
			}	
		}

		//only called if !mainScreen
		if(restart) {
			restart = 0;
			contGame = 1;									//start game
		}

		//setting default values
		paused = 0;
		resetTime = 1;
		tEnter = t-300;
		delayMicroseconds(100000);	
		levelChosen = 0;
		currentLine = 23;
		updateLevel = 0;
		noPowerTimeYet = 1;
		oldLine = 23;
		goBackToLoop = 0;
		//create a new game state
		generateGame(&g, width, height, widthBy24);

		while(contGame) {
			//In between Levels
			if(updateLevel != 0) {	
				drawClearMem();
				drawBackgroundBlue();						//re draw blank active screen
				//if press up while already in level 3 (winner)
				if(updateLevel==1 && levelChosen==3) {
					won = 1;
					contGame = 0;							//end game, and you won
				}
				else
					levelChosen+=updateLevel;				//update the level (either up or down)

				if(powerUpOnScreen) {
					removePowerUp(&g);						//remove power up in next stage
					powerUpOnScreen = 0;
				}

				updateLevel = 0;
				poweredUp = 0;
				noPowerTimeYet = 1;
				goBackToLoop = 0;
				tEnter = t-300;
			}

			//Reading an input
			if(!readPress) {
				if(press == 3) {							//press start
					if(paused == 1)							//if paused, unpause
						paused = 0;
					else {									//if unpaused, pause (set cursor to 0)
						paused = 1;	
						cursor = 0;
					}
				}

				//not paused
				if(!paused) {
					if(press!=3) {
						oldLine = currentLine;				//keep track of old line
						currentLine = movePlayer(&g, levelChosen, width, press, widthBy24, bWidth); 
															//update with new line
						if(oldLine == 0 && currentLine == 23){
							//going up a level
							updateLevel = 1;
							goBackToLoop = 1;
						}
						else if(oldLine == 23 && currentLine == 0) {
							//going down a level
							updateLevel = -1;
							goBackToLoop = 1;
						}
					}
				}
				//is paused
				else {
					if(press==8) {							//press A
						if(cursor) {						//"Quit"
							contGame = 0;
							mainScreen = 1;
							cursor = 0;
						}
						else {								//"Restart"
							contGame = 0;
							restart = 1;
							mainScreen = 0;
							cursor = 0;
						}
					}
					else if(press==4){						//press Up (Restart)
						if(cursor==1) {						
							cursor=0;
						}
					}
					else if(press==5) {						//press Down (Quit Game)
						if(cursor==0) {
							cursor=1;
						}
					}
				}
				readPress = 1;								//input read, back to controller
				if(goBackToLoop)							//don't draw if level is outdated
					continue;
			}

			//Responding to updates
			if(!paused) {
				if(updateTime(&g, levelChosen, width, bWidth, widthBy24, currentLine)==0) {
					// frog dies: loses a life, moves to middle of first line of level
					lives--;
					moveToStart(&g, levelChosen, width, widthBy24, currentLine);
					currentLine = 23;
				}
				if(powerUpOnScreen) {
					if(collectPowerUp(&g, currentLine, levelChosen, widthBy24)) {
						// collected a power up: remove power up, add a life, add 10s
						removePowerUp(&g);
						powerUpOnScreen = 0;
						poweredUp = 1;
						lives++;
						t+=100;
						tEnter = t-300;
					}
				}

				//Draw the screen
				drawBackground(&g, levelChosen);
				drawSprites(&g, levelChosen);

				if(powerUpOnScreen) {
					if(noPowerTimeYet) {
						//set up the location of the power up if it is not on screen yet
						setUpPowerUp(&g, movesLeft, currentLine);
						noPowerTimeYet = 0;
					}
					//draw the power up if it is supposed to be on screen
					drawPowerUp(&g);
				}
				drawInfo(t, movesLeft, lives);
				drawTotal();
				//drawTime(t, timeLimit);
			}
			else {
				drawPaused(cursor);
				drawTotal();
				//paused menu screen draw
			}

			//Wait for next 1/24 
			delayMicroseconds(42);
		}

		//end of game
		if(won!=0) { 										//either won or lost, not just skipping
			drawBackgroundBlue();
			if(won==1) {									//won the game
				mainScreen = 1;
				points = t+movesLeft+100*lives;				//calculate points
				drawWinScreen();
				drawNumber(600, 400, points);				//draw points calculated
			}
			else if(won==2) {								//lost the game
				mainScreen = 1;
				drawLostScreen();
			}
			drawTotal();
			resetTime = 1;									//reset the values
			while(readPress);								//stay here until button press
			readPress = 1;									//input read, back to controller
		}
		
		won = 0;

	}

	//cleanup
	munmap(framebufferstruct.fptr, framebufferstruct.screenSize);

   	pthread_join(cThread, NULL);
   	pthread_join(clThread, NULL);

    return 0;
}
