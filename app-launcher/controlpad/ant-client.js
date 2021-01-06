function ANTClient() {}

ANTClient.prototype._sendHTTPRequest = function (
  url,
  method,
  data,
  responseHandler
) {
  var xhr = new XMLHttpRequest();
  xhr.open(method, url, true);
  xhr.setRequestHeader('Content-Type', 'application/json');
  try {
    xhr.onload = function () {
      if (responseHandler !== undefined) {
        responseHandler(this.status, this.responseText);
      }
    };
    if (data === undefined) {
      data = null;
    }
    xhr.send(data);
  } catch (e) {
    changeStatusLabel(e.toString());
  }
};

ANTClient.prototype.getAppList = function () {
  var url = 'http://' + getTargetUrl() + '/runtime/currentApp/code';
  var data = $('#appCode').val();
  this._sendHTTPRequest(
    url,
    'GET',
    data,
    function (responseCode, responseText) {
      if (responseCode == 200) {
        setAppCode(responseText);
      }
      handler(responseCode, responseText);
    }
  );
};

ANTClient.prototype.installApp = function () {};
