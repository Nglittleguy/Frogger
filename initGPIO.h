#ifndef initGPIO
#define initGPIO

/*
Retrieves the pointer to the general purpose input output pins address in mapped memory from the pi
Returns - the address of the gpio base address
*/
unsigned int *getGPIOPtr(void);

/*
Sets the gpio pin's function to 001
- algorithm was taken from class lectures
*/
void inputGPIO(unsigned* gpio, int pin);

/*
Sets the gpio pin's function to 001
- algorithm was taken from class lectures
*/
void outputGPIO(unsigned* gpio, int pin);

#endif
