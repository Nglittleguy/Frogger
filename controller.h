#ifndef controller
#define controller

/*
Reads SNES Controller inputs from the GPIO pins of the Raspberry Pi
- Stephen Ng 30038689
*/

/*
Writes states of SNES latch into the array - arguments; gpioBase address, and buttons array
- algorithm was taken from class lectures
*/
void readSNES(unsigned* gpioBase, int* buttons);

/*
Writes the name of the button that is pressed - arguments: the number of the button pressed
*/
void printMessage(int i);

#endif