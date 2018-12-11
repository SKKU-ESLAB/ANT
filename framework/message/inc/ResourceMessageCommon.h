/* Copyright (c) 2017-2018 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
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
#ifndef __RESOURCE_MESSAGE_COMMON_H__
#define __RESOURCE_MESSAGE_COMMON_H__

// Use namespace + enum for readability
namespace ResourceOperationType {
enum Value {
  NotDetermined = 0,
  POST = 1,
  GET = 2,
  PUT = 3,
  DELETE = 4,
  DISCOVER = 5,
  SUBSCRIBE = 6,
  UNSUBSCRIBE = 7
};

inline ResourceOperationType::Value get(int opTypeInt) {
  ResourceOperationType::Value opType;
  switch (opTypeInt) {
  case ResourceOperationType::POST:
    opType = ResourceOperationType::POST;
    break;
  case ResourceOperationType::GET:
    opType = ResourceOperationType::GET;
    break;
  case ResourceOperationType::PUT:
    opType = ResourceOperationType::PUT;
    break;
  case ResourceOperationType::DELETE:
    opType = ResourceOperationType::DELETE;
    break;
  case ResourceOperationType::DISCOVER:
    opType = ResourceOperationType::DISCOVER;
    break;
  case ResourceOperationType::SUBSCRIBE:
    opType = ResourceOperationType::SUBSCRIBE;
    break;
  case ResourceOperationType::UNSUBSCRIBE:
    opType = ResourceOperationType::UNSUBSCRIBE;
    break;
  default:
    opType = ResourceOperationType::NotDetermined;
    break;
  }
  return opType;
}
}; // namespace ResourceOperationType

#define RESOURCE_STATUS_OK 200
#define RESOURCE_STATUS_BAD_REQUEST 400
#define RESOURCE_STATUS_UNAUTHORIZED 401
#define RESOURCE_STATUS_FORBIDDEN 403
#define RESOURCE_STATUS_NOT_FOUND 404
#define RESOURCE_STATUS_METHOD_NOT_ALLOWED 405

#endif // !defined(__RESOURCE_MESSAGE_COMMON_H__)