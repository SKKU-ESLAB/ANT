#include "ANTRecordingAPI.h"

static unsigned getPid()
{
  return getpid();
}

DBusMessage* ANTRecording::sendDbusMsg(const char* msg, 
    dbusRequest *dbus_request)
{
  assert(msg != NULL && dbus_request != NULL);
  DBusMessage* message;
  message = dbus_message_new_signal(dbus_path, dbus_interface, msg);
  dbus_message_append_args(message,
      DBUS_TYPE_UINT64, &(dbus_request->pid),
      DBUS_TYPE_UINT64, &(dbus_request->camera_id),
      DBUS_TYPE_UINT64, &(dbus_request->play_seconds),
      DBUS_TYPE_STRING, &(dbus_request->file_path),
      DBUS_TYPE_UINT64, &(dbus_request->width),
      DBUS_TYPE_UINT64, &(dbus_request->height),
      DBUS_TYPE_UINT64, &(dbus_request->fps),
      DBUS_TYPE_INVALID);
  dbus_connection_send (conn, message, NULL);
  return message;
}

DBusMessage* ANTRecording::sendStreamingDbusMsg(const char* msg,
    dbusStreamingRequest *dbus_request)
{
  assert(msg != NULL && dbus_request != NULL);
  DBusMessage* message;
  message = dbus_message_new_signal(dbus_path, dbus_interface, msg);
  dbus_message_append_args(message,
      DBUS_TYPE_UINT64, &(dbus_request->pid),
      DBUS_TYPE_UINT64, &(dbus_request->camera_id),
      DBUS_TYPE_STRING, &(dbus_request->ip_address),
      DBUS_TYPE_UINT64, &(dbus_request->port),
      DBUS_TYPE_INVALID);
  dbus_connection_send(conn, message, NULL);
  return message;
}

DBusMessage* ANTRecording::sendSensorOverlayDbusMsg(const char* msg,
    dbusSensorOverlayRequest *dbus_request)
{
  assert(msg != NULL && dbus_request != NULL);
  DBusMessage* message;
  message = dbus_message_new_signal(dbus_path, dbus_interface, msg);
  dbus_message_append_args(message,
      DBUS_TYPE_UINT64, &(dbus_request->pid),
      DBUS_TYPE_UINT64, &(dbus_request->camera_id),
      DBUS_TYPE_STRING, &(dbus_request->sensor_name),
      DBUS_TYPE_INVALID);
  dbus_connection_send(conn, message, NULL);
  return message;
}

DBusMessage* ANTRecording::sendPreRecInitDbusMsg(const char* msg,
    dbusRequest *dbus_request)
{
  assert(msg != NULL && dbus_request != NULL);
  DBusMessage* message;
  message = dbus_message_new_signal(dbus_path, dbus_interface, msg);
  dbus_message_append_args(message,
      DBUS_TYPE_UINT64, &(dbus_request->pid),
      DBUS_TYPE_UINT64, &(dbus_request->camera_id),
      DBUS_TYPE_UINT64, &(dbus_request->play_seconds),
      DBUS_TYPE_INVALID);
  dbus_connection_send(conn, message, NULL);
return message;
}

DBusMessage* ANTRecording::sendPreRecDbusMsg(const char* msg,
    dbusRequest *dbus_request)
{
  assert(msg != NULL && dbus_request != NULL);
  DBusMessage* message;
  message = dbus_message_new_signal(dbus_path, dbus_interface, msg);
  dbus_message_append_args(message,
      DBUS_TYPE_UINT64, &(dbus_request->pid),
      DBUS_TYPE_UINT64, &(dbus_request->camera_id),
      DBUS_TYPE_UINT64, &(dbus_request->play_seconds),
      DBUS_TYPE_STRING, &(dbus_request->file_path),
      DBUS_TYPE_UINT64, &(dbus_request->width),
      DBUS_TYPE_UINT64, &(dbus_request->height),
      DBUS_TYPE_UINT64, &(dbus_request->fps),
      DBUS_TYPE_INVALID);
  dbus_connection_send(conn, message, NULL);
  return message;
}

void ANTrecordingAsync::Execute()
{
  assert(this->conn != NULL && this->msg != NULL);
//  unsigned success;
//  DBusError *err;
/*  this->reply = dbus_connection_send_with_reply_and_block(this->conn,
    this->msg, this->seconds*1000 + 2000, err);
  */
  sleep(seconds+1);
/*  if(reply == NULL)
  {
    this->is_success = true;
//    dbus_error_free(err);
    dbus_message_unref(msg);
    return;
  }
  else{
//    dbus_error_free(err);
    dbus_message_unref(msg);
    dbus_message_get_args(reply, NULL,
      DBUS_TYPE_UINT64, &success,
      DBUS_TYPE_INVALID);
    if(success == 0)
    {
      this->is_success = 1;
    }
    this->is_success = 1;
  }*/
  this->is_success = 1;
}

void ANTrecordingAsync::HandleOKCallback()
{
  Nan::HandleScope scope;
  v8::Local<v8::Value> argv[] = { Nan::New<v8::Number>(this->is_success) };
  callback->Call(1, argv);
}

NAN_METHOD(ANTRecording::recStart)
{
  ANTRecording *recObj = Nan::ObjectWrap::Unwrap<ANTRecording>(info.This());
  
  unsigned camera_id;
  unsigned seconds;
  dbusRequest *dbus_request = NULL;
  
  ANTrecordingAsync *recordingAsync;
  
  DBusMessage* message;
  if(!info[0]->IsNumber())
  {
    Nan::ThrowTypeError("First parameter should be Camera ID");
    return;
  }
  if(!info[1]->IsString())
  {
    Nan::ThrowTypeError("Second parameter should be File Path");
    return;
  }
  if(!info[2]->IsNumber())
  {
    Nan::ThrowTypeError("Third Parameter Should be Number for Seconds");
    return;
  }
  if(!info[3]->IsFunction())
  {
    Nan::ThrowTypeError("Fourth Parameter Should be a Callback Function");
    return;
  }
  Nan::Callback *callback = new Nan::Callback(info[3].As<v8::Function>());
  v8::String::Utf8Value param1(info[1]->ToString());
  std::string path = std::string(*param1);

  camera_id = Nan::To<int>(info[0]).FromJust();
  seconds = Nan::To<int>(info[2]).FromJust();

  if(seconds == 0)
  {
    Nan::ThrowTypeError("Seconds cannot be 0");
    return;
  }
  if(!(recObj->initDbus()))
  {
      Nan::ThrowError("D-Bus Initialization Failed\n");
      return;
  }

  dbus_request = new dbusRequest();
  dbus_request->pid = getPid();
  dbus_request->camera_id = camera_id;
  dbus_request->play_seconds = seconds;
  dbus_request->file_path = path;
  dbus_request->width = 1920;
  dbus_request->height = 1080;
  dbus_request->fps = 30;
  message = recObj->sendDbusMsg(rec_start_request, dbus_request);
  
  recordingAsync = new ANTrecordingAsync(callback, recObj->conn, 
      message, seconds);
  
  Nan::AsyncQueueWorker(recordingAsync);
}

NAN_METHOD(ANTRecording::recStop)
{
}

NAN_METHOD(ANTRecording::jpegStart)
{
  ANTRecording *recObj = Nan::ObjectWrap::Unwrap<ANTRecording>(info.This());
  DBusMessage *reply;
  dbusRequest *dbus_request = NULL; 
  DBusMessage *message;
  DBusError err;
  unsigned camera_id;
  if(!info[0]->IsNumber())
  {
    Nan::ThrowTypeError("First parameter should be Camera ID");
    return;
  }
  if(!info[1]->IsString())
  {
    Nan::ThrowTypeError("Second parameter should be File Path");
    return;
  }
  if(!recObj->conn){
    if(!(recObj->initDbus()))
    {
      Nan::ThrowError("D-Bus Initialization Failed\n");
      return;
    }
  }
  v8::String::Utf8Value param1(info[1]->ToString());
  std::string path = std::string(*param1);

  camera_id = Nan::To<int>(info[0]).FromJust();
  
  dbus_request = new dbusRequest();
  dbus_request->pid = getPid();
  dbus_request->camera_id = camera_id;
  dbus_request->play_seconds = 1;
  dbus_request->file_path = path;
  dbus_request->width = 1920;
  dbus_request->height = 1080;

  message = recObj->sendDbusMsg(snap_start_request, dbus_request);
  
//  reply = dbus_connection_send_with_reply_and_block(recObj->conn,
//      message, 500, &err);
  sleep(1);
  dbus_message_unref(message);
//  dbus_message_unref(reply);
}

NAN_METHOD(ANTRecording::streamingStart)
{
  ANTRecording *recObj = Nan::ObjectWrap::Unwrap<ANTRecording>(info.This());
  DBusMessage* message;
  std::string ip_address;
  dbusStreamingRequest *dbus_request;
  unsigned camera_id;
  unsigned port;
  if(!info[0]->IsNumber())
  {
    Nan::ThrowTypeError("First parameter should be Camera ID");
    return;
  }
  if(!info[1]->IsString())
  {
    Nan::ThrowTypeError("Second Parameter Should be IP Address");
    return;
  }
  if(!info[2]->IsNumber())
  {
    Nan::ThrowTypeError("Third Parameter should be a Port Number");
    return;
  }
  v8::String::Utf8Value param1(info[1]->ToString());
  ip_address = std::string(*param1);
  camera_id = Nan::To<int>(info[0]).FromJust();  
  port = Nan::To<int>(info[2]).FromJust();  
  
  if(!(recObj->initDbus()))
  {
    Nan::ThrowError("D-Bus Initiailization Failed\n");
    return;
  }
  dbus_request = new dbusStreamingRequest();
  dbus_request->pid = getPid();
  dbus_request->camera_id = camera_id;
  dbus_request->ip_address = ip_address;
  dbus_request->port = port;
  message = recObj->sendStreamingDbusMsg(streaming_start_request, dbus_request);
  
}

NAN_METHOD(ANTRecording::streamingStop)
{
  ANTRecording *recObj = Nan::ObjectWrap::Unwrap<ANTRecording>(info.This());
  DBusMessage* message;
  std::string ip_address;
  dbusStreamingRequest *dbus_request;
  unsigned camera_id;
  unsigned port;
  if(!info[0]->IsNumber())
  {
    Nan::ThrowTypeError("First parameter should be Camera ID");
    return;
  }
  if(!info[1]->IsString())
  {
    Nan::ThrowTypeError("Second Parameter Should be IP Address");
    return;
  }
  if(!info[2]->IsNumber())
  {
    Nan::ThrowTypeError("Third Parameter should be a Port Number");
    return;
  }
  v8::String::Utf8Value param1(info[1]->ToString());
  ip_address = std::string(*param1);
  camera_id = Nan::To<int>(info[0]).FromJust();  
  port = Nan::To<int>(info[2]).FromJust();  

  if(!(recObj->initDbus()))
  {
    Nan::ThrowError("D-Bus Initiailization Failed\n");
    return;
  }
	dbus_request = new dbusStreamingRequest();
    dbus_request->pid = getPid();
	dbus_request->camera_id = camera_id;
  dbus_request->ip_address = ip_address;
  dbus_request->port = port;
	message = recObj->sendStreamingDbusMsg(streaming_stop_request, dbus_request);
  //message = dbus_message_new_signal(dbus_path, dbus_interface, streaming_stop_request);
  //dbus_connection_send(recObj->conn, message, NULL);
}


NAN_METHOD(ANTRecording::sensorOverlayStart)
{
  ANTRecording *recObj = Nan::ObjectWrap::Unwrap<ANTRecording>(info.This());
  dbusSensorOverlayRequest *dbus_request = NULL; 
  DBusMessage *message;
  DBusError err;
  unsigned camera_id;

  if(!info[0]->IsNumber())
  {
    Nan::ThrowTypeError("First parameter should be Camera ID");
    return;
  }
  if(!info[1]->IsString())
  {
    Nan::ThrowTypeError("Second parameter should be Sensor Name");
    return;
  }
  if(!recObj->conn){
    if(!(recObj->initDbus()))
    {
      Nan::ThrowError("D-Bus Initialization Failed\n");
      return;
    }
  }
  v8::String::Utf8Value param1(info[1]->ToString());
  std::string sensor_name = std::string(*param1);

  camera_id = Nan::To<int>(info[0]).FromJust();
  
  dbus_request = new dbusSensorOverlayRequest();
  dbus_request->pid = getPid();
  dbus_request->camera_id = camera_id;
  dbus_request->sensor_name = sensor_name;

  message = recObj->sendSensorOverlayDbusMsg(sensor_overlay_start_request, dbus_request);
  
  //sleep(1);
  dbus_message_unref(message);
}

NAN_METHOD(ANTRecording::sensorOverlayStop)
{
  ANTRecording *recObj = Nan::ObjectWrap::Unwrap<ANTRecording>(info.This());
  dbusSensorOverlayRequest *dbus_request = NULL; 
  DBusMessage *message;
  DBusError err;
  unsigned camera_id;

  if(!info[0]->IsNumber())
  {
    Nan::ThrowTypeError("First parameter should be Camera ID");
    return;
  }
  if(!info[1]->IsString())
  {
    Nan::ThrowTypeError("Second parameter should be Sensor Name");
    return;
  }
  if(!recObj->conn){
    if(!(recObj->initDbus()))
    {
      Nan::ThrowError("D-Bus Initialization Failed\n");
      return;
    }
  }
  v8::String::Utf8Value param1(info[1]->ToString());
  std::string sensor_name = std::string(*param1);

  camera_id = Nan::To<int>(info[0]).FromJust();
  
  dbus_request = new dbusSensorOverlayRequest();
  dbus_request->pid = getPid();
  dbus_request->camera_id = camera_id;
  dbus_request->sensor_name = sensor_name;

  message = recObj->sendSensorOverlayDbusMsg(sensor_overlay_stop_request, dbus_request);
  
  //sleep(1);
  dbus_message_unref(message);
}

NAN_METHOD(ANTRecording::preRecInit)
{
  ANTRecording *recObj = Nan::ObjectWrap::Unwrap<ANTRecording>(info.This());
  DBusMessage* message;
  std::string ip_address;
  dbusRequest *dbus_request;
  unsigned camera_id;
  unsigned pre_rec_time;
  if(!info[0]->IsNumber())
  {
    Nan::ThrowTypeError("First parameter should be Camera ID");
    return;
  }
  if(!info[1]->IsNumber())
  {
    Nan::ThrowTypeError("Second Parameter should be a Pre Recording Time");
    return;
  }
  camera_id = Nan::To<int>(info[0]).FromJust();  
  pre_rec_time = Nan::To<int>(info[1]).FromJust();  

  if(!(recObj->initDbus()))
  {
    Nan::ThrowError("D-Bus Initiailization Failed\n");
    return;
  }
  dbus_request = new dbusRequest();
  dbus_request->pid = getPid();
  dbus_request->camera_id= camera_id;
  dbus_request->play_seconds = pre_rec_time;
  message = recObj->sendPreRecInitDbusMsg(pre_rec_init_request, dbus_request);
}

// TODO: preRecFinish?

NAN_METHOD(ANTRecording::preRecStart)
{
  ANTRecording *recObj = Nan::ObjectWrap::Unwrap<ANTRecording>(info.This());
  DBusMessage* message;
  std::string file_path;
  dbusRequest *dbus_request;
  unsigned camera_id;
  unsigned play_seconds;
  if(!info[0]->IsNumber())
  {
    Nan::ThrowTypeError("First parameter should be Camera ID");
    return;
  }
  if(!info[1]->IsString())
  {
    Nan::ThrowTypeError("Second Parameter Should be File Path");
    return;
  }
  if(!info[2]->IsNumber())
  {
    Nan::ThrowTypeError("Third Parameter should be Play Seconds");
    return;
  }
  v8::String::Utf8Value param1(info[1]->ToString());
  file_path = std::string(*param1);
  camera_id = Nan::To<int>(info[0]).FromJust();  
  play_seconds = Nan::To<int>(info[2]).FromJust();  
  
  if(!(recObj->initDbus()))
  {
    Nan::ThrowError("D-Bus Initiailization Failed\n");
    return;
  }
  dbus_request = new dbusRequest();
  dbus_request->pid = getPid();
  dbus_request->camera_id = camera_id;
  dbus_request->play_seconds = play_seconds;
  dbus_request->file_path = file_path;
  dbus_request->width = 1920;
  dbus_request->height = 1080;
  dbus_request->fps = 30;
  message = recObj->sendPreRecDbusMsg(pre_rec_start_request, dbus_request);
}

NAN_METHOD(ANTRecording::preRecStop)
{
  ANTRecording *recObj = Nan::ObjectWrap::Unwrap<ANTRecording>(info.This());
  DBusMessage* message;
  dbusRequest *dbus_request;
  unsigned camera_id;
  if(!info[0]->IsNumber())
  {
    Nan::ThrowTypeError("First parameter should be Camera ID");
    return;
  }
  camera_id = Nan::To<int>(info[0]).FromJust();  
  
  if(!(recObj->initDbus()))
  {
    Nan::ThrowError("D-Bus Initiailization Failed\n");
    return;
  }
  dbus_request = new dbusRequest();
  dbus_request->pid = getPid();
  dbus_request->camera_id = camera_id;
  // TODO: Really need file path??
  message = recObj->sendPreRecDbusMsg(pre_rec_stop_request, dbus_request);
}

bool ANTRecording::initDbus()
{
  dbus_error_init(&err);
  conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
  if(!conn)
  {
    dbus_error_free(&err);
    return false;
  }
  return true;
}

NAN_METHOD(ANTRecording::New)
{
  if(info.IsConstructCall())
  {
    ANTRecording *recObj = new ANTRecording();
    recObj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  }
  else
  {
    v8::Local<v8::Function> cons = Nan::New(constructor());
    info.GetReturnValue().Set(cons->NewInstance(0, 0));
  }
}

NAN_MODULE_INIT(ANTRecording::Init)
{
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  
  tpl->SetClassName(Nan::New("ANTRecording").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  SetPrototypeMethod(tpl, "recordingStart", recStart);
  SetPrototypeMethod(tpl, "recordingStop", recStop);

  SetPrototypeMethod(tpl, "snapshot", jpegStart);

  SetPrototypeMethod(tpl, "streamingStart", streamingStart);
  SetPrototypeMethod(tpl, "streamingStop", streamingStop);

  SetPrototypeMethod(tpl, "sensorOverlayStart", sensorOverlayStart);
  SetPrototypeMethod(tpl, "sensorOverlayStop", sensorOverlayStop);

  SetPrototypeMethod(tpl, "preRecInit", preRecInit);
  SetPrototypeMethod(tpl, "preRecStart", preRecStart);
  SetPrototypeMethod(tpl, "preRecStop", preRecStop);

  constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("ANTRecording").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NODE_MODULE(ANTRecording, ANTRecording::Init)
