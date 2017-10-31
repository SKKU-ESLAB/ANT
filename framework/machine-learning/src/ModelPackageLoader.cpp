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

#include "ModelPackageLoader.h"
#include "DNNInferenceRunner.h"
#include "ANNInferenceRunner.h"
#include "MLDataUnit.h"
#include "MLTensor.h"
#include "ANTdbugLog.h"

InferenceUnit* ModelPackageLoader::loadBuiltin(std::string modelName) {
  // Load built-in model and create a new inference unit

  // Properties of new inference unit
  InferenceRunner* inferenceRunner = NULL;
  MLDataUnitLayout* inputLayout = new MLDataUnitLayout();
  MLDataUnitLayout* outputLayout = new MLDataUnitLayout();
  MLDataUnit* parameters = new MLDataUnit();

  // Set properties of new inference unit
  if(modelName.compare("motionclassifier") == 0) {
    // Set inference runner
    inferenceRunner = new ANNInferenceRunner();

    // TODO: determine input & output layout of motion classifier
    // Set input layout : float[3] input
    int inputShape[] = {3};
    MLTensorLayout inputTensorLayout(1, inputShape, MLDataType::Float);
    inputLayout->insertTensorLayout("input", inputTensorLayout);
        
    // Set output layout: char[30] output
    int outputShape[] = {30};
    MLTensorLayout outputTensorLayout(1, outputShape, MLDataType::Char);
    outputLayout->insertTensorLayout("output", outputTensorLayout);

    // Not set parameters
  } else if(modelName.compare("imageclassifier") == 0) {
    // Set inference runner
    inferenceRunner = new DNNInferenceRunner();

    // TODO: determine input & output layout of image classifier
    // Set input layout : float[224][224] input
    int inputShape[] = {224, 224};
    MLTensorLayout inputTensorLayout(2, inputShape, MLDataType::Float);
    inputLayout->insertTensorLayout("input", inputTensorLayout);
        
    // Set output layout: char[30] output
    int outputShape[] = {30};
    MLTensorLayout outputTensorLayout(1, outputShape, MLDataType::Char);
    outputLayout->insertTensorLayout("output", outputTensorLayout);

    // Not set parameters
  } else {
    ANT_DBG_ERR("Cannot find builtin model for %s", modelName.c_str());
    return NULL;
  }
  InferenceUnit* newIU = new InferenceUnit(
      modelName, // name
      "", // modelPackagePath
      inferenceRunner, // inferenceRunner
      inputLayout, // inputLayout
      outputLayout, // outputLayout
      parameters);
  return newIU;
}

InferenceUnit* ModelPackageLoader::load(
    std::string modelPackagePath, MLDataUnit& params) {
  // TODO: implement it
  return NULL;
}
