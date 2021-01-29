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

/* CreateAppDialogView */
function CreateAppDialogView() {
  View.apply(this, ['create-app-dialog']);

  var self = this;
  this.mConfirmButton = document.getElementById(
    'create-app-dialog-confirm-button'
  );
  this.mConfirmButton.onclick = function () {
    self.confirm();
  };
  this.mDismissButton = document.getElementById(
    'create-app-dialog-dismiss-button'
  );
  this.mDismissButton.onclick = function () {
    self.dismiss();
  };
  var dialogForm = document.getElementById('create-app-dialog-form');
  dialogForm.onsubmit = function (event) {
    event.preventDefault();
    self.confirm();
  };
  /* TODO: for older browers, it may require 'dialog-polyfill'.
     https://github.com/GoogleChrome/dialog-polyfill
   */
}
CreateAppDialogView.prototype = Object.create(View.prototype);
CreateAppDialogView.prototype.constructor = CreateAppDialogView;

CreateAppDialogView.prototype.show = function () {
  var appNameTextField = document.getElementById(
    'create-app-dialog-appname-textfield'
  );
  appNameTextField.value = '';

  this.mRootDom.showModal();
};

CreateAppDialogView.prototype.confirm = function () {
  var appNameTextField = document.getElementById(
    'create-app-dialog-appname-textfield'
  );
  var appName = appNameTextField.value;
  if (
    appName === undefined ||
    typeof appName !== 'string' ||
    appName.length <= 0 ||
    appName.indexOf(' ') >= 0 ||
    appName.indexOf('\n') >= 0 ||
    appName.indexOf('.') >= 0
  ) {
    alert(
      'Invalid app name: ' +
        appName +
        "\nApp name cannot include newline('\\n'), space(' '), and period ('.') characters."
    );
    return;
  }

  // Create app
  var self = this;
  var onSuccess = function (appName) {
    // Update UI for the new app
    gUIController.addAppToUI(appName);
    gUIController.selectApp(appName);

    self.mRootDom.close();

    gUIController.showSuccessMessage(appName + ': app created and installed.');
    gUIController.selectNavItem('navitem-codeeditor');
  };
  var onFailure = function (text) {
    gUIController.showErrorMessage(text);
  };

  gAppController.createApp(appName, onSuccess, onFailure);
};

CreateAppDialogView.prototype.dismiss = function () {
  this.mRootDom.close();
};
