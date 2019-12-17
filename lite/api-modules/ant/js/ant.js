var console = require('console');

/** ANT main object start **/
function ANT() { }

console.log("ANT API checking...");

try {
  ANT.prototype.runtime = require('antruntime');
} catch (e) {
  throw new Error("ANT Runtime API is always required!");
}
try {
  ANT.prototype.resource = require('antresource');
} catch (e) {
  console.log("> Not found Resource API");
}
try {
  ANT.prototype.remoteui = require('antremoteui');
} catch (e) {
  console.log("> Not found Remote UI API");
}
try {
  ANT.prototype.stream = require('antstream');
} catch (e) {
  console.log("> Not found Stream API");
}
try {
  ANT.prototype.ml = require('antml');
} catch (e) {
  console.log("> Not found ML API");
}
try {
  ANT.prototype.companion = require('antcompanion');
} catch (e) {
  console.log("> Not found Companion API");
}

/** ANT main object end **/

module.exports = new ANT();
module.exports.ANT = ANT;