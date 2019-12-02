#include "stream/ant_native_stream.h"

#include "iotjs_def.h"

JS_FUNCTION(test_pipeline) {
  iotjs_string_t argIpAddress;
  bool result;

  // Function input arguments
  DJS_CHECK_ARGS(1, string);
  argIpAddress = JS_GET_ARG(0, string);

  result = ant_stream_test_pipeline(iotjs_string_data(&argIpAddress));

  // Destroy iotjs objects
  iotjs_string_destroy(&argIpAddress);
  return jerry_create_boolean(result);
}

jerry_value_t InitANTNative() {
  jerry_value_t mymodule = jerry_create_object();
  // iotjs_jval_set_property_string_raw(mymodule, "message", "Hello world!");
  iotjs_jval_set_method(mymodule, "test_pipeline", test_pipeline);
  return mymodule;
}