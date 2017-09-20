/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>
#include <comm_core.h>
#include <cstring>
#include <string>

static ANT_Server *op_server;
static int connected;

using v8::Function;
using v8::Local;
using v8::Number;
using v8::Value;
using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;
using Nan::Callback;
using Nan::HandleScope;
using Nan::New;
using Nan::Null;
using Nan::To;

static Nan::Callback *main_cb;
void onAck(ANT_MSG *op_msg, int status)
{
	Nan::Callback *cb = NULL;
  v8::Isolate *isolate = v8::Isolate::GetCurrent();
	if(NULL != op_msg->priv)
		cb = (Nan::Callback *)op_msg->priv;

	if(NULL != cb){
		Local<Value> argv[] = {
			v8::String::NewFromUtf8(isolate, (const char *)op_msg->get_data())
			, New<Number>(status)
		};

		cb->Call(2, argv);
	}
}

void onRead(ANT_MSG *op_msg, int status)
{
  v8::Isolate *isolate = v8::Isolate::GetCurrent();
	if(main_cb != NULL){
		Local<Value> argv[] = {
			v8::String::NewFromUtf8(isolate, (const char *)op_msg->get_data())
				, New<Number>(status)
		};

		main_cb->Call(2, argv);
	}
}
/* OpenChannel (String, Callback(msg, err)) */
NAN_METHOD(OpenChannel) {
	char intf_name[256];
	if(!info[0]->IsString())
		Nan::ThrowTypeError("First argument must be a string");

	v8::String::Utf8Value ufv(info[0]->ToString());
	int len = ufv.length() + 1;
	std::strncpy(intf_name, *ufv, len);
	
	main_cb = new Nan::Callback(info[1].As<Function>());

	op_server = new ANT_Server(intf_name, onRead);
}

/* SendMsg (String, MSG_req_id, ack handler) */
NAN_METHOD(SendMsg) {
	char msg_buff[1024] = {0};
	Nan::Callback *cb;
	int req_id = 0;
	if(!info[0]->IsString())
		Nan::ThrowTypeError("First argument must be a string");

	v8::String::Utf8Value ufv(info[0]->ToString());
	int len = ufv.length() + 1;
	std::strncpy(msg_buff, *ufv, len);

	printf("Send:%s\n", msg_buff);

	if(info.Length() == 1){
		op_server->msg_write(msg_buff, len);
	}
	else if(info.Length() == 2){
		if(info[1]->IsFunction()){
			cb = new Nan::Callback(info[1].As<Function>());
			op_server->msg_write(msg_buff, len, NULL, onAck, 0, (void *)cb);
		}
		else{
			req_id = info[1]->Int32Value();
			ANT_MSG *op_msg = new ANT_MSG();
			op_msg->set_req_id(req_id); 
			op_server->msg_write(msg_buff, len, op_msg);
		}
	}
	else{
		req_id = info[1]->Int32Value();
		ANT_MSG *op_msg = new ANT_MSG();
		op_msg->set_req_id(req_id); 

		cb = new Nan::Callback(info[2].As<Function>());
		op_server->msg_write(msg_buff, len, op_msg, onAck, 0, (void *)cb);
	}
}
/* SendFile (String File Name, MSG_req_id, ack handler) */
NAN_METHOD(SendFile) {
	char name_buff[24] = {0};
	Nan::Callback *cb;
	int req_id = 0;
	if(!info[0]->IsString())
		Nan::ThrowTypeError("First argument must be a string");

	v8::String::Utf8Value ufv(info[0]->ToString());
	int len = ufv.length() + 1;
	if(len > 24)
		Nan::ThrowTypeError("First argument must be less than 24 characters");
	
	std::strncpy(name_buff, *ufv, len);

	printf("Send:%s\n", name_buff);

	if(info.Length() == 1){
		op_server->file_write(name_buff);
	}
	else if(info.Length() == 2){
		if(info[1]->IsFunction()){
			cb = new Nan::Callback(info[1].As<Function>());
			op_server->file_write(name_buff);
		//	op_server->file_write(msg_buff, len, NULL, onAck, 0, (void *)cb);
		}
		else{
			req_id = info[1]->Int32Value();
			ANT_MSG *op_msg = new ANT_MSG();
			op_msg->set_req_id(req_id); 
			op_server->file_write(name_buff, op_msg);
		}
	}
	else{
		req_id = info[1]->Int32Value();
		ANT_MSG *op_msg = new ANT_MSG();
		op_msg->set_req_id(req_id); 

		cb = new Nan::Callback(info[2].As<Function>());
		//op_server->msg_write(msg_buff, len, op_msg, onAck, 0, (void *)cb);
		op_server->file_write(name_buff, op_msg);
	}
}

