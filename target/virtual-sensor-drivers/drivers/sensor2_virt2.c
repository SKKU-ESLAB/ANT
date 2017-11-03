#include "driver_common.h"

#define TEMP	6

void VIRT2_start(void *data)
{
	printf("VIRT2 sensor is start\n");
}

void VIRT2_stop(void *data)
{
	printf("VIRT2 sensor is terminated\n");
}

char* VIRT2_get(void *data)
{
	static char value_c[20];
	float value = (float)read_sensor_value();

	value = (500 * value) / 1024;
	sprintf(value_c, "%.2f\n", value);

	return value_c;
	
}


