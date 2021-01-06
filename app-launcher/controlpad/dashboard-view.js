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
