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
  this.maxOutputBufferLength = 1000;
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
  this.mOutputBuffer = [];
  this.mOutputBufferTS = 0;
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
  this.mOutputBuffer.splice(0, this.mOutputBuffer.length);

  /*
   * TODO: replacing local-TCP-socket with dbus-based-IPC is required.
   * Dbus-based-IPC can be implemented by node-dbus npm package.
   */
  // Make socket to communicate with child app process
  this.mSocket = net.createServer(function (client) {
    self.mClientSocket = client;
    client.setTimeout(500);
    client.on('data', function (data) {
      console.log('data from app: ' + data);
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
    var stdoutEntry = {ts: self.mOutputBufferTS++, t: 'O', d: data.toString()};
    self.mOutputBuffer.push(stdoutEntry);
    while (self.mOutputBuffer.length > gAppConfig.maxOutputBufferLength) {
      self.mOutputBuffer.shift();
    }
  });
  this.mProcess.stderr.on('data', function (data) {
    // Handler on stderr event
    var stderrEntry = {ts: self.mOutputBufferTS++, t: 'E', d: data.toString()};
    self.mOutputBuffer.push(stderrEntry);
    while (self.mOutputBuffer.length > gAppConfig.maxOutputBufferLength) {
      self.mOutputBuffer.shift();
    }
  });
  this.mProcess.on('close', function (code) {
    // Handler on app-terminated event
    self.mProcess = undefined;
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
  if (this.mClientSocket === undefined) {
    console.error(
      'Cannot terminate app ' + this.mName + ': client not connected'
    );
    return false;
  }

  // Set attributes
  this.setState(AppState.Terminating);

  // Send 'terminate' command to the child app process
  this.mClientSocket.write('Terminate');
};

App.prototype.readCode = function () {
  var appCode = fs.readFileSync(this.mFilePath);
  return appCode;
};

App.prototype.getOutputs = function (fromTs) {
  var outputs = [];
  for (var i in this.mOutputBuffer) {
    if (this.mOutputBuffer.hasOwnProperty(i)) {
      var entry = this.mOutputBuffer[i];
      if (fromTs < 0 || entry.ts >= fromTs) outputs.push(entry);
    }
  }
  return outputs;
};

module.exports = App;
