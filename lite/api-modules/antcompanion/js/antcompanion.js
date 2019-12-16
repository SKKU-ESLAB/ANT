function ANTCompanion() { }

ANTCompanion.prototype._mCompanionHost = undefined;
ANTCompanion.prototype._mCompanionPort = undefined;
ANTCompanion.prototype._mCompanionPath = undefined;
ANTCompanion.prototype._mHandlers = [];

ANTCompanion.prototype._setCompanionAddress = function (
  companionHost, companionPort, companionPath) {
  this._mCompanionHost = companionHost;
  this._mCompanionPort = companionPort;
  this._mCompanionPath = companionPath;
  return true;
};

ANTCompanion.prototype._onReceiveMessageFromCompanion = function (message) {
  for (var i in this._mHandlers) {
    this._mHandlers[i](message);
  }
};

ANTCompanion.prototype.sendMessage = function (message) {
  if (this._mCompanionPath === undefined) {
    console.log('Error: failed to send message due to no companion address');
    return false;
  }

  // IoT.js ----
  var options = {
    method: 'POST',
    host: this._mCompanionHost,
    port: this._mCompanionPort,
    path: this._mCompanionPath,
    headers: { 'Content-Length': message.length },
  };
  var client_request = http.request(options);
  client_request.write(message);
  client_request.end();
  // node.js ----
  // var options = {
  //   method: 'POST',
  //   uri: 'http://' + this._mCompanionHost + ':' + this._mCompanionPort + '/' + this._mCompanionPath,
  //   body: message
  // };
  // request.post(options, function (err, httpResponse, body) {
  // });
  return true;
};

ANTCompanion.prototype.registerOnReceiveMessage = function (handler) {
  this._mHandlers.push(handler);
};
ANTCompanion.prototype.unregisterOnReceiveMessage = function (handler) {
  if (handler === undefined) return false;
  for (var i in this._mHandlers) {
    if (this._mHandlers[i] === handler) {
      this._mHandlers.splice(i, 1);
      return true;
    }
  }
  return false;
};

ANTCompanion.prototype.getMyIPAddress = function (interfaceName) {
  var antcompanion = require("antcompanion");
  return antcompanion.getMyIPAddress(interfaceName);
};

ANTCompanion.prototype.getMyIPAddress = function(interfaceName) {
  return native.companion_getMyIPAddress(interfaceName);
};

module.exports = new ANTCompanion();
module.exports.ANTCompanion = ANTCompanion;