#ifndef _ANT_RECORDING_H_
#define _ANT_RECORDING_H_
extern "C"{
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <asm/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <assert.h>
#include <dbus/dbus.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <getopt.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <poll.h>
}
#include <v8.h>
#include <node.h>
#include <uv.h>
#include <nan.h>
#include "ANTdbusInterface.h"


class ANTrecordingAsync : public Nan::AsyncWorker
{
	public:
		ANTrecordingAsync(Nan::Callback *cb, DBusConnection *_conn, 
				DBusMessage *_msg, int _seconds) : Nan::AsyncWorker(cb), 
		conn(_conn), msg(_msg), seconds(_seconds) {}
	  ~ANTrecordingAsync() {}
		void Execute();
		void HandleOKCallback();

	private:
		bool is_success;
		DBusMessage *reply;
		DBusConnection *conn;
		DBusMessage *msg;
		int seconds;
};

class ANTRecording : public Nan::ObjectWrap{

	public:
		static NAN_MODULE_INIT(Init);
		DBusMessage * sendDbusMsg(const char* msg, dbusRequest *dbus_request);
		DBusMessage * sendStreamingDbusMsg(const char *msg, 
				dbusStreamingRequest *dbus_request);
		DBusMessage * sendSensorOverlayDbusMsg(const char* msg,
				dbusSensorOverlayRequest *dbus_request);
		DBusMessage * sendPreRecInitDbusMsg(const char* msg,
				dbusRequest *dbus_request);
		DBusMessage * sendPreRecDbusMsg(const char* msg,
				dbusRequest *dbus_request);
		bool initDbus();

		DBusConnection* conn;
		DBusError err;
	private:
		explicit ANTRecording() {};
		~ANTRecording(){};

		static NAN_METHOD(New);

		static NAN_METHOD(recStart);
		static NAN_METHOD(recStop);
		
		static NAN_METHOD(jpegStart);

		static NAN_METHOD(streamingStart);
		static NAN_METHOD(streamingStop);

		static NAN_METHOD(sensorOverlayStart);
		static NAN_METHOD(sensorOverlayStop);
    
		static NAN_METHOD(preRecInit);
		static NAN_METHOD(preRecStart);
		static NAN_METHOD(preRecStop);

		static inline Nan::Persistent<v8::Function>& constructor()
		{
			static Nan::Persistent<v8::Function> my_constructor;
			return my_constructor;
		}
};
//NODE_MODULE(ANTRecording, ANTRecording::Init)

#endif /* _ANT_RECORDING_H_ */

