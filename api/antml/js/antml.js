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

var console = require('console');
var fs = require('fs');

var RuntimeAPI = undefined; 
var StreamAPI = undefined;
try {
  StreamAPI = require('antstream');
} catch (e) {
  throw new Error('ML API Dependency Error: not found Stream API');
}

try {
  RuntimeAPI = require('antruntime');
} catch (e) {
  throw new Error('ML API Dependency Error: not found Stream API');
}

var shapeArrayToStr = function (shapeArray) {
  var shapeStr = '';
  for (var i = 0; i < shapeArray.length; i++) {
    if (i == 0) {
      shapeStr = shapeStr + shapeArray[i];
    } else {
      shapeStr = shapeStr + ':' + shapeArray[i];
    }
  }
  return shapeStr;
};

var getAntRootDir = function () {
  var antRootDir = RuntimeAPI.getEnv('ANT_ROOT');
  if(antRootDir.length == 0) {
    return undefined;
  }
  return antRootDir;
};

var getTaskPath = function (taskName) {
  var antRootDir = getAntRootDir();
  if(antRootDir === undefined) {
    console.error("ERROR: ANT_ROOT is not defined!");
    return undefined;
  }

  var mlRootDir = antRootDir + '/ml/';
  var taskPath = mlRootDir + 'mlelement_' + taskName + '.py';
  if(!fs.existsSync(taskPath)) {
    console.error("ERROR: cannot find file! " + taskPath);
    return undefined;
  }
  return taskPath;
};

var availableTaskNames = [
  "imgcls_imagenet",
  "objdet_coco"
];

/**
 * ANT ML API
 */
function ANTML() {}

ANTML.prototype.getAvailableTaskNames = function () {
  return availableTaskNames;
};

ANTML.prototype.createMLElement = function (
  modelName,
  inputShape,
  inputType,
  outputShape,
  outputType,
  taskName 
) {
  // Checking arguments
  if (modelName.indexOf(' ') >= 0) {
    console.error('ERROR: Invalid modelName! ' + modelName);
    return undefined;
  }
  if (inputShape.indexOf(' ') >= 0) {
    console.error('ERROR: Invalid inputShape! ' + inputShape);
    return undefined;
  }
  if (inputType.indexOf(' ') >= 0) {
    console.error('ERROR: Invalid inputType! ' + inputType);
    return undefined;
  }
  if (outputShape.indexOf(' ') >= 0) {
    console.error('ERROR: Invalid outputShape! ' + outputShape);
    return undefined;
  }
  if (outputType.indexOf(' ') >= 0) {
    console.error('ERROR: Invalid outputType! ' + outputType);
    return undefined;
  }
  if (!StreamAPI.isInitialized()) {
    console.error('ERROR: Stream API is not initialized');
    return undefined;
  }
  if(taskName === undefined) {
    // Default task: imgcls_imagenet
    taskName = "imgcls_imagenet";
  }
  var taskPath = getTaskPath(taskName);
  if(taskPath === undefined) {
    console.error("ERROR: task does not exist: " + taskPath);
    return undefined;
  }

  var inputShapeStr = shapeArrayToStr(inputShape);
  var outputShapeStr = shapeArrayToStr(outputShape);

  var tensorFilter = StreamAPI.createElement('tensor_filter');
  tensorFilter.setProperty('framework', 'python3');
  tensorFilter.setProperty('model', taskPath);
  tensorFilter.setProperty('input', inputShapeStr);
  tensorFilter.setProperty('inputtype', inputType);
  tensorFilter.setProperty('output', outputShapeStr);
  tensorFilter.setProperty('outputtype', outputType);
  var custom =
    modelName +
    ' ' +
    inputShapeStr +
    ' ' +
    inputType +
    ' ' +
    outputShapeStr +
    ' ' +
    outputType;
  tensorFilter.setProperty('custom', custom);
  return tensorFilter;
};

ANTML.prototype.getMaxOfBuffer = function (buffer, type) {
  return native.ant_ml_getMaxOfBuffer(buffer, type);
};

ANTML.prototype.downloadModel = function (modelUrl, overwriteIfExists) {
  if(overwriteIfExists === undefined) {
    overwriteIfExists = false;
  }

  var antRootDir = getAntRootDir();
  if(antRootDir === undefined) {
    console.error("ERROR: ANT_ROOT is not defined!");
    return undefined;
  }
  var fileName = modelUrl.substring(modelUrl.lastIndexOf('/')+1, modelUrl.length);
  var modelName = fileName.substring(0, fileName.lastIndexOf('.'));
  var modelRootDir = antRootDir + '/ml/';
  var modelArchivePath = modelRootDir + fileName;
  var modelDirectoryPath = modelRootDir + modelName;

  console.log('Download model from ' + modelUrl
    + '\n => ' + modelArchivePath
    + '\n => ' + modelDirectoryPath);

  // Make model root directory if it does not exist
  if(!fs.existsSync(modelRootDir)) {
    fs.mkdirSync(modelRootDir);
  }
  
  // Check if the model archive file
  var isDownload = true;
  if(fs.existsSync(modelArchivePath)) {
    console.log('Model archive file already exists. (' + modelArchivePath + ')');
    if(!overwriteIfExists) {
      console.log('Skip downloading the model archive file...');
      isDownload = false;
    }
  }

  // Download model archive file
  if(isDownload) {
    console.log('Model download from ' + modelUrl + ' to ' + modelArchivePath);
    var res_download = RuntimeAPI.downloadFileViaHTTP(modelUrl, modelArchivePath);
    if(!res_download) {
      console.error('ERROR: downloading model failed');
      return undefined;
    }
  }

  // Check if the model directory
  var isArchive = true;
  if(fs.existsSync(modelDirectoryPath)) {
    console.log('Model directory already exists. (' + modelDirectoryPath + ')');
    if(!overwriteIfExists) {
      console.log('Skip unarchiving the model archive file...');
      isArchive = false;
    }
  } else {
    fs.mkdirSync(modelDirectoryPath);
  }

  // Unarchive model archive file
  if(isArchive) {
    var res_archive = RuntimeAPI.unarchive(modelArchivePath, modelDirectoryPath);
    if(!res_archive) {
      console.error('ERROR: archiving the model archive file failed');
      return undefined;
    }
  }
  return modelDirectoryPath;
};

module.exports = new ANTML();
module.exports.ANTML = ANTML;
