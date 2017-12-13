#include "driver_common.h"

#define VIBRATION	2		//Digital, wiringPI

static unsigned char state = 0;
static unsigned char working = 0;
static unsigned char init = 0;

void vib_fuc(){
	if (working)
		state++;
}

void VIBRATION_start(void *data)
{
	working = 1;
	
	if (!init){
		setupWiringPi();
		pinMode(VIBRATION, INPUT);
		wiringPiISR(VIBRATION, INT_EDGE_FALLING, &vib_fuc); //Falling function!!!!!!!!!!
		init = 1;
	}

	printf("Vibration sensor is start\n");
}

void VIBRATION_stop(void *data)
{
	working = 0;
	printf("Vibration sensor is terminated\n");
}

char* VIBRATION_get(void *data)
{
	static char value_c[20];
	int value;

	if (state != 0){
		state = 0;
		value = 1;
	}
	else
		value = 0;

	sprintf(value_c, "%d\n", value);

	return value_c;
}


