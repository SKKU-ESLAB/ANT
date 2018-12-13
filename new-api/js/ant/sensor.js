/* Copyright (c) 2017-2018 SKKU ESLAB, and contributors. All rights reserved.
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

var path = require('path');

// RemoteUI Object
exports.getSensorList = function() {
  var ant_api_dir = process.env.ANT_BIN_DIR + '/api/';
  var appAPI = require(ant_api_dir + 'ant').app();
  var resourceAPI = require(ant_api_dir + 'ant').resource();

  var senderUri = '/thing/apps/' + appAPI.getAppId();
  var targetUri = '/thing/sensors';
  var body = {};
  var bodyJson = JSON.stringify(body);
  resourceAPI.get(senderUri, targetUri, bodyJson, function(responseText) {
    callback(JSON.parse(responseText));
  });
};

exports.get = function(sensor_name, callback) {
  var ant_api_dir = process.env.ANT_BIN_DIR + '/api/';
  var appAPI = require(ant_api_dir + 'ant').app();
  var resourceAPI = require(ant_api_dir + 'ant').resource();

  var senderUri = '/thing/apps/' + appAPI.getAppId();
  var targetUri = '/thing/sensors/' + sensor_name;
  var body = {};
  var bodyJson = JSON.stringify(body);
  resourceAPI.get(senderUri, targetUri, bodyJson, function(responseText) {
    callback(JSON.parse(responseText));
  });
};

exports.subscribe = function() {
  // TODO: to be implemented
};