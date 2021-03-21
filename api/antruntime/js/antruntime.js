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

var RESULT_SUCCESS = 'Success';
var RESULT_FAILED = 'Failed';
var fs = require('fs');

/**
 * ANT Resource API
 */
function ANTRuntime() {}
ANTRuntime.prototype._mCurrentApp = undefined;

ANTRuntime.prototype._removeCurrentApp = function () {
  this._mCurrentApp = undefined;
};

ANTRuntime.prototype.setCurrentApp = function (onInitialize, onStart, onStop) {
  if (
    !(onInitialize instanceof Function) ||
    !(onStart instanceof Function) ||
    !(onStop instanceof Function)
  ) {
    return RESULT_FAILED;
  }
  this._mCurrentApp = new App(onInitialize, onStart, onStop);

  return RESULT_SUCCESS;
};

ANTRuntime.prototype.getCurrentApp = function () {
  return this._mCurrentApp;
};

ANTRuntime.prototype.getPssInKB = function () {
  return native.ant_runtime_getPssInKB();
};
ANTRuntime.prototype.getPSSInKB = ANTRuntime.prototype.getPssInKB;

ANTRuntime.prototype.getEnv = function (envKey) {
  return native.ant_runtime_getEnv(envKey);
};

ANTRuntime.prototype.downloadFileViaHTTP = function (url, downloadPath) {
  return native.ant_runtime_downloadFileViaHTTP(url, downloadPath);
};

ANTRuntime.prototype.unarchive = function (archiveFilePath, targetDirPath) {
  if (!fs.existsSync(archiveFilePath)) {
    console.log('Error: cannot find archive file! ' + archiveFilePath);
    return false;
  }
  if (!fs.existsSync(targetDirPath)) {
    console.log('Error: cannot find target directory! ' + archiveDirPath);
    return false;
  }
  return native.ant_runtime_unarchive(archiveFilePath, targetDirPath);
};

ANTRuntime.prototype.disableStdoutBuffering = function() {
  return native.ant_runtime_disableStdoutBuffering();
};

/**
 * Application
 * @param {function} onInitialize callback handler invoked on application's
 * initialization.
 * @param {function} onStart callback handler invoked when application starts.
 * @param {function} onStop callback handler invoked when application stops.
 */
function App(onInitialize, onStart, onStop) {
  this.state = this.STATE.IDLE;
  this.onInitialize = onInitialize;
  this.onStart = onStart;
  this.onStop = onStop;

  this.onInitialize();
}

App.prototype.STATE = {};
App.prototype.STATE.IDLE = 0;
App.prototype.STATE.RUNNING = 1;

App.prototype.start = function () {
  if (this.state == this.STATE.RUNNING) {
    return RESULT_FAILED;
  }
  this.state = this.STATE.RUNNING;
  this.onStart();
  return RESULT_SUCCESS;
};

App.prototype.stop = function () {
  if (this.state == this.STATE.IDLE) {
    return RESULT_FAILED;
  }
  this.state = this.STATE.IDLE;
  this.onStop();
  return RESULT_SUCCESS;
};

App.prototype.getState = function () {
  if (this.state == this.STATE.IDLE) {
    return 'Idle';
  } else if (this.state == this.STATE.RUNNING) {
    return 'Running';
  } else {
    return 'Unknown';
  }
};

App.prototype.getInfo = function () {
  var appInfo = {state: this.getState()};
  return appInfo;
};

/* App end */

module.exports = new ANTRuntime();
module.exports.ANTRuntime = ANTRuntime;
