#include "stream/ant_native_stream.h"

#include "iotjs_def.h"

JS_FUNCTION(ant_stream_testPipeline) {
  iotjs_string_t argIpAddress;
  bool result;

  // Function input arguments
  DJS_CHECK_ARGS(1, string);
  argIpAddress = JS_GET_ARG(0, string);

  // Internal native function call
  result = ant_stream_testPipeline_internal(iotjs_string_data(&argIpAddress));

  // Destroy iotjs objects
  iotjs_string_destroy(&argIpAddress);
  return jerry_create_boolean(result);
}

JS_FUNCTION(ant_stream_callDbusMethod) {
  iotjs_string_t argMessage;
  char result_string[100];

  // Function input arguments
  DJS_CHECK_ARGS(1, string);
  argMessage = JS_GET_ARG(0, string);

  // Internal native function call
  ant_stream_callDbusMethod_internal(iotjs_string_data(&argMessage),
                                     result_string);

  // Destroy iotjs objects
  iotjs_string_destroy(&argMessage);
  return jerry_create_string((const jerry_char_t *)result_string);
}

jerry_value_t InitANTNative() {
  jerry_value_t mymodule = jerry_create_object();
  // iotjs_jval_set_property_string_raw(mymodule, "message", "Hello world!");
  iotjs_jval_set_method(mymodule, "stream_testPipeline",
                        ant_stream_testPipeline);
  iotjs_jval_set_method(mymodule, "stream_callDbusMethod",
                        ant_stream_callDbusMethod);

  initANTStream();

  return mymodule;
}