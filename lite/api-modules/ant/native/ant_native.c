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

JS_FUNCTION(ant_stream_sendDbusSignal) {
  iotjs_string_t argMessage;
  bool result;

  // Function input arguments
  DJS_CHECK_ARGS(1, string);
  argMessage = JS_GET_ARG(0, string);

  // Internal native function call
  result = ant_stream_sendDbusSignal_internal(iotjs_string_data(&argMessage));

  // Destroy iotjs objects
  iotjs_string_destroy(&argMessage);
  return jerry_create_boolean(result);
}

jerry_value_t InitANTNative() {
  jerry_value_t mymodule = jerry_create_object();
  // iotjs_jval_set_property_string_raw(mymodule, "message", "Hello world!");
  iotjs_jval_set_method(mymodule, "stream_testPipeline",
                        ant_stream_testPipeline);
  iotjs_jval_set_method(mymodule, "stream_sendDbusSignal", ant_stream_sendDbusSignal);

  initANTStream();

  return mymodule;
}