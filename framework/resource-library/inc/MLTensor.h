/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
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

#ifndef __ML_TENSOR_H__
#define __ML_TENSOR_H__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <string.h>

#include "cJSON.h"

namespace MLDataType {
  enum Value {
    Char = 1,
    Int32 = 2,
    Float = 3,
    Double = 4
  };

  inline int size(MLDataType::Value dataType) {
    switch(dataType) {
      case MLDataType::Char:
        return 1;
        break;
      case MLDataType::Int32:
        return 4;
        break;
      case MLDataType::Float:
        return 4;
        break;
      case MLDataType::Double:
        return 8;
        break;
      default:
        return 0;
        break;
    }
  }
};

class MLTensorLayout {
  public:
    MLTensorLayout(int rank, int* shape, MLDataType::Value dataType)
      : mRank(rank), mDataType(dataType) {
      this->mShape = new int[rank];
      for(int i = 0; i < rank; i++) {
        this->mShape[i] = shape[i];
      }
    }

    MLTensorLayout(const MLTensorLayout &input) {
      this->mRank = input.mRank;
      this->mDataType = input.mDataType;

      int* shape = input.mShape;
      this->mShape = new int[this->mRank];
      for(int i = 0; i < this->mRank; i++) {
        this->mShape[i] = shape[i];
      }
    }

    ~MLTensorLayout() {
      delete[] this->mShape;
    }

    MLTensorLayout& operator=(MLTensorLayout input) {
      this->mRank = input.getRank();
      this->mDataType = input.getDataType();

      int* shape = input.getShape();
      this->mShape = new int[this->mRank];
      for(int i = 0; i < this->mRank; i++) {
        this->mShape[i] = shape[i];
      }
      return *this;
    }

    // encoding to JSON
    cJSON* toJSON();

    // decoding from JSON
    static MLTensorLayout* makeFromJSON(cJSON* tensorLayoutObj);

    // Handle tensor layout
    bool isMatched(MLTensorLayout* tensorLayout);

    int getNumElements() {
      int numElements = 1;
      for(int i = 0; i < this->mRank; i++) {
        numElements = numElements * this->mShape[i];
      }
      return numElements;
    }

    int getElementSize() {
      return MLDataType::size(this->mDataType);
    }

    int getBytesSize() {
      return this->getNumElements() * this->getElementSize();
    }

    int getRank() { return this->mRank; }
    int* getShape() { return this->mShape; }
    MLDataType::Value getDataType() { return this->mDataType; }
    
  protected:
    int mRank;
    int* mShape;
    MLDataType::Value mDataType;
};

class MLTensor {
  public:
    MLTensor(MLTensorLayout layout) : mLayout(layout) {
      this->mBufferSize = layout.getBytesSize();
      this->mBuffer = (void*)malloc(this->mBufferSize);
    }

    ~MLTensor() {
      free(this->mBuffer);
    }

    // encoding to JSON
    cJSON* toJSON();

    // decoding from JSON
    static MLTensor* makeFromJSON(cJSON* tensorObj);

    // Handle tensor
    bool isMatched(MLTensorLayout& targetLayout);
    bool isMatched(MLTensor* targetTensor);

    MLTensorLayout& getLayout() { return this->mLayout; }
    int getBufferSize() { return this->mBufferSize; }

    void assignData(void* data) {
      memcpy(this->mBuffer, data, this->mBufferSize);
    }
    void* bytesValue() { return this->mBuffer; }
    std::string stringValue() { return std::string((char*)this->mBuffer); }
    char charValue() { return *((char*)this->mBuffer); }
    bool boolValue() { return (this->charValue() == 1); }
    int int32Value() { return *((int*)this->mBuffer); }
    float floatValue() { return *((float*)this->mBuffer); }
    double doubleValue() { return *((double*)this->mBuffer); }

  protected:
    MLTensorLayout mLayout;
    int mBufferSize;
    void* mBuffer;
};

#endif // !defined(__ML_TENSOR_H__)
