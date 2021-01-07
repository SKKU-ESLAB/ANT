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

var childProcess = require('child_process');
var net = require('net');

var AppConfig = function () {
  this.maxStdoutBufferLength = 100;
  this.maxStderrBufferLength = 100;
};
var gAppConfig = new AppConfig();

var AppState = Object.freeze({
  Inactive: 0,
  Launching: 1,
  Active: 2,
  Terminating: 3,
  0: 'Inactive',
  1: 'Launching',
  2: 'Active',
  3: 'Terminating'
});

function App(name, filePath) {
  this.mName = name;
  this.mFilePath = filePath;
  this.setState(AppState.Inactive);
  this.mProcess = undefined;
  this.mStdoutBuffer = [];
  this.mStderrBuffer = [];
  this.mSocket = undefined;
}

App.prototype.getName = function () {
  return this.mName;
};

App.prototype.getFilePath = function () {
  return this.mFilePath;
};

App.prototype.getState = function () {
  return AppState[this.mState];
};

App.prototype.setState = function (status) {
  if (typeof status === string) {
    this.mState = AppState[status];
  } else if (typeof status === number) {
    if (
      Number.isInteger(status) &&
      status >= 0 &&
      status < AppState.length / 2
    ) {
      this.mState = status;
    } else {
      this.mState = AppState.Inactive;
    }
  }
};

App.prototype.launch = function () {
  var self = this;

  // Check status
  if (this.mState !== AppState.Inactive) {
    throw 'Cannot launch app ' + this.mName + ': invalid state=' + this.mState;
  }
  if (this.mProcess !== undefined) {
    throw 'Cannot launch app ' + this.mName + ': already launched';
  }

  // Set attributes
  this.setState(AppState.Launching);
  this.mStdoutBuffer.splice(0, this.mStdoutBuffer.length);
  this.mStderrBuffer.splice(0, this.mStderrBuffer.length);

  // Make socket to communicate with child app process
  this.mSocket = net.createServer();
  this.mSocket.addListener('data', function (data) {
    // Handler on app-launched event
    var pid = parseInt(data);
    if (pid === NaN) {
      throw 'Invalid message from child process socket: ' + data;
    }
    if (self.mProcess.pid !== pid) {
      throw 'Pid not match: ' + data + ' !== ' + self.mProcess.pid;
    }
    self.setState(AppState.Active);
  });
  this.mSocket.listen();

  // Spawn the child app process
  var antRootDir = _getAntRootDir();
  var iotjsPath = path.join(antRootDir, 'iotjs');
  var appLauncherPath = path.join(
    antRootDir,
    'app-launcher',
    'app-launcher.js'
  );
  this.mProcess = childProcess.spawn(iotjsPath, [
    appLauncherPath,
    this.mName,
    this.mSocket.localPort
  ]);

  // Set stdout and stderr handlers
  this.mProcess.stdout.on('data', function (data) {
    // Handler on stdout event
    self.mStdoutBuffer.push(data);
    while (self.mStdoutBuffer.length > gAppConfig.maxStdoutBufferLength) {
      self.mStdoutBuffer.shift();
    }
  });
  this.mProcess.stderr.on('data', function (data) {
    // Handler on stderr event
    self.mStderrBuffer.push(data);
    while (self.mStderrBuffer.length > gAppConfig.maxStderrBufferLength) {
      self.mStderrBuffer.shift();
    }
  });
  this.mProcess.on('close', function (code) {
    // Handler on app-terminated event
    console.log('App ' + self.mName + ' exited with code ' + code);
    this.mProcess = undefined;
    self.setState(AppState.Inactive);
  });
};

App.prototype.terminateInForce = function () {
  // Check status
  if (this.mProcess === undefined) {
    console.error(
      'Cannot terminate app ' + this.mName + ': already terminated'
    );
    return false;
  }

  // Set attributes
  this.setState(AppState.Terminating);

  // Terminate the child app process in force
  this.mProcess.kill('SIGKILL');
};

App.prototype.terminate = function () {
  // Check status
  if (this.mState !== AppState.Active) {
    console.error(
      'Cannot terminate app ' + this.mName + ': invalid state=' + this.mState
    );
    return false;
  }
  if (this.mProcess === undefined) {
    console.error(
      'Cannot terminate app ' + this.mName + ': already terminated'
    );
    return false;
  }
  if (this.mSocket === undefined) {
    console.error(
      'Cannot terminate app ' + this.mName + ': socket does not exist'
    );
    return false;
  }

  // Set attributes
  this.setState(AppState.Terminating);

  // Send 'terminate' command to the child app process
  this.mSocket.write('Terminate');
};

module.exports = App;
