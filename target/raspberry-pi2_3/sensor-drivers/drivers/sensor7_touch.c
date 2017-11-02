#include "driver_common.h"

#define TOUCH	25			//Digital, wiringPI
static int touch_flag = 0;


void TOUCH_start(void *data)
{
	if (!touch_flag){
		setupWiringPi();
		pinMode(TOUCH, INPUT);
		touch_flag = 1;
		printf("Touch init\n");
	}
	printf("Touch sensor is start\n");
}

void TOUCH_stop(void *data)
{
	printf("Touch sensor is terminated\n");
}

char* TOUCH_get(void *data)
{
	int value;
	static char value_c[20];

	value = digitalRead(TOUCH);

	sprintf(value_c, "%d\n", value);
	printf("[SM] Touch Value :%d \n", value);

	
	return value_c;
}


