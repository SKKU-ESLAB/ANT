#include "driver_common.h"

#define MOTION_PIN	1			//Digital, wiringPI

static int flag = 0;

void MOTION_start(void *data)
{
	if(!flag){
		setupWiringPi();
		pinMode(MOTION_PIN, INPUT);
		flag = 1;
	}
	printf("Motion sensor is start\n");
}

void MOTION_stop(void *data)
{
	printf("Motion sensor is terminated\n");
}

char* MOTION_get(void *data)
{	
	static char value_c[20];
	int value = digitalRead(MOTION_PIN);
	
	sprintf(value_c, "%d\n", value);

	return value_c;
	
}


