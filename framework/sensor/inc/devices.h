/*
 * deviced
 *
 * Copyright (c) 2012 - 2013 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <pthread.h>
#include "sensor_manager.h"


#ifndef __DEVICES_H__
#define __DEVICES_H__

#define MAX_SENSOR_VALUE_NUMBER	10

typedef struct{
	void *data;
	char *type;
}sensorData;

struct device_ops {
	char *name;
	int status;
	char *valueType;
	char *valueName;
	//Data type 배열 추가 
	void (*start) (void *data);
	void (*stop) (void *data);
	char* (*get) (void *data);
};

struct device_ops *device_ops_list;

enum device_ops_status {
	SENSOR_STOP,
	SENSOR_RUNNING,
};

typedef struct _sensorList{
	struct device_ops *dev;
	requestHeader* rh;
	int status;

	double sensor_data[MAX_SENSOR_VALUE_NUMBER];
	char *sensor_data_ori;
	char *sensor_data_prev;
	pthread_t senseor_thread;
	pthread_cond_t cond;
	pthread_mutex_t mutex;

	struct _sensorList* next;
}sensorList;

typedef struct {
	sensorList *start;
	int	count;
}sensorHead;



void addDevice(const struct device_ops *dev);
void addSensor(const struct device_ops *dev);

#define DEVICE_OPS_REGISTER(dev)	\
static void __attribute__((constructor))  module_init(void) \
{ \
	addSensor(dev); \
} 
/*
static void module_exit(void)	\
	{	\
	remove_device(dev);	\
	}
*/
sensorHead* initSensors(void);
sensorHead* getSensorHead(void);
sensorList* getSensorByName(sensorHead* sh, char* sensor_name);


static inline int sensorStart(sensorList* sl, void *data)
{
	const struct device_ops *dev = sl->dev;

	if (dev && dev->start){
		dev->start(data);
		sl->status = SENSOR_RUNNING;
		return 0;
	}

	return -1;
}

static inline int sensorStop(sensorList* sl, void *data)
{
	const struct device_ops *dev = sl->dev;

	if (dev && dev->stop) {
		dev->stop(data);  
		sl->status = SENSOR_STOP;
			
		return 0;
	}

	return -1;
}

static inline char* sensorGet(sensorList* sl, void *data)
{
	const struct device_ops *dev = sl->dev;

	if (dev && dev->get) {		
		return (dev->get(data));
	}

	return '\0';
}

#endif




/*
void devices_init(void *data);
void devices_exit(void *data);

static inline int device_start(const struct device_ops *dev)
{
	if (dev && dev->start)
		return dev->start(NORMAL_MODE);

	return -EINVAL;
}

static inline int device_stop(const struct device_ops *dev)
{
	if (dev && dev->stop)
		return dev->stop(NORMAL_MODE);

	return -EINVAL;
}

static inline int device_exit(const struct device_ops *dev, void *data)
{
	if (dev && dev->exit) {
		dev->exit(data);
		return 0;
	}

	return -EINVAL;
}

static inline int device_execute(const struct device_ops *dev, void *data)
{
	if (dev && dev->execute)
		return dev->execute(data);

	return -EINVAL;
}

static inline int device_get_status(const struct device_ops *dev)
{
	if (dev && dev->status)
		return dev->status();

	return -EINVAL;
}


void add_device(const struct device_ops *dev);
void remove_device(const struct device_ops *dev);

const struct device_ops *find_device(const char *name);
int check_default(const struct device_ops *dev);

#define NOT_SUPPORT_OPS(dev) \
	((check_default(dev))? 1 : 0)

#define FIND_DEVICE_INT(dev, name) do { \
	if (!dev) dev = find_device(name); if(check_default(dev)) return -ENODEV; \
} while(0)

#define FIND_DEVICE_VOID(dev, name) do { \
	if (!dev) dev = find_device(name); if(check_default(dev)) return; \
} while(0)


*/
