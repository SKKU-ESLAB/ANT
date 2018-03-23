#include <stdlib.h>
#include <glib.h>
#include <dbus/dbus.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <uv.h>
#include <signal.h>
#include <sys/time.h>

#include "cJSON.h"
#include "nil.h"


#define MAX_STRING_LENTGH	256
#define MAX_DATA_LENGTH		20
#define SENSOR_REQUEST	1
#define SM_INTERFACE "/org/ant/sensorManager"
#define SM_PATH	"org.ant.sensorManager"
#define REQUEST_DELAY	50 //ms

/* Maintain only one connection */
static DBusConnection *antCon = NULL; 
/*
 * Temporary variable: It's for delaying when register events.
 */
static struct timeval time_to_delay;
requestList *rList;
pid_t pid;
int sensor_num;
char **sensor_list;

int check_sensor_name(const char* name){
	int i;

	for (i = 0; sensor_num; i++){
		if (!strncmp(name, sensor_list[i], strlen(name)))
			return i;
	}
  printf("aa\n");
	return -1;
}

/* Handler function for dbus message
 * 1. On (Periodic)
 * 2. On (Notify)
 */
/*
DBusHandlerResult sensorGetRepeatedly(DBusConnection *connection, DBusMessage *message, void *iface_user_data){
	Isolate *isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  int rq_num;
	char* sensorValue;
	char* valueType;
	char* valueName;
	requestList* rl;
	TryCatch try_catch(isolate);
	DBusError err;
	dbus_error_init(&err);

	//printRequset(rList);

	dbus_message_get_args(message, &err,
		DBUS_TYPE_INT32, &rq_num,
		DBUS_TYPE_STRING, &sensorValue,
		DBUS_TYPE_STRING, &valueType,
		DBUS_TYPE_STRING, &valueName,
		DBUS_TYPE_INVALID);

	if (dbus_error_is_set(&err))
	{
		printf("Get Repeatedly - Error get data: %s \n", err.message);
		dbus_error_free(&err);
	}
	//printf("[NIL] Receive rq_num : %d / value :%d\n", rq_num, sensorValue);
	
	rl = getRequest(rList, rq_num);
	
	//printf("Received data : value[%s] type[%s] name[%s]\n", sensorValue, valueType, valueName);
	parsingToArgv(rl, sensorValue, valueType, valueName);
	
	return DBUS_HANDLER_RESULT_HANDLED;
}

DBusHandlerResult sensorEventNotify(DBusConnection *connection, DBusMessage *message, void *iface_user_data){
  Isolate *isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  int rq_num;
	requestList* rl;
	TryCatch try_catch(isolate);
	DBusError err;

	dbus_error_init(&err);

	//printRequset(rList);
	dbus_message_get_args(message, &err,
		DBUS_TYPE_INT32, &rq_num,
		DBUS_TYPE_INVALID);

	if (dbus_error_is_set(&err))
	{
		printf("Notify Error get data: %s\n", err.message);
		dbus_error_free(&err);
	}

	printf("[NIL] Event Notify rq_num : %d\n", rq_num);

	rl = getRequest(rList, rq_num);

	Local<Function> fn = Local<Function>::New(isolate, rl->callback);
	fn->Call(isolate->GetCurrentContext()->Global(), 0, NULL);

	if (try_catch.HasCaught()) {
		//node::FatalException(try_catch);
		Local<Value> exception = try_catch.Exception();
		String::Utf8Value exception_str(exception);
		printf("Exception! %s\n", *exception_str);
	}

	return DBUS_HANDLER_RESULT_HANDLED;
}

int wait_delay(){
	//Node.js Multi thread 환경에서 sleep으로 인한 문제가 발생하기 때문에
	//어쩔 수 없이 busy wait로 구현
	struct timeval temp;
	int i;
	
	while (1){
		gettimeofday(&temp, NULL);

		if (temp.tv_sec > time_to_delay.tv_sec)
			break;
		else if (temp.tv_usec >= (time_to_delay.tv_usec + (REQUEST_DELAY * 1000)))
			break;
		else{
			for (i = 0; i < 10000; i++);
		}
	}

	gettimeofday(&time_to_delay, NULL);

	return 0;
}
*/

/* Sensor manager API
 * 1. On
 * 2. Get
 */

/*
void On(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

	requestList* rl;
	DBusMessage* msg;
	DBusError err;
	int pid;
	int rq_num;
	const char* sensor_name;
	int sensing_interval;
	int handle_type = 0;
	const char* handle_type_s;
	int sensing_level = 0; //현재는 사용하지 않음.
	
	wait_delay(); //Perform wait.
*/
	/*
	Receive Args
	1. sensor name
	2. handle type
	3. sensing interval
	4. Function
	*/
/*
	//----------------------------------------------------------------//
	//						1. Argument Check
	printf("Length : %d\n", args.Length());
	if ((args.Length() < 4) ||	
		!args[0]->IsString() || 
		!args[1]->IsString() ||
		!args[2]->IsInt32() ||
		!args[3]->IsFunction() ) {
		isolate->ThrowException(Exception::TypeError(
								String::NewFromUtf8(isolate,
										"Invalid Use : 4 arguments expected \
										[Sensor Name, Handling Type, Interval, Function]")));
		return ;
	}
	//
	//----------------------------------------------------------------//


	//--------------------- 1.1 Sensor Name Check --------------------//
	// 서포트 리스트와 비교해서, 지원하는 센서인지 체크
	//
	v8::String::Utf8Value param0(args[0]->ToString());
	std::string name_c = std::string(*param0);
	sensor_name = name_c.c_str();

  if (check_sensor_name(sensor_name) == -1){
    isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, 
            "This sensor is not supported sensor\n")));
    return ;
  }
	//
	//----------------------------------------------------------------//

	//--------------------- 1.2 Handle Type Check --------------------//
	//
	v8::String::Utf8Value param1(args[1]->ToString());
	std::string type_c = std::string(*param1);
	handle_type_s = type_c.c_str();

	if (!(strcmp(handle_type_s, "PERIODIC"))){
		handle_type = SENSING_INTERVAL;
	}
	else if (!(strcmp(handle_type_s, "NOTIFY"))){
		handle_type = SENSING_EVENT_DRIVEN;
	}

	if (!handle_type){
		isolate->ThrowException(Exception::TypeError(
								String::NewFromUtf8(isolate, "Not supported handling type\n")));
		return ;
	}
	//
	//----------------------------------------------------------------//

	//--------------------- 1.3 interval check --------------------//
	//
	sensing_interval = args[2]->IntegerValue();
	
	if (sensing_interval < MIN_INTERVAL_TIME){
		isolate->ThrowException(Exception::TypeError(
								String::NewFromUtf8(isolate, "Too small interval time\n")));
		return ;
	}
	//
	//----------------------------------------------------------------//


	//----------------------------------------------------------------//
	//			2. Request Creation (For function callback)
	rl = newRequest(rList);

	Local<Function> cb = Local<Function>::Cast(args[3]);
  rl->callback.Reset(isolate, cb);	
	rl->type = SENSOR_REQUEST;
	pid = (unsigned int)getpid();
	rq_num = rl->rq_num;
	//
	//----------------------------------------------------------------//
	


	//----------------------------------------------------------------//
	//				3. Send Message (Request struct) 
	//				Send message with reply or not
	dbus_error_init(&err);
	msg = dbus_message_new_signal(SM_INTERFACE, SM_PATH, "register");

	dbus_message_append_args(msg,
		DBUS_TYPE_INT32, &pid,
		DBUS_TYPE_INT32, &rq_num,
		DBUS_TYPE_STRING, &(sensor_name),
		DBUS_TYPE_INT32, &(handle_type),
		DBUS_TYPE_INT32, &(sensing_interval),
		DBUS_TYPE_INT32, &sensing_level,
		DBUS_TYPE_INVALID);
*/
	/* Send the signal */
/*
	dbus_connection_send(antCon, msg, NULL);
	dbus_message_unref(msg);

	//printf("[NIL] Send message (Event Register) to %s | %s\n", SM_PATH, SM_INTERFACE);
	//
	//----------------------------------------------------------------//

}
*/

void Get(char* _sensor, char** output) {
  printf("Get function\n");
  char* sensorValue;
	char* valueType;
	char* valueName;

	DBusMessage* msg;
	DBusMessage* reply;
	DBusError error;

   
	if ( check_sensor_name(_sensor) == -1){
    fprintf(stderr, "This sensor is not support!\n");
		return ;
	}

	/*-------------------- DBus Message Initilizing -----------------*/ 

	msg = dbus_message_new_method_call(
    "org.ant.sensorManager", /* target for method call */
		"/", /* object to call on */
		"org.ant.sensorManager", /* interface to call on */
		"Get" /* method name */
  );
	
  if (NULL == msg) {
		printf("Message Null\n");
    fprintf(stderr, "Fail to create message \n");
		return ;
	}

	dbus_message_append_args(msg,
	  DBUS_TYPE_STRING, &_sensor,
		DBUS_TYPE_INVALID
    );

	/* send message and get a reply */
	dbus_error_init(&error);

	if (antCon == NULL){
		printf("Why null? \n");
		antCon = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
	}

	// Timeout 500 milli seconds
	reply = dbus_connection_send_with_reply_and_block(antCon, msg, 500, &error); 
	if (reply == NULL){
		printf("Get Null Reply \n");
		printf("Error : %s\n", error.message);
	}
	dbus_error_free(&error);
	dbus_message_unref(msg);
	 
  dbus_message_get_args(reply, NULL,
		DBUS_TYPE_STRING, &sensorValue,
		DBUS_TYPE_STRING, &valueType,
		DBUS_TYPE_STRING, &valueName,
		DBUS_TYPE_INVALID);

	dbus_message_unref(reply);

  /* Return value */
  strncpy(*output, sensorValue, sizeof(sensorValue));  

  return ;
}

/*
void Update(const FunctionCallbackInfo<Value>& args){
	requestList* rl;
	DBusMessage* msg;
	DBusError err;
	int pid;
	int rq_num;
	int sensing_interval = 0;
	int handle_type = 0;
	const char* handle_type_s;
	int sensing_level = 0; //현재는 사용하지 않음.
	int request_change_flag = 0;


	//----------------------------------------------------------------//
	//						1. Argument Check
	printf("Length : %d\n", args.Length());
	if ((args.Length() < 4) ||
		!args[0]->IsInt32() ||
		!(args[1]->IsString() || args[1]->IsNull()) ||
		!(args[2]->IsInt32() || args[2]->IsNull()) ||
		!(args[3]->IsFunction() || args[3]->IsNull()) ) {
		isolate->ThrowException(Exception::TypeError(
								String::NewFromUtf8(isolate, 
										"Invalid Use : 4 arguments expected [Request ID, Handling Type[or NULL], Interval[or NULL], Function[or NULL]]")));
		return ;
	}
	//
	//----------------------------------------------------------------//
	
	rq_num = args[0]->IntegerValue();
	pid = (unsigned int)getpid();

	//--------------------- 1.2 Handle Type Check --------------------//
	// 서포트 리스트와 비교해서, 지원하는 센서인지 체크
	//
	if (args[1]->IsString()){
		v8::String::Utf8Value param1(args[1]->ToString());
		std::string type_c = std::string(*param1);
		handle_type_s = type_c.c_str();

		if (!(strcmp(handle_type_s, "PERIODIC"))){
			handle_type = SENSING_INTERVAL;
		}
		else if (!(strcmp(handle_type_s, "NOTIFY"))){
			handle_type = SENSING_EVENT_DRIVEN;
		}//Need to oneshot?

		if (!handle_type){
			isolate->ThrowException(Exception::TypeError(
									String::NewFromUtf8(isolate, "Not supported handling type\n")));
			return ;
		}

		request_change_flag = 1;
	}
	//
	//----------------------------------------------------------------//

	//--------------------- 1.3 interval check --------------------//
	// 서포트 리스트와 비교해서, 지원하는 센서인지 체크
	//
	if (args[2]->IsInt32()){
		sensing_interval = args[2]->IntegerValue();

		if (sensing_interval < MIN_INTERVAL_TIME){
			isolate->ThrowException(Exception::TypeError(
									String::NewFromUtf8(isolate, "Too small interval time\n")));
			return ;
		}
		request_change_flag = 1;
	}
	//
	//----------------------------------------------------------------//


	//-----------------------------------------------------------------//
	if (args[3]->IsFunction()){
		rl = getRequest(rList, rq_num);
		
		Handle<Function> arg0 = Handle<Function>::Cast(args[3]);
		//Persistent<Function> cb(isolate, arg0);
    rl->callback.Reset(isolate, arg0);
		//rl->callback = Persistent<Function>::New(Local<Function>::Cast(args[3]));
	}

	if (request_change_flag){
		dbus_error_init(&err);
		msg = dbus_message_new_signal(SM_INTERFACE, SM_PATH, "update");

		dbus_message_append_args(msg,
			DBUS_TYPE_INT32, &pid,
			DBUS_TYPE_INT32, &rq_num,
			DBUS_TYPE_INT32, &(handle_type),
			DBUS_TYPE_INT32, &(sensing_interval),
			DBUS_TYPE_INT32, &sensing_level,
			DBUS_TYPE_INVALID);
*/
		/* Send the signal */
/*
		dbus_connection_send(antCon, msg, NULL);
		dbus_message_unref(msg);

		printf("[NIL] send message to %s | %s\n", SM_PATH, SM_INTERFACE);

	}
	//-----------------------------------------------------------------//
	
	return ;
}
*/
/*
void Unregister(const FunctionCallbackInfo<Value>& args){
	DBusMessage* msg;
	DBusError err;
	int pid;
	int rq_num;

	
	//----------------------------------------------------------------//
	//						1. Argument Check
	printf("Length : %d\n", args.Length());
	if ((args.Length() < 1) ||
		!args[0]->IsInt32() ){
		isolate->ThrowException(Exception::TypeError(
								String::NewFromUtf8(isolate, "Invalid Use : 1 arguments expected [Request ID]")));
		return ;
	}
	//
	//----------------------------------------------------------------//
	
	rq_num = args[0]->IntegerValue();
	pid = (unsigned int)getpid();

	//1.UNREGISTER DBUS MESSAGE
	dbus_error_init(&err);
	msg = dbus_message_new_signal(SM_INTERFACE, SM_PATH, "unregister");

	dbus_message_append_args(msg,
		DBUS_TYPE_INT32, &pid,

		DBUS_TYPE_INVALID);
*/
	/* Send the signal */
/*
	dbus_connection_send(antCon, msg, NULL);
	dbus_message_unref(msg);

	printf("[NIL] send message to %s | %s\n", SM_PATH, SM_INTERFACE);


	//2.DELETE REQEUST FROM NIL

	deleteRequest(rList, rq_num);

	return ;
}

*/

/*----------------------- exit handler--------------------------*/
void all_request_unregister(){
	DBusMessage* msg;
	DBusError err;
	int pid;
	int rq_num;

	pid = (unsigned int)getpid();
	rq_num = -1;

	dbus_error_init(&err);
	msg = dbus_message_new_signal(SM_INTERFACE, SM_PATH, "unregister");

	dbus_message_append_args(msg,
		DBUS_TYPE_INT32, &pid,
		DBUS_TYPE_INT32, &rq_num,
		DBUS_TYPE_INVALID);

	/* Send the signal */
	dbus_connection_send(antCon, msg, NULL);
	dbus_message_unref(msg);

	printf("[NIL] send All_UNREGISTER message to %s | %s\n", SM_PATH, SM_INTERFACE);
}


void exit_handler(void){
	all_request_unregister();
	//Perform unregister
}
void sigint_handler(int signo)
{
	//Perform unregister
	all_request_unregister();
	signal(SIGINT, SIG_DFL);
	//Sig call
	kill(getpid(), SIGINT);
}
//
//-------------------------------------------------------------//

void nativeInterfaceLayerInit(){
	pid = getpid();
	rList = (requestList*)malloc(sizeof(requestList));
	initRequestList(rList);
	gettimeofday(&time_to_delay, NULL);
}

void read_sensorlist()
{
  char *ant_sensor_dir;
  const char *error = NULL;


  ant_sensor_dir = getenv("ANT_SENSOR_DRIVER_DIR");
  if(!ant_sensor_dir){
    fprintf(stderr,"No environment variable: ANT_SENSOR_DRIVER_DIR\n");
    exit(1);
  }
  
  //load the configuration file (json)
  FILE *infile;
  char *buffer;
  long numbytes;
  char json_file_path[200];
  
  sprintf(json_file_path, "%s/sensor_config.json", ant_sensor_dir);
  infile = fopen(json_file_path, "r");
  if(infile == NULL){
    fprintf(stderr, "Cannot read the sensor configuration file\n");
    exit(1);
  }
  fseek(infile, 0L, SEEK_END);
  numbytes = ftell(infile);
  fseek(infile, 0L, SEEK_SET);

  buffer = (char*) calloc(numbytes, sizeof(char));
  if(buffer == NULL)
  {
    fprintf(stderr,"error while allocating buffer!");
    exit(1);
  }
  fread(buffer,sizeof(char), numbytes, infile);
  fclose(infile);
  
  // Get the root object of the json
  cJSON *root = NULL;
  root = cJSON_Parse(buffer);

  if(!root){
    fprintf(stderr, "Error before: [%s]\n", cJSON_GetErrorPtr());
    exit(1);
  }
  // find the target name and # of sensors
  char *target_name = cJSON_GetObjectItem(root, "target_name")->valuestring;
  sensor_num = cJSON_GetObjectItem(root, "sensor_num")->valueint;
  sensor_list = (char**) malloc(sensor_num * sizeof(char*));
  int i;
  for(i=0; i<sensor_num; i++){
    char sensor_index[10];
    sprintf(sensor_index, "sensor%d", i+1);
    cJSON *sensor_object = cJSON_GetObjectItem(root, sensor_index);

    sensor_list[i] = (char*) malloc(30 * sizeof(char));
    
    char* sensor_name = cJSON_GetObjectItem(sensor_object,"name")->valuestring;
    if(!sensor_name){
      fprintf(stderr, "error while reading the sensor configuration (json)\n");
    }
    // add to the sensor_list
    strcpy(sensor_list[i], sensor_name);
    printf("sensor %d: %s\n", i+1, sensor_list[i]);
  }  
  
  
}

int main(int argc, char* argv[]) {
  if(argc != 1){
    /* print usage */

  }
 
  /* Initialize the system */
	read_sensorlist();
	nativeInterfaceLayerInit();
  /* Init dbus message receiver (by PID) */
	antCon = DbusInit();


	if (atexit(exit_handler)) fprintf(stderr, "Failed to register exit_handler1\n"); 
  signal(SIGINT, sigint_handler);

  char *tmp;
  char *command;
  char *target_sensor;
  char *output;
  int str_len;
  tmp = (char*) malloc(100 * sizeof(char));

  command = (char*) malloc(100 * sizeof(char));
  target_sensor = (char*) malloc(100 * sizeof(char));
  output = (char*) malloc(100 * sizeof(char));

  while(1){
    printf("[sensor-client] Input Command: ");
   
    if(fgets(command, 100, stdin) != NULL){
      printf("Input Sensor Name: ");
      if(fgets(target_sensor, 100, stdin) != NULL){

        if(!strncmp(command, "Get", 3)){
            printf("Execute Get command\n");
            
            str_len = strlen(target_sensor);
            target_sensor[str_len-1] = '\0';
            
            Get(target_sensor, &output);
            printf("sensor value: %s\n", output);

        } else if(!strncmp(command, "On", 2)){
          printf("not implemented yet\n");
  
        } else if(!strncmp(command, "Exit", 4)){
          printf("exit the sensor client program.\n");
          break;
        } else if(!strncmp(command, "exit", 4)){
          printf("exit the sensor client program.\n");
          break;
        } else {
          printf("Command not found! \n ");
          printf("Please refer to the sensor list when the program starts\n\n");
          
        }
  
        //Get("ACC", &tmp);
      }
    }

  } /* End While */
  


}

