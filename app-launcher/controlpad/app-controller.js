/* Copyright (c) 2017-2021 SKKU ESLAB, and contributors. All rights reserved.
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

function AppController(context, antClient) {
  this.mContext = context;
  this.mANTClient = antClient;
}

AppController.prototype.createApp = function (appName, onSuccess, onFailure) {
  // Send "create app request"
  try {
    var initialCode = Config.initialCode;
    initialCode = initialCode.replace('{APP_NAME}', appName);
    this.mANTClient.installApp(
      appName,
      initialCode,
      function (isSuccess, text) {
        if (isSuccess) {
          if (onSuccess !== undefined) onSuccess(appName);
        } else {
          if (onFailure !== undefined) onFailure(text);
        }
      }
    );
  } catch (e) {
    onFailure(e);
  }
};

AppController.prototype.updateCurrentAppCode = function (
  appCode,
  onSuccess,
  onFailure
) {
  // Send "create app request"
  try {
    var appName = this.mContext.currentAppName;
    this.mANTClient.installApp(appName, appCode, function (isSuccess, text) {
      if (isSuccess) {
        if (onSuccess !== undefined) onSuccess(appName);
      } else {
        if (onFailure !== undefined) onFailure(text);
      }
    });
  } catch (e) {
    onFailure(e);
  }
};

AppController.prototype.launchCurrentApp = function (onSuccess, onFailure) {
  // Send "launch app request"
  try {
    var appName = this.mContext.currentAppName;
    this.mANTClient.launchApp(appName, function (isSuccess, text) {
      if (isSuccess) {
        if (onSuccess !== undefined) onSuccess(appName);
      } else {
        if (onFailure !== undefined) onFailure(text);
      }
    });
  } catch (e) {
    gUIController.showErrorMessage(e);
  }
};

AppController.prototype.terminateCurrentApp = function (onSuccess, onFailure) {
  // Send "terminate app request"
  try {
    var appName = this.mContext.currentAppName;
    this.mANTClient.terminateApp(appName, function (isSuccess, text) {
      if (isSuccess) {
        if (onSuccess !== undefined) onSuccess(appName);
      } else {
        if (onFailure !== undefined) onFailure(text);
      }
    });
  } catch (e) {
    gUIController.showErrorMessage(e);
  }
};

AppController.prototype.removeCurrentApp = function (onSuccess, onFailure) {
  // Send "remove app request"
  try {
    var appName = this.mContext.currentAppName;
    this.mANTClient.removeApp(appName, function (isSuccess, text) {
      if (isSuccess) {
        if (onSuccess !== undefined) onSuccess(appName);
      } else {
        if (onFailure !== undefined) onFailure(text);
      }
    });
  } catch (e) {
    gUIController.showErrorMessage(e);
  }
};
