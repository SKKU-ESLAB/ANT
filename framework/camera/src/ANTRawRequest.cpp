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

#include "ANTRawRequest.h"

const char *sem_path = "ORG.ANT.CAMERA";

ANTRawRequest *ANTRawRequest::ant_raw_request = NULL;

ANTRawRequest::ANTRawRequest(RequestType type, GstElement *bin)
    : CameraRequest(type, bin)
{
}

ANTRawRequest::~ANTRawRequest()
{
  uinitSharedMemorySpace(this->shm_id);
  uinitSemaphore(sem_path, this->sem);
  ant_raw_request = NULL;
}

ANTRawRequest *ANTRawRequest::getInstance(RequestType type, GstElement *bin)
{
  if (ant_raw_request == NULL)
    ant_raw_request = new ANTRawRequest(type, bin);
  return ant_raw_request;
}

bool ANTRawRequest::initializeSemAndSharedMemory(void)
{
  int ret = initSharedMemorySpace(1, RAW_DEFAULT_BUF_SIZE,
      &(this->shm_ptr), SHM_KEY_FOR_BUFFER);
  if(this->shm_ptr == NULL)
  {
    ANT_LOG_ERR(CAM, "Shared memory space has NULL address");
    return false;
  }
  if(ret == -1){
    ANT_LOG_ERR(CAM, "Shared memory space initialization failed");
    return false;
  }
  this->shm_id = ret;

  if(initSemaphore(sem_path, &(this->sem)) == false)
  {
    ANT_LOG_ERR(CAM, "Semaphore initailization failed");
    return false;
  }
  
  return true;
}

bool ANTRawRequest::uninitializeSemAndSharedMemory(void)
{
  uinitSharedMemorySpace(this->shm_id);
  uinitSemaphore(sem_path, this->sem);
}

static int initSharedMemorySpace(int _req_count, int _buffer_size,
    void** _shm_ptr, key_t _shmkey)
{
  int shmid;
  shmid = shmget((key_t)_shmkey,
      (_buffer_size*_req_count)+(sizeof(int)*_req_count), 0666|IPC_CREAT);

  if(shmid == -1)
  {
    ANT_LOG_ERR(CAM, "shmget failed : ");
    return -1;
  }
  *_shm_ptr = shmat(shmid, NULL, 0);

  if(*_shm_ptr == (void*)-1)
  {
    ANT_LOG_ERR(CAM, "shmget failed : ");
    return -1;
  }
  return shmid;
}

static int uinitSharedMemorySpace(int _shm_id)
{
  if(-1 == shmctl(_shm_id, IPC_RMID, 0))
  {
    ANT_LOG_ERR(CAM, "Failed to remove shared memory space");
    return 0;
  }
  return 1;
}

static bool initSemaphore(const char *path, sem_t **_sem)
{
  assert(path != NULL);
  *_sem = sem_open(path, O_CREAT, 0666, 1);
  if((*_sem) == SEM_FAILED)
  {
    ANT_LOG_ERR(CAM, "Semaphore Open Failed");
    sem_unlink(path);
    return false;
  }
  return true;		
}

static void uinitSemaphore(const char *path, sem_t *sem)
{
  sem_close(sem);
  sem_unlink(path);
  sem_destroy(sem);
}

