var console = require('console');

var ResourceAPI = undefined;
try {
  ResourceAPI = require('antresource');
} catch (e) {
  throw new Error("Remote UI API Dependency Error: not found Resource API");
}

var RESULT_SUCCESS = 'Success';
var RESULT_FAILED = 'Failed';

function ANTRemoteUI() {
}
ANTRemoteUI.prototype.setStreamingViewPipeline = function (pipeline, handler) {
  ResourceAPI.requestPost("/remoteui/streamingview/pipeline", pipeline, handler);
};
ANTRemoteUI.prototype.setStreamingViewLabelText = function (labelText, handler) {
  ResourceAPI.requestPost("/remoteui/streamingview/labelText", labelText, handler);
};

module.exports = new ANTRemoteUI();
module.exports.ANTRemoteUI = ANTRemoteUI;