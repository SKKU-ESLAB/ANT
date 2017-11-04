#include "ANTdbusInterface.h"
#include <string.h> 
#include <stdio.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib-lowlevel.h>
#include <dbus/dbus-protocol.h>
#include <dbus/dbus-glib.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <stdlib.h>

static void send_event_rec_start(DBusConnection *conn, char *file_name)
{
  printf("Event recording start\n");
  DBusMessage *message;
  message = dbus_message_new_signal("/org/ant/cameraManager", "org.ant.cameraManager", event_rec_start_request);

  char* ant_data_dir = getenv("ANT_DATA_DIR");
  char str[256] = "";
  strcat(str, ant_data_dir);
  strcat(str, "/");
	strcat(str, file_name);
	const char *file_path = str;
	printf("file_path : %s\n", str);

  unsigned camera_num = 0;
  printf("Camera Number : ");
  scanf("%u", &camera_num);
  unsigned play_seconds = 5;

  dbus_message_append_args(message,
      DBUS_TYPE_UINT64, &camera_num,
      DBUS_TYPE_STRING, &file_path,
      DBUS_TYPE_UINT64, &play_seconds,
      DBUS_TYPE_INVALID);

  dbus_connection_send (conn, message, NULL);
  dbus_message_unref(message);
}


static void send_delay_streaming_start(DBusConnection *conn)
{
  printf("Delay streaming start\n");
  DBusMessage *message;
  message = dbus_message_new_signal("/org/ant/cameraManager", "org.ant.cameraManager", delay_streaming_start_request);
  char sensor[8] = "BUTTON";
  unsigned camera_num = 0;
  printf("Camera Number : ");
  scanf("%u", &camera_num);
  unsigned delay = 10;

  dbus_message_append_args(message,
      DBUS_TYPE_UINT64, &camera_num,
      DBUS_TYPE_UINT64, &delay,
      DBUS_TYPE_INVALID);

  dbus_connection_send (conn, message, NULL);
  dbus_message_unref(message);
}

static void send_text_overlay(DBusConnection *conn)
{
  printf("Sensor overlay start\n");
  DBusMessage *message;
  message = dbus_message_new_signal("/org/ant/cameraManager", "org.ant.cameraManager", sensor_overlay_start_request);
  char sensor[8] = "BUTTON";
  unsigned camera_num = 0;
  printf("Camera Number : ");
  scanf("%u", &camera_num);
  unsigned pid = getpid();
  char *sensor_name = sensor;

  dbus_message_append_args(message,
      DBUS_TYPE_UINT64, &camera_num,
      DBUS_TYPE_STRING, &sensor_name,
      DBUS_TYPE_UINT64, &pid,
      DBUS_TYPE_INVALID);

  dbus_connection_send (conn, message, NULL);
  dbus_message_unref(message);
}

static void send_text_overlay_stop(DBusConnection *conn)
{
  printf("Sensor overlay stop\n");
  DBusMessage *message;
  message = dbus_message_new_signal("/org/ant/cameraManager", "org.ant.cameraManager", sensor_overlay_stop_request);
  char sensor[8] = "BUTTON";
  unsigned camera_num = 0;
  printf("Camera Number : ");
  scanf("%u", &camera_num);
  unsigned pid = getpid();
  char *sensor_name = sensor;

  dbus_message_append_args(message,
      DBUS_TYPE_UINT64, &camera_num,
      DBUS_TYPE_STRING, &sensor_name,
      DBUS_TYPE_UINT64, &pid,
      DBUS_TYPE_INVALID);

  dbus_connection_send (conn, message, NULL);
  dbus_message_unref(message);
}

static void send_rec_start(DBusConnection *conn, char* file_name)
{
  printf("Recording Start\n");
  DBusMessage *message;
  message = dbus_message_new_signal("/org/ant/cameraManager", "org.ant.cameraManager", "recStart");
  char* ant_data_dir = getenv("ANT_DATA_DIR");
	char str[256] = "";
  strcat(str, ant_data_dir);
  strcat(str, "/");
	strcat(str, file_name);
	const char *file_path = str;
	unsigned pid = getpid();
	std::cout << "pid : " << pid << std::endl;
	unsigned fps = 30;
	unsigned width = 1920;
	unsigned height = 1080;
	unsigned play_seconds = 20;

  unsigned camera_num = 0;
  printf("Camera Number : ");
  scanf("%d", &camera_num);

	dbus_message_append_args(message, 
      DBUS_TYPE_UINT64, &camera_num,
			DBUS_TYPE_STRING, &file_path,
			DBUS_TYPE_UINT64, &pid,
			DBUS_TYPE_UINT64, &fps, 
			DBUS_TYPE_UINT64, &width,
			DBUS_TYPE_UINT64, &height,
			DBUS_TYPE_UINT64, &play_seconds,
			DBUS_TYPE_INVALID);
  dbus_connection_send (conn, message, NULL);
  dbus_message_unref(message);
}

static void send_snapshot_start(DBusConnection *conn, char* file_name)
{
  printf("start\n");
  DBusMessage *message;
		message = dbus_message_new_signal(dbus_path, dbus_interface, snap_start_request);
  char* ant_data_dir = getenv("ANT_DATA_DIR");
	char str[512] = "";
  strcat(str, ant_data_dir);
  strcat(str, "/");
	char prop[256] = ".jpeg";
	strcat(str, file_name);	
	strcat(str, prop);
	const char *file_path = str;
	printf("file_path : %s\n", str);	
	unsigned width = 1920;
	unsigned height = 1080;
	unsigned pid = getpid();
	printf("pid : %d\n", pid);

  unsigned camera_num = 0;
  printf("Camera Number : ");
  scanf("%d", &camera_num);

	dbus_message_append_args(message, 
      DBUS_TYPE_UINT64, &camera_num,
			DBUS_TYPE_STRING, &file_path,
			DBUS_TYPE_UINT64, &pid,
			DBUS_TYPE_UINT64, &width,
			DBUS_TYPE_UINT64, &height,
			DBUS_TYPE_INVALID);
	dbus_connection_send (conn, message, NULL);
  dbus_message_unref(message);
}

static void send_copy_shm_start(DBusConnection *conn)
{
  printf("Copy Shared Memory Start\n");
  DBusMessage *message;
  message = dbus_message_new_signal(dbus_path, dbus_interface, copy_shm_start_request);

	unsigned pid = getpid();
  unsigned camera_num = 0;
  printf("Camera Number : ");

  scanf("%d", &camera_num);
	dbus_message_append_args(message,
      DBUS_TYPE_UINT64, &camera_num,
			DBUS_TYPE_UINT64, &pid,
			DBUS_TYPE_INVALID);
  dbus_connection_send (conn, message, NULL);
  dbus_message_unref(message);
}

static void send_stream_start(DBusConnection *conn)
{
	printf("streaming start");
	DBusMessage *message;
	message = dbus_message_new_signal(dbus_path, dbus_interface, streaming_start_request);
	//const char *ip_addr = "127.0.0.1";
	const char *ip_addr = "192.168.0.2";
	unsigned port = 5000;

  unsigned camera_num = 0;
  printf("Camera Number : ");
  scanf("%d", &camera_num);
  port += camera_num;

	dbus_message_append_args(message,
      DBUS_TYPE_UINT64, &camera_num,
			DBUS_TYPE_STRING, &ip_addr,
			DBUS_TYPE_UINT64, &port,
			DBUS_TYPE_INVALID);
	
	dbus_connection_send(conn, message, NULL);
	dbus_message_unref(message);

}

static void send_stream_stop(DBusConnection *conn)
{
	printf("streaming stop\n");
	DBusMessage *message;
	message = dbus_message_new_signal(dbus_path, dbus_interface, streaming_stop_request);
	dbus_connection_send(conn, message, NULL);
	dbus_message_unref(message);

}

static void send_show_window_start(DBusConnection *conn)
{
	printf("show window start\n");
	DBusMessage *message;
	message = dbus_message_new_signal(dbus_path, dbus_interface, "showWindowStart");

  unsigned pid = getpid();
  unsigned camera_num = 0;
  printf("Camera Number : ");
  scanf("%d", &camera_num);

	dbus_message_append_args(message,
      DBUS_TYPE_UINT64, &pid,
      DBUS_TYPE_UINT64, &camera_num,
			DBUS_TYPE_INVALID);
	
	dbus_connection_send(conn, message, NULL);
	dbus_message_unref(message);
}

int main(int argc, char** argv)
{
 RequestType num;
 DBusConnection *conn;
 DBusError error;
 dbus_error_init (&error);
 conn = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
 if(dbus_error_is_set(&error))
 {
   printf("fail\n");
   dbus_error_free(&error);
   return 1;
 }
 if(argc <= 1)
	 return 0;

 printf("argv[1] = %s\n", argv[1]);

 for(;;)
 {
	 scanf("%d", &num); 
   if(num == kRecordingStart)
	 	send_rec_start(conn, argv[1]); 
	 else if(num == kSnapshot) 
		send_snapshot_start(conn, argv[1]);	 
	 else if(num == kCopyShmStart)
		send_copy_shm_start(conn);
	 else if(num == kStreamingStart)
		 send_stream_start(conn);
	 else if(num == kStreamingStop)
		 send_stream_stop(conn);
   else if(num == kTextOverlayStart)
     send_text_overlay(conn);
	 else if(num == kTextOverlayStop)
		 send_text_overlay_stop(conn);
	 else if(num == kPreRecordingInit)
		 send_delay_streaming_start(conn);
	 else if(num == kPreRecordingStart)
		 send_event_rec_start(conn, argv[1]);
   else if(num == kShowWindowStop)
     send_show_window_start(conn);
	 else
		 break;
 }
 return 0;
}
