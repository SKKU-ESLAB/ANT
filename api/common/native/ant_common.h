/* Copyright (c) 2017-2020 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __ANT_COMMON_H__
#define __ANT_COMMON_H__

#ifdef __cplusplus
#include <string>
#endif

// TODO(RedCarrottt): hardcoded result message length
#define MAX_RESULT_MESSAGE_LENGTH 100

#define ANT_API_STRING_TO_STRING(api_name, function_name)                      \
  JS_FUNCTION(api_name##_##function_name) {                                    \
    iotjs_string_t argString;                                                  \
    char result_string[MAX_RESULT_MESSAGE_LENGTH];                             \
    DJS_CHECK_ARGS(1, string);                                                 \
    argString = JS_GET_ARG(0, string);                                         \
    api_name##_##function_name##_internal(iotjs_string_data(&argString),       \
                                          result_string);                      \
    iotjs_string_destroy(&argString);                                          \
    return jerry_create_string((const jerry_char_t *)result_string);           \
  }

#define ANT_API_STRING_TO_VOID(api_name, function_name)                        \
  JS_FUNCTION(api_name##_##function_name) {                                    \
    iotjs_string_t argString;                                                  \
    DJS_CHECK_ARGS(1, string);                                                 \
    argString = JS_GET_ARG(0, string);                                         \
    api_name##_##function_name##_internal(iotjs_string_data(&argString));      \
    iotjs_string_destroy(&argString);                                          \
    return jerry_create_undefined();                                           \
  }

#define ANT_API_VOID_TO_STRING(api_name, function_name)                        \
  JS_FUNCTION(api_name##_##function_name) {                                    \
    char result_string[MAX_RESULT_MESSAGE_LENGTH];                             \
    api_name##_##function_name##_internal(result_string);                      \
    return jerry_create_string((const jerry_char_t *)result_string);           \
  }

#define ANT_API_VOID_TO_NUMBER(api_name, function_name)                        \
  JS_FUNCTION(api_name##_##function_name) {                                    \
    int result_number;                                                         \
    result_number = api_name##_##function_name##_internal();                   \
    return jerry_create_number(result_number);                                 \
  }

#define ANT_API_VOID_TO_VOID(api_name, function_name)                          \
  JS_FUNCTION(api_name##_##function_name) {                                    \
    api_name##_##function_name##_internal();                                   \
    return jerry_create_undefined();                                           \
  }

#define REGISTER_ANT_API(module, api_name, function_name)                      \
  iotjs_jval_set_method(module, #api_name "_" #function_name,                  \
                        api_name##_##function_name);

#define GET_NATIVE_INFO(name) name##_native_info
#define ANT_DEFINE_NATIVE_HANDLE_INFO(name)                                    \
  static void name##_destroy(##name##_t *native_object);                       \
  static const jerry_object_native_info_t GET_NATIVE_INFO(name) = {            \
      .free_cb = (jerry_object_native_free_callback_t)name##_destroy}
#define GET_NOBJECT_P(jobject, name)                                           \
  (name##_t *) jerry_get_object_native_pointer(jobject, NULL,                  \
    &GET_NATIVE_INFO(name)

#endif /* !defined(__ANT_COMMON_H__) */
