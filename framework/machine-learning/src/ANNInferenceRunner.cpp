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

#include <sys/stat.h>
#include <sys/types.h>

#include "ANNInferenceRunner.h"
#include "ANTdbugLog.h"
#include "fann.h"

// Utility function
#define PATH_BUFFER_SIZE 1024
std::string getDataDir() {
  char* dirString = getenv("ANT_DATA_DIR");
  char dataDir[PATH_BUFFER_SIZE];
  struct stat st = {0};
  std::string dataDirStr;

  if(dirString != NULL) {
    snprintf(dataDir, PATH_BUFFER_SIZE, "%s", dirString);
    if(stat(dataDir, &st) == -1) {
      mkdir(dataDir, 0755);
    }
  } else {
    ANT_DBG_ERR("Cannot read ANT_DATA_DIR");
    return dataDirStr;
  }
  dataDirStr.assign(dataDir);
  return dataDirStr;
}

MLDataUnit* ANNInferenceRunner::run(MLDataUnit* inputData) {
  // Now DNN output is given as dummy data.
 
  const char* act[7] = {"None", "Biking", "Sitting", "Standing", "Walking",
    "Stair Up", "Stair down"};
  fann_type *calc_out;
  fann_type inputstr[3];
  std::string dataDir = getDataDir();
  std::string modelPath = dataDir.append("/models/accelerometer.net");
  struct fann *ann = fann_create_from_file(modelPath.c_str());

  if(!ann)
  {
    printf("Error creating ann --- ABORTING\n");
    return NULL;
  }

  MLTensor* inputTensor = inputData->findTensor("input");
  if(inputTensor == NULL) return NULL;
  void* inputBuffer = inputTensor->bytesValue();
  float input0 = *(((float*)inputBuffer) + 0);
  float input1 = *(((float*)inputBuffer) + 1);
  float input2 = *(((float*)inputBuffer) + 2);
  int outputShape[] = {30};
  MLTensorLayout outputTensorLayout(1, outputShape, MLDataType::Char);

  MLTensor* outputTensor = new MLTensor(outputTensorLayout);
  char outputStr[30];
  int outputidx = 0;

  inputstr[0] = input0;
  inputstr[1] = input1;
  inputstr[2] = input2;
  calc_out = fann_run(ann, inputstr);

  for (int i = 0; i < 7; i++)
  {
    if (calc_out[outputidx]<calc_out[i]) outputidx = i;
  }

#ifdef TEST_ON
  printf("\ninputstr: %f, %f, %f\noutputidx: %d\ncalc_out:%f, %f, %f, %f, %f, %f, %f\n\n", 
      inputstr[0], inputstr[1], inputstr[2], outputidx, calc_out[0], calc_out[1], 
      calc_out[2], calc_out[3], calc_out[4], calc_out[5], calc_out[6]);
#endif

  snprintf(outputStr, 30, "%s", act[outputidx]);
  outputTensor->assignData(outputStr);

  MLDataUnit* outputData = new MLDataUnit();
  outputData->insertTensor("output", outputTensor);

  fann_destroy(ann);
  return outputData;
}

// Get resource usage of inference runner
std::string ANNInferenceRunner::getResourceUsage() {
  std::string data("");
  // not implemented yet
  
  return data;
}
