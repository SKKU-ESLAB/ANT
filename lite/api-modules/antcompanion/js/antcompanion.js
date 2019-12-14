function ANTCompanion() { }

ANTCompanion.prototype.getMyIPAddress = function(interfaceName) {
  return native.companion_getMyIPAddress(interfaceName);
};

module.exports = new ANTCompanion();
module.exports.ANTCompanion = ANTCompanion;