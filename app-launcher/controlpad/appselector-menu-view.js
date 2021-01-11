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

function AppSelectorMenuEntryView(iconType, title, onClickHandler) {
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

AppSelectorMenuEntryView.prototype.getDom = function () {
  return this.mRootDom;
};

AppSelectorMenuEntryView.prototype.append = function (childView) {
  if (typeof childView === 'object' && childView.getDom !== undefined) {
    this.mRootDom.append(childView.getDom());
  } else {
    this.mRootDom.append(childView);
  }
};

AppSelectorMenuEntryView.prototype.getTitle = function () {
  return this.mTitle;
};

AppSelectorMenuEntryView.prototype.getIconType = function () {
  return this.mIconType;
};

function AppSelectorMenuView(onClickAppEntry, onClickCreateAppEntry) {
  this.mOnClickAppEntry = onClickAppEntry;
  this.mOnClickCreateAppEntry = onClickCreateAppEntry;
  this.mRootDom = document.getElementById('app-selector-menu');

  var self = this;
  this.mCreateAppEntry = new AppSelectorMenuEntryView(
    'add',
    'Create new app',
    function () {
      self.mOnClickCreateAppEntry();
    }
  );
  this.append(this.mCreateAppEntry);
}

AppSelectorMenuView.prototype.append = function (childView) {
  if (typeof childView === 'object' && childView.getDom !== undefined) {
    this.mRootDom.append(childView.getDom());
  } else {
    this.mRootDom.append(childView);
  }
};

AppSelectorMenuView.prototype.insertBefore = function (childView, beforeView) {
  if (typeof childView === 'object' && childView.getDom !== undefined) {
    childView = childView.getDom();
  }
  if (typeof beforeView === 'object' && beforeView.getDom !== undefined) {
    beforeView = beforeView.getDom();
  }
  this.mRootDom.insertBefore(childView, beforeView);
};

AppSelectorMenuView.prototype.getIndex = function (appName) {
  for (var i in this.mRootDom.children) {
    var childView = this.mRootDom.children[i];
    if (childView.id == 'app-selector-menu-entry-' + appName) {
      return i;
    }
  }
  return -1;
};

AppSelectorMenuView.prototype.getAppNameAt = function (index) {
  if (index < 0) return undefined;
  var childView = this.mRootDom.children[index];
  var appName = childView.id.substring('app-selector-menu-entry-'.length);
  return appName;
};

AppSelectorMenuView.prototype.getAppsCount = function () {
  return this.mRootDom.children.length - 1;
};

AppSelectorMenuView.prototype.addApp = function (appName) {
  var self = this;
  // Check duplicated entry
  for (var i in this.mRootDom.children) {
    var childView = this.mRootDom.children[i];
    if (childView.id == 'app-selector-menu-entry-' + appName) {
      return;
    }
  }

  // Make and insert new entry
  var entry = new AppSelectorMenuEntryView(undefined, appName, function (e) {
    var id = e.target.id;
    var appName = id.substring('app-selector-menu-entry-'.length);
    self.mOnClickAppEntry(appName);
  });
  this.insertBefore(entry, this.mCreateAppEntry);
};

AppSelectorMenuView.prototype.removeApp = function (appName) {
  for (var i in this.mRootDom.children) {
    var childView = this.mRootDom.children[i];
    if (childView.id == 'app-selector-menu-entry-' + appName) {
      childView.remove();
      return;
    }
  }
};
