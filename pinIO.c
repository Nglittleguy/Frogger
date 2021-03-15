#include <stdio.h>
#include "pinIO.h"

/*
Sets the pin value to high - arguments: gpio base address, and the pin to set to 1
*/
void setVal(unsigned* gpioBase, int pin) {			//sets the pin to high
	gpioBase[7] = 1<<pin;
}

/*
Sets the pin value to low - arguments: gpio base address, and the pin to set to 0
*/
void clrVal(unsigned* gpioBase, int pin) {			//sets the pins to low
	gpioBase[10] = 1<<pin;
}

/*
Reads value of this pin - arguments: gpio base address, and the pin to read
*/
int readVal(unsigned* gpioBase, int pin) {			//returns value of GPLEV0 at pin
	return (gpioBase[13] >> pin) & 1;
}

/*
Writes this value to the latch pin - arguments: gpio base address, value to set latch
*/
void writeLatch(unsigned* gpioBase, int in) {		//writes to latch
	if(in==0)
		clrVal(gpioBase, 9);
	else
		setVal(gpioBase, 9);
}

/*
Writes this value to the clock pin - arguments: gpio base address, value to set clock
*/
void writeClock(unsigned* gpioBase, int in) {		//writes to clock
	if(in==0)
		clrVal(gpioBase, 11);
	else
		setVal(gpioBase, 11);
}

/*
Returs the value of data pin - arguments: gpio base address
*/
int readData(unsigned* gpioBase) {					//reads from data
	return readVal(gpioBase, 10);
}