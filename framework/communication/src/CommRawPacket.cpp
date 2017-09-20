/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
 *              Eunsoo Park<esevan.park@gmail.com>
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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <arpa/inet.h>

#include "CommLog.h"

#include "CommRawPacket.h"

CommRawPacket* CommRawPacket::makeMessageMetadataPacket(
    char headerId, int totalDataSize, bool isFileAttached) {
  CommPayloadMessageMetadata* payload = new CommPayloadMessageMetadata(
      totalDataSize, isFileAttached);
  CommRawPacketHeader* header = new CommRawPacketHeader(
      headerId, payload->getBytesSize(), 0,
      false, false, false, true);
  return new CommRawPacket(header, payload);
}

CommRawPacket* CommRawPacket::makeFileMetadataPacket(
    char headerId, const char* fileName, int fileSize) {
  CommPayloadFileMetadata* payload = new CommPayloadFileMetadata(
      (int) fileSize, (int) (strlen(fileName) + 1), fileName);
  CommRawPacketHeader* header = new CommRawPacketHeader(
      headerId, payload->getBytesSize(), 0,
      false, false, false, true);
  return new CommRawPacket(header, payload);
}

CommRawPacket* CommRawPacket::makeDataPacket(
    char headerId, char* data, int currOffset, short size,
    bool isEnd, bool isFile) {
  CommPayloadData* payload = new CommPayloadData(
      data, size);
  CommRawPacketHeader* header = new CommRawPacketHeader(
      headerId, payload->getBytesSize(), 0,
      isFile, !isFile, isEnd, false);
  return new CommRawPacket(header, payload);
}

char* CommRawPacket::toByteArray() {
  int resLength = this->getBytesSize();
  char* resBytes = new char[resLength];
  char* headerBytes = NULL;
  char* payloadBytes = NULL;

  // Payload header
  headerBytes = this->mHeader->toByteArray();
  if(headerBytes == NULL) return NULL;
  memcpy(resBytes + 0,
      headerBytes + 0,
      this->mHeader->getBytesSize());

  // Payload
  payloadBytes = this->mPayload->toByteArray();
  if(payloadBytes == NULL) return NULL;
  memcpy(resBytes + this->mHeader->getBytesSize(),
      payloadBytes + 0,
      this->mPayload->getBytesSize());

  return resBytes;
}

#define READ_SOCKET_C(socketFd, data) \
  do { \
    int readSize = read(socketFd, &data, sizeof(char)); \
    if(readSize < 0) { \
      CommLog("read socket error: %s", strerror(errno)); \
      return NULL; \
    } \
  } while(0);
#define READ_SOCKET_S(socketFd, data) \
  do { \
    int readSize = read(socketFd, &data, sizeof(short)); \
    if(readSize < 0) { \
      CommLog("read socket error: %s", strerror(errno)); \
      return NULL; \
    } \
    data = ntohs(data); \
  } while(0);
#define READ_SOCKET_I(socketFd, data) \
  do { \
    int readSize = read(socketFd, &data, sizeof(int)); \
    if(readSize < 0) { \
      CommLog("read socket error: %s", strerror(errno)); \
      return NULL; \
    } \
    data = ntohl(data); \
  } while(0);
#define READ_SOCKET(socketFd, data, size) \
  do { \
    int readSize = 0; \
    char* dataPtr = data; \
    while(readSize < size) { \
      int thisSize = size - readSize; \
      int thisReadSize = read(socketFd, dataPtr, thisSize); \
      if(thisReadSize < 0) { \
        CommLog("read socket error: %s", strerror(errno)); \
        return NULL; \
      } \
      readSize += thisReadSize; \
      dataPtr += thisReadSize; \
    } \
  } while(0);

CommRawPacketHeader* CommRawPacketHeader::readFromSocket(int socketFd) {
  int readRes;
  char headerId;
  char headerFlag;
  short payloadSize;
  int currOffset;
  READ_SOCKET_C(socketFd, headerId);
  READ_SOCKET_C(socketFd, headerFlag);
  READ_SOCKET_S(socketFd, payloadSize);
  READ_SOCKET_I(socketFd, currOffset);
  CommLog("read header: id=%d / flag=%d / payloadSize=%d / currOffset=%d",
      (int)headerId, (int)headerFlag, (int)payloadSize, currOffset);
  CommRawPacketHeader* header = new CommRawPacketHeader(
      headerId, payloadSize, currOffset, headerFlag);
  return header;
}

#define NEW_BYTEARRAY(byteArray, size) \
  char* byteArray = new char[size]; \
  char* writePtr = byteArray;
#define WRITE_BYTEARRAY_C(x) do { \
  char temp = x; \
  memcpy(writePtr, &temp, sizeof(char)); \
  writePtr += sizeof(char); \
} while(0);
#define WRITE_BYTEARRAY_S(x) do { \
  short temp = htons(x); \
  memcpy(writePtr, &temp, sizeof(short)); \
  writePtr += sizeof(short); \
} while(0);
#define WRITE_BYTEARRAY_I(x) do { \
  int temp = htonl(x); \
  memcpy(writePtr, &temp, sizeof(int)); \
  writePtr += sizeof(int); \
} while(0);
#define WRITE_BYTEARRAY_SIZE(x, s) do { \
  memcpy(writePtr, x, s); \
  writePtr += (s); \
} while(0);

char* CommRawPacketHeader::toByteArray() {
  NEW_BYTEARRAY(byteArray, this->getBytesSize());
  WRITE_BYTEARRAY_C(this->mHeaderId);
  WRITE_BYTEARRAY_C(this->mHeaderFlag);
  WRITE_BYTEARRAY_S(this->mPayloadSize);
  WRITE_BYTEARRAY_I(this->mCurrOffset);
  CommLog("write header: id=%d / flag=%d / payloadSize=%d / currOffset=%d",
      (int)this->mHeaderId, (int)this->mHeaderFlag, (int)this->mPayloadSize,
      this->mCurrOffset);
  return byteArray;
}

CommPayloadMessageMetadata* CommPayloadMessageMetadata::readFromSocket(
    int socketFd) {
  int messageDataLength;
  int isFileAttached;
  READ_SOCKET_I(socketFd, messageDataLength);
  READ_SOCKET_I(socketFd, isFileAttached);
  CommPayloadMessageMetadata* messageMetadata = new CommPayloadMessageMetadata(
      messageDataLength, (isFileAttached != 0));
  return messageMetadata;
}

char* CommPayloadMessageMetadata::toByteArray() {
  if(this->mMessageDataLength == 0) return NULL;

  NEW_BYTEARRAY(byteArray, this->getBytesSize());
  WRITE_BYTEARRAY_I(this->mMessageDataLength);
  WRITE_BYTEARRAY_I(this->mIsFileAttached);
  return byteArray;
}

CommPayloadFileMetadata* CommPayloadFileMetadata::readFromSocket(int socketFd) {
  int fileSize;
  int fileNameLength;
  char* fileName;
  READ_SOCKET_I(socketFd, fileSize);
  READ_SOCKET_I(socketFd, fileNameLength);
  fileName = new char[fileNameLength];
  READ_SOCKET(socketFd, fileName, fileNameLength);
  CommPayloadFileMetadata* fileMetadata = new CommPayloadFileMetadata(
      fileSize, fileNameLength, fileName);
  return fileMetadata;
}

char* CommPayloadFileMetadata::toByteArray() {
  NEW_BYTEARRAY(byteArray, this->getBytesSize());
  WRITE_BYTEARRAY_I(this->mFileSize);
  WRITE_BYTEARRAY_I(this->mFileNameLength);
  WRITE_BYTEARRAY_SIZE(this->mFileName, this->mFileNameLength);
  return byteArray;
}

CommPayloadData* CommPayloadData::readFromSocket(int socketFd, short dataSize) {
  char* data = new char[dataSize];
  READ_SOCKET(socketFd, data, dataSize);
  CommPayloadData* dataPayload = new CommPayloadData(data, dataSize);
  return dataPayload;
}

char* CommPayloadData::toByteArray() {
  return this->mData;
}
