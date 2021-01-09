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

function DashboardView() {
  this.mRootGrid = new CardGrid();

  this.mAppListCard = new CardView('app-list-card', 'Applications', 6);
  this.mAppListCard.enableListView();
  {
    this.mAppListCard.addListItem('pause', 'image-classification.js');
    this.mAppListCard.addListItem('pause', 'face-detection.js');
    this.mAppListCard.addListItem('pause', 'segmentation.js');
  }
  this.append(this.mAppListCard);

  this.mDeviceStatusCard = new CardView('device-status-card', 'Device', 6);
  this.mDeviceStatusCard.enableListView();
  {
    this.mDeviceStatusCard.addListItem('memory', 'Status: Connected');
    this.mDeviceStatusCard.addListItem(
      'memory',
      'Device Type: Nvidia Jetson TX2'
    );
    this.mDeviceStatusCard.addListItem('analytics', 'CPU Util: 51%');
    this.mDeviceStatusCard.addListItem('analytics', 'GPU Util: 20%');
  }
  this.append(this.mDeviceStatusCard);

  this.mSensorListCard = new CardView('sensor-list-card', 'Sensors', 6);
  this.mSensorListCard.enableListView();
  {
    this.mSensorListCard.addListItem('linked_camera', '/gateway/camera');
    this.mSensorListCard.addListItem('image', '/gateway/imgcls');
    this.mSensorListCard.addListItem('face', '/gateway/facedet');
    this.mSensorListCard.addListItem('image_search', '/gateway/segm');
  }
  this.append(this.mSensorListCard);
}
DashboardView.prototype.getDom = function () {
  return this.mRootGrid.getDom();
};
DashboardView.prototype.onAddedDom = function () {
  // Not yet implemented
};
DashboardView.prototype.append = function (childView) {
  if (childView.getDom !== undefined) {
    this.mRootGrid.append(childView.getDom());
  } else {
    this.mRootGrid.append(childView);
  }
};
