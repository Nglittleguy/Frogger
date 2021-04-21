#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "initGPIO.h"

/*
Initializes the GPIO pins of the Raspberry Pi
- Stephen Ng 30038689
*/

/*
Retrieves the pointer to the general purpose input output pins address in mapped memory from the pi
Returns - the address of the gpio base address
*/
unsigned int *getGPIOPtr(void)
{
	int fdgpio = open("/dev/gpiomem", O_RDWR);
	
	if (fdgpio <0) {
		printf("unable to open");
	}
	
	unsigned int *gpioPtr = (unsigned int *)mmap(0,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fdgpio,0);
	
	return gpioPtr;
}


/*
Sets the gpio pin's function to 000
- algorithm was taken from class lectures
*/
void inputGPIO(unsigned* gpioBase, int pin) {		//sets the pins to 000
	*(gpioBase+(pin/10)) &= ~(7<<((pin%10)*3));
}

/*
Sets the gpio pin's function to 001
- algorithm was taken from class lectures
*/
void outputGPIO(unsigned* gpioBase, int pin) {		//sets the pins to 001
	*(gpioBase+(pin/10)) |= 1<<((pin%10)*3);
}
