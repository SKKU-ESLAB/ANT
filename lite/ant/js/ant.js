var console = require('console');

function ANT() {
}

ANT.prototype.STATE = {};
ANT.prototype.STATE.INITIALIZED = 0;

ANT.prototype.start = function() {
  console.log('ant set_initialize');
};

ANT.prototype.stop = function() {
  console.log('ant set_main');
};

module.exports = new ANT();
module.exports.ANT = ANT;