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

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <curl/curl.h>
#include <libtar.h>

#include "../../../common/native/ant_common.h"
#include "./ant_runtime_native_internal.h"

int ant_runtime_getPssInKB_internal() {
  FILE *cmd;
  char result[24] = {0x0};
  int pssInKB = -1;
  char commandLine[200];
  snprintf(commandLine, sizeof(commandLine),
           "cat /proc/%d/smaps | grep -i pss |  awk '{Total+=$2} END "
           "{print Total}'",
           (int)getpid());
  cmd = popen(commandLine, "r");
  while (fgets(result, sizeof(result), cmd) != NULL) {
    sscanf(result, "%d", &pssInKB);
  }
  pclose(cmd);

  return pssInKB;
}

void ant_runtime_getEnv_internal(const char *envKey, char *resultEnvValue) {
  char *_envValue = getenv(envKey);
  if (_envValue == NULL) {
    resultEnvValue[0] = '\0';
  } else {
    strncpy(resultEnvValue, _envValue, strlen(_envValue) + 1);
  }
  return;
}

static size_t http_get_file_cb(void *ptr, size_t size, size_t nmemb,
                               void *stream) {
  fflush(stream);
  size_t n = fwrite(ptr, size, nmemb, stream);
  return n;
}

bool ant_runtime_downloadFileViaHTTP_internal(const char *url,
                                              const char *local_path) {
  CURL *req = curl_easy_init();
  if (!req) {
    printf("ERROR: cannot initialize curl!\n");
    return false;
  }

  FILE *fp = fopen(local_path, "wb");
  if (!fp) {
    printf("ERROR: cannot open file!: %s\n", local_path);
    return false;
  }

  curl_easy_setopt(req, CURLOPT_URL, url);
  curl_easy_setopt(req, CURLOPT_HTTPGET, 1);
  curl_easy_setopt(req, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt(req, CURLOPT_WRITEFUNCTION, http_get_file_cb);
  curl_easy_setopt(req, CURLOPT_WRITEDATA, fp);
  int res_curl = curl_easy_perform(req);

  uint32_t status;
  curl_easy_getinfo(req, CURLINFO_RESPONSE_CODE, &status);

  curl_easy_cleanup(req);
  fclose(fp);

  bool res = (200 == status) && (CURLE_ABORTED_BY_CALLBACK != res_curl);
  if (!res) {
    printf("ERROR: error on downloading %s (status code: %s)", url, status);
  }
  return res;
}

bool ant_runtime_unarchive_internal(const char *archiveFilePath,
                                    const char *targetDirectoryPath) {
  TAR *tar_handler;
  int ret;
  ret = tar_open(&tar_handler, archiveFilePath, NULL, O_RDONLY, 0644, TAR_GNU);
  if (ret < 0) {
    printf("Error: failed to open tar file %s / %s\n", archiveFilePath,
           strerror(errno));
    return false;
  }

  ret = tar_extract_all(tar_handler, (char *)targetDirectoryPath);
  if (ret < 0) {
    printf("Error: failed to extract all from tar file %s / %s\n",
           archiveFilePath, targetDirectoryPath, strerror(errno));
    return false;
  }

  ret = tar_close(tar_handler);
  if (ret < 0) {
    printf("Error: failed to close tar file %s / %s\n", archiveFilePath,
           strerror(errno));
    return false;
  }
  return true;
}

void initANTRuntime(void) {
  // Empty function
}
