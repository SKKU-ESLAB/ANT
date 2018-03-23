#include <stdio.h>
#include <stdlib.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>


#ifndef __DRIVER_COMMON_H__


unsigned int ADC_Read(unsigned char channel);


int isWiringPiSetup();
int setupWiringPi();


#endif

