/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Hayun Lee<lhy920806@gmail.com>
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

#ifndef ANT_RAW_REQUEST_H
#define ANT_RAW_REQUEST_H

#include "CameraDevice.h"
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <semaphore.h>

#define SHM_KEY_FOR_BUFFER 5315

#define RAW_DEFAULT_BUF_SIZE RAW_BGRX_480P_BUF_SIZE * 2
//#define RAW_DEFAULT_BUF_SIZE 4194304 

#define RAW_FORMAT_WIDTH RAW_480P_WIDTH
#define RAW_FORMAT_HEIGHT RAW_480P_HEIGHT

#define RAW_480P_WIDTH  640
#define RAW_480P_HEIGHT  480
#define RAW_BGRX_480P_BUF_SIZE 1228800

#define RAW_720P_WIDTH  1280
#define RAW_720P_HEIGHT  720
#define RAW_BGRX_720P_BUF_SIZE 3686400

#define RAW_1080P_WIDTH  1920
#define RAW_1080P_HEIGHT  1080
#define RAW_BGRX_1080P_BUF_SIZE 8294400

class ANTRawRequest : public CameraRequest
{
  public:
    static ANTRawRequest* getInstance(RequestType type, GstElement *bin);
    ~ANTRawRequest();

    bool initializeSemAndSharedMemory(void);
    bool uninitializeSemAndSharedMemory(void);
    
    int getShmId(void) 
    { return this->shm_id; }
    char* getShmPtr(void)
    { return (char*)this->shm_ptr; }
    void* getBufferPtr(void)
    { return this->shm_ptr; }
    char* getBufferSizePtr(void)
    { return (char*)this->shm_ptr + RAW_DEFAULT_BUF_SIZE; }
    sem_t* getSemaphore(void)
    { return this->sem; }
    
    bool getIsRun(void) const
    { return this->is_run; }
    void setIsRun(bool _is_run)
    { this->is_run = _is_run; }
    
  private:
    static ANTRawRequest *ant_raw_request;
    ANTRawRequest(RequestType type, GstElement *bin);

    int shm_id;
    void *shm_ptr;
    sem_t *sem;

    bool is_run;
};

static int initSharedMemorySpace(int _req_count, int _buffer_size,
    void** _shm_ptr, key_t _shmkey);
static int uinitSharedMemorySpace(int _shm_id);
static bool initSemaphore(const char *path, sem_t **_sem);
static void uinitSemaphore(const char *path, sem_t *_sem);

#endif /* ANT_RAW_REQUEST_H */
