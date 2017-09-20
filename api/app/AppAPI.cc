#include <node.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "AppAPIInternal.h"
#include "AppBase.h"
#include "ANTdbugLog.h"

using namespace v8;

#define MAX_NUMBER_OF_ITEMS 20
#define MESSAGE_BUFFER_SIZE 1024

// Utility Functions
#define getV8String(isolate, cstr) (String::NewFromOneByte(isolate, \
      (const uint8_t*)cstr))

AppBase* gAppBase;

void initAppBase(){
  printf("initialize AppBase");
  gAppBase = new AppBase();
  gAppBase->run();

  // Send CompleteLaunchingApp command to appcore
  gAppBase->completeLaunchingApp();
}

void makeEventPage(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  const char* description;

  v8::String::Utf8Value param1(args[0]->ToString());
  std::string inputJsonData = std::string(*param1);
  description = inputJsonData.c_str();

  if (args.Length() == 0) {
    args.GetReturnValue().Set(getV8String(isolate,
          "{\"noti\":\"noti\",\"description\":\"\"}"));
    return;
  } else if (args.Length() == 1) {
    char newJsonData[sizeof(char) * strlen(description) + 32];
    sprintf(newJsonData, "{\"noti\":\"noti\",\"description\":\"%s\"}",
        description);
    args.GetReturnValue().Set(getV8String(isolate, newJsonData));
    return;
  }

  isolate->ThrowException(Exception::TypeError(getV8String(isolate,
          "Invalid Use : arguments expected [Null or Description(string)]")));
}

void addEventText(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  int newJsonLength;

  const char* oldJsonData;
  const char* textView;

  if (args.Length() != 2) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "Invalid Use : 2 arguments expected [Page Name]")));
    return;
  } else if (!args[0]->IsString()) { //IsInteger, IsFunction, etc...
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "Wrong arguments")));
    return;
  } else if (!args[1]->IsString()) { //IsInteger, IsFunction, etc...
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "Wrong arguments")));
    return;
  }

  //--------------------- Page Name Check -------------------------//
  v8::String::Utf8Value param1(args[0]->ToString());
  std::string inputJsonData = std::string(*param1);
  oldJsonData = inputJsonData.c_str();

  v8::String::Utf8Value param2(args[1]->ToString());
  std::string inputText = std::string(*param2);
  textView = inputText.c_str();

  if (check_page_name(oldJsonData) != 1) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addEventItem argument error : " \
            "first arg is not the object of event page")));
    return;				
  }

  newJsonLength = strlen(oldJsonData) + strlen(textView) + 20;
  char newJsonData[sizeof(char) * newJsonLength];
  memset(newJsonData, '\0', newJsonLength);

  char tmpStr[strlen(oldJsonData)];
  strncpy(tmpStr, oldJsonData, strlen(oldJsonData)-1 );
  tmpStr[strlen(oldJsonData)-1] = '\0';

  sprintf(newJsonData, "%s,\"text\":\"%s\"}" , tmpStr, textView);

  args.GetReturnValue().Set(getV8String(isolate, newJsonData));
}

// [MORE] addDescription
// TODO: attach image file
void addEventImg(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  int newJsonLength;

  const char* oldJsonData;
  const char* imgPath;

  if (args.Length() != 2) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "Invalid Use : 2 arguments expected [Page Name]")));
    return;
  } else if (!args[0]->IsString()) { //IsInteger, IsFunction, etc...
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "Wrong arguments")));
    return;
  } else if (!args[1]->IsString()) { //IsInteger, IsFunction, etc...
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "Wrong arguments")));
    return;
  }

  //--------------------- Page Name Check -------------------------//
  v8::String::Utf8Value param1(args[0]->ToString());
  std::string inputJsonData = std::string(*param1);
  oldJsonData = inputJsonData.c_str();

  v8::String::Utf8Value param2(args[1]->ToString());
  std::string inputText = std::string(*param2);
  imgPath = inputText.c_str();

  if (check_page_name(oldJsonData) != 1) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addEventItem argument error : " \
            "first arg is not the object of event page")));
    return;				
  }

  newJsonLength = strlen(oldJsonData) + strlen(imgPath)+256 ;
  char newJsonData[sizeof(char) * newJsonLength];
  memset(newJsonData, '\0', newJsonLength);

  char tmpStr[strlen(oldJsonData)];
  strncpy(tmpStr, oldJsonData, strlen(oldJsonData)-1 );
  tmpStr[strlen(oldJsonData)-1] = '\0';

  sprintf(newJsonData, "%s,\"img\":\"%s\"}" , tmpStr, imgPath);

  args.GetReturnValue().Set(getV8String(isolate, newJsonData));
}

// send CompanionMessage.SendEventPage
void sendEventPage(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  // Arguments
  const char* jsonData;

  // Check arguments
  if ((args.Length() != 1) ||	!args[0]->IsString() ) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "Invalid Use : 1 arguments expected [Page obj]")));
    return;
  }

  // Get argument 0
  v8::String::Utf8Value param1(args[0]->ToString());
  std::string name_c = std::string(*param1);
  jsonData = name_c.c_str();
  if (check_page_name(jsonData) != 1) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "sendEventItem argument error : " \
            "first arg is not the object of event page")));
    return;
  }

  // send CompanionMessage.SendEventPage
  gAppBase->sendEventPageToCompanion(jsonData, false);
  printf("[NIL] SendEventPage to companion: %s\n", jsonData);

  return;
}

// send CompanionMessage.SendEventPage
void sendEventPageWithNoti(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  // Arguments
  const char* jsonData;

  // Check arguments
  if ((args.Length() != 1) ||	!args[0]->IsString()) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "Invalid Use : 1 arguments expected [Page obj]")));
    return;
  }

  // Get argument 0
  v8::String::Utf8Value param1(args[0]->ToString());
  std::string name_c = std::string(*param1);
  jsonData = name_c.c_str();
  if (check_page_name(jsonData) != 1) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "sendEventItem argument error : " \
            "first arg is not the object of event page")));
    return;				
  }

  // Send CompanionMessage.SendEventPage
  gAppBase->sendEventPageToCompanion(jsonData, true);
  printf("[NIL] SendEventPage to companion:  %s\n", jsonData);

  return;
}

//JSON Warnning : any string shouln't has '"' char
//OUTPUT Json format : 
/*
   {
   "conf":"conf",
   "strTB":"Name[descript/10]",
   "numTB":"Name[descript/1/10]",
   "sDialog":"Name[descript/a/b/c/d/e]",
   "mDialog":"Name[descript/a[a]/b/c/d/e]",
   "dateDialog":Name[descript/flag]",
   "timeDialog":Name[descript/flag]"
   }


*/

//----------------------   config page -------------------------//
void makeConfigPage(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  args.GetReturnValue().Set(getV8String(isolate, "{\"conf\":\"conf\"}"));
}

// addStrTextbox(page, name, description, length);
// Return new Json string
void addStrTextbox(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  int newJsonLength;
  const char* oldJsonData;
  const char* description;
  const char* name;
  const char* length;

  if (args.Length() != 4) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "Invalid Use : 1st arguments expected " \
            "[Page, Name(str), Description(str), length(int)]")));
    return;
  } else if (!args[0]->IsString()
      || !args[1]->IsString()
      || !args[2]->IsString()
      || !args[3]->NumberValue()) { //IsInteger, IsFunction, etc...
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "Wrong arguments")));
    return;
  }

  //--------------------- Page Name Check -------------------------//
  v8::String::Utf8Value param1(args[0]->ToString());
  std::string json = std::string(*param1);
  oldJsonData = json.c_str();

  v8::String::Utf8Value param2(args[1]->ToString());
  std::string name_c = std::string(*param2);
  name = name_c.c_str();

  v8::String::Utf8Value param11(args[2]->ToString());
  std::string des = std::string(*param11);
  description= des.c_str();

  v8::String::Utf8Value param3(args[3]->ToString());
  std::string length_c = std::string(*param3);
  length = length_c.c_str();

  if (check_page_name(oldJsonData) != 2){
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addConfigItem argument error :" \
            "first arg is not the object of config page")));
    return;				
  } else if (check_key_overlap(oldJsonData, name) == false){
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addConfigItem argument error : " \
            "the name(key) is already used in this page")));
    return;	
  }

  newJsonLength = strlen(oldJsonData) + strlen(name) + strlen(length) + 32;

  char newJsonData [sizeof(char) * newJsonLength];
  memset(newJsonData, '\0', newJsonLength);

  char tmpStr[strlen(oldJsonData)];
  strncpy(tmpStr, oldJsonData, strlen(oldJsonData)-1 );
  tmpStr[strlen(oldJsonData)-1] = '\0';

  sprintf(newJsonData, "%s,\"strTB\":\"%s[%s/%s]\"}",
      tmpStr, name, description,length);

  args.GetReturnValue().Set(getV8String(isolate, newJsonData));
}



// addNumberTextbox(page, name, description, range1, range2);
// Return new Json string
void addNumberTextbox(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  const char* oldJsonData;

  //	char* newJsonData;
  int newJsonLength;

  const char* name;
  const char* description;
  const char* range1;
  const char* range2;

  if (args.Length() != 5) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addNumberTextbox::Invalid Use : 5 arguments expected " \
            "[page,name,description,range1,range2]")));
    return;
  } else if (!args[0]->IsString()) { 
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addNumberTextbox::1st argument is wrong [page]")));
    return;
  } else if (!args[1]->IsString() ) { 
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addNumberTextbox::2nd argument is wrong [string:Name]")));
    return;
  } else if (!args[2]->IsString() ) { 
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addNumberTextbox::3rd argument is wrong [string:description]")));
    return;
  } else if (!args[3]->IsNumber()|| !args[4]->IsNumber() ) { 
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addNumberTextbox::4,5th argument is wrong " \
            "[int:range1, int:range2]")));
    return;
  }

  //--------------------- Page Name Check -------------------------//
  v8::String::Utf8Value param1(args[0]->ToString());
  std::string json = std::string(*param1);
  oldJsonData = json.c_str();

  v8::String::Utf8Value param2(args[1]->ToString());
  std::string name_c = std::string(*param2);
  name = name_c.c_str();

  v8::String::Utf8Value param11(args[2]->ToString());
  std::string des = std::string(*param11);
  description= des.c_str();

  v8::String::Utf8Value param3(args[3]->ToString());
  std::string range_1 = std::string(*param3);

  v8::String::Utf8Value param4(args[4]->ToString());
  std::string range_2 = std::string(*param4);

  if ( atof(range_1.c_str()) < atof(range_2.c_str())){
    range1 = range_1.c_str();
    range2 = range_2.c_str();
  } else {
    range1 = range_2.c_str();
    range2 = range_1.c_str();	
  }

  if (check_page_name(oldJsonData) != 2) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate, 
            "addConfigItem argument error : " \
            "first arg is not the object of config page")));
    return;				
  } if (check_key_overlap(oldJsonData, name) == false ) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addConfigItem argument error : " \
            "the name(key) is already used in this page")));
    return;	
  }

  newJsonLength = strlen(oldJsonData) + strlen(name) + strlen(range1)
    + strlen(range2) + 32;
  char newJsonData [sizeof(char) * newJsonLength];
  memset(newJsonData, '\0', newJsonLength);

  char tmpStr[strlen(oldJsonData)];
  strncpy(tmpStr, oldJsonData, strlen(oldJsonData)-1 );
  tmpStr[strlen(oldJsonData)-1] = '\0';

  sprintf(newJsonData, "%s,\"numTB\":\"%s[%s/%s/%s]\"}"
      , tmpStr, name, description, range1, range2);

  args.GetReturnValue().Set(getV8String(isolate, newJsonData));
}


// addSingleDialog(page, name, description, item1,2,3...);
// Return new Json string (+"sDialog":"Name[description/a/b/c/d/e]"),
void addSingleDialog(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  const char* oldJsonData;
  int newJsonLength;
  const char* name;
  char** items;
  int i;

  if (!args[0]->IsString()) { 
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addSingleDialog::1st argument is wrong [page]")));
    return;
  } else if (!args[1]->IsString() ) { 
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addSingleDialog::2nd argument is wrong [string:Name]")));
    return;
  }

  v8::String::Utf8Value param1(args[0]->ToString());
  std::string json = std::string(*param1);
  oldJsonData = json.c_str();

  v8::String::Utf8Value param2(args[1]->ToString());
  std::string name_c = std::string(*param2);
  name = name_c.c_str();

  if (check_page_name(oldJsonData) != 2){
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addSingleDialog argument error : " \
            "first arg is not the object of config page")));
    return;				
  } else if (check_key_overlap(oldJsonData, name) == false){
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addConfigItem argument error : " \
            "the name(key) is already used in this page")));
    return;	
  }

  // get items
  items = (char**) malloc(sizeof(char*) * MAX_NUMBER_OF_ITEMS);

  int itemsTotalLength=0;

  for (i=2; args[i]->IsString(); i++) {
    v8::String::Utf8Value param3(args[i]->ToString());
    std::string item = std::string(*param3);
    items[i-2] = (char*) malloc(sizeof(char) * strlen(item.c_str()) );
    strcpy(items[i-2], item.c_str());
    itemsTotalLength += strlen(item.c_str());


    if (i > MAX_NUMBER_OF_ITEMS+1 ) {

      isolate->ThrowException(Exception::TypeError(getV8String(isolate,
              "addSingleDialog argument error : " \
              "over the maximum number of items")));
      return;
    }
  }

  char* itemsString = (char*) malloc( (sizeof(char) * itemsTotalLength) + 4*i );
  memset(itemsString, '\0', (sizeof(char) * itemsTotalLength) + 4*i );

  for(int j = 0; j < i-2; j++) {
    if(strchr(items[j], '/') != NULL) {
      // some items contain '/' character -> cannot use that cuz it's token char
      isolate->ThrowException(Exception::TypeError(getV8String(isolate,
              "addSingleDialog::item cannot contain '/' character")));
      free(items);
      free(itemsString);
      return;
    }
    strcat(itemsString, items[j]);
    if(j == i-3){
      strcat(itemsString, "\0");
    } else{
      strcat(itemsString, "/");
    }
  }

  newJsonLength = strlen(oldJsonData) + strlen(name) + strlen(itemsString)+ 32;
  //	newJsonData = (char*)malloc(sizeof(char) * newJsonLength);
  char newJsonData [sizeof(char) * newJsonLength];
  memset(newJsonData, '\0', newJsonLength);

  char tmpStr[strlen(oldJsonData)];
  strncpy(tmpStr, oldJsonData, strlen(oldJsonData)-1 );
  tmpStr[strlen(oldJsonData)-1] = '\0';

  sprintf(newJsonData, "%s,\"sDialog\":\"%s[%s]\"}" , tmpStr, name, itemsString);
  free(items);
  free(itemsString);
  args.GetReturnValue().Set(getV8String(isolate, newJsonData));
}

// addMultipleDialog(page, name, description, item1,2,3...);
// Return new Json string (+"mDialog":"Name[description/a/b/c/d/e]")
void addMultipleDialog(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  const char* oldJsonData;
  int newJsonLength;
  const char* name;
  char** items;
  int i;

  if (!args[0]->IsString()) { 
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addMultipleDialog::1st argument is wrong [page]")));
    return;
  } else if (!args[1]->IsString() ) { 
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addMultipleDialog::2nd argument is wrong [string:Name]")));
    return;
  }

  v8::String::Utf8Value param1(args[0]->ToString());
  std::string json = std::string(*param1);
  oldJsonData = json.c_str();

  v8::String::Utf8Value param2(args[1]->ToString());
  std::string name_c = std::string(*param2);
  name = name_c.c_str();

  if (check_page_name(oldJsonData) != 2) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addMultipleDialog argument error : " \
            "first arg is not the object of config page")));
    return;				
  } else if (check_key_overlap(oldJsonData, name) == false) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addConfigItem argument error : " \
            "the name(key) is already used in this page")));
    return;	
  }

  // get items
  items = (char**) malloc(sizeof(char*) * MAX_NUMBER_OF_ITEMS);

  int itemsTotalLength=0;

  for(i=2; args[i]->IsString(); i++) {
    v8::String::Utf8Value param3(args[i]->ToString());
    std::string item = std::string(*param3);
    items[i-2] = (char*) malloc(sizeof(char) * strlen(item.c_str()) );
    strcpy(items[i-2], item.c_str());
    itemsTotalLength += strlen(item.c_str());


    if(i > MAX_NUMBER_OF_ITEMS + 1 ) {
      isolate->ThrowException(Exception::TypeError(getV8String(isolate,
              "addSingleDialog argument error : " \
              "over the maximum number of items")));
      return;
    }
  }

  char* itemsString = (char*) malloc( (sizeof(char) * itemsTotalLength) + 4*i );
  memset(itemsString, '\0', (sizeof(char) * itemsTotalLength) + 4*i );

  for(int j = 0; j < i-2; j++) {
    if(strchr(items[j], '/') != NULL) {
      // some items contain '/' character -> cannot use that cuz it's token char
      isolate->ThrowException(Exception::TypeError(getV8String(isolate,
              "addSingleDialog::item cannot contain '/' character")));
      free(items);
      free(itemsString);
      return;
    }
    strcat(itemsString, items[j]);
    if(j == i-3) {
      strcat(itemsString, "\0");
    } else {
      strcat(itemsString, "/");
    }
  }

  newJsonLength = strlen(oldJsonData) + strlen(name) + strlen(itemsString)+ 32;
  char newJsonData [sizeof(char) * newJsonLength];
  memset(newJsonData, '\0', newJsonLength);

  char tmpStr[strlen(oldJsonData)];
  strncpy(tmpStr, oldJsonData, strlen(oldJsonData)-1 );
  tmpStr[strlen(oldJsonData)-1] = '\0';

  sprintf(newJsonData, "%s,\"mDialog\":\"%s[%s]\"}" , tmpStr, name, itemsString);
  free(items);
  free(itemsString);
  args.GetReturnValue().Set(getV8String(isolate, newJsonData));
}

// addMultipleDialog(page, name, description, flag(0||1));
// Return new Json string (+"dateDialog":"Name[flag]")
// flag 0 = no constraint , 1 = allow only after current date
void addDateDialog(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  const char* oldJsonData;
  const char* name;
  const char* description;
  const char* flag;

  if (args.Length() != 4) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addDateDialog::Invalid Use : 4 arguments expected " \
            "[page,name,description, flag]")));
    return;
  } else if (!args[0]->IsString()) { 
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addDateDialog::1st argument is wrong [page]")));
    return;
  } else if (!args[1]->IsString() ) { 
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addDateDialog::2nd argument is wrong [string:Name]")));
    return;
  } else if (!args[2]->IsString() ) { 
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addDateDialog::3rd argument is wrong [string:description]")));
    return;
  } else if (!args[3]->IsNumber() ) { 
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addDateDialog::4th argument is wrong [int 0 or 1]")));
    return;
  }

  //--------------------- Page Name Check -------------------------//
  v8::String::Utf8Value param1(args[0]->ToString());
  std::string json = std::string(*param1);
  oldJsonData = json.c_str();

  v8::String::Utf8Value param2(args[1]->ToString());
  std::string name_c = std::string(*param2);
  name = name_c.c_str();

  v8::String::Utf8Value param11(args[2]->ToString());
  std::string des = std::string(*param11);
  description= des.c_str();

  v8::String::Utf8Value param3(args[3]->ToString());
  std::string flag_c = std::string(*param3);
  flag = flag_c.c_str();

  if (check_page_name(oldJsonData) != 2) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addDateDialog argument error : " \
            "first arg is not the object of config page")));
    return;				
  } else if (check_key_overlap(oldJsonData, name) == false) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addConfigItem argument error : " \
            "the name(key) is already used in this page")));
    return;	
  } else if (atoi(flag)!=0 && atoi(flag)!=1) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addDateDialog::3rd argument is wrong [int 0 or 1]")));
    return;
  }

  char newJsonData [MESSAGE_BUFFER_SIZE];
  memset(newJsonData, '\0', MESSAGE_BUFFER_SIZE);

  char tmpStr[strlen(oldJsonData)];
  strncpy(tmpStr, oldJsonData, strlen(oldJsonData)-1 );
  tmpStr[strlen(oldJsonData)-1] = '\0';

  sprintf(newJsonData, "%s,\"dateDialog\":\"%s[%s/%s]\"}",
      tmpStr, name, description, flag);

  args.GetReturnValue().Set(getV8String(isolate, newJsonData));
}

// addTimeDialog(page, name, flag(0||1));
// Return new Json string (+"timeDialog":"Name[flag]")
// flag 0 = no constraint , 1 = allow only after current time
void addTimeDialog(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  const char* oldJsonData;

  const char* name;
  const char* description;
  const char* flag;

  if (args.Length() != 4) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addDateDialog::Invalid Use : 4 arguments expected " \
            "[page,name,description, flag]")));
    return;
  } else if (!args[0]->IsString()) { 
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addDateDialog::1st argument is wrong [page]")));
    return;
  } else if (!args[1]->IsString() ) { 
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addDateDialog::2nd argument is wrong [string:Name]")));
    return;
  } else if (!args[2]->IsString() ) { 
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addDateDialog::3rd argument is wrong [string:description]")));
    return;
  } else if (!args[3]->IsNumber() ) { 
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addDateDialog::4th argument is wrong [int 0 or 1]")));
    return;
  }

  //--------------------- Page Name Check -------------------------//
  v8::String::Utf8Value param1(args[0]->ToString());
  std::string json = std::string(*param1);
  oldJsonData = json.c_str();

  v8::String::Utf8Value param2(args[1]->ToString());
  std::string name_c = std::string(*param2);
  name = name_c.c_str();

  v8::String::Utf8Value param11(args[2]->ToString());
  std::string des = std::string(*param11);
  description= des.c_str();

  v8::String::Utf8Value param3(args[3]->ToString());
  std::string flag_c = std::string(*param3);
  flag = flag_c.c_str();

  if (check_page_name(oldJsonData) != 2) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addDateDialog argument error : " \
            "first arg is not the object of config page")));
    return;				
  } else if (check_key_overlap(oldJsonData, name) == false) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addConfigItem argument error : " \
            "the name(key) is already used in this page")));
    return;	
  } else if (atoi(flag)!=0 && atoi(flag)!=1) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "addDateDialog::3rd argument is wrong [int 0 or 1]")));
    return;
  }

  char newJsonData [MESSAGE_BUFFER_SIZE];
  memset(newJsonData, '\0', MESSAGE_BUFFER_SIZE);

  char tmpStr[strlen(oldJsonData)];
  strncpy(tmpStr, oldJsonData, strlen(oldJsonData)-1 );
  tmpStr[strlen(oldJsonData)-1] = '\0';

  sprintf(newJsonData, "%s,\"timeDialog\":\"%s[%s/%s]\"}",
      tmpStr, name, description, flag);

  args.GetReturnValue().Set(getV8String(isolate, newJsonData));
}

/*//SEND Format to Android
  {
  "conf":"Pid",
  "rqID":"_rqID",
  "strTB":"Name[StrTextBoxDialog/10]",
  "numTB":"Age[NumberTextBoxDialog/0/100]",
  "sDialog":"Gender[SingleDialog/Male/Female]",
  "mDialog":"Hobby[MultiDialog/Book/Movie/Tennis/Jogging]",
  "dateDialog":"Birthday[DateDialog/0]",
  "timeDialog":"time[TimeDialog/0]"	
  }

*/
// send CompanionMessage.SendConfigPage
void sendConfigPage(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  // Arguments
  const char* jsonData;
  Local<Function> onUpdateAppConfigCallback;

  // Check arguments
  if ((args.Length() != 2) ||	!args[1]->IsFunction() || !args[0]->IsString() ) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "Invalid Use : 2 arguments expected [ConfigPage, Function]")));
    return;
  }

  // Get argument 0
  v8::String::Utf8Value param1(args[0]->ToString());
  std::string json_c = std::string(*param1);
  jsonData = json_c.c_str();
  if (check_page_name(jsonData) != 2){
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "sendConfig argument error : " \
            "first arg is not the object of config page")));
    return;				
  }

  // Get argument 1
  onUpdateAppConfigCallback = Local<Function>::Cast(args[1]);

  // Register onUpdateAppConfig callback
  gAppBase->setOnUpdateAppConfig(isolate, onUpdateAppConfigCallback);

  // Send CompanionMessage.SendConfigPage command
  gAppBase->sendConfigPageToCompanion(jsonData);

  printf("[NIL] SendConfigPage >>  %s \n", jsonData);

  return;
}

// getData(page[return page], name);
// Return name(key)->value [string]
void getData(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  const char* rcvJson;
  const char* name;


  if (args.Length() != 2) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "getData::Invalid Use : 2 arguments expected [configurableData,Key]")));
    return;
  } else if (!args[0]->IsString()) { 
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "getData::1st argument is wrong [page]")));
    return;
  } else if (!args[1]->IsString()) { 
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "getData::2nd argument is wrong [string:Name]")));
    return;
  }

  //--------------------- Page Name Check -------------------------//
  v8::String::Utf8Value param1(args[0]->ToString());
  std::string json = std::string(*param1);
  rcvJson = json.c_str();

  v8::String::Utf8Value param2(args[1]->ToString());
  std::string name_c = std::string(*param2);
  name = name_c.c_str();

  int position = 0;
  char key[512];
  char value[512];
  memset(key, '\0', 512);
  memset(value, '\0', 512);

  for(unsigned int i=0; i<strlen(rcvJson); i++){
    if (rcvJson[i] == '{' )
      continue;
    else if (rcvJson[i] == '}' ) {
      if(!strcmp(key, name)) {
        args.GetReturnValue().Set(getV8String(isolate, value));
      }
    }
    else if (rcvJson[i] == ':' ) {
      if(rcvJson[i+1] == '"') {
        position++;
      }
      else{
        char tmp[2];
        tmp[1] = '\0';
        tmp[0] = rcvJson[i];			

        if(!(position % 2)) // even num -> key
          strcat(key, tmp);
        else				// odd num -> value
          strcat(value, tmp);
      }
    } else if (rcvJson[i] == ',' ) {
      if(rcvJson[i+1] == '"') {
        position++;
        if(!strcmp(key, name)) {
          args.GetReturnValue().Set(getV8String(isolate, value));
        } else {
          memset(key, '\0', 512);
          memset(value, '\0', 512);	
        }
      }
    } else if (rcvJson[i] == '"' ) {
      continue;
    } else {
      char tmp[2];
      tmp[1] = '\0';
      tmp[0] = rcvJson[i];			

      if(!(position % 2)) // even num -> key
        strcat(key, tmp);
      else				// odd num -> value
        strcat(value, tmp);
    }
  }

  printf("[NIL] There is no value with the key[%s]\n", name);			
  args.GetReturnValue().Set(getV8String(isolate, "N/A"));
}
// [MORE] return multiple choice -> array

// onTermination(function)
void onTermination(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  // Arguments
  Local<Function> onTerminateCallback;

  // Check arguments
  if ((args.Length() != 1) ||	!args[0]->IsFunction()) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "Invalid Use : 1 arguments expected [Callback Function]")));
    return;
  }

  // Get argument 0
  onTerminateCallback = Local<Function>::Cast(args[0]);

  // Register onTerminate callback
  gAppBase->setOnTerminate(isolate, onTerminateCallback);

  return;
}

// send CompanionMessage.UpdateSensorData
void sendMsgToSensorViewer(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  // Arguments
  const char* jsonData;

  // Check arguments
  if ((args.Length() != 1) ||	!args[0]->IsString() ) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "Invalid Use : 1 arguments expected [SensorData obj]")));
    return;
  }

  // Get argument 0
  v8::String::Utf8Value param1(args[0]->ToString());
  std::string name_c = std::string(*param1);
  jsonData = name_c.c_str();

  // Send CompanionMessage.UpdateSensorData
  gAppBase->updateSensorDataToCompanion(jsonData);

  printf("[NIL] sendMsgToSensorViewer to companion: %s\n", jsonData);
  return;
}

void init(Handle<Object> exports) {
  Isolate* isolate = Isolate::GetCurrent();
  initAppBase();

  //-----------------------EventPage-------------------------------------------
  exports->Set(getV8String(isolate, "makeEventPage"),
      FunctionTemplate::New(isolate, makeEventPage)->GetFunction());
  exports->Set(getV8String(isolate, "addEventText"),
      FunctionTemplate::New(isolate, addEventText)->GetFunction());
  exports->Set(getV8String(isolate, "addEventImg"),
      FunctionTemplate::New(isolate, addEventImg)->GetFunction());
  exports->Set(getV8String(isolate, "sendEventPageWithNoti"),
      FunctionTemplate::New(isolate, sendEventPageWithNoti)->GetFunction());
  exports->Set(getV8String(isolate, "sendEventPage"),
      FunctionTemplate::New(isolate, sendEventPage)->GetFunction());


  //-----------------------ConfigPage-------------------------------------------

  exports->Set(getV8String(isolate, "makeConfigPage"),
      FunctionTemplate::New(isolate, makeConfigPage)->GetFunction());
  exports->Set(getV8String(isolate, "addStrTextbox"),
      FunctionTemplate::New(isolate, addStrTextbox)->GetFunction());
  exports->Set(getV8String(isolate, "addNumberTextbox"),
      FunctionTemplate::New(isolate, addNumberTextbox)->GetFunction());
  exports->Set(getV8String(isolate, "addSingleDialog"),
      FunctionTemplate::New(isolate, addSingleDialog)->GetFunction());
  exports->Set(getV8String(isolate, "addMultipleDialog"),
      FunctionTemplate::New(isolate, addMultipleDialog)->GetFunction());
  exports->Set(getV8String(isolate, "addDateDialog"),
      FunctionTemplate::New(isolate, addDateDialog)->GetFunction());
  exports->Set(getV8String(isolate, "addTimeDialog"),
      FunctionTemplate::New(isolate, addTimeDialog)->GetFunction());
  exports->Set(getV8String(isolate, "sendConfigPage"),
      FunctionTemplate::New(isolate, sendConfigPage)->GetFunction());

  exports->Set(getV8String(isolate, "getData"),
      FunctionTemplate::New(isolate, getData)->GetFunction());

  //------------------------termination-----------------------------------------
  exports->Set(getV8String(isolate, "onTermination"),
      FunctionTemplate::New(isolate, onTermination)->GetFunction());

  //------------------------sensor viewer---------------------------------------
  exports->Set(getV8String(isolate, "sendMsgToSensorViewer"),
      FunctionTemplate::New(isolate, sendMsgToSensorViewer)->GetFunction());
}

NODE_MODULE(nil, init)
