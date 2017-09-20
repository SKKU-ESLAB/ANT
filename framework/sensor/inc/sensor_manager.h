#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stdint.h>
#include <glib.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-lowlevel.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>


#ifndef HEADER_GUARD
#define HEADER_GUARD

#define ANT_BUS_NAME	"org.ant.sensorManager"

#define SW              1 // BCM_GPIO 18
#define LED1    		4 // BCM_GPIO 23
#define MOTION  		5 //BCM_GPIO 24
#define TEMP    		6

#define MAXTIMINGS      100
#define MOTION_FUNCTION        			 9
#define LED_FUNCTION            		10
#define ACC_FUNCTION                    11
#define TEMP_FUNCTION                   12

#define API_TO_SM       1234
#define SM_TO_API       4040

#define dbusAddress "type='signal', interface='ant.sensor.sensorManager'";
#define FAIL_TO_DELETE_REQUEST	-1

//--------------------------------------------------------------------------------------------//
//                                              Request 처리 관련
//리퀘스트 리스트, 나중에 Linked list 등 동적으로 관리할 수 있도록 하는게 좋아 보임...
//Request list, It looks better to make it dynamically manageable such as linked list

/*
struct RequestData
{
	pthread_t tid;
	int rq_num;
	int sensor_type;
	int handle_type;
	int sensing_interval;
	int sensing_level;
	int data; //

	struct RequestList *upper;
	struct RequestData *next;
};

struct RequestList
{
	unsigned int pid;
	char path[40];
	char interface[40];
	struct RequestData *data;
	struct RequestList *next;
};
 */
typedef struct _requestData{
	unsigned int pid;
	char	target_path[20];
	char	target_interface[20];

	int rq_num;
	const char* sensor_name;
	int handle_type;
	int sensing_interval; //ms
	int sensing_level;
	char sensor_data[100];
	char prev_sensor_data[100];

	struct timeval next_run_time;
	
	struct _requestData* next;
}requestData;


typedef struct{
	int num_of_request;
	requestData* start;
}requestHeader;

extern void doNothing();

#endif

