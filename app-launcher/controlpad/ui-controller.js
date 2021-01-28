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

/* UI Controller */
function UIController(context, antClient) {
  /* Attributes */
  this.mContext = context;
  this.mANTClient = antClient;

  /* Content views */
  this.mDashboard = undefined;
  this.mCodeEditor = undefined;
  this.mConsole = undefined;

  /* Main UI components */
  this.mAppSelector = undefined;
  this.mAppSelectorLabel = undefined;
  this.mAppSelectorMenu = undefined;
  this.mNavMenuView = undefined;
  this.mCreateAppDialog = undefined;
  this.mCreateAppFabButton = undefined;
  this.mSnackbar = undefined;
}

/* Message display functions */
UIController.prototype.showInfoMessage = function (text, useSnackbar = true) {
  if (useSnackbar) this.mSnackbar.showShort(text, undefined, undefined, 'INFO');
  console.log(text);
};

UIController.prototype.showSuccessMessage = function (
  text,
  useSnackbar = true
) {
  if (useSnackbar)
    this.mSnackbar.showShort(text, undefined, undefined, 'SUCCESS');
  console.log(text);
};

UIController.prototype.showErrorMessage = function (text, useSnackbar = true) {
  var errorMsg = 'Error: ' + text;
  if (useSnackbar)
    this.mSnackbar.showLong(errorMsg, undefined, undefined, 'ERROR');
  console.error(errorMsg);
};

/* App-related UI control */
UIController.prototype.refreshAppList = function (onFinish) {
  var self = this;
  this.mANTClient.getAppList(function (isSuccess, appList) {
    if (isSuccess) {
      for (var i in appList) {
        var app = appList[i];
        self.addAppToUI(app.name);
      }
      onFinish(appList);
    } else {
      self.showErrorMessage('Get app list failed');
    }
  });
  // TODO: apply deleted app entry that is removed by another controlpad
};

UIController.prototype.selectApp = function (appName) {
  this.mContext.currentAppName = appName;

  // Update app selector label
  var label = appName !== undefined ? appName : 'No app selected';
  this.mAppSelectorLabel.setText(label);

  // Update code editor when code editor is opened
  this.refreshCodeEditor(true);

  // Update console when console is opened
  this.refreshConsole();
};

UIController.prototype.showCreateAppDialog = function () {
  // Show create app dialog
  this.mCreateAppDialog.show();
};

UIController.prototype.addAppToUI = function (appName) {
  // Update app selector menu
  this.mAppSelectorMenu.addApp(appName);

  // TODO: Update app list card when dashboard is opened
};

UIController.prototype.removeAppFromUI = function (appName) {
  // Update app selector menu (remove the app and select another one)
  var index = this.mAppSelectorMenu.getIndex(appName);
  if (index < 0) {
    this.showErrorMessage('Invalid app index: ' + appName + ' / ' + index);
    return;
  }
  var appsCount = this.mAppSelectorMenu.getAppsCount();
  var nextIndex = -1;
  if (appsCount > 1) {
    nextIndex = index > 0 ? index - 1 : 0;
  }
  this.mAppSelectorMenu.removeApp(appName);
  this.selectApp(this.mAppSelectorMenu.getAppNameAt(nextIndex), false);

  // TODO: Update app list card when dashboard is opened
};

/* App selector menu control */
UIController.prototype.toggleAppSelectorMenu = function () {
  this.mAppSelectorMenu.toggle();
};

UIController.prototype.hideAppSelectorMenu = function () {
  this.mAppSelectorMenu.hide();
};

/* Refresh content views */
UIController.prototype.selectNavItem = function (key) {
  this.mNavMenuView.selectItem(this.mNavMenuView.getItem(key));
};

UIController.prototype.refreshCodeEditor = function (isReloadCode) {
  var self = this;
  try {
    if (this.mContext.currentNavItem.getId() === 'navitem-codeeditor') {
      appName = this.mContext.currentAppName;
      if (appName !== undefined) {
        if (isReloadCode) {
          this.mANTClient.getAppCode(appName, function (isSuccess, appCode) {
            if (isSuccess) {
              self.mCodeEditor.setAppCode(appCode);
            } else {
              self.showErrorMessage('Cannot get the code of app ' + appName);
            }
          });
        }
        this.mANTClient.getAppState(appName, function (isSuccess, appState) {
          if (isSuccess) {
            self.mCodeEditor.setCurrentAppState(appState);
          }
        });
      } else {
        this.selectNavItem('navitem-dashboard');
      }
    }
  } catch (e) {
    this.showErrorMessage(e);
  }
};

UIController.prototype.refreshConsole = function () {
  // Update console when console is opened
  var self = this;
  if (this.mContext.currentNavItem.getId() === 'navitem-console') {
    appName = this.mContext.currentAppName;

    fromTs = this.mContext.consoleTsPointers[appName];
    if (fromTs === undefined) fromTs = -1;

    if (appName !== undefined) {
      // Send "get app output request"
      this.mANTClient.getOutput(appName, fromTs, function (isSuccess, outputs) {
        if (isSuccess) {
          // Update ts pointer of the app
          var maxTs = -1;
          for (var i in outputs) {
            var entry = outputs[i];
            if (maxTs < entry.ts) maxTs = entry.ts;
          }
          if (maxTs >= 0) {
            if (
              self.mContext.consoleTsPointers[appName] === undefined ||
              maxTs > self.mContext.consoleTsPointers[appName]
            ) {
              self.mContext.consoleTsPointers[appName] = maxTs + 1;
            }
          }

          // Update console
          self.mConsole.updateOutputs(appName, outputs);
        } else {
          self.showErrorMessage(
            'Cannot get the outputs of app ' + appName,
            false
          );
        }
      });
    } else {
      this.selectNavItem('navitem-dashboard');
    }
  }
};

/* Initializers */
UIController.prototype.initialize = function () {
  // Initialize content views
  this.mDashboard = new DashboardView();
  this.mCodeEditor = new CodeEditorView(
    onAppearCodeEditorView,
    onRefreshCodeEditorView,
    onSaveButtonCodeEditorView,
    onLaunchButtonCodeEditorView,
    onTerminateButtonCodeEditorView,
    onRemoveButtonCodeEditorView
  );
  this.mConsole = new ConsoleView(onRefreshConsoleView);

  // Initialize main UI components
  this.mAppSelectorView = new AppSelectorView(onClickAppSelectorView);
  this.mAppSelectorLabel = new AppSelectorLabelView();
  this.mAppSelectorMenu = new AppSelectorMenuView(
    onClickAppEntryAppSelectorMenuView,
    onClickCreateAppEntryAppSelectorMenuView
  );
  this.mNavMenuView = new NavMenuView(this.mContext);
  this.mCreateAppDialog = new CreateAppDialogView();
  this.mCreateAppFabButton = new CreateAppFabButtonView();
  this.mSnackbar = new SnackbarView();
  this._initializeNavMenuView();
  this._initializeCreateAppFabButtonView();

  // Periodic app-list refresh
  var self = this;
  this.refreshAppList(function (appList) {
    if (appList.length > 0) self.selectApp(appList[0].name);
    else self.selectApp(undefined);
  });
};

UIController.prototype._initializeNavMenuView = function () {
  this.mNavMenuView.addItem(
    'navitem-dashboard',
    'home',
    'Dashboard',
    this.mDashboard
  );
  this.mNavMenuView.addItem(
    'navitem-codeeditor',
    'inbox',
    'Code Editor',
    this.mCodeEditor
  );
  this.mNavMenuView.addItem(
    'navitem-console',
    'wysiwyg',
    'Console',
    this.mConsole
  );

  this.selectNavItem('navitem-dashboard');
};

UIController.prototype._initializeCreateAppFabButtonView = function () {
  var self = this;
  this.mCreateAppFabButton.setOnClickHandler(function () {
    self.showCreateAppDialog();
  });
};

/* Code editor view handlers */
function onAppearCodeEditorView() {
  gUIController.refreshCodeEditor(true);

  if (gUIController.mContext.currentAppName === undefined) {
    gUIController.showErrorMessage('App is not selected.');
    return;
  }
}

function onRefreshCodeEditorView() {
  if (gUIController.mContext.currentAppName !== undefined) {
    gUIController.refreshCodeEditor(false);
  }
}

function onSaveButtonCodeEditorView(appCode) {
  var onSuccess = function (appName, text) {
    gUIController.showSuccessMessage(
      appName + ': app code saved successfully.'
    );
  };
  var onFailure = function (text) {
    gUIController.showErrorMessage(text);
  };

  gAppController.updateCurrentAppCode(appCode, onSuccess, onFailure);
}

function onLaunchButtonCodeEditorView() {
  var onSuccess = function (appName, text) {
    gUIController.showSuccessMessage(appName + ': launched successfully.');

    if (
      gUIController.mContext.currentNavItem.getId() === 'navitem-codeeditor'
    ) {
      gUIController.mCodeEditor.setCurrentAppState('Launching');
    }
  };
  var onFailure = function (text) {
    gUIController.showErrorMessage(text);
  };

  gAppController.launchCurrentApp(onSuccess, onFailure);
}

function onTerminateButtonCodeEditorView() {
  var onSuccess = function (appName, text) {
    gUIController.showSuccessMessage(appName + ': terminated successfully.');

    if (
      gUIController.mContext.currentNavItem.getId() === 'navitem-codeeditor'
    ) {
      gUIController.mCodeEditor.setCurrentAppState('Terminating');
    }
  };
  var onFailure = function (text) {
    gUIController.showErrorMessage(text);
  };

  gAppController.terminateCurrentApp(onSuccess, onFailure);
}

function onRemoveButtonCodeEditorView() {
  var onSuccess = function (appName, text) {
    gUIController.removeAppFromUI(appName);

    gUIController.showSuccessMessage(appName + ': removed successfully.');
  };
  var onFailure = function (text) {
    gUIController.showErrorMessage(text);
  };

  gAppController.removeCurrentApp(onSuccess, onFailure);
}

/* Console view handlers */
function onRefreshConsoleView() {
  gUIController.refreshConsole();

  if (gUIController.mContext.currentAppName === undefined) {
    gUIController.showErrorMessage('App is not selected.');
    return false;
  }
  return true;
}

/* App selector menu view handlers */
function onClickAppSelectorView() {
  gUIController.toggleAppSelectorMenu();
}
function onClickAppEntryAppSelectorMenuView(appName) {
  gUIController.selectApp(appName);
  gUIController.hideAppSelectorMenu();
  
}
function onClickCreateAppEntryAppSelectorMenuView() {
  gUIController.showCreateAppDialog();
}
