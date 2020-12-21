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

var StreamAPI = undefined;
try {
  StreamAPI = require('antstream');
} catch (e) {
  throw new Error('Gateway API Dependency Error: not found Stream API');
}

/**
 * ANT Camera API
 */
function ANTCamera() {}

ANTCamera.prototype.createCameraElement = function (deviceType) {
  var cameraType = deviceTypeToCameraType[deviceType];
  if (cameraType == undefined) {
    return undefined;
  }
  if (cameraType == 'rpi') {
    source = this.createRPiCameraElement();
  } else if (cameraType == 'test') {
    source = this.createTestCameraElement();
  } else if (cameraType == 'v4l2') {
    source = this.createV4L2CameraElement();
  } else if (cameraType == 'nvidia') {
    source = this.createNvidiaCameraElement();
  }
  return source;
};

ANTCamera.prototype.createRPiCameraElement = function () {
  source = StreamAPI.createElement('rpicamsrc');
  return source;
};

ANTCamera.prototype.createTestCameraElement = function () {
  source = StreamAPI.createElement('videotestsrc');
  source.setProperty('pattern', 1);
  return source;
};

ANTCamera.prototype.createV4L2CameraElement = function (sourcePath) {
  source = StreamAPI.createElement('v4l2src');
  if (sourcePath === undefined) {
    sourcePath = '/dev/video0';
  }
  source.setProperty('device', sourcePath);
  return source;
};

ANTCamera.prototype.createNvidiaCameraElement = function () {
  source = StreamAPI.createElement('nvcamsrc');
  source.setProperty('fpsRange', '30.0 30.0');
  return source;
};

var deviceTypeToCameraType = {
  rpi2: 'rpi',
  rpi3: 'rpi',
  xu3: 'v4l2',
  xu4: 'v4l2',
  tx1: 'nvidia',
  tx2: 'nvidia'
};

module.exports = new ANTCamera();
module.exports.ANTCamera = ANTCamera;
