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

function AppSelectorMenuEntry(iconType, title, onClickHandler) {
  this.mIconType = iconType;
  this.mTitle = title;

  this.mRootDom = document.createElement('li');
  this.mRootDom.setAttribute('id', 'app-selector-menu-entry-' + title);
  this.mRootDom.setAttribute('class', 'mdl-menu__item');
  this.mRootDom.onclick = onClickHandler;

  if (iconType !== undefined) {
    var icon = document.createElement('i');
    icon.setAttribute('class', 'material-icons');
    icon.setAttribute('style', 'margin-right:5px;');
    icon.innerHTML = this.mIconType;
    this.append(icon);
  }
  this.append(title);
}

AppSelectorMenuEntry.prototype.getDom = function () {
  return this.mRootDom;
};

AppSelectorMenuEntry.prototype.append = function (childView) {
  if (typeof childView === 'object' && childView.getDom !== undefined) {
    this.mRootDom.append(childView.getDom());
  } else {
    this.mRootDom.append(childView);
  }
};

AppSelectorMenuEntry.prototype.getTitle = function () {
  return this.mTitle;
};

AppSelectorMenuEntry.prototype.getIconType = function () {
  return this.mIconType;
};

function AppSelectorMenu(onClickAppEntry, onClickCreateAppEntry) {
  this.mOnClickAppEntry = onClickAppEntry;
  this.mOnClickCreateAppEntry = onClickCreateAppEntry;
  this.mRootDom = document.getElementById('app-selector-menu');

  var self = this;
  this.mCreateAppEntry = new AppSelectorMenuEntry(
    'add',
    'Create new app',
    function () {
      self.mOnClickCreateAppEntry();
    }
  );
  this.append(this.mCreateAppEntry);
}

AppSelectorMenu.prototype.append = function (childView) {
  if (typeof childView === 'object' && childView.getDom !== undefined) {
    this.mRootDom.append(childView.getDom());
  } else {
    this.mRootDom.append(childView);
  }
};

AppSelectorMenu.prototype.insertBefore = function (childView, beforeElement) {
  if (typeof childView === 'object' && childView.getDom !== undefined) {
    this.mRootDom.insertBefore(childView.getDom(), beforeElement);
  } else {
    this.mRootDom.insertBefore(childView, beforeElement);
  }
};

AppSelectorMenu.prototype.addApp = function (appName) {
  var self = this;
  var entry = new AppSelectorMenuEntry(undefined, appName, function (e) {
    var id = e.target.id;
    var appName = id.substring(id.indexOf('app-selector-menu-entry-') + 1);
    self.mOnClickAppEntry(appName);
  });

  this.insertBefore(entry, this.mCreateAppEntry);
};

AppSelectorMenu.prototype.removeApp = function (appName) {
  for (var i in this.mRootDom.children) {
    var childView = this.mRootDom.children[i];
    if (childView.id == 'app-selector-menu-entry-' + appName) {
      childView.remove();
      return;
    }
  }
};
