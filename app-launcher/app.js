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
var fs = require('fs');
var path = require('path');
var net = require('net');
var Util = require('./util.js');

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
  this.mStdoutBufferTS = 0;
  this.mStderrBufferTS = 0;
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

App.prototype.setState = function (state) {
  if (typeof state === 'string') {
    this.mState = AppState[state];
  } else if (typeof state === 'number') {
    if (Number.isInteger(state) && state >= 0) {
      this.mState = state;
    } else {
      this.mState = AppState.Inactive;
    }
  }
};

App.prototype.launch = function () {
  var self = this;

  // Check state
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
  var antRootDir = Util.getAntRootDir();
  var iotjsPath = path.join(antRootDir, 'iotjs');
  var appMainPath = path.join(antRootDir, 'app-core', 'main.js');
  this.mProcess = childProcess.spawn(iotjsPath, [
    appMainPath,
    this.mName,
    this.mFilePath,
    this.mSocket.address().port
  ]);

  // Set stdout and stderr handlers
  this.mProcess.stdout.on('data', function (data) {
    // Handler on stdout event
    var stdoutEntry = {ts: self.mStdoutBufferTS++, d: data.toString()};
    self.mStdoutBuffer.push(stdoutEntry);
    while (self.mStdoutBuffer.length > gAppConfig.maxStdoutBufferLength) {
      self.mStdoutBuffer.shift();
    }
  });
  this.mProcess.stderr.on('data', function (data) {
    // Handler on stderr event
    var stderrEntry = {ts: self.mStderrBufferTS++, d: data.toString()};
    self.mStderrBuffer.push(stderrEntry);
    while (self.mStderrBuffer.length > gAppConfig.maxStderrBufferLength) {
      self.mStderrBuffer.shift();
    }
  });
  this.mProcess.on('close', function (code) {
    // Handler on app-terminated event
    console.log('App ' + self.mName + ' exited with code ' + code);
    console.log('stdout:');
    for (var i in self.mStdoutBuffer) {
      console.log(self.mStdoutBuffer[i].d);
    }
    console.log('stderr:');
    for (var i in self.mStderrBuffer) {
      console.log(self.mStderrBuffer[i].d);
    }
    this.mProcess = undefined;
    self.setState(AppState.Inactive);
  });
};

App.prototype.terminateInForce = function () {
  // Check state
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
  // Check state
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

App.prototype.readCode = function () {
  var appCode = fs.readFileSync(this.mFilePath);
  return appCode;
};

App.prototype.getStdouts = function (fromTs) {
  var stdouts = [];
  for (var i in this.mStdoutBuffer) {
    var entry = this.mStdoutBuffer[i];
    if (entry.ts >= fromTs) stdouts.push(entry);
  }
  return stdouts;
};

App.prototype.getStderrs = function (fromTs) {
  var stderrs = [];
  for (var i in this.mStderrBuffer) {
    var entry = this.mStderrBuffer[i];
    if (entry.ts >= fromTs) stderrs.push(entry);
  }
  return stderrs;
};

module.exports = App;
