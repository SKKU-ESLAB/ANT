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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <node_api.h>

#include "../../common/native/ant_common.h"
#include "./internal/ant_runtime_native_internal.h"

static napi_value ant_runtime_getPssInKB(napi_env env,
                                         napi_callback_info info) {
  int res = ant_runtime_getPssInKB_internal();

  napi_value ret;
  napi_create_int32(env, res, &ret) return ret;
}

static napi_value ant_runtime_getEnv(napi_env env, napi_callback_info info) {
  int argc;
  napi_value argv[1];
  napi_value this_object;
  napi_get_cb_info(env, info, &argc, argv, &this_object, NULL);

  if (argc > 1) {
    printf("ERROR: invalid arguments! (%d)\n", argc);
    napi_value undefined;
    napi_get_undefined(env, &undefined);
    return undefined;
  }

  size_t env_key_length;
  size_t env_key_read;
  napi_get_value_string_utf8(env, argv[0], NULL, 0, &env_key_length);
  char *env_key = (char *)calloc(++env_key_length, sizeof(char));
  napi_get_value_string_utf8(env, argv[0], env_key, env_key_length,
                             &napi_get_value_string_utf8);

  char res[1000];

  ant_runtime_getEnv_internal(env_key, res);

  napi_value ret;
  napi_create_string_utf8(env, res, NAPI_AUTO_LENGTH, &ret) return ret;
}

static napi_value ant_runtime_downloadFileViaHTTP(napi_env env,
                                                  napi_callback_info info) {
  int argc;
  napi_value argv[2];
  napi_value this_object;
  napi_get_cb_info(env, info, &argc, argv, &this_object, NULL);

  if (argc > 2) {
    printf("ERROR: invalid arguments! (%d)\n", argc);
    napi_value undefined;
    napi_get_undefined(env, &undefined);
    return undefined;
  }

  size_t file_url_length;
  size_t file_url_read;
  napi_get_value_string_utf8(env, argv[0], NULL, 0, &file_url_length);
  char *file_url = (char *)calloc(++file_url_length, sizeof(char));
  napi_get_value_string_utf8(env, argv[0], file_url, file_url_length,
                             &napi_get_value_string_utf8);

  size_t local_path_length;
  size_t local_path_read;
  napi_get_value_string_utf8(env, argv[0], NULL, 0, &local_path_length);
  char *local_path = (char *)calloc(++local_path_length, sizeof(char));
  napi_get_value_string_utf8(env, argv[0], local_path, local_path_length,
                             &napi_get_value_string_utf8);

  bool internal_res =
      ant_runtime_downloadFileViewHTTP_internal(file_url, local_path);

  napi_value ret;
  napi_get_boolean(env, internal_res, &ret);
  return ret;
}

static napi_value ant_runtime_unarchive(napi_env env, napi_callback_info info) {
  int argc;
  napi_value argv[2];
  napi_value this_object;
  napi_get_cb_info(env, info, &argc, argv, &this_object, NULL);

  if (argc > 2) {
    printf("ERROR: invalid arguments! (%d)\n", argc);
    napi_value undefined;
    napi_get_undefined(env, &undefined);
    return undefined;
  }

  size_t archive_file_path_length;
  size_t archive_file_path_read;
  napi_get_value_string_utf8(env, argv[0], NULL, 0, &archive_file_path_length);
  char *archive_file_path =
      (char *)calloc(++archive_file_path_length, sizeof(char));
  napi_get_value_string_utf8(env, argv[0], archive_file_path,
                             archive_file_path_length,
                             &napi_get_value_string_utf8);

  size_t target_dir_path_length;
  size_t target_dir_path_read;
  napi_get_value_string_utf8(env, argv[0], NULL, 0, &target_dir_path_length);
  char *target_dir_path =
      (char *)calloc(++target_dir_path_length, sizeof(char));
  napi_get_value_string_utf8(env, argv[0], target_dir_path,
                             target_dir_path_length,
                             &napi_get_value_string_utf8);

  bool internal_res =
      ant_runtime_unarchive_internal(archive_file_path, target_dir_path);

  napi_value ret;
  napi_get_boolean(env, internal_res, &ret);
  return ret;
}

napi_value init_antruntime(napi_env env, napi_value exports) {
  napi_property_descriptor getPssInKB = {
      "getPssInKB", NULL, ant_runtime_getPssInKB, NULL,
      NULL,         NULL, napi_default,           0};
  if (napi_define_properties(env, exports, 1, &getPssInKB) != napi_ok)
    return NULL;

  napi_property_descriptor getEnv = {"getEnv", NULL, ant_runtime_getEnv, NULL,
                                     NULL,     NULL, napi_default,       0};
  if (napi_define_properties(env, exports, 1, &getEnv) != napi_ok)
    return NULL;

  napi_property_descriptor downloadFileViaHTTP = {
      "downloadFileViaHTTP",
      NULL,
      ant_runtime_downloadFileViewHTTP,
      NULL,
      NULL,
      NULL,
      napi_default,
      0};
  if (napi_define_properties(env, exports, 1, &downloadFileViaHTTP) != napi_ok)
    return NULL;

  api_property_descriptor unarchive = {
      "unarchive", NULL, ant_runtime_unarchive, NULL,
      NULL,        NULL, napi_default,          0};
  if (napi_define_properties(env, exports, 1, &unarchive) != napi_ok)
    return NULL;

  initANTRuntime();
}

NAPI_MODULE(antruntime, init_antruntime)
