#include <wiringPi.h>
#include <stdio.h>
#include "pinIO.h"

/*
Writes states of SNES latch into the array - arguments; gpioBase address, and buttons array
- algorithm was taken from class lectures
*/
void readSNES(unsigned* gpioBase, int* buttons) {
	writeClock(gpioBase, 1);
	writeLatch(gpioBase, 1);

	delayMicroseconds(12);
	writeLatch(gpioBase, 0);

	for(int i = 0; i<16; i++) {				//repeat 16 times, once per button
		delayMicroseconds(6);
		writeClock(gpioBase, 0);
		delayMicroseconds(6);
		buttons[i]=readData(gpioBase);		//reading data into buttons array
		writeClock(gpioBase, 1);
	}
}

/*
Writes the name of the button that is pressed - arguments: the number of the button pressed
*/
void printMessage(int i) {
	printf("You have pressed ");
	switch(i) {
		case 0:
			printf("B");
			break;
		case 1:
			printf("Y");
			break;
		case 2:
			printf("Select");
			break;
		case 3:
			printf("Start");
			break;
		case 4:
			printf("Joy-pad UP");
			break;
		case 5:
			printf("Joy-pad DOWN");
			break;
		case 6:
			printf("Joy-pad LEFT");
			break;
		case 7:
			printf("Joy-pad RIGHT");
			break;
		case 8:
			printf("A");
			break;
		case 9:
			printf("X");
			break;
		case 10:
			printf("Left Trigger");
			break;
		case 11:
			printf("Right Trigger");
			break;
		default:
			printf("a button that doesn't exist...");
			break;
	}
	printf("\n");
}



