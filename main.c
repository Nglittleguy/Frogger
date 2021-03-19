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

/*
This is the main function for the 359 Project. For now, it is just used to read SNES controller inputs.
Later functionality will be based around the Frogger game - requiring a separate thread to handle the
controller inputs. For now though, this should suffice.
*/

int press;										//READ: main, WRITE: controller (up down left right start select a)
int readPress;									//READ: controller, main, WRITE: controller (1), main (0),
Game g;
// int cont = 1;									//READ: controller, main, WRITE: main


// void *controls(void *param) {
// 	unsigned int *gpioPtr = (unsigned int *)param;
// 	int buttons[16] = {1};						//all button states stored in array						
// 	int hold[16] = {0};							//prevents multiple presses while holding (50 cycles gap)
// 	int pressed = 1;
// 	while(cont){
// 		readSNES(gpioPtr, buttons);				//controller functionality
		
// 		for(int i = 0; i<16; i++) {				//read through all button states
// 			//for(int j = 0; j<16; j++)
// 			//	printf("%d ", buttons[j]);
			
// 			if(buttons[i] != 1) {
				
// 				if(i==3) {						//end execution if "Start"
// 					cont = 0;
// 					printf("Program is terminating...\n");
// 					break;
// 				}
// 				if(hold[i]==0 && readPress)	{	//if held spinlock is released and controller turn to write, 
// 												//		print the message
// 					press = i;
// 					hold[i] = 1000;				//reset spinlock value
// 					pressed = 1;				//if pressed, rewrite the "Press a button"
// 					readPress = 0;				//have not read this press yet
// 				}
// 			}
// 			if(hold[i]>0)						//decrease spinlock value
// 				hold[i]--;
			
// 		}

// 		if(pressed) {
// 			printf("Please press a button...\n");
// 			pressed = 0;
// 		}
		
// 	}
// 	pthread_exit(0);
// }

int main()
{
	printf("Created by: Stephen Ng 30038689\n");
	printf("Please press a button...\n");
	// get gpio pointer
 //    unsigned int *gpioPtr = getGPIOPtr();  		//init GPIO Pointer
	// inputGPIO(gpioPtr, 9);
	// inputGPIO(gpioPtr, 10);
	// inputGPIO(gpioPtr, 11);
	// outputGPIO(gpioPtr, 9);
	// outputGPIO(gpioPtr, 11);
	// //initilaizing the GPIO pins

	// pthread_t cThread;
	// pthread_attr_t cAttr;
	// pthread_attr_init(&cAttr);
	// long check = pthread_create(&cThread, &cAttr, controls, (void*)gpioPtr);
	// if (check != 0) {
 //     	printf("Oops, pthread_create returned error code %ld\n", check);
 //      	exit(-1);
 //    }
	
	framebufferstruct = initFbInfo();
	
	generateGame(&g, widthBy24, sizeBy12, width);
	int levelChosen = 0;	

	// while(cont) {
	// 	if(!readPress) {
	// 		movePlayer(&g, levelChosen, width, press, widthBy24, bWidth);
	// 		readPress = 1;
	// 	}
		
		drawBackground(&g, levelChosen);
		drawSprites(&g, levelChosen);
		//delayMicroseconds(42);
	//}

	munmap(framebufferstruct.fptr, framebufferstruct.screenSize);
	

   // pthread_join(cThread, NULL);

	


    return 0;
}
