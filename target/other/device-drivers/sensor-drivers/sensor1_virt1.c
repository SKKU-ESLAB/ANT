#include "driver_common.h"


void VIRT1_start(void *data)
{
	printf("VIRT1 sensor is started\n");
}

void VIRT1_stop(void *data)
{
	printf("VIRT1 sensor is terminated\n");
}

char* VIRT1_get(void *data)
{
	int i;
	unsigned int val_x, val_y, val_z;
	double b;
	float a;
	static char value[20];

	val_x = 0;
	val_y = 0;
	val_z = 0;

	for (i = 0; i<10; i++)
	{
		val_x += read_sensor_value(); 
		val_y += read_sensor_value(); 
		val_z += read_sensor_value(); 
	}

	val_x = val_x / 10;
	val_y = val_y / 10;
	val_z = val_z / 10;


	sprintf(value, "%d %d %d\n", val_x, val_y, val_z);

	return value;
	
}


