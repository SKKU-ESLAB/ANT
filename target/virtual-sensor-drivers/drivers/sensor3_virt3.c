#include "driver_common.h"


void VIRT3_start(void *data)
{
	printf("VIRT3 sensor is start\n");
}

void MOTION_stop(void *data)
{
	printf("VIRT3 sensor is terminated\n");
}

char* MOTION_get(void *data)
{	
	static char value_c[20];
	int value = read_sensor_value();
	
	sprintf(value_c, "%d\n", value);

	return value_c;
	
}


