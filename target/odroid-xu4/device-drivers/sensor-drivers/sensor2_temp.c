#include "driver_common.h"

#define TEMP	6

void TEMP_start(void *data)
{
	printf("Temperature sensor is start\n");
}

void TEMP_stop(void *data)
{
	printf("Temperature sensor is terminated\n");
}

char* TEMP_get(void *data)
{
	static char value_c[20];
	float value = (float)ADC_Read(TEMP);

	value = (500 * value) / 1024;
	sprintf(value_c, "%.2f\n", value);

	return value_c;

	//sensor1Data sData;
	//sData.value = 10;

	//data->data = (void *)malloc(sizeof(sensor1Data));
	//memcpy(data->data, &sData, sizeof(sensor1Data));
	//char* test = "77";

	//printf("Get data from sensor 1\n");

	
}


