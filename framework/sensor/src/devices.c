#include <stdio.h>
#include "devices.h"


static sensorHead *sensor_head = NULL;


sensorList* getSensorByName(sensorHead* sh, char* sensor_name){
	sensorList* temp;
	temp = sh->start;

	while (temp != NULL){
		if (!strcmp(temp->dev->name, sensor_name))
			break;

		temp = temp->next;
	}
	if (temp == NULL){
		printf("Error : %s is not supported sensor! \n", sensor_name);
	}
	return temp;
}

sensorHead* getSensorHead(void){
	return sensor_head;
}
sensorHead* initSensors(void){
  // It might not free the sensor_list ?
	sensor_head = (sensorHead*)malloc(sizeof(sensorHead));
	sensor_head->start = NULL;
	sensor_head->count = 0;

	return sensor_head;
}
void addSensor(const struct device_ops *dev)
{
	sensorList* sl;
	sensorList* new_sl;

	if (sensor_head == NULL){
		printf("Sensor head is not initilized! Perform initilizing\n");
		initSensors();
	}

	sl = sensor_head->start;
	if (sl == NULL){
		sensor_head->start = (sensorList*)malloc(sizeof(sensorList));
		new_sl = sensor_head->start;
	}
	else{
		while (1){
			if (sl->next == NULL)
				break;
			sl = sl->next;
		}

		sl->next = (sensorList*)malloc(sizeof(sensorList));
		new_sl = sl->next;
	}
	new_sl->dev = dev;
	new_sl->next = NULL;

	//new_sl->num_of_request = 0;
	new_sl->rh = initRequestHeader();
	new_sl->status = SENSOR_STOP;

	sensor_head->count++;

	

	printf("[SM] %s is added [Total number of sensor :%d ]\n", dev->name, sensor_head->count);
}

void deleteSensor(const struct device_ops *dev){
}

int countSensor(void);
/*
	Sensor On/Off 등의 컨트롤에 대한 정보도 추가.
*/

