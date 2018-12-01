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

// Hardcoded for old RemoteNotiUI Android-side object
function NotificationPage(title) {
  this.title = (title == null) ? '' : title;
  this.descriptor = {appTitle: title, description: title};
};

NotificationPage.prototype.addText = function(text) {
  this.descriptor.text = text;
};

NotificationPage.prototype.addImg = function(imgPath) {
  this.descriptor.img = imgPath;
};

NotificationPage.prototype.sendToCompanion = function() {
  var ant_api_dir = process.env.ANT_BIN_DIR + '/api/';
  var commAPI = require(ant_api_dir + 'ant').comm();
  if (this.descriptor.img != undefined) {
    var imgFullPath = this.descriptor.img;
    this.descriptor.img = path.basename(imgFullPath);
    commAPI.sendToCompanion(
        'RemoteNotiUI', JSON.stringify(this.descriptor), imgFullPath);
  } else {
    commAPI.sendToCompanion('RemoteNotiUI', JSON.stringify(this.descriptor));
  }
};

// TODO: ConfigPage is not yet ported...
function ConfigPage(title) {
  this.title = (title == null) ? '' : title;
};

// RemoteUI Object
exports.makeNotiPage = function(title) {
  return new NotificationPage(title);
};

exports.makeConfigPage = function(title) {
  return new ConfigPage(title);
};