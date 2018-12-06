#include "driver_common.h"

#define ACC_X	0
#define ACC_Y	1
#define ACC_Z	2

void ACC_start(void *data)
{
	printf("ACC sensor is started\n");
}

void ACC_stop(void *data)
{
	printf("ACC sensor is terminated\n");
}

char* ACC_get(void *data)
{
	int i;
	int val_x, val_y, val_z;
  double b;
	float a;
	static char value[20];

	val_x = 0;
	val_y = 0;
	val_z = 0;

	for (i = 0; i<10; i++)
	{
		val_x += ADC_Read(ACC_X); delay(2);
		val_y += ADC_Read(ACC_Y); delay(2);
		val_z += ADC_Read(ACC_Z); delay(2);
	}

	val_x = val_x / 10;
	val_y = val_y / 10;
	val_z = val_z / 10;

	delay(300);

	printf("X_Axis : %d \n", val_x);
	printf("Y_Axis : %d \n", val_y);
	printf("Z_Axis : %d \n", val_z);

	b = (double)(abs(val_x - 320)) / (abs(val_z - 320));

	//printf("B : %lf\n", b);

	a = atan(b);
	a = a / 3.14 * 180; //the value of Angle

	//printf("A : %lf\n", a);

	sprintf(value, "%d %d %d\n", val_x, val_y, val_z);

	return value;

	//sensor1Data sData;
	//sData.value = 10;

	//data->data = (void *)malloc(sizeof(sensor1Data));
	//memcpy(data->data, &sData, sizeof(sensor1Data));
	//char* test = "77";

	//printf("Get data from sensor 1\n");

	
}


