#include <node.h>
#include <v8.h>

#include <stdlib.h>
#include <glib.h>
#include <dbus/dbus.h>
#include <string.h>
#include <string>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>

using namespace v8;
//using namespace node;

//------------------------- Sensor Manager define value ----------------------------//
//
#define DEBUG_MODE	0


#define SENSING_INTERVAL		1	
#define SENSING_EVENT_DRIVEN	2
#define SENSING_ONESHOT			3

#define MIN_INTERVAL_TIME		50 //50 msec

#define SEND_SENSOR_DATA		"sensorData"
#define SEND_SENSOR_NOTIFY		"sensorNotify"


//
//----------------------------------------------------------------------------------//




#ifndef __NATIVE_INTERFACE_LAYER_H__
#define __NATIVE_INTERFACE_LAYER_H__

//------------------------------------------------------------------------------------//
//								 Sensor Manager Structure                            //
#define MAX_REQUEST 20

typedef struct request {
	char sensor_name[20];
	int sensor_type;
	int id;
	int handle_type;
	int sensing_interval;
	int sensing_level;
	Persistent<Function> callback;

	int data; //Data
} RequestData;

typedef struct _requestList{
	int rq_num;						//Request number
	int type;						//Request Type, SENSOR_MANAGER//CAMERA_MANAGER
	Persistent<Function> callback;	//callback function
	void *data;						//User data
	struct _requestList* next;				//Linked list
}requestList;

static char nilPath[100];
static char nilInterface[100];

//
//------------------------------------------------------------------------------------//

void read_sensorlist();
void delete_sensorlist();

void _debug(char* text);
gboolean NIL_DbusConnection(DBusConnection *bus_cnxn);
void pid_to_char(char* output);
int check_sensor_name(const char* name);
DBusConnection *DbusInit(void);

void initRequestList(requestList *rList);
void printRequset(requestList *rList);
int countRequest(requestList *rList);
int deleteRequest(requestList *rList, int rq_num);
requestList * newRequest(requestList *rList);
requestList * getRequest(requestList *rList, int rq_num);
DBusHandlerResult sensorGetRepeatedly(DBusConnection *connection, DBusMessage *message, void *iface_user_data);
DBusHandlerResult sensorEventNotify(DBusConnection *connection, DBusMessage *message, void *iface_user_data);

#endif  
