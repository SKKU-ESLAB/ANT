#include "ANTFaceDetection.h"
#include <nan.h>

NAN_METHOD(FaceDetection);

NAN_MODULE_INIT(InitAll) {
	Nan::Set(target, Nan::New<v8::String>("faceDetection").ToLocalChecked(), Nan::GetFunction(Nan::New<v8::FunctionTemplate>(FaceDetection)).ToLocalChecked());
}

NODE_MODULE(ANTFaceDetection, InitAll)
