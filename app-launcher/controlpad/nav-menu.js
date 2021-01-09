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

function NavItem(id, iconType, title, view) {
  this.mId = id;
  this.mIconType = iconType;
  this.mTitle = title;
  this.mView = view;

  this.mRootDom = document.createElement('a');
  this.mRootDom.setAttribute('id', this.mId);
  this.mRootDom.setAttribute('class', 'mdl-navigation__link');
  this.mRootDom.setAttribute('href', '#');
  {
    var icon = document.createElement('i');
    icon.setAttribute('class', 'mdl-color-text--blue-grey-400 material-icons');
    icon.setAttribute('role', 'presentation');
    icon.innerHTML = this.mIconType;
    this.append(icon);
    this.append(this.mTitle);
  }
}

NavItem.prototype.getDom = function () {
  return this.mRootDom;
};

NavItem.prototype.append = function (childView) {
  if (typeof childView === 'object' && childView.getDom !== undefined) {
    this.mRootDom.append(childView.getDom());
  } else {
    this.mRootDom.append(childView);
  }
};

NavItem.prototype.getId = function () {
  return this.mId;
};

NavItem.prototype.getTitle = function () {
  return this.mTitle;
};

NavItem.prototype.getView = function () {
  return this.mView;
};

function NavMenu() {
  // Set attributes
  this.mNavItems = [];
  this.mRootDom = document.getElementById('nav-menu');
}

NavMenu.prototype.getDom = function () {
  return this.mRootDom;
};

NavMenu.prototype.append = function (childView) {
  if (childView.getDom !== undefined) {
    this.mRootDom.append(childView.getDom());
  } else {
    this.mRootDom.append(childView);
  }
};

NavMenu.prototype.getItem = function (index) {
  return this.mNavItems[index];
};

NavMenu.prototype.updateForItem = function (navItem) {
  // Update header title
  var titleText = 'ANT Controlpad: ' + navItem.getTitle();
  $('#header_title').html(titleText);

  // Update content view
  $('#content_root').html(navItem.getView().getDom());
  navItem.getView().onAddedDom();
};

NavMenu.prototype.addItem = function (id, iconType, title, view) {
  var self = this;
  var onClickNavItem = function (e) {
    var _nav_item = undefined;
    for (var j in self.mNavItems) {
      if (self.mNavItems[j].getId() == e.target.id) {
        _nav_item = self.mNavItems[j];
        break;
      }
    }
    if (_nav_item !== undefined) {
      self.updateForItem(_nav_item);
    }
  };

  // New nav menu item
  var navItem = new NavItem(id, iconType, title, view);
  navItem.getDom().onclick = onClickNavItem;

  // Add navItem to the list
  this.mNavItems.push(navItem);

  // Add to DOM tree
  this.append(navItem);
};
