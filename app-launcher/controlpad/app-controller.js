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

AppController.prototype.createApp = function (appName) {
  // Send "create app request"
  try {
    this.mANTClient.installApp(
      appName,
      initialCode,
      function (isSuccess, text) {
        if (isSuccess) {
          gUIController.addAppToUI(appName);

          // Select this app
          gUIController.selectApp(appName);
        } else {
          alert(text);
        }
      }
    );
  } catch (e) {
    gUIController.showErrorMessage(e);
  }
};

AppController.prototype.updateCurrentAppCode = function (appCode) {
  // Send "create app request"
  try {
    var appName = this.mContext.currentAppName;
    this.mANTClient.installApp(appName, appCode, function (isSuccess, text) {
      if (isSuccess) {
        gUIController.showInfoMessage('App code loaded successfully');
      } else {
        gUIController.showErrorMessage(text);
      }
    });
  } catch (e) {
    gUIController.showErrorMessage(e);
  }
};

AppController.prototype.launchCurrentApp = function () {
  // Send "launch app request"
  try {
    var appName = this.mContext.currentAppName;
    this.mANTClient.launchApp(appName, function (isSuccess, text) {
      if (isSuccess) {
        gUIController.showInfoMessage(
          'App ' + appName + ' launched successfully'
        );

        if (this.mContext.currentNavItem.getId() === 'navitem-codeeditor') {
          gCodeEditor.setRunButtonMode(false);
        }
      } else {
        gUIController.showErrorMessage(text);
      }
    });
  } catch (e) {
    gUIController.showErrorMessage(e);
  }
};

AppController.prototype.terminateCurrentApp = function () {
  // Send "terminate app request"
  try {
    var appName = this.mContext.currentAppName;
    this.mANTClient.terminateApp(appName, function (isSuccess, text) {
      if (isSuccess) {
        gUIController.showInfoMessage(
          'App ' + appName + ' terminated successfully'
        );

        if (this.mContext.currentNavItem.getId() === 'navitem-codeeditor') {
          gCodeEditor.setRunButtonMode(true);
        }
      } else {
        gUIController.showErrorMessage(text);
      }
    });
  } catch (e) {
    gUIController.showErrorMessage(e);
  }
};

AppController.prototype.removeCurrentApp = function () {
  // Send "remove app request"
  try {
    var appName = this.mContext.currentAppName;
    this.mANTClient.removeApp(appName, function (isSuccess, text) {
      if (isSuccess) {
        gUIController.removeAppFromUI(appName);

        gUIController.showInfoMessage(
          'App ' + appName + ' removed successfully'
        );
      } else {
        gUIController.showErrorMessage(text);
      }
    });
  } catch (e) {
    gUIController.showErrorMessage(e);
  }
};
