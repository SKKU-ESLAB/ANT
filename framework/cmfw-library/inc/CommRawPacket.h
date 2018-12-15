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

#ifndef __COMM_RAW_PACKET_H__
#define __COMM_RAW_PACKET_H__

#include <string.h>

class CommRawPacket;
class CommRawPacketHeader;
class CommRawPacketPayload;

class CommRawPacketHeader {
  public:
    CommRawPacketHeader(char headerId, short payloadSize, int currOffset, char headerFlag)
      : mHeaderId(headerId), mPayloadSize(payloadSize), mCurrOffset(currOffset),
      mHeaderFlag(headerFlag) { }
    CommRawPacketHeader(char headerId, short payloadSize, int currOffset,
        bool isFile, bool isData, bool isEndData, bool isMetadata)
      : mHeaderId(headerId), mPayloadSize(payloadSize), mCurrOffset(currOffset) {
      char headerFlag = 0x00;
      if(isFile) headerFlag = (char) (headerFlag | 0x80);
      if(isData) headerFlag = (char) (headerFlag | 0x40);
      if(isEndData) headerFlag = (char) (headerFlag | 0x20);
      if(isMetadata) headerFlag = (char) (headerFlag | 0x10);
      this->mHeaderFlag = headerFlag;
    }

    static CommRawPacketHeader* readFromSocket(int socketFd);

    char* toByteArray();
    short getBytesSize() { return this->kPacketHeaderSize; }

    char getHeaderId() { return this->mHeaderId; }
    char getHeaderFlag() { return this->mHeaderFlag; }
    bool getFlagIsFile() { return ((this->mHeaderFlag & 0x80) != 0); }
    bool getFlagIsData() { return ((this->mHeaderFlag & 0x40) != 0); }
    bool getFlagIsEndData() { return ((this->mHeaderFlag & 0x20) != 0); }
    bool getFlagIsMetadata() { return ((this->mHeaderFlag & 0x10) != 0); }
    short getPayloadSize() { return this->mPayloadSize; }
    int getCurrOffset() { return this->mCurrOffset; }

    static const short kMaxPacketSize = 1008;
    static const short kPacketHeaderSize = 8;
    static const short kMaxPacketPayloadSize = kMaxPacketSize - kPacketHeaderSize;

  protected:
    char mHeaderId;
    char mHeaderFlag;
    short mPayloadSize;
    int mCurrOffset;
};

class CommRawPacketPayload {
  public:
    virtual char* toByteArray() = 0;
    virtual short getBytesSize() = 0;
  protected:
};

class CommPayloadMessageMetadata : public CommRawPacketPayload {
  public:
    CommPayloadMessageMetadata(int messageDataLength, bool isFileAttached)
      : mMessageDataLength(messageDataLength), mIsFileAttached(isFileAttached) { }

    static CommPayloadMessageMetadata* readFromSocket(int socketFd);

    virtual char* toByteArray();
    virtual short getBytesSize() { return (short) this->kMessageHeaderSize; }

    int getMessageDataLength() { return this->mMessageDataLength; }
    int getIsFileAttached() { return this->mIsFileAttached; }
  protected:
    const short kMessageHeaderSize = 8;
    int mMessageDataLength;
    int mIsFileAttached;
};

class CommPayloadFileMetadata : public CommRawPacketPayload {
  public:
    CommPayloadFileMetadata(int fileSize, int fileNameLength, const char* fileName)
      : mFileSize(fileSize), mFileNameLength(fileNameLength) {
      this->mFileName = new char[fileNameLength];
      strncpy(this->mFileName, fileName, fileNameLength);
    }
    ~CommPayloadFileMetadata() {
      if(this->mFileName != NULL) delete[] this->mFileName;
    }

    static CommPayloadFileMetadata* readFromSocket(int socketFd);

    virtual char* toByteArray();
    virtual short getBytesSize() { return (short) (4 + 4 + this->mFileNameLength); }

    int getFileSize() { return this->mFileSize; }
    char getSrcFileNameLength() { return this->mFileNameLength; }
    char* getSrcFileName() { return this->mFileName; }

  protected:
    int mFileSize;
    int mFileNameLength;
    char* mFileName;
};

class CommPayloadData : public CommRawPacketPayload {
  public:
    CommPayloadData(char* data, short size)
      : mData(data), mSize(size) { }
    ~CommPayloadData() {
      if(this->mData != NULL) delete this->mData;
    }

    static CommPayloadData* readFromSocket(int socketFd, short dataSize);

    virtual char* toByteArray();
    virtual short getBytesSize() { return this->mSize; }

  protected:
    short mSize;
    char* mData;
};

class CommRawPacket {
  private:
    CommRawPacket(CommRawPacketHeader* header, CommRawPacketPayload* payload) {
      this->mHeader = header;
      this->mPayload = payload;
    }
  public:
    ~CommRawPacket() {
      if(this->mHeader != NULL) delete this->mHeader;
      if(this->mPayload != NULL) delete this->mPayload;
    }

  public:
    static CommRawPacket* makeMessageMetadataPacket(
        char headerId, int totalDataSize, bool isFileAttached);
    static CommRawPacket* makeFileMetadataPacket(
        char headerId, const char* fileName, int fileSize);
    static CommRawPacket* makeDataPacket(
        char headerId, char* data, int currOffset, short size, bool isEnd, bool isFile);

    char* toByteArray();
    int getBytesSize() {
      return this->mHeader->getBytesSize() + this->mPayload->getBytesSize();
    }

  protected:
    CommRawPacketHeader* mHeader;
    CommRawPacketPayload* mPayload;
};

#endif // !defined(__COMM_RAW_PACKET_H__)
