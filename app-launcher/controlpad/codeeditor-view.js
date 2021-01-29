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

/* CodeEditorView */
function CodeEditorView(
  onAppear,
  onRefresh,
  onSaveButton,
  onLaunchButton,
  onTerminateButton,
  onRemoveButton,
  updatePeriodMS = 1000
) {
  View.apply(this, ['code-editor-wrapper', 'div']);

  // Attributes
  this.mOnAppear = onAppear;
  this.mOnRefresh = onRefresh;
  this.mCommandBarHeight = 64;
  this.mUpdatePeriodMS = updatePeriodMS;

  // Command bar
  this.mCommandBar = new CommandBar(
    'command-bar',
    this,
    this.mCommandBarHeight,
    10,
    onSaveButton,
    onLaunchButton,
    onTerminateButton,
    onRemoveButton
  );
  this.append(this.mCommandBar);

  // Code Editor
  this.mCodeEditor = document.createElement('div');
  this.mCodeEditor.setAttribute('id', 'code-editor');
  this.append(this.mCodeEditor);

  this.mMonacoEditor = undefined;
}
CodeEditorView.prototype = Object.create(View.prototype);
CodeEditorView.prototype.constructor = CodeEditorView;

CodeEditorView.prototype.onAddedDom = function () {
  // Initialize code editor on added the DOM element is added to root
  var self = this;
  var codeEditor = this.mCodeEditor;
  codeEditor.setAttribute(
    'style',
    'width: 100%; height: calc(100% - ' +
      this.mCommandBarHeight +
      'px); border: 1px solid grey'
  );
  if (this.mMonacoEditor === undefined) {
    require.config({paths: {vs: './vs'}});
    require(['vs/editor/editor.main'], function () {
      self.mMonacoEditor = monaco.editor.create(codeEditor, {
        value: '\n',
        language: 'javascript'
      });
      self.mOnAppear();
      self.startPeriodicUpdate();
    });
  } else {
    this.mOnAppear();
    this.startPeriodicUpdate();
  }
};

CodeEditorView.prototype.startPeriodicUpdate = function () {
  var self = this;
  this.mPeriodicUpdate = setInterval(function () {
    self.mOnRefresh();
  }, this.mUpdatePeriodMS);
};
CodeEditorView.prototype.endPeriodicUpdate = function () {
  if (this.mPeriodicUpdate !== undefined) clearInterval(this.mPeriodicUpdate);
};

CodeEditorView.prototype.setAppCode = function (appCode) {
  if (this.mMonacoEditor !== undefined) {
    this.mMonacoEditor.setValue(appCode);
  }
};

CodeEditorView.prototype.setCurrentAppState = function (state) {
  this.mCommandBar.setRunButtonState(state);
};

CodeEditorView.prototype.saveAppCode = function () {
  this.mCommandBar.saveAppCode();
};

CodeEditorView.prototype.toggleRunApp = function () {
  this.mCommandBar.toggleRunApp();
};

CodeEditorView.prototype.removeApp = function () {
  this.mCommandBar.removeApp();
};

/* CommandBar */
function CommandBar(
  id,
  mother,
  height,
  margin,
  onSaveButton,
  onLaunchButton,
  onTerminateButton,
  onRemoveButton
) {
  View.apply(this, [id, 'div']);

  var self = this;
  this.mMother = mother;
  this.mMargin = margin;
  this.mHeight = height - margin * 2;

  this.mOnSaveButton = onSaveButton;
  this.mOnLaunchButton = onLaunchButton;
  this.mOnTerminateButton = onTerminateButton;
  this.mOnRemoveButton = onRemoveButton;

  this.mRootDom.setAttribute(
    'style',
    'height: ' + this.mHeight + 'px;' + ' margin: ' + this.mMargin + 'px;'
  );

  this.mSaveButton = new ButtonView('save-button', 'save', 'Save');
  this.mSaveButton.setOnClickHandler(function () {
    self.saveAppCode();
  });
  this.append(this.mSaveButton);

  this.mRunButton = new ButtonView('run-button', 'play_arrow', 'Run');
  this.mRunButton.setOnClickHandler(function () {
    self.toggleRunApp();
  });
  this.append(this.mRunButton);

  this.mRemoveButton = new ButtonView('remove-button', 'delete', 'Remove');
  this.mRemoveButton.setOnClickHandler(function () {
    self.removeApp();
  });
  this.append(this.mRemoveButton);
}
CommandBar.prototype = Object.create(View.prototype);
CommandBar.prototype.constructor = CommandBar;

CommandBar.prototype.saveAppCode = function () {
  if (this.mMother.mMonacoEditor === undefined) {
    gUIController.showErrorMessage('Code editor not loaded');
  }
  var appCode = this.mMother.mMonacoEditor.getValue();
  this.mOnSaveButton(appCode);
};

CommandBar.prototype.toggleRunApp = function () {
  var buttonText = this.mRunButton.getText();
  if (buttonText === 'Run') {
    var appCode = this.mMother.mMonacoEditor.getValue();
    this.mOnLaunchButton(appCode);
  } else {
    this.mOnTerminateButton();
  }
};

CommandBar.prototype.removeApp = function () {
  this.mOnRemoveButton();
};

CommandBar.prototype.setRunButtonState = function (state) {
  if (state == 'Inactive') {
    this.mRunButton.setIconType('play_arrow');
    this.mRunButton.setText('Run');
    this.mRunButton.setDisabled(false);
  } else if (state == 'Active') {
    this.mRunButton.setIconType('pause');
    this.mRunButton.setText('Stop');
    this.mRunButton.setDisabled(false);
  } else if (state == 'Launching') {
    this.mRunButton.setIconType('hourglass_empty');
    this.mRunButton.setText('Launching');
    this.mRunButton.setDisabled(true);
  } else if (state == 'Terminating') {
    this.mRunButton.setIconType('hourglass_empty');
    this.mRunButton.setText('Terminating');
    this.mRunButton.setDisabled(true);
  } else {
    gUIController.showErrorMessage('Invalid state: ' + state);
  }
};
