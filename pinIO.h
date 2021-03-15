#ifndef pinIO
#define pinIO

/*
Sets the pin value to high - arguments: gpio base address, and the pin to set to 1
*/
void setVal(unsigned* gpio, int pin);

/*
Sets the pin value to low - arguments: gpio base address, and the pin to set to 0
*/
void clrVal(unsigned* gpio, int pin);

/*
Reads value of this pin - arguments: gpio base address, and the pin to read
*/
int readVal(unsigned* gpio, int pin);

/*
Writes this value to the latch pin - arguments: gpio base address, value to set latch
*/
void writeLatch(unsigned* gpioBase, int in);


/*
Writes this value to the clock pin - arguments: gpio base address, value to set clock
*/
void writeClock(unsigned* gpioBase, int in);


/*
Returs the value of data pin - arguments: gpio base address
*/
int readData(unsigned* gpioBase);

#endif
